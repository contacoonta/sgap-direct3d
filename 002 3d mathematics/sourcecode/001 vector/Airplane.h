#pragma once

using std::cout;
using std::endl;

class Transform
{
public:
	Transform()	
	{
		cout << "Transform 持失切 " << endl;
	}
	virtual ~Transform()
	{
		cout << "Transform 社瑚切 " << endl;
	}
};

class Airplane : public Transform
{
public:
	Airplane()	
	{
		cout << "Airplane 持失切 " << endl;
	}
	~Airplane()	
	{
		cout << "Airplane 社瑚切 " << endl;
	}

	void setPosition(XMFLOAT3 pos)
	{
		position = pos;
	}
	void setPositionXM(XMVECTOR pos)
	{
		XMStoreFloat3(&position, pos);
	}

	XMFLOAT3 getPosition() const
	{
		return position;
	}

	XMVECTOR getPositionXM() const
	{
		return XMLoadFloat3(&position);
	}


	void Show()
	{
		std::cout << "pos = "
			<< position.x << position.y << position.z
			<< std::endl;
	}

private:
	XMFLOAT3 position;
};