#include "MyScene.h"

void MyScene::SetVisualisation() {
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eSCALE, 1.0f);
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eACTOR_AXES, 1.0f);
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);

    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.0f);
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eCLOTH_VERTICAL, 1.0f);
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eCLOTH_BENDING, 1.0f);
    px_scene->setVisualizationParameter(PE::PxVisualizationParameter::eCLOTH_SHEARING, 1.0f);

    loopFinished = false;
    moveRimDisaster = false;
}

void MyScene::CustomInit() {
    SetVisualisation();

    PE::GetMaterial()->setDynamicFriction(.2f);

    simEventCallback = new MySimulationEventCallback();
    px_scene->setSimulationEventCallback(simEventCallback);

    // -- Plane --
    plane = new Plane(PE::PxVec3(0.f, 1.f, 0.f), 0);
    //Setting plane material to be similar to the friction of hardwood flooring to represent a basketball court
    PE::PxMaterial* planeMat = PE::GetPhysics()->createMaterial(0.55f, 0.79f, .75f);
    plane->Material(planeMat);

    plane->Color(PE::PxVec3(210.f / 255.f, 210.f / 255.f, 210.f / 255.f));
    Add(plane);

    // -- Ball Throw Object --
    throwObject = new CompoundObjectDynamic(PE::PxTransform(PE::PxVec3(0.0f, 2.0f, 4.24f),
        PE::PxQuat(1.5708f, PE::PxVec3(1.0f, 0.0f, 0.0f)) *
        PE::PxQuat(1.5708f, PE::PxVec3(0.0f, 0.0f, 1.0f))));

    throwObject->ThrowObj();
    throwObject->SetKinematic(false);
    throwObject->DisableGravity(true);
    throwObject->LockAxis(true);

    throwObject->Color(color_palette[1]);
    Add(throwObject);
    // set position and rotate the throw object to face the hoop
    throwObject->SetGlobalPose(PE::PxTransform(PE::PxVec3(-0.5f, 1.0f, 12.94f), PE::PxQuat(0.75f, PE::PxVec3(0.0f, 1.0f, 0.0f)) * PE::PxQuat(0.15f, PE::PxVec3(1.0f, 0.0f, 0.0f))));


    sphere = new Sphere(PE::PxTransform(PE::PxVec3(0.0f, 2.0f, 12.44f)), 0.3745f, 0.62369f); //0.62369f

    //Setting sphere material to be more bouncy and have more resistance (similar friction to of a basketball or leather material against wood)
    
    PE::PxMaterial* sphereMat = PE::GetPhysics()->createMaterial(0.4f, 0.4f, 0.82f);
    sphere->Material(sphereMat);
    sphere->Color(color_palette[2]);
    sphere->Name("Sphere");
    Add(sphere);


    /// - - - - - - - - Basketball Hoop and Net - - - - - - - - \\\.

    // -- Court --
    court = new StaticBox(PE::PxTransform(PE::PxVec3(0.0f, 0.02f, 5.0f)), PE::PxVec3(10.f, 0.05f, 10.0f), 100);
    PE::PxMaterial* courtMat = PE::GetPhysics()->createMaterial(0.55f, 0.79f, .82f);
    court->Material(courtMat);
    court->Color(PE::PxVec3(0.0f, 0.0f, 0.0f));
    Add(court);

    freeThrowLine = new StaticBox(PE::PxTransform(PE::PxVec3(0.0f, 0.03f, 11.51f)), PE::PxVec3(3.66f, 0.05f, 0.05f), 100);
    freeThrowLine->Color(PE::PxVec3(255.0f, 255.0f, 255.0f));
    Add(freeThrowLine);

    // -- Pole and Backboard --
    hoopObject = new CompoundObjectStatic(PE::PxTransform(PE::PxVec3(0.0f, 6.70f, 0.0f),
        PE::PxQuat(1.5708f, PE::PxVec3(1.0f, 0.0f, 0.0f)) *
        PE::PxQuat(1.5708f, PE::PxVec3(0.0f, 0.0f, 1.0f))));
    PE::PxMaterial* backboardMat = PE::GetPhysics()->createMaterial(1.0f, 1.0f, .50f);

    hoopObject->Hoop();
    hoopObject->Material(backboardMat);
    hoopObject->SetKinematic(true);
    hoopObject->Color(PE::PxVec3(0.0f, 0.0f, 0.0f));
    Add(hoopObject);

    // -- Rim and Backboard Connector

    hoopConnectorBox = new StaticBox(PE::PxTransform(PE::PxVec3(0.05f, 6.23f, 1.2f)), PE::PxVec3(.2f, .06f, 0.2f), 5);
    hoopConnectorBox->Color(PE::PxVec3(0.0f, 0.0f, 0.0f));
    Add(hoopConnectorBox);

    // -- Rim --

    donut = new Donut(PE::PxTransform(PE::PxVec3(-0.55f, 6.25f, 2.0f), PE::PxQuat(1.5708f, PE::PxVec3(1.0f, 0.0f, 0.0f))));
   
    donut->CreateTorusShape(0.6f, 0.6f, 90);
    PE::PxMaterial* rimMat = PE::GetPhysics()->createMaterial(0.6f, 0.56f, 0.40f);
    for (size_t i = 0; i < donut->GetShapes().size(); i++)
    {
        donut->Material(rimMat);
    }
    donut->Color(color_palette[2]);
    donut->SetKinematic(true);
    Add(donut);

    
    // -- Net --

    cloth = new Cloth(PE::PxTransform(PE::PxVec3(0.05f, 6.15f, 2.0f)), PE::PxVec2(1.0f, 0.7f), 20, 20);
    cloth->Color(color_palette[5]);
    Add(cloth);

    /// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \\\.

    /// - - - - - - - - Creating A Trigger Box to Count Score - - - - - - - - \\\.
    triggerBox = new StaticBox(PE::PxTransform(PE::PxVec3(0.05f, 5.9f, 2.0f)), PE::PxVec3(.05f, .05f, 0.05f), 5);
    triggerBox->SetTrigger(true);
    triggerBox->Color(color_palette[1]);
    
    Add(triggerBox);
    PE::PxTriggerPair triggerPair;

    triggerPair.triggerActor = triggerBox->GetShape()->getActor();
    triggerPair.otherActor = sphere->GetShape()->getActor();

    triggerPair.triggerShape = triggerBox->GetShape();
    triggerPair.otherShape = sphere->GetShape();
    triggerPair.otherShape->setName("Sphere");

    simEventCallback->onTrigger(&triggerPair, 0);
    

}

