#include "DXUT.h"
#include "SDKmisc.h"

#include "CompileShader.h"
#include "MeshMd5.h"




MeshMd5::MeshMd5()
{
}

MeshMd5::~MeshMd5()
{
	Release();
}

Mesh* MeshMd5::Clone()
{
	Mesh* pmesh = new MeshMd5;

	/*
		������ �ε��� , ���ؽ� ���۸� �����Ѵ�.
	*/
	/*pmesh->m_vertexbuffer = m_vertexbuffer;
	pmesh->m_indexbuffer = m_indexbuffer;
	pmesh->m_indexCnt = m_indexCnt;
	pmesh->m_bClone = true;*/

	return pmesh;
}

HRESULT MeshMd5::Initialize()
{
	HRESULT hr = S_OK;
	return hr;
}


void MeshMd5::Update(float deltaTime)
{
	UpdateFrame(deltaTime, 0);
}

void MeshMd5::Render( CompileShader* pshader )
{
	if (pshader == nullptr)
		return;

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	for (int i = 0; i < m_model.numSubsets; i++)
	{
		//�ε��� ����
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
		//���ؽ� ����
		DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_model.subsets[i].vertBuff, &stride, &offset);

		//�ؽ��� ����
		DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, &m_textures[m_model.subsets[i].texArrayIndex]);
		DXUTGetD3D11DeviceContext()->PSSetSamplers(0, 1, &pshader->m_samplerLinear);
		
		//�׸���
		DXUTGetD3D11DeviceContext()->DrawIndexed(m_model.subsets[i].indices.size(), 0, 0);
	}
}

void MeshMd5::Release()
{
	if (m_bClone)
		return;

	for (int i = 0; i < m_model.numSubsets; ++i)
	{
		SAFE_RELEASE(m_model.subsets[i].indexBuff);
		SAFE_RELEASE(m_model.subsets[i].vertBuff);
	}

	m_textureNames.clear();
	for (auto x : m_textures) { SAFE_RELEASE(x); }

}


/*
	Update Frame - Animation
*/

