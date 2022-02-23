//
// Created by azuloo on 10.01.22.
//

#include "PhysicsManager.h"

using lg::physics::PhysicsManager;
using ArbPair = std::pair<ArbiterKey, Arbiter>;

std::unique_ptr<PhysicsManager> PhysicsManager::mInstance = nullptr;

PhysicsManager::PhysicsManager()
    : mGravity(DEFAULT_GRAVITY), mIterations(DEFAULT_ITERATIONS)
{}

/// -------------------------------------------------
PhysicsManager* PhysicsManager::instance()
{
    if (!mInstance) {
        mInstance = std::make_unique<PhysicsManager>();
    }

    return mInstance.get();
}

/// -------------------------------------------------
void PhysicsManager::startUp()
{

}

/// -------------------------------------------------
void PhysicsManager::shutDown()
{

}

/// -------------------------------------------------
void PhysicsManager::run()
{
    auto dt = 1.f / 10.f;
    auto invDt = dt > 0.0f ? 1.0f / dt : 0.0f;
    auto objects = SimulationManager::instance()->getGameObjects();

    /// Determining overlapping bodies and update contact points
    _broadPhase();

    /// Integrating forces (gravity)
    for (auto * mObj : *objects) {
        auto gObj = mObj->physics();
        if (gObj == nullptr || gObj->getInvMass() == 0.0f)
            continue;

        /// Updating linear velocity
        lgVec2 currV = gObj->getVelocity();
        lgVec2 newV = currV + dt * (mGravity + gObj->getInvMass() * gObj->getForce());

        gObj->setVelocity(newV);

        /// Updating angular velocity
        lgFloat currAV = gObj->getAngularVelocity();
        lgFloat newAV = currAV + dt * gObj->getInvMomentOfInertia() * gObj->getTorque();

        gObj->setAngularVelocity(newAV);
    }

    /// Performing pre-step
    for (auto & mArbiter : mArbiters) {
        mArbiter.second.preStep(invDt);
    }

    /// Performing (impulse) iterations
    for (int i = 0; i < mIterations; i++) {
        for (auto & mArbiter : mArbiters) {
            mArbiter.second.applyImpulse();
        }
    }

    /// Integrating velocities
    for (auto * mObj : *objects) {
        if (mObj->physics() == nullptr)
            continue;

        auto newPos = mObj->getCenterPos() + dt * lgVec3{mObj->physics()->getVelocity(), 0.0f };
        auto newRot = mObj->getRotation() + dt * mObj->physics()->getAngularVelocity();

        /// Subtracting center pos from size to get back to the left corner pos
        mObj->setPosition(newPos - lgVec3{ mObj->getSize(), 0.0f } * 0.5f);
        mObj->setRotation(newRot);

        mObj->physics()->setForce(lgVec2{ 0.0f });
        mObj->physics()->setTorque(0.0f);
    }
}

/// -------------------------------------------------
void PhysicsManager::_broadPhase()
{
    // TODO: Ref should be good here?
    auto objects = *SimulationManager::instance()->getGameObjects();

    // O(n^2) broad-phase
    // TODO: check for other types of broad phases
    for (auto it = objects.begin(); it != objects.end(); it++) {
        auto gObjFirst = *it;
        if (gObjFirst->physics() == nullptr) continue;

        for (auto jt = ++it; jt != objects.end(); ++jt) {
            auto gObjSecond = *jt;
            if (gObjSecond->physics() == nullptr) continue;

            if (gObjFirst->physics()->getInvMass() == 0.0f && gObjSecond->physics()->getInvMass() == 0.0f)
                continue;

            Arbiter newArb{ gObjFirst->physics(), gObjSecond->physics() };
            ArbiterKey key{ gObjFirst->physics(), gObjSecond->physics() };

            if (newArb.getNumContacts() > 0) {
                auto arbIter = mArbiters.find(key);
                if (arbIter == mArbiters.end()) {
                    mArbiters.insert(ArbPair{ key, newArb });
                } else {
                    arbIter->second.update(newArb.getContacts(), newArb.getNumContacts());
                }
            } else {
                mArbiters.erase(key);
            }
        }
    }
}