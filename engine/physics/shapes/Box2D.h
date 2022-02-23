//
// Created by azuloo on 09.12.21.
//

#ifndef PLATFORMER_GAME_BOX2D_H
#define PLATFORMER_GAME_BOX2D_H

#include "../rigid_body/RigidBody2D.h"
#include "../../Types.h"
#include "../Box2DCommons.h"
#include "../arbiter/ContactPoint.h"
#include <memory>
#include <utility>

using lg::physics::RigidBody2D;
using lg::graphics::GameObject;
using lg::physics::box2D::EdgeNumbers;
using lg::physics::ContactPoint;

namespace lg::physics::shapes {

struct ClipVertex {
    ClipVertex() : v{}, fp{} { fp.value = 0; }
    lgVec2 v;
    FeaturePair fp;
};

class Box2D : public RigidBody2D {
public:
    Box2D(GameObject* gameObject, lgFloat width, lgFloat height);
    ~Box2D() override = default;

    void setMass(lgFloat mass) override;

    void run() override;
    void init() override { };

    static lgInt clipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2],
                                   const lgVec2& normal, lgFloat offset, EdgeNumbers clipEdge);
    static lgInt collide(ContactPoint* contacts, RigidBody2D* firstBody, RigidBody2D* secondBody);

protected:
    void computeForceAndTorque();
    void computeAngle();

    static void computeIncidentEdge(ClipVertex cv[2], const lgVec2& h, const lgVec2& pos,
                                    const lgMat2x2& rot, const lgVec2& normal);

    void updateState();

private:
    lgFloat mWidth;
    lgFloat mHeight;
};

}

#endif //PLATFORMER_GAME_BOX2D_H
