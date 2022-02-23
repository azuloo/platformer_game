//
// Created by azuloo on 15.12.21.
//

#include "Arbiter.h"
#include "glm.hpp"
#include "../Math.h"
#include "../shapes/Box2D.h"
#include "../../graphics/abstracts/GameObject.h"

using lg::physics::Arbiter;
using lg::physics::shapes::Box2D;
using lg::graphics::GameObject;

/// -------------------------------------------------
Arbiter::Arbiter(RigidBody2D* first, RigidBody2D* second)
    : mFirstBody(first), mSecondBody(second)
{
    mNumContacts = Box2D::collide(mContacts.data(), mFirstBody, mSecondBody);
    mFriction = sqrtf(mFirstBody->getFriction() * mSecondBody->getFriction());
}

/// -------------------------------------------------
void Arbiter::update(ContactPoint* contacts, lgInt numContacts)
{
    std::array<ContactPoint, NUM_POINTS> mergedContacts;

    for (int i = 0; i < numContacts; i++) {
        ContactPoint* newCp = contacts + i;
        lgInt k = -1;

        for (int j = 0; j < mNumContacts; j++) {
            ContactPoint oldCp = mContacts[j];
            if (newCp->feature.value == oldCp.feature.value) {
                k = j;
                break;
            }
        }

        /// We get here if new cp value = old cp value
        if (k > -1) {
            ContactPoint cOld = mContacts[k];
            mergedContacts[i] = *newCp;
            mergedContacts[i].Pn = cOld.Pn;
            mergedContacts[i].Pt = cOld.Pt;
            mergedContacts[i].Pnb = cOld.Pnb;
        } else { /// New value is different from the old value
            mergedContacts[i] = contacts[i];
        }
    }

    for (int i = 0; i < numContacts; i++) {
        mContacts[i] = mergedContacts[i];
    }

    mNumContacts = numContacts;
}

/// -------------------------------------------------
void Arbiter::preStep(lgFloat invDt)
{
    static const lgFloat allowedPenetration = 0.01f;
    static const lgFloat biasFactor = 0.2f; /// If position correction is enabled (original code)

    for (int i = 0; i < mNumContacts; i++) {
        ContactPoint* cp = &mContacts[i];

        lgVec3 r1 = cp->position - mFirstBody->getGameObject()->getCenterPos();
        lgVec3 r2 = cp->position - mSecondBody->getGameObject()->getCenterPos();

        /// Precomputations:

        /// Normal mass
        lgFloat rn1 = glm::dot(r1, lgVec3{cp->normal, 0.0f });
        lgFloat rn2 = glm::dot(r2, lgVec3{cp->normal, 0.0f });

        lgFloat kNormal = mFirstBody->getInvMass() + mSecondBody->getInvMass();
        kNormal += mFirstBody->getInvMomentOfInertia() * (glm::dot(r1, r1) - rn1 * rn1) +
                mSecondBody->getInvMomentOfInertia() * (glm::dot(r2, r2) - rn2 * rn2);
        cp->massNormal = 1.0f / kNormal;

        /// Tangent mass
        lgVec2 tangent = lg::cross(cp->normal, 1.0f);
        lgFloat rt1 = glm::dot(r1, lgVec3{ tangent, 0.0f });
        lgFloat rt2 = glm::dot(r2, lgVec3{ tangent, 0.0f });
        lgFloat kTangent = mFirstBody->getInvMass() + mSecondBody->getInvMass();
        kTangent += mFirstBody->getInvMomentOfInertia() * (glm::dot(r1, r1) - rt1 * rt1) +
                mSecondBody->getInvMomentOfInertia() * (glm::dot(r2, r2) - rt2 * rt2);
        cp->massTangent = 1.0f / kTangent;

        /// Bias
        cp->bias = -biasFactor * invDt * std::min(0.0f, cp->separation + allowedPenetration);

        /// if (World::accumulateImpulses)
        lgVec2 P = cp->Pn * cp->normal + cp->Pt * tangent;

        mFirstBody->setVelocity(mFirstBody->getVelocity() - mFirstBody->getInvMass() * P);
        mFirstBody->setAngularVelocity(
            mFirstBody->getAngularVelocity() - mFirstBody->getInvMomentOfInertia() * lg::cross(r1, P)
        );

        mSecondBody->setVelocity(mSecondBody->getVelocity() + mSecondBody->getInvMass() * P);
        mSecondBody->setAngularVelocity(
            mSecondBody->getAngularVelocity() + mSecondBody->getInvMomentOfInertia() * lg::cross(r2, P)
        );
    }
}

