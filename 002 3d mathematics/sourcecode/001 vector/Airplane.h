#pragma once

using std::cout;
using std::endl;

class Transform
{
public:
	Transform()	
	{
		cout << "Transform ������ " << endl;
	}
	virtual ~Transform()
	{
		cout << "Transform �Ҹ��� " << endl;
	}
};

class Airplane : public Transform
{
public:
	Airplane()	
	{
		cout << "Airplane ������ " << endl;
	}
	~Airplane()	
	{
		cout << "Airplane �Ҹ��� " << endl;
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