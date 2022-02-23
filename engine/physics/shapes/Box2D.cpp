//
// Created by azuloo on 09.12.21.
//

#include "../../graphics/abstracts/GameObject.h"
#include "Box2D.h"
#include "../../State.h"
#include "../Math.h"
#include "../Commons.h"

using lg::physics::shapes::Box2D;
using lg::physics::shapes::ClipVertex;
using lg::physics::box2D::Axis;
using lg::physics::box2D::flipFp;
using lg::graphics::GameObject;

/// -------------------------------------------------
Box2D::Box2D(GameObject* gameObject, lgFloat width, lgFloat height)
    : RigidBody2D(gameObject), mWidth(width), mHeight(height)
{
    updateState();
}

/// -------------------------------------------------
void Box2D::setMass(lgFloat mass)
{
    mMass = mass;
    updateState();
}

/// -------------------------------------------------
void Box2D::updateState()
{
    if (mMass < STATIC_OBJECT_MASS) {
        mInvMass = 1.0f / mMass;
        /// Predefined formula: https://en.wikipedia.org/wiki/List_of_moments_of_inertia
        mMomentOfInertia = mMass * (mWidth * mWidth + mHeight * mHeight) / 12.0f;
        mInvMomentOfInertia = 1.0f / mMomentOfInertia;
    } else {
        mInvMass = 0.0f;
        mMomentOfInertia = DEFAULT_MOMENT_OF_I;
        mInvMomentOfInertia = 0.0f;
    }
}

/// -------------------------------------------------
lgInt Box2D::clipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2], const lgVec2& normal, lgFloat offset,
                               EdgeNumbers clipEdge)
{
    /// Start with no output points
    lgInt numOut = 0;

    /// Calculate the distance of end points to the line
    lgFloat fDistance = glm::dot(normal, vIn[0].v) - offset;
    lgFloat sDistance = glm::dot(normal, vIn[1].v) - offset;

    /// If the points are behind the plane
    if (fDistance <= 0.0f) vOut[numOut++] = vIn[0];
    if (sDistance <= 0.0f) vOut[numOut++] = vIn[1];

    /// If the points are on different sides of the plane
    if (fDistance * sDistance < 0.0f) {
        /// Find intersection point of edge and plane
        lgFloat interp = fDistance / (fDistance - sDistance);
        vOut[numOut].v = vIn[0].v + interp * (vIn[1].v - vIn[0].v);

        if (fDistance > 0.0f) {
            vOut[numOut].fp = vIn[0].fp;
            vOut[numOut].fp.edges.inEdge1 = clipEdge;
            vOut[numOut].fp.edges.inEdge2 = EdgeNumbers::NoEdge;
        } else {
            vOut[numOut].fp = vIn[1].fp;
            vOut[numOut].fp.edges.outEdge1 = clipEdge;
            vOut[numOut].fp.edges.outEdge2 = EdgeNumbers::NoEdge;
        }

        ++numOut;
    }

    return numOut;
}

/// -------------------------------------------------
void Box2D::computeIncidentEdge(ClipVertex cv[2], const lgVec2& h, const lgVec2& pos, const lgMat2x2& rot,
                                const lgVec2& normal)
{
    /// The normal is from the reference box
    /// Convert it to the incident box's frame and flip sign
    lgMat2x2 rotT = glm::transpose(rot);
    lgVec2 n = -(rotT * normal);
    lgVec2 nAbs = lg::abs(n);

    if (nAbs.x > nAbs.y)
    {
        if (n.x > 0.0f) {
            cv[0].v = { h.x, -h.y };
            cv[0].fp.edges.inEdge2 = EdgeNumbers::Edge3;
            cv[0].fp.edges.outEdge2 = EdgeNumbers::Edge4;

            cv[1].v = { h.x, h.y };
            cv[1].fp.edges.inEdge2 = EdgeNumbers::Edge4;
            cv[1].fp.edges.outEdge2 = EdgeNumbers::Edge1;
        } else {
            cv[0].v = { -h.x, h.y };
            cv[0].fp.edges.inEdge2 = EdgeNumbers::Edge1;
            cv[0].fp.edges.outEdge2 = EdgeNumbers::Edge2;

            cv[1].v = { -h.x, -h.y };
            cv[1].fp.edges.inEdge2 = EdgeNumbers::Edge2;
            cv[1].fp.edges.outEdge2 = EdgeNumbers::Edge3;
        }
    } else {
        if (n.y > 0.0f) {
            cv[0].v = { h.x, h.y };
            cv[0].fp.edges.inEdge2 = EdgeNumbers::Edge4;
            cv[0].fp.edges.outEdge2 = EdgeNumbers::Edge1;

            cv[1].v = { -h.x, h.y };
            cv[1].fp.edges.inEdge2 = EdgeNumbers::Edge1;
            cv[1].fp.edges.outEdge2 = EdgeNumbers::Edge2;
        } else {
            cv[0].v = { -h.x, -h.y };
            cv[0].fp.edges.inEdge2 = EdgeNumbers::Edge2;
            cv[0].fp.edges.outEdge2 = EdgeNumbers::Edge3;

            cv[1].v =  { h.x, -h.y };
            cv[1].fp.edges.inEdge2 = EdgeNumbers::Edge3;
            cv[1].fp.edges.outEdge2 = EdgeNumbers::Edge4;
        }
    }

    cv[0].v = pos + rot * cv[0].v;
    cv[1].v = pos + rot * cv[1].v;
}

