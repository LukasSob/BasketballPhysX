#pragma once
#include "PhysicsEngine.h"
#include "Donut.h"
//#include "MyScene.h"

namespace PE = PhysicsEngine;


class CompoundObjectStatic : public PE::DynamicActor
{
    public:
        CompoundObjectStatic(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity));

        Donut* donut;

        void Hoop(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity));

};

class CompoundObjectDynamic : public PE::DynamicActor
{
public:
    CompoundObjectDynamic(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity));

    void ThrowObj(const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity));

};


    