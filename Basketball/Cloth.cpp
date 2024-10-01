#include "Cloth.h"

Cloth::Cloth(PE::PxTransform pose, const PE::PxVec2& size, PE::PxU32 width, PE::PxU32 height, bool fix_top)
{
    // Prepare vertices
    PE::PxReal w_step = size.x / width;
    PE::PxReal h_step = size.y / height;

    PE::PxClothParticle* vertices = new PE::PxClothParticle[(width + 1) * (height + 1)];
    PE::PxU32* quads = new PE::PxU32[width * height * 4];

    for (PE::PxU32 j = 0; j < (height + 1); j++)
    {
        for (PE::PxU32 i = 0; i < (width + 1); i++)
        {
            PE::PxU32 offset = i + j * (width + 1);

            // Calculate radius based on height (linearly decreasing) to make it look similar to a basketball net
            float radius = size.x / 2 * (0.65f + (1.0f - float(j) / float(height)) / 2);

            // Calculate circular motion for the tube vertices
            float angle = (float(i) / float(width)) * 2 * PE::PxPi;
            vertices[offset].pos = PE::PxVec3(radius * sin(angle), h_step * j, radius * cos(angle));

            if (fix_top && (j == 0)) // Fix the top row of vertices
                vertices[offset].invWeight = 0.f;
            else
                vertices[offset].invWeight = 1.f;
        }
    }

    // Set quad indices to form the tube structure
    for (PE::PxU32 j = 0; j < height; j++)
    {
        for (PE::PxU32 i = 0; i < width; i++)
        {
            PE::PxU32 quad_offset = (i + j * width) * 4;
            quads[quad_offset + 0] = i + j * (width + 1);
            quads[quad_offset + 1] = (i + 1) + j * (width + 1);
            quads[quad_offset + 2] = (i + 1) + (j + 1) * (width + 1);
            quads[quad_offset + 3] = i + (j + 1) * (width + 1);

            // Connect the last column to the first column to make a closed "tube" 
            if (i == width - 1)
            {
                quads[quad_offset + 1] = j * (width + 1);            // First vertex in the same column
                quads[quad_offset + 2] = (j + 1) * (width + 1);      // First vertex in the next column
            }
        }
    }



    // Initialize cloth mesh description
    mesh_desc.points.data = vertices;
    mesh_desc.points.count = (width + 1) * (height + 1);
    mesh_desc.points.stride = sizeof(PE::PxClothParticle);

    mesh_desc.invMasses.data = &vertices->invWeight;
    mesh_desc.invMasses.count = (width + 1) * (height + 1);
    mesh_desc.invMasses.stride = sizeof(PE::PxClothParticle);

    mesh_desc.quads.data = quads;
    mesh_desc.quads.count = width * height;
    mesh_desc.quads.stride = sizeof(PE::PxU32) * 4;

    // Create cloth fabric (cooking)
    PE::PxClothFabric* fabric = PxClothFabricCreate(*PE::GetPhysics(), mesh_desc, PE::PxVec3(0, -1, 0));

    // Create cloth
    actor = (PE::PxActor*)PE::GetPhysics()->createCloth(pose, *fabric, vertices, PE::PxClothFlags());

    // Enable collisions with scene objects
    ((PE::PxCloth*)actor)->setClothFlag(PE::PxClothFlag::eSCENE_COLLISION, true);
    ((PE::PxCloth*)actor)->setClothFlag(PE::PxClothFlag::eSWEPT_CONTACT, true); // Enable swept contact (CCD)
    ((PE::PxCloth*)actor)->setStretchConfig(PE::PxClothFabricPhaseType::eVERTICAL, PE::PxClothStretchConfig(1)); // Adjust stiffness value
    ((PE::PxCloth*)actor)->setStretchConfig(PE::PxClothFabricPhaseType::eHORIZONTAL, PE::PxClothStretchConfig(1)); // Adjust stiffness value

    ((PE::PxCloth*)actor)->setSolverFrequency(500); // Adjust solver frequency value
    ((PE::PxCloth*)actor)->setFrictionCoefficient(1); // Adjust friction value
    ((PE::PxCloth*)actor)->setDampingCoefficient(PE::PxVec3(0.2f, 0.2f, 0.2f));



    // Set color for visualization
    colors.push_back(PE::PxVec3(1.0f, 0.5f, 2.0f));
    actor->userData = new UserData(&colors.back(), &mesh_desc);
}

void Cloth::SetGlobalPosition(const PE::PxTransform& pose) {
    ((PE::PxCloth*)actor)->setGlobalPose(pose);
}

Cloth::~Cloth()
{
    delete (UserData*)actor->userData;
}