//
// Created by azuloo on 24.10.21.
//

#ifndef PLATFORMER_GAME_SIMULATIONMANAGER_H
#define PLATFORMER_GAME_MAINSCENE_H

#include <set>
#include <memory>
#include "../engine/SimulationManager.h"
#include "glm.hpp"

using lg::SimulationManager;
using lg::graphics::GameObject;

namespace app {

class MainScene : public SimulationManager {
public:
    ~MainScene() override = default;

    static MainScene* instance();

    void processInputs(GLFWwindow* window, int key, int scancode, int action, int mods);

    void render();
    void applyPhysics();

    /// Setters
    void setPlayer(GameObject* player) noexcept { mPlayer = player; }

    /// Getters
    [[nodiscard]] GameObject* getPlayer() const noexcept { return mPlayer; }

    MainScene(const MainScene&) = delete;
    MainScene& operator=(const MainScene&) = delete;

protected:
    MainScene();

private:
    static std::unique_ptr<MainScene> mInstance;
    GameObject* mPlayer;
};

}

#endif //PLATFORMER_GAME_SIMULATIONMANAGER_H
