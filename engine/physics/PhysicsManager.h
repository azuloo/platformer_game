//
// Created by azuloo on 10.01.22.
//

#ifndef PLATFORMER_GAME_PHYSICSMANAGER_H
#define PLATFORMER_GAME_PHYSICSMANAGER_H

#include "../abstracts/AbstractManager.h"
#include "../Types.h"
#include "../graphics/abstracts/GameObject.h"
#include "./arbiter/Arbiter.h"
#include "../SimulationManager.h"
#include <memory>
#include <map>

#define DEFAULT_GRAVITY lgVec2{ 0.0f, -10.0f }
#define DEFAULT_ITERATIONS 10

using lg::AbstractManager;
using lg::SimulationManager;
using lg::physics::Arbiter;
using lg::physics::ArbiterKey;

namespace lg::physics {

class PhysicsManager : public AbstractManager {
public:
    PhysicsManager();

    ~PhysicsManager() override = default;

    void startUp() override;
    void shutDown() override;
    void run() override;

    static PhysicsManager *instance();

private:
    static std::unique_ptr<PhysicsManager> mInstance;

    lgVec2 mGravity;
    lgInt mIterations;

    std::map<ArbiterKey, Arbiter> mArbiters;

private:
    void _broadPhase();
};

}

#endif //PLATFORMER_GAME_PHYSICSMANAGER_H
