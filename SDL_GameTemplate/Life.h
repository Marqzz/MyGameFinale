#pragma once

class Life
{
	int lifes;
public:
	Life()
	{
		lifes = 9;
	}
	~Life()
	{}
	int getLifes()
	{
		return lifes;
	}
	void minusLife()
	{
		lifes--;
	}
	void setLife(int lf)
	{
		lifes = lf;
	}
};