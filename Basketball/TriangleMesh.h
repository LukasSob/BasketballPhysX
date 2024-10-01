#pragma once
#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PE = PhysicsEngine;

class TriangleMesh : public PE::StaticActor
{
public:
	//constructor
	TriangleMesh(const std::vector<PE::PxVec3>& verts, const std::vector<PE::PxU32>& trigs, const PE::PxTransform& pose);

	//mesh cooking (preparation)
	PE::PxTriangleMesh* CookMesh(const PE::PxTriangleMeshDesc& mesh_desc);
};

