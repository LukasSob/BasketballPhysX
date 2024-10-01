#pragma once
#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PE = PhysicsEngine;

class StaticBox : public PE::StaticActor
{
public:
	//a Box with default parameters:
	// - pose in 0,0,0
	// - dimensions: 1m x 1m x 1m
	// - denisty: 1kg/m^3
	StaticBox(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity), PE::PxVec3 dimensions = PE::PxVec3(.5f, .5f, .5f), PE::PxReal density = 1.f);
	
};

class DynamicBox : public PE::DynamicActor
{
public:
	//a Box with default parameters:
	// - pose in 0,0,0
	// - dimensions: 1m x 1m x 1m
	// - denisty: 1kg/m^3
	DynamicBox(const PE::PxTransform& pose, PE::PxVec3 dimensions, PE::PxReal density);
};

