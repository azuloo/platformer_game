//
// Created by azuloo on 15.12.21.
//

#ifndef PLATFORMER_GAME_ARBITER_H
#define PLATFORMER_GAME_ARBITER_H

#include "../rigid_body/RigidBody2D.h"
#include "./ContactPoint.h"
#include <array>

using lg::physics::RigidBody2D;
using lg::physics::ContactPoint;

#define NUM_POINTS 2

namespace lg::physics {

struct ArbiterKey {
    ArbiterKey(RigidBody2D* first, RigidBody2D* second)
        : mFirstBody(first), mSecondBody(second) {}

    RigidBody2D* mFirstBody;
    RigidBody2D* mSecondBody;
};

class Arbiter {
public:
    Arbiter(RigidBody2D* first, RigidBody2D* second);

    [[nodiscard]] inline lgInt getNumContacts() const { return mNumContacts; }
    inline ContactPoint* getContacts() { return mContacts.data(); }

    void update(ContactPoint* contacts, lgInt numContacts);

    void preStep(lgFloat invDt);
    void applyImpulse();


private:
    std::array<ContactPoint, NUM_POINTS> mContacts;
    lgInt mNumContacts;

    RigidBody2D* mFirstBody;
    RigidBody2D* mSecondBody;

    lgFloat mFriction;
};

inline bool operator< (const ArbiterKey& lhs, const ArbiterKey& rhs)
{
    if (lhs.mFirstBody < rhs.mFirstBody)
        return true;
    if (lhs.mFirstBody == rhs.mFirstBody && lhs.mSecondBody < rhs.mSecondBody)
        return true;

    return false;
}

}

#endif //PLATFORMER_GAME_ARBITER_H
