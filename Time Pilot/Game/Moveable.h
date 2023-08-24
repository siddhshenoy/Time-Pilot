#pragma once
class Moveable
{
private:
	float mVelocity;
public:
	Moveable();
	void SetVelocity(float velocity);
	float GetVelocity();
	~Moveable();
};

