#include "Moveable.h"

Moveable::Moveable()
{
}

void Moveable::SetVelocity(float velocity)
{
    mVelocity = velocity;
}

float Moveable::GetVelocity()
{
    return mVelocity;
}

Moveable::~Moveable()
{
}
