//
// Created by azuloo on 07.12.21.
//

#ifndef PLATFORMER_GAME_SIMULATIONMANAGER_H
#define PLATFORMER_GAME_SIMULATIONMANAGER_H

#include "./Types.h"
#include "../engine/graphics/abstracts/GameScene.h"
#include "../engine/graphics/abstracts/GameObject.h"
#include "./physics/arbiter/Arbiter.h"
#include "./abstracts/AbstractManager.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <map>

using lg::AbstractManager;
using lg::graphics::GameObject;
using lg::physics::Arbiter;
using lg::physics::ArbiterKey;

namespace lg {

// TODO: Abstract graphics libs (OpenGL, Vulkan etc.)
class SimulationManager : public AbstractManager {
public:
    SimulationManager();
    ~SimulationManager() override = default;

    static SimulationManager* instance();

    void startUp() override;
    void shutDown() override;
    void run() override;

    // TODO: Implement
    void processInputs(GLFWwindow* window, int key, int scancode, int action, int mods);

    inline void addObject(GameObject* object) { mObjects.push_back(object); }
    // TODO: Implement
    inline void removeObject(GameObject* object) { }

    /// Setters
    void setPlayer(GameObject* player) noexcept { mPlayer = player; }

    /// Getters
    inline std::vector<GameObject*>* getGameObjects() { return &mObjects; }
    [[nodiscard]] GameObject* getPlayer() const noexcept { return mPlayer; }

protected:
    std::vector<GameObject*> mObjects;
    GameObject* mPlayer;

private:
    static std::unique_ptr<SimulationManager> mInstance;

private:
    static void initKeyCallback();
};

}

#endif //PLATFORMER_GAME_SIMULATIONMANAGER_H