void MyScene::MoveRimDisaster(bool value) {
    moveRimDisaster = value;
}

// increases rim disaster speed based on the input in the VisualDebugger class
void MyScene::IncreaseRimSpeed(bool value) {
    if (value == true)
    {
        rimMovementSpeed = 0.1f;
    }
    else
    {
        rimMovementSpeed = 0.01f;
    }
}

// make the materials restitution reflect a deflated ball
void MyScene::DeflatedBallDisaster() {
    PE::PxMaterial* deflatedBall = PE::GetPhysics()->createMaterial(0.55f, 0.79f, 0.0f);
    sphere->Material(deflatedBall);
}

// reset the court material to its normal bounciness 
void MyScene::ResetBall() {
    PE::PxMaterial* resetBall = PE::GetPhysics()->createMaterial(0.55f, 0.79f, 0.82f);
    sphere->Material(resetBall);
}

//makes the rim material bouncy
void MyScene::BouncyRimDisaster() {
    PE::PxMaterial* disasterMat = PE::GetPhysics()->createMaterial(0.4f, 0.4f, 2.75f);
    for (size_t i = 0; i < donut->GetShapes().size(); i++)
    {
        //put in a for loop as there are many little different spheres which create an appearance of a torus shape
        donut->Material(disasterMat);
    }
}

// reset rim material to not be as bouncy
void MyScene::ResetRim() {
    PE::PxMaterial* resetMat = PE::GetPhysics()->createMaterial(0.6f, 0.56f, 0.10f);
    for (size_t i = 0; i < donut->GetShapes().size(); i++)
    {
        donut->Material(resetMat);
    }
}

void MyScene::CustomUpdate() {
    // Custom update logic here
    
    currentScore = simEventCallback->timesTriggered;
    if (moveRimDisaster == true)
    {
        float minRimMovement = -1.7f;
        float maxRimMovement = 0.4f;
        static float currentRimPos = -0.55f;
        static float currentNetPos = 0.05f;
        static bool movingLeft = true;

        // check if the rim position exceeds the specified range and alternates direction
        if (currentRimPos <= minRimMovement || currentRimPos >= maxRimMovement)
        {
            movingLeft = !movingLeft; // Change the direction of movement
        }

        // update the rim position based on the movement direction
        if (movingLeft)
        {
            currentRimPos -= rimMovementSpeed; // Move Rim left
            currentNetPos -= rimMovementSpeed; // Move Net left (seperate to rim as they are placed differently in world position)
        }
        else
        {
            currentRimPos += rimMovementSpeed; // Move right
            currentNetPos += rimMovementSpeed; // Move Net left
        }

        // set the new global pose for the required objects which are involved in the 'disaster'
        donut->SetGlobalPose(PE::PxTransform(PE::PxVec3(currentRimPos, 6.05f, 2.0f), PE::PxQuat(1.5708f, PE::PxVec3(1.0f, 0.0f, 0.0f))));
        hoopConnectorBox->SetGlobalPose(PE::PxTransform(PE::PxVec3(currentNetPos, 6.035f, 1.2f))); // (0.05f, 6.23f, 1.2f))
        triggerBox->SetGlobalPose(PE::PxTransform(PE::PxVec3(currentNetPos, 5.5f, 2.0f))); // default pos >> 0.05f, 5.7f, 2.0f
        cloth->SetGlobalPosition(PE::PxTransform(PE::PxVec3(currentNetPos, 5.98f, 2.0f)));

    }

    else
    {
        // reset the global pose for the required objects which are involved in the 'disaster'
        donut->SetGlobalPose(PE::PxTransform(PE::PxVec3(-0.55f, 6.05f, 2.0f), PE::PxQuat(1.5708f, PE::PxVec3(1.0f, 0.0f, 0.0f))));
        hoopConnectorBox->SetGlobalPose(PE::PxTransform(PE::PxVec3(0.05f, 6.035f, 1.2f))); // (0.05f, 6.23f, 1.2f))
        triggerBox->SetGlobalPose(PE::PxTransform(PE::PxVec3(0.05f, 5.5f, 2.0f)));
        cloth->SetGlobalPosition(PE::PxTransform(PE::PxVec3(0.05f, 5.98f, 2.0f)));
    }

   
};

// resets sphere (basketball) position to its spawn location
void MyScene::ResetSphere() {
    sphere->SetGlobalPose(PE::PxTransform(PE::PxVec3(0.0f, 2.0f, 12.44f)));
    sphere->ResetShapeVel();
}

MyScene::~MyScene() {
    delete plane;
    delete box;
    delete sphere;
}
