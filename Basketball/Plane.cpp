#include "Plane.h"


Plane::Plane(PE::PxVec3 normal = PE::PxVec3(0.f, 1.f, 0.f), PE::PxReal distance = 0.f)
	: StaticActor(PE::PxTransformFromPlaneEquation(PE::PxPlane(normal, distance)))
{
	CreateShape(PE::PxPlaneGeometry());
}