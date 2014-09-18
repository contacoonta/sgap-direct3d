#include "DXUT.h"
#include "Mesh.h"


Mesh::Mesh()
{
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}

Mesh::~Mesh()
{
}
