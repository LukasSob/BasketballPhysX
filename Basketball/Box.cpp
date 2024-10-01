#include "Box.h"

StaticBox::StaticBox(const PE::PxTransform& pose, PE::PxVec3 dimensions, PE::PxReal density)
	: StaticActor(pose)
{
	CreateShape(PE::PxBoxGeometry(dimensions), density);
}

DynamicBox::DynamicBox(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity), PE::PxVec3 dimensions = PE::PxVec3(.5f, .5f, .5f), PE::PxReal density = 1.f)
	: DynamicActor(pose)
{
	CreateShape(PE::PxBoxGeometry(dimensions), density);
}