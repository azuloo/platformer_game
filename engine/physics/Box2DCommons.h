//
// Created by azuloo on 20.12.21.
//

#ifndef PLATFORMER_GAME_BOX2DCOMMONS_H
#define PLATFORMER_GAME_BOX2DCOMMONS_H

namespace lg::physics::box2D {

enum class Axis {
    FaceAX,
    FaceAY,
    FaceBX,
    FaceBY
};

enum class EdgeNumbers {
    NoEdge = 0,
    Edge1,
    Edge2,
    Edge3,
    Edge4
};

union FeaturePair
{
    struct Edges
    {
        EdgeNumbers inEdge1;
        EdgeNumbers outEdge1;
        EdgeNumbers inEdge2;
        EdgeNumbers outEdge2;
    } edges;
    int value;
};

static void flipFp(FeaturePair& fp)
{
    std::swap(fp.edges.inEdge1, fp.edges.inEdge2);
    std::swap(fp.edges.outEdge1, fp.edges.outEdge2);
}

}

#endif //PLATFORMER_GAME_BOX2DCOMMONS_H
