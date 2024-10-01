#pragma once
#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PE = PhysicsEngine;

class ConvexMesh : public PE::DynamicActor
{
public:
	//constructor
	ConvexMesh(const std::vector<PE::PxVec3>& verts, const PE::PxTransform& pose, PE::PxReal density);

	//mesh cooking (preparation)
	PE::PxConvexMesh* CookMesh(const PE::PxConvexMeshDesc& mesh_desc);
};

