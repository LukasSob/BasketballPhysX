#pragma once
#include "PhysicsEngine.h"
#include <string>;
#include <iostream>;


namespace PE = PhysicsEngine;

class MySimulationEventCallback : public PE::PxSimulationEventCallback
{
public:
	//an example variable that will be checked in the main simulation loop
	bool trigger;
	int timesTriggered = 0;


	///Method called when the contact with the trigger object is detected.
	virtual void onTrigger(PE::PxTriggerPair* pairs, PE::PxU32 count);
	

	///Method called when the contact by the filter shader is detected.
	virtual void onContact(const PE::PxContactPairHeader& pairHeader, const PE::PxContactPair* pairs, PE::PxU32 nbPairs);


	virtual void onConstraintBreak(PE::PxConstraintInfo* constraints, PE::PxU32 count);
	virtual void onWake(PE::PxActor** actors, PE::PxU32 count);
	virtual void onSleep(PE::PxActor** actors, PE::PxU32 count);
#if PX_PHYSICS_VERSION >= 0x304000
	virtual void onAdvance(const PE::PxRigidBody* const* bodyBuffer, const PE::PxTransform* poseBuffer, const PE::PxU32 count);
#endif
};