/// -------------------------------------------------
lgInt Box2D::collide(ContactPoint* contacts, RigidBody2D* firstBody, RigidBody2D* secondBody)
{
    /// Setup
    GameObject* fb = firstBody->getGameObject();
    GameObject* sb = secondBody->getGameObject();

    lgVec2 hA = fb->getSize() * 0.5f;
    lgVec2 hB = sb->getSize() * 0.5f;

    lgVec3 posF = fb->getCenterPos();
    lgVec3 posS = sb->getCenterPos();

    lgMat2x2 rotA = lg::getRotationMat(fb->getRotation());
    lgMat2x2 rotB = lg::getRotationMat(sb->getRotation());

    lgMat2x2 rotFT = glm::transpose(rotA);
    lgMat2x2 rotST = glm::transpose(rotB);

    lgVec2 dp = posS - posF;
    lgVec2 dA = rotFT * dp;
    lgVec2 dB = rotST * dp;

    lgMat2x2 C = rotFT * rotB;
    lgMat2x2 absC = lg::abs(C);
    lgMat2x2 absCT = glm::transpose(absC);

    /// First box faces
    lgVec2 faceF = lg::abs(dA) - hA - absC * hB;
    if (faceF.x > 0.0f || faceF.y > 0.0f)
        return 0;

    /// Second box faces
    lgVec2 faceS = lg::abs(dB) - absCT * hA - hB;
    if (faceS.x > 0.0f || faceS.y > 0.0f)
        return 0;

    /// Find best axis
    Axis axis;
    lgFloat separation;
    lgVec2 normal;

    /// First box faces
    axis = Axis::FaceAX;
    separation = faceF.x;
    normal = dA.x > 0.0f ? lgVec2 { rotA[0][0], rotA[0][1] } : -lgVec2 { rotA[0][0], rotA[0][1] };

    const lgFloat relativeTol = 0.95f;
    const lgFloat absoluteTol = 0.01f;

    if (faceF.y > relativeTol * separation + absoluteTol * hA.y) {
        axis = Axis::FaceAY;
        separation = faceF.y;
        normal = dA.y > 0.0f ? lgVec2 { rotA[1][0], rotA[1][1] } : -lgVec2 { rotA[1][0], rotA[1][1] };
    }

    /// Second box faces
    if (faceS.x > relativeTol * separation + absoluteTol * hB.x) {
        axis = Axis::FaceBX;
        separation = faceS.x;
        normal = dB.x > 0.0f ? lgVec2 { rotB[0][0], rotB[0][1] } : -lgVec2 { rotB[0][0], rotB[0][1] };
    }

    if (faceS.y > relativeTol * separation + absoluteTol * hB.y) {
        axis = Axis::FaceBY;
        separation = faceS.y;
        normal = dB.y > 0.0f ? lgVec2 { rotB[1][0], rotB[1][1] } : -lgVec2 { rotB[1][0], rotB[1][1] };
    }

    /// Setup clipping plane data based on the separating axis
    lgVec2 frontNormal, sideNormal;
    ClipVertex incidentEdge[2];
    lgFloat front, negSide, posSide;
    EdgeNumbers negEdge, posEdge;

    /// Compute the clipping lines and the line segment to be clipped
    switch (axis) {
        case Axis::FaceAX: {
            frontNormal = normal;
            front = glm::dot(posF, { frontNormal, 0.0f }) + hA.x;
            sideNormal = { rotA[1][0], rotA[1][1] };
            lgFloat side = glm::dot(posF, { sideNormal, 0.0f });
            negSide = -side + hA.y;
            posSide = side + hA.y;
            negEdge = EdgeNumbers::Edge3;
            posEdge = EdgeNumbers::Edge1;
            computeIncidentEdge(incidentEdge, hB, posS, rotB, frontNormal);
            break;
        }

        case Axis::FaceAY: {
            frontNormal = normal;
            front = glm::dot(posF, { frontNormal, 0.0f }) + hA.y;
            sideNormal = { rotA[0][0], rotA[0][1] };
            lgFloat side = glm::dot(posF, { sideNormal, 0.0f });
            negSide = -side + hA.x;
            posSide = side + hA.x;
            negEdge = EdgeNumbers::Edge2;
            posEdge = EdgeNumbers::Edge4;
            computeIncidentEdge(incidentEdge, hB, posS, rotB, frontNormal);
            break;
        }

        case Axis::FaceBX: {
            frontNormal = -normal;
            front = glm::dot(posS, { frontNormal, 0.0f }) + hB.x;
            sideNormal = { rotB[1][0], rotB[1][1] };
            lgFloat side = glm::dot(posS, { sideNormal, 0.0f });
            negSide = -side + hB.y;
            posSide = side + hB.y;
            negEdge = EdgeNumbers::Edge3;
            posEdge = EdgeNumbers::Edge1;
            computeIncidentEdge(incidentEdge, hA, posF, rotA, frontNormal);
            break;
        }

        case Axis::FaceBY: {
            frontNormal = -normal;
            front = glm::dot(posS, { frontNormal, 0.0f }) + hB.y;
            sideNormal = { rotB[0][0], rotB[0][1] };
            lgFloat side = glm::dot(posS, { sideNormal, 0.0f });
            negSide = -side + hB.x;
            posSide = side + hB.x;
            negEdge = EdgeNumbers::Edge2;
            posEdge = EdgeNumbers::Edge4;
            computeIncidentEdge(incidentEdge, hA, posF, rotA, frontNormal);
            break;
        }
    }

    /// Clip other face with 5 box planes (1 face plane, 4 edge planes)
    ClipVertex clipPoints1[2];
    ClipVertex clipPoints2[2];
    lgInt np;

    /// Clip to box side 1
    np = clipSegmentToLine(clipPoints1, incidentEdge, -sideNormal, negSide, negEdge);
    if (np < 2)
        return 0;

    /// Clip to negative box side 1
    np = clipSegmentToLine(clipPoints2, clipPoints1, sideNormal, posSide, posEdge);
    if (np < 2)
        return 0;

    /// Now clipPoints2 contains the clipping points
    /// Due to roundoff, it's possible that clipping removes all points
    lgInt  numContacts = 0;
    for (int i = 0; i < 2; i++) {
        lgFloat sep = glm::dot(frontNormal, clipPoints2[i].v) - front;
        if (sep <= 0) {
            contacts[numContacts].separation = sep;
            contacts[numContacts].normal = normal;
            /// Slide contact point onto reference face (easy to cull)
            lgVec2 newPos = clipPoints2[i].v - sep * frontNormal;
            contacts[numContacts].position = {newPos, 0.0f };
            contacts[numContacts].feature = clipPoints2[i].fp;
            if (axis == Axis::FaceBX || axis == Axis::FaceBY) {
                flipFp(contacts[numContacts].feature);
            }
            numContacts++;
        }
    }

    return numContacts;
}

/// -------------------------------------------------
void Box2D::computeForceAndTorque()
{
    lgVec2 r {mWidth / 2, mHeight / 2 };
    mTorque = r.x * mForce.y - r.y * mForce.x;
}

/// -------------------------------------------------
void Box2D::computeAngle()
{
    auto dt = (float) State::instance()->getDeltaTime();

    lgFloat angularAcceleration = mTorque / mMomentOfInertia;
    mAngularVelocity += angularAcceleration * dt;
    mAngle += mAngularVelocity * dt;

    mGameObject->setRotation(mAngle);
}

/// -------------------------------------------------
void Box2D::run()
{
    if (mMass <= 0) return;

    computeForceAndTorque();
    computeAngle();
}