#include "Sphere.h"

Sphere::Sphere(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity), PE::PxReal radius = 1.f, PE::PxReal density = 1.f)
	: DynamicActor(pose)
{
	CreateShape(PE::PxSphereGeometry(radius), density);
}