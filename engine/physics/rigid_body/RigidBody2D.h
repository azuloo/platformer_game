//
// Created by azuloo on 09.12.21.
//

#ifndef PLATFORMER_GAME_RIGIDBODY2D_H
#define PLATFORMER_GAME_RIGIDBODY2D_H

#include "../../Types.h"
#include "../arbiter/ContactPoint.h"

namespace lg::graphics {
    class GameObject;
}

using lg::graphics::GameObject;
using lg::physics::ContactPoint;

#define DEFAULT_FRICTION 0.5f
#define DEFAULT_MASS 1.0f
#define DEFAULT_MOMENT_OF_I FLT_MAX

namespace lg::physics {

class RigidBody2D {
public:
    explicit RigidBody2D(GameObject* gameObject);
    virtual ~RigidBody2D() = default;

    virtual void init() = 0;
    virtual void run() = 0;

    /// Setters
    inline void setVelocity(lgVec2 velocity) noexcept { mLinearVelocity = velocity; }
    inline void applyVelocity(lgVec2 velocity) noexcept { mLinearVelocity += velocity; }
    inline void setForce(lgVec2 force) noexcept { mForce = force; }
    inline virtual void setMass(lgFloat mass) { mMass = mass; }
    inline void setAcceleration(lgVec2 acceleration) noexcept { mAcceleration = acceleration; }
    inline void setTorque(lgFloat torque) noexcept { mTorque = torque; }
    inline void setAngularVelocity(lgFloat velocity) noexcept { mAngularVelocity = velocity; }
    inline void setAngle(lgFloat angle) noexcept { mAngle = angle; }
    inline void setFriction(lgFloat friction) noexcept { mFriction = friction; }
    inline void setMomentOfInertia(lgFloat i) noexcept { mMomentOfInertia = i; }

    /// Getters
    [[nodiscard]] inline lgVec2 getVelocity() const noexcept { return mLinearVelocity; }
    [[nodiscard]] inline lgVec2 getForce() const noexcept { return mForce; }
    [[nodiscard]] inline lgFloat getMass() const noexcept { return mMass; }
    [[nodiscard]] inline lgFloat getInvMass() const noexcept { return mInvMass; }
    [[nodiscard]] inline lgVec2 getAcceleration() const noexcept { return mAcceleration; }
    [[nodiscard]] inline lgFloat getTorque() const noexcept { return mTorque; }
    [[nodiscard]] inline lgFloat getAngularVelocity() const noexcept { return mAngularVelocity; }
    [[nodiscard]] inline lgFloat getAngle() const noexcept { return mAngle; }
    [[nodiscard]] inline lgFloat getFriction() const noexcept { return mFriction; }
    [[nodiscard]] inline GameObject* getGameObject() const noexcept { return mGameObject; }
    [[nodiscard]] inline lgFloat getMomentOfInertia() const noexcept { return mMomentOfInertia; }
    [[nodiscard]] inline lgFloat getInvMomentOfInertia() const noexcept { return mInvMomentOfInertia; }

    static bool isCollidedAABB(GameObject* first, GameObject* second);

protected:
    GameObject* mGameObject;

    lgVec2 mForce;
    lgFloat mMass;
    lgFloat mInvMass;
    lgVec2 mAcceleration;
    lgFloat mTorque;
    lgFloat mFriction;

    lgVec2 mLinearVelocity;
    lgFloat mAngularVelocity;
    lgFloat mAngle;
    lgFloat mMomentOfInertia;
    lgFloat mInvMomentOfInertia;
};

}

#endif //PLATFORMER_GAME_RIGIDBODY2D_H
