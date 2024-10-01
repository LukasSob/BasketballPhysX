#pragma once

#include "PhysicsEngine.h"
#include "TriangleMesh.h"
#include "ConvexMesh.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

//pyramid vertices
static PE::PxVec3 pyramid_verts[] = { PE::PxVec3(0,1,0), PE::PxVec3(1,0,0), PE::PxVec3(-1,0,0), PE::PxVec3(0,0,1), PE::PxVec3(0,0,-1) };
//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
static PE::PxU32 pyramid_trigs[] = { 1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1 };

class Pyramid : public ConvexMesh {
public:
    Pyramid(PE::PxTransform pose = PE::PxTransform(PE::PxIdentity), PE::PxReal density = 1.f);
};

class PyramidStatic : public TriangleMesh
{
public:
	PyramidStatic(PE::PxTransform pose = PE::PxTransform(PE::PxIdentity));
};
