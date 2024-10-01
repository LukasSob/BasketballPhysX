#pragma once
#include <iostream>
#include <iomanip>
#include "PhysicsEngine.h"

//Typedef to make the code more readable
namespace PE = PhysicsEngine;

///Plane class
class Plane : public PE::StaticActor
{
public:
	//A plane with default paramters: XZ plane centred at (0,0,0)
	Plane(PE::PxVec3 normal, PE::PxReal distance);
};