void MeshMd5::UpdateFrame(float deltaTime, int animationIdx)
{
	if (m_model.animations.size() <= 0)
		return;

	m_model.animations[animationIdx].currAnimTime += deltaTime;			// Update the current animationIdx time

	if (m_model.animations[animationIdx].currAnimTime > m_model.animations[animationIdx].totalAnimTime)
		m_model.animations[animationIdx].currAnimTime = 0.0f;

	// Which frame are we on
	float currentFrame = m_model.animations[animationIdx].currAnimTime * m_model.animations[animationIdx].frameRate;
	int frame0 = floorf( currentFrame );
	int frame1 = frame0 + 1;

	// Make sure we don't go over the number of frames	
	if (frame0 == m_model.animations[animationIdx].numFrames - 1)
		frame1 = 0;

	float interpolation = currentFrame - frame0;	// Get the remainder (in time) between frame0 and frame1 to use as interpolation factor

	std::vector<Joint> interpolatedSkeleton;		// Create a frame skeleton to store the interpolated skeletons in

	// Compute the interpolated skeleton
	for (int i = 0; i < m_model.animations[animationIdx].numJoints; i++)
	{
		Joint tempJoint;
		Joint joint0 = m_model.animations[animationIdx].frameSkeleton[frame0][i];		// Get the i'th joint of frame0's skeleton
		Joint joint1 = m_model.animations[animationIdx].frameSkeleton[frame1][i];		// Get the i'th joint of frame1's skeleton

		tempJoint.parentID = joint0.parentID;											// Set the tempJoints parent id

		// Turn the two quaternions into XMVECTORs for easy computations
		XMVECTOR joint0Orient = XMVectorSet(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
		XMVECTOR joint1Orient = XMVectorSet(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

		// Interpolate positions
		tempJoint.pos.x = joint0.pos.x + (interpolation * (joint1.pos.x - joint0.pos.x));
		tempJoint.pos.y = joint0.pos.y + (interpolation * (joint1.pos.y - joint0.pos.y));
		tempJoint.pos.z = joint0.pos.z + (interpolation * (joint1.pos.z - joint0.pos.z));

		// Interpolate orientations using spherical interpolation (Slerp)
		XMStoreFloat4(&tempJoint.orientation, XMQuaternionSlerp(joint0Orient, joint1Orient, interpolation));

		interpolatedSkeleton.push_back(tempJoint);		// Push the joint back into our interpolated skeleton
	}

	for (int k = 0; k < m_model.numSubsets; k++)
	{
		for (int i = 0; i < m_model.subsets[k].vertices.size(); ++i)
		{
			Vertex tempVert = m_model.subsets[k].vertices[i];
			tempVert.pos = XMFLOAT3(0, 0, 0);	// Make sure the vertex's pos is cleared first
			tempVert.normal = XMFLOAT3(0, 0, 0);	// Clear vertices normal

			// Sum up the joints and weights information to get vertex's position and normal
			for (int j = 0; j < tempVert.WeightCount; ++j)
			{
				Weight tempWeight = m_model.subsets[k].weights[tempVert.StartWeight + j];
				Joint tempJoint = interpolatedSkeleton[tempWeight.jointID];

				// Convert joint orientation and weight pos to vectors for easier computation
				XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
				XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

				// We will need to use the conjugate of the joint orientation quaternion
				XMVECTOR tempJointOrientationConjugate = XMQuaternionInverse(tempJointOrientation);

				// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
				// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
				XMFLOAT3 rotatedPoint;
				XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

				// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights bias into account
				tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
				tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
				tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias;

				// Compute the normals for this frames skeleton using the weight normals from before
				// We can comput the normals the same way we compute the vertices position, only we don't have to translate them (just rotate)
				XMVECTOR tempWeightNormal = XMVectorSet(tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f);

				// Rotate the normal
				//XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightNormal), tempJointOrientationConjugate));

				// Add to vertices normal and ake weight bias into account
				/*tempVert.normal.x -= rotatedPoint.x * tempWeight.bias;
				tempVert.normal.y -= rotatedPoint.y * tempWeight.bias;
				tempVert.normal.z -= rotatedPoint.z * tempWeight.bias;*/

				tempVert.normal.x += (rotatedPoint.x) * tempWeight.bias;
				tempVert.normal.y += (rotatedPoint.y) * tempWeight.bias;
				tempVert.normal.z += (rotatedPoint.z) * tempWeight.bias;

			}

			m_model.subsets[k].positions[i] = tempVert.pos;				// Store the vertices position in the position vector instead of straight into the vertex vector
			m_model.subsets[k].vertices[i].normal = tempVert.normal;		// Store the vertices normal
			XMStoreFloat3(&m_model.subsets[k].vertices[i].normal, XMVector3Normalize(XMLoadFloat3(&m_model.subsets[k].vertices[i].normal)));
		}

		// Put the positions into the vertices for this subset
		for (int i = 0; i < m_model.subsets[k].vertices.size(); i++)
		{
			m_model.subsets[k].vertices[i].pos = m_model.subsets[k].positions[i];
		}

		// Update the subsets vertex buffer
		// First lock the buffer
		//HRESULT hr = S_OK;
		//D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
		//hr = DXUTGetD3D11DeviceContext()->Map(m_model.subsets[k].vertBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);
		//if (FAILED(hr))
		//	return;

		//// Copy the data into the vertex buffer.
		//memcpy(mappedVertBuff.pData, &m_model.subsets[k].vertices[0], (sizeof(Vertex)* m_model.subsets[k].vertices.size()));

		//DXUTGetD3D11DeviceContext()->Unmap(m_model.subsets[k].vertBuff, 0);

		//// The line below is another way to update a buffer. You will use this when you want to update a buffer less
		//// than once per frame, since the GPU reads will be faster (the buffer was created as a DEFAULT buffer instead
		//// of a DYNAMIC buffer), and the CPU writes will be slower. You can try both methods to find out which one is faster
		//// for you. if you want to use the line below, you will have to create the buffer with D3D11_USAGE_DEFAULT instead
		//// of D3D11_USAGE_DYNAMIC
		//DXUTGetD3D11DeviceContext()->UpdateSubresource(m_model.subsets[k].vertBuff, 0, NULL, &m_model.subsets[k].vertices[0], 0, 0);
	
		D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
		DXUTGetD3D11DeviceContext()->Map(m_model.subsets[k].vertBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);
		memcpy(mappedVertBuff.pData, &m_model.subsets[k].vertices[0], (sizeof(Vertex)* m_model.subsets[k].vertices.size()));
		DXUTGetD3D11DeviceContext()->Unmap(m_model.subsets[k].vertBuff, 0);

	}//for()
}