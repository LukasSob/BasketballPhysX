#pragma once
#include "PhysicsEngine.h"

namespace PE = PhysicsEngine;

//static const PE::PxVec3 color_palette[] = { PE::PxVec3(46.f / 255.f,9.f / 255.f,39.f / 255.f),PE::PxVec3(217.f / 255.f,0.f / 255.f,0.f / 255.f),
		//PE::PxVec3(255.f / 255.f,45.f / 255.f,0.f / 255.f),PE::PxVec3(255.f / 255.f,140.f / 255.f,54.f / 255.f),PE::PxVec3(4.f / 255.f,117.f / 255.f,111.f / 255.f) };

class Cloth : public PE::Actor
{
    PE::PxClothMeshDesc mesh_desc;

public:
    // Constructor
    Cloth(PE::PxTransform pose = PE::PxTransform(PE::PxIdentity), const PE::PxVec2& size = PE::PxVec2(1.f, 1.f), PE::PxU32 width = 1, PE::PxU32 height = 1, bool fix_top = true);

    void SetGlobalPosition(const PE::PxTransform& pose);

    // Destructor
    ~Cloth();
};