/// -------------------------------------------------
void Arbiter::applyImpulse()
{
//    std::cout << "-----------------------------------------------" << std::endl;
    for (int i = 0; i < mNumContacts; i++) {
        ContactPoint* cp = &mContacts[i];

        cp->r1 = cp->position - mFirstBody->getGameObject()->getCenterPos();
        cp->r2 = cp->position - mSecondBody->getGameObject()->getCenterPos();

        /// Relative velocity at contact
        lgVec2 dv = mSecondBody->getVelocity() + lg::cross(mSecondBody->getAngularVelocity(), cp->r2) -
                    mFirstBody->getVelocity() - lg::cross(mFirstBody->getAngularVelocity(), cp->r1);

        lgFloat vn = glm::dot(dv, cp->normal);
        lgFloat dPn = cp->massNormal * (-vn + cp->bias);

        /// if (World::accumulateImpulses)
        /// Clamp the accumulated impulse
        lgFloat Pn0 = cp->Pn;
        cp->Pn = std::max(Pn0 + dPn, 0.0f);
        dPn = cp->Pn - Pn0;

        /// Apply contact impulses (Pn)
        lgVec2 Pn = dPn * cp->normal;

        mFirstBody->setVelocity(mFirstBody->getVelocity() - mFirstBody->getInvMass() * Pn);
        mFirstBody->setAngularVelocity(mFirstBody->getAngularVelocity() - mFirstBody->getInvMomentOfInertia() * lg::cross(cp->r1, Pn));

        mSecondBody->setVelocity(mSecondBody->getVelocity() + mSecondBody->getInvMass() * Pn);
        mSecondBody->setAngularVelocity(mSecondBody->getAngularVelocity() + mSecondBody->getInvMomentOfInertia() * lg::cross(cp->r2, Pn));

        /// Relative velocity at contact
        dv = mSecondBody->getVelocity() + lg::cross(mSecondBody->getAngularVelocity(), cp->r2) -
             mFirstBody->getVelocity() - lg::cross(mFirstBody->getAngularVelocity(), cp->r1);

        lgVec2 tangent = lg::cross(cp->normal, 1.0f);
        lgFloat vt = glm::dot(dv, tangent);
        lgFloat dPt = cp->massTangent * (-vt);

        /// if (World::accumulateImpulses)
        /// Compute friction impulse
        lgFloat maxPt = mFriction * cp->Pn;

        /// Clamp friction
        lgFloat oldTangentImpulse = cp->Pt;
        cp->Pt = lg::clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
        dPt = cp->Pt - oldTangentImpulse;

        lgVec2 Pt = dPt * tangent;

        mFirstBody->setVelocity(mFirstBody->getVelocity() - mFirstBody->getInvMass() * Pt);
        mFirstBody->setAngularVelocity(mFirstBody->getAngularVelocity() - mFirstBody->getInvMomentOfInertia() * lg::cross(cp->r1, Pt));

        mSecondBody->setVelocity(mSecondBody->getVelocity() + mSecondBody->getInvMass() * Pt);
        mSecondBody->setAngularVelocity(mSecondBody->getAngularVelocity() + mSecondBody->getInvMomentOfInertia() * lg::cross(cp->r2, Pt));
    }
}