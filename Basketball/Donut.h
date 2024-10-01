#pragma once
#include "PhysicsEngine.h"


namespace PE = PhysicsEngine;

class Donut : public PE::DynamicActor
{
public:
    Donut(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity));

    void CreateTorusShape(float mainRadius, float tubeRadius, int tubeSegments);
};


