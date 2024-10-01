#pragma once
#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PE = PhysicsEngine;

class Sphere : public PE::DynamicActor
{
public:
	//a sphere with default parameters:
	// - pose in 0,0,0
	// - dimensions: 1m
	// - denisty: 1kg/m^3
	Sphere(const PE::PxTransform& pose, PE::PxReal radius, PE::PxReal density);
};

