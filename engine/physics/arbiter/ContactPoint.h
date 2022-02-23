//
// Created by azuloo on 15.12.21.
//

#ifndef PLATFORMER_GAME_CONTACTPOINT_H
#define PLATFORMER_GAME_CONTACTPOINT_H

#include "../../Types.h"
#include "../Box2DCommons.h"

using lg::physics::box2D::FeaturePair;

namespace lg::physics {

struct ContactPoint {
    ContactPoint() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {}

    lgVec3 position;
    lgVec2 normal;
    lgVec2 r1, r2;

    lgFloat separation;
    lgFloat Pn; // accumulated normal impulse
    lgFloat Pt; // accumulated tangent impulse
    lgFloat Pnb; // accumulated normal impulse for position bias
    lgFloat massNormal;
    lgFloat massTangent;
    lgFloat bias;

    FeaturePair feature;
};

}

#endif //PLATFORMER_GAME_CONTACTPOINT_H
