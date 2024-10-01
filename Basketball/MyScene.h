#ifndef MY_SCENE_H
#define MY_SCENE_H

#include "PhysicsEngine.h"
#include "Plane.h"
#include "Box.h"
#include "Sphere.h"
#include "Pyramid.h"
#include "CompoundObject.h"
#include "Donut.h"
#include "Cloth.h"
#include "MySimulationEventCallback.h"

static const PE::PxVec3 color_palette[] = { PE::PxVec3(46.f / 255.f,9.f / 255.f,39.f / 255.f),PE::PxVec3(217.f / 255.f,0.f / 255.f,0.f / 255.f),
        PE::PxVec3(255.f / 255.f,45.f / 128.f,0.f / 255.f),PE::PxVec3(255.f / 255.f,140.f / 255.f,54.f / 255.f),PE::PxVec3(4.f / 255.f,117.f / 255.f,120.f / 255.f),
        PE::PxVec3(255.0f / 120.f,255.0f / 120.f,255.0f / 120.f) };


class MyScene : public PE::Scene {
    Plane* plane;
    DynamicBox* box;
    Sphere* sphere;

    StaticBox* court;
    StaticBox* freeThrowLine;
    StaticBox* hoopConnectorBox;

    StaticBox* triggerBox;
    Pyramid* pyramid;
    CompoundObjectDynamic* throwObject; 
    Cloth* cloth;

	MySimulationEventCallback* simEventCallback;

	

    bool loopFinished;
	bool moveRimDisaster;

public:
	int currentScore;
    float rimMovementSpeed = 0.01f;

	CompoundObjectStatic* hoopObject;
	Donut* donut;

    void SetVisualisation();

    virtual void CustomInit() override;

    void ResetSphere();

	void MoveRimDisaster(bool value);
    void IncreaseRimSpeed(bool value);

    void DeflatedBallDisaster();
    void ResetBall();

	void BouncyRimDisaster();
	void ResetRim();

    virtual void CustomUpdate() override;

    virtual ~MyScene();
};

#endif // MY_SCENE_H
