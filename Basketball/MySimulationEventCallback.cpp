#include "MySimulationEventCallback.h"



void MySimulationEventCallback::onTrigger(PE::PxTriggerPair* pairs, PE::PxU32 count)
{
	//you can read the trigger information here
	for (PE::PxU32 i = 0; i < count; i++)
	{
		//filter out contact with the planes
		if (pairs[i].otherShape->getName() == "Sphere")
		{
			//check if eNOTIFY_TOUCH_FOUND trigger
			if (pairs[i].status & PE::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				timesTriggered++;
				std::cerr << "onTrigger::eNOTIFY_TOUCH_FOUND ~ Times Triggered: " + std::to_string(timesTriggered) << std::endl;
				trigger = true;
			}
			//check if eNOTIFY_TOUCH_LOST trigger
			if (pairs[i].status & PE::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				std::cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << std::endl;
				trigger = false;
			}
		}
	}
}

void MySimulationEventCallback::onContact(const PE::PxContactPairHeader& pairHeader, const PE::PxContactPair* pairs, PE::PxU32 nbPairs)
{
	std::cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << std::endl;

	//check all pairs
	for (PE::PxU32 i = 0; i < nbPairs; i++)
	{
		//check eNOTIFY_TOUCH_FOUND
		if (pairs[i].events & PE::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cerr << "onContact::eNOTIFY_TOUCH_FOUND" << std::endl;
		}
		//check eNOTIFY_TOUCH_LOST
		if (pairs[i].events & PE::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			std::cerr << "onContact::eNOTIFY_TOUCH_LOST" << std::endl;
		}
	}
}

void MySimulationEventCallback::onConstraintBreak(PE::PxConstraintInfo* constraints, PE::PxU32 count) {}

void MySimulationEventCallback::onWake(PE::PxActor** actors, PE::PxU32 count) {}

void MySimulationEventCallback::onSleep(PE::PxActor** actors, PE::PxU32 count) {}

#if PX_PHYSICS_VERSION >= 0x304000
void MySimulationEventCallback::onAdvance(const PE::PxRigidBody* const* bodyBuffer, const PE::PxTransform* poseBuffer, const PE::PxU32 count) {}
#endif