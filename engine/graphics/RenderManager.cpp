//
// Created by azuloo on 10.01.22.
//

#include "RenderManager.h"
#include "./utils/Camera.h"
#include <memory>
#include <iostream>

using lg::graphics::RenderManager;

std::unique_ptr<RenderManager> RenderManager::mInstance = nullptr;

/// -------------------------------------------------
RenderManager::RenderManager()
    : mWidth(WINDOW_WIDTH), mHeight(WINDOW_HEIGHT), mWindowName("Window"), mCurrentTime(0.f), mDeltaTime(0.f),
      mLastFrameTime(0.f), mCamera{}, mWindow(nullptr)
{
    mProjection = Camera::orthographic(0.f, (float) mWidth, 0.f, (float) mHeight, -1.f, 1.f);
}

/// -------------------------------------------------
RenderManager* RenderManager::instance()
{
    if (!mInstance) {
        mInstance = std::make_unique<RenderManager>();
    }

    return mInstance.get();
}

/// -------------------------------------------------
void RenderManager::startUp()
{
    // TODO: Move from hard-coded versions

    /// GLFW init section
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /// GLFW init section end

    _createWindow();
    _setupWindow();
}

/// -------------------------------------------------
void RenderManager::shutDown()
{
    // TODO: ABSTRACTION
    glfwMakeContextCurrent(nullptr);
    glfwTerminate();
}

/// -------------------------------------------------
void RenderManager::_frameBufferResizeCb(GLFWwindow* window, lgInt width, lgInt height)
{
    glViewport(0, 0, width, height);
}

/// -------------------------------------------------
lgInt RenderManager::_createWindow()
{
    mWindow = glfwCreateWindow(mWidth, mHeight, mWindowName, nullptr, nullptr);
    // TODO: Write error log instead of using iostream
    if (nullptr == mWindow) {
        std::cerr << "failed to initialize window";
        APP_TERMINATE;
    }

    return 0;
}

/// -------------------------------------------------
lgInt RenderManager::_setupWindow()
{
    /// NOTE: If you don't make context current window BEFORE gladLoadGLLoader - GLAD will fail to initialize
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, _frameBufferResizeCb);

    /// Loading GLAD func pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "failed to init GLAD";
        APP_TERMINATE;
    }

    glViewport(0, 0, mWidth, mHeight);

    return 0;
}

// TODO: Window abstraction
// TODO: Move to InputsManager?

/// -------------------------------------------------
void RenderManager::setKeyCallback(void (*cb)(GLFWwindow* w, int key, int scancode, int action, int mods))
{
    glfwSetWindowUserPointer(mWindow, SimulationManager::instance());
    glfwSetKeyCallback(mWindow, cb);
}

/// -------------------------------------------------
void RenderManager::_render(std::vector<GameObject*>* objects)
{
    for (auto const & obj : *objects) {
        obj->setProjection(mProjection);
        obj->render();
    }
}

/// ---------------- MAIN GAME LOOP -----------------
/// -------------------------------------------------
void RenderManager::run()
{
    // TODO: CHANGE THE LOOP CONDITION BY A MORE GENERAL ONE
    while (!glfwWindowShouldClose(mWindow)) {
        mCurrentTime = glfwGetTime();
        mDeltaTime = mCurrentTime - mLastFrameTime;
        mLastFrameTime = mCurrentTime;

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // TODO: ABSTRACTION
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto objects = SimulationManager::instance()->getGameObjects();

        // TODO: Should it be explicit like the RenderManager->run() ?
        PhysicsManager::instance()->run();
        _render(objects);

        // TODO: ABSTRACTION
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}