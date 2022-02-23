//
// Created by azuloo on 24.10.21.
//

#include "MainScene.h"
#include <memory>
#include <iostream>

using lg::physics::RigidBody2D;
using app::MainScene;

std::unique_ptr<MainScene> MainScene::mInstance = nullptr;

/// -------------------------------------------------
MainScene* MainScene::instance()
{
    if (mInstance == nullptr) {
        // TODO: make_unique friend method?
        mInstance = std::unique_ptr<MainScene>(new MainScene);
    }

    return mInstance.get();
}

/// -------------------------------------------------
MainScene::MainScene()
{}

/// -------------------------------------------------
// TODO: ABSTRACT INPUTS
void MainScene::processInputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
//    if (mPlayer == nullptr) return;
//
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//        glfwSetWindowShouldClose(getWindow(), true);
//    }
//    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
//        mPlayer->physics()->applyVelocity({0.0f, -50.0f});
//    }
//    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
//        mPlayer->physics()->applyVelocity({-50.0f, 0.0f});
//    }
//    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
//        mPlayer->physics()->applyVelocity({50.0f, 0.0f});
//    }
//    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
//        mPlayer->physics()->applyVelocity({0.0f, 50.0f});
//        mPlayer->physics()->setAngularVelocity(-1.0f);
//    }
}

/// -------------------------------------------------
void MainScene::render()
{
//    for (auto& mObject : mObjects) {
//        mObject->setProjection(mProjection);
//        mObject->render();
//    }
}

/// -------------------------------------------------
void MainScene::applyPhysics()
{
}