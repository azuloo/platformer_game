//
// Created by azuloo on 07.12.21.
//

#include "SimulationManager.h"
#include "./graphics/utils/Camera.h"
#include "./State.h"
#include "./graphics/RenderManager.h"
#include <GLFW/glfw3.h>

using lg::SimulationManager;
using lg::graphics::Camera;
using lg::State;
using lg::graphics::RenderManager;

std::unique_ptr<SimulationManager> SimulationManager::mInstance = nullptr;

/// -------------------------------------------------
SimulationManager::SimulationManager()
    : mPlayer(nullptr)
{}


// TODO: Move it from here
/// -------------------------------------------------
void SimulationManager::processInputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (mPlayer == nullptr) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        mPlayer->physics()->applyVelocity({0.0f, -50.0f});
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        mPlayer->physics()->applyVelocity({-50.0f, 0.0f});
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        mPlayer->physics()->applyVelocity({50.0f, 0.0f});
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        mPlayer->physics()->applyVelocity({0.0f, 50.0f});
        mPlayer->physics()->setAngularVelocity(-1.0f);
    }
}

/// -------------------------------------------------
void SimulationManager::initKeyCallback()
{
    auto _cb = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        auto _sm = static_cast<SimulationManager*>(glfwGetWindowUserPointer(w));
        _sm->processInputs(w, key, scancode, action, mods);
    };

    RenderManager::instance()->setKeyCallback(_cb);
}

/// -------------------------------------------------
SimulationManager* SimulationManager::instance()
{
    if (!mInstance) {
        mInstance = std::make_unique<SimulationManager>();
    }

    return mInstance.get();
}

/// Here goes the global Manager init order
/// -------------------------------------------------
void SimulationManager::startUp()
{
    PhysicsManager::instance()->startUp();
    RenderManager::instance()->startUp();
}

/// Here goes the global Manager shut down order (reverse)
/// -------------------------------------------------
void SimulationManager::shutDown()
{
    RenderManager::instance()->shutDown();
    PhysicsManager::instance()->shutDown();
}

/// -------------------------------------------------
void SimulationManager::run()
{
    initKeyCallback();
    RenderManager::instance()->run();
}