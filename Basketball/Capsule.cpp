#include "Capsule.h"

Capsule::Capsule(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity), PE::PxVec2 dimensions = PE::PxVec2(1.f, 1.f), PE::PxReal density = 1.f)
	: DynamicActor(pose)
{
	CreateShape(PE::PxCapsuleGeometry(dimensions.x, dimensions.y), density);
}