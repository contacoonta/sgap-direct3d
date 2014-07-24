#pragma once

typedef unsigned int UINT;

typedef class input
{
public:
	static bool CreateInput(input** ppinput);
	static void ReleaseInput(input** ppinput);

public:
	input();
	input(const input&);
	~input();

	void initialize();

	virtual void KeyDown(UINT ukey);
	virtual void KeyUp(UINT ukey);
	virtual bool GetKeyDown(UINT ukey);

private:
	bool m_keys[256];

}INPUT;

