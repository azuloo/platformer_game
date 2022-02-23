//
// Created by azuloo on 09.12.21.
//

#include "RigidBody2D.h"
#include "../../graphics/abstracts/GameObject.h"
#include "../engine/State.h"

using lg::physics::RigidBody2D;
using lg::State;
using lg::graphics::GameObject;

/// -------------------------------------------------
RigidBody2D::RigidBody2D(GameObject* gameObject)
    : mGameObject(gameObject), mLinearVelocity(lgVec2{0.0f, 0.0f }), mForce(lgVec2{0.0f, 0.0f }),
      mAcceleration(lgVec2{0.0f, 0.0f }), mTorque(0.0f), mAngularVelocity(0.0f), mAngle(0.0f),
      mInvMass(0.0f), mInvMomentOfInertia(0.0f),
      mMomentOfInertia(DEFAULT_MOMENT_OF_I), mFriction(DEFAULT_FRICTION), mMass(DEFAULT_MASS)
{}

/// -------------------------------------------------
bool RigidBody2D::isCollidedAABB(GameObject* first, GameObject* second)
{
    // x coordinate collision
    bool xCollided = first->getPosition().x + first->getSize().x >= second->getPosition().x &&
                     second->getPosition().x + second->getSize().x >= first->getPosition().x;
    // y coordinate collision
    bool yCollided = first->getPosition().y + first->getSize().y >= second->getPosition().y &&
                     second->getPosition().y + second->getSize().y >= first->getPosition().y;

    return xCollided && yCollided;
}