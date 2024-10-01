#include "CompoundObject.h"

CompoundObjectStatic::CompoundObjectStatic(const PE::PxTransform& pose) : PE::DynamicActor(pose) {
    
    
}

void CompoundObjectStatic::Hoop(const PE::PxTransform& pose) {
    // Create the pole and backboard connector
    CreateShape(PE::PxBoxGeometry(1.0f, 0.2f, 0.2f), 10.0f);
    

    // Create the backboard
    CreateShape(PE::PxBoxGeometry(0.1f, 1.829f, 1.219f), 20.0f);

    // Create the pole
    CreateShape(PE::PxBoxGeometry(0.2f, 0.2f, 3.45f), 50.0f);

    // Move the pieces in the right positions
    GetShape(1)->setLocalPose(PE::PxTransform(PE::PxVec3(1.0f, 0.0f, 0.0f)));

    GetShape(2)->setLocalPose(PE::PxTransform(PE::PxVec3(-1.0f, 0.0f, 3.25f)));

}

CompoundObjectDynamic::CompoundObjectDynamic(const PE::PxTransform& pose) : PE::DynamicActor(pose) {


}

void CompoundObjectDynamic::ThrowObj(const PE::PxTransform& pose) {
    CreateShape(PE::PxBoxGeometry(0.3f, 0.3f, 0.3f), 2.5f);
    CreateShape(PE::PxBoxGeometry(0.3f, 0.3f, 0.3f), 2.5f);
    CreateShape(PE::PxBoxGeometry(0.3f, 0.3f, 0.3f), 2.5f);

    

    GetShape(0)->setLocalPose(PE::PxTransform(PE::PxVec3(0.6f, 0.6f, 0.6f)));
    GetShape(1)->setLocalPose(PE::PxTransform(PE::PxVec3(0.6f, 0.0f, 0.0f)));
    GetShape(2)->setLocalPose(PE::PxTransform(PE::PxVec3(0.0f, 0.6f, 0.0f)));
}

