#pragma once
#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PE = PhysicsEngine;

class Capsule : public PE::DynamicActor
{
public:
	Capsule(const PE::PxTransform& pose, PE::PxVec2 dimensions, PE::PxReal density);
};

