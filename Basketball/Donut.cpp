#include "Donut.h"

Donut::Donut(const PE::PxTransform& pose) : PE::DynamicActor(pose) {
    
}

void Donut::CreateTorusShape(float mainRadius, float tubeRadius, int tubeSegments) {
    // Create the main body of the torus using long boxes or cylinders
    // Adjust the size and orientation to form the main body of the torus
    // Set the local pose to position the shapes correctly
    CreateShape(PE::PxSphereGeometry(0.042f), 5);
    GetShape(0)->setLocalPose(PE::PxTransform(PE::PxVec3(0.0f, 0.0f, 0.0f)));

    // Create smaller shapes to form the circular cross-section of the torus
    float angleIncrement = 2 * PE::PxPi / tubeSegments;
    for (int i = 0; i < tubeSegments; ++i) {
        float angle = angleIncrement * i;
        float x = mainRadius + tubeRadius * cos(angle);
        float y = tubeRadius * sin(angle);
        CreateShape(PE::PxSphereGeometry(.042f), 5);
        GetShape(i)->setLocalPose(PE::PxTransform(PE::PxVec3(x, y, 0.0f)));
    }
}
