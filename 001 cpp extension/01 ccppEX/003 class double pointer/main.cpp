#include <iostream>


class Manager
{
public:
	Manager()	{}
	~Manager()	{}

	static void Create(Manager** ppmgr);
	static void Release(Manager** ppmgr);

public:
	bool GetActive() const	{ return bActive;	}
	void SetActive(bool b)	{ bActive = b;		}

private:
	bool bActive = true;
};

void Manager::Create(Manager** ppmgr)
{
	*ppmgr = new Manager;

}

void Manager::Release(Manager** ppmgr)
{
	if (*ppmgr == nullptr)
		return;

	delete *ppmgr;
	*ppmgr = nullptr;
}


/*
*/
void main()
{
	/*
	Manager* pmgr = new Manager();
	pmgr->bActive = false;	
	delete pmgr;
	*/

	Manager* pmgr = nullptr;

	Manager::Create(&pmgr);

	pmgr->SetActive(true);

	Manager::Release(&pmgr);
}