//
// Created by azuloo on 10.01.22.
//

#ifndef PLATFORMER_GAME_RENDERMANAGER_H
#define PLATFORMER_GAME_RENDERMANAGER_H

#include "../abstracts/AbstractManager.h"
#include "../Types.h"
#include "../graphics/abstracts/GameObject.h"
#include "../SimulationManager.h"
#include "../physics/PhysicsManager.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define APP_TERMINATE glfwTerminate(); \
                      return EXIT_FAILURE

using lg::AbstractManager;
using lg::SimulationManager;
using lg::physics::PhysicsManager;
using lg::graphics::GameObject;

namespace lg::graphics {

class RenderManager : public AbstractManager {
public:
    RenderManager();
    ~RenderManager() override = default;

    void startUp() override;
    void shutDown() override;
    void run() override;

    static RenderManager* instance();

    void setKeyCallback(void (*cb)(GLFWwindow* w, int key, int scancode, int action, int mods));

    /// Getters
    [[nodiscard]] inline lgDouble getDeltaTime() const noexcept { return mDeltaTime; }
    [[nodiscard]] inline GLFWwindow* getWindow() const { return mWindow; } // TODO: ABSTRACTION (window)
    [[nodiscard]] inline lgInt getWidth() const noexcept { return mWidth; }
    [[nodiscard]] inline lgInt getHeight() const noexcept { return mHeight; }
    [[nodiscard]] inline lgStringLiteral getWindowName() const noexcept { return mWindowName; }

    /// Setters
    inline void setProjection(lgMat4 projection) noexcept { mProjection = projection; }
    inline void setCamera(lgMat4 camera) noexcept { mCamera = camera; }
    inline void setWidth(lgInt width) noexcept { mWidth = width; }
    inline void setHeight(lgInt height) noexcept { mHeight = height; }
    inline void setWindowName(lgStringLiteral name) noexcept { mWindowName = name; }

private:
    static std::unique_ptr<RenderManager> mInstance;

    lgInt mWidth;
    lgInt mHeight;
    lgStringLiteral mWindowName;

    lgMat4 mProjection;
    lgMat4 mCamera;

    lgDouble mCurrentTime;
    lgDouble mDeltaTime;
    lgDouble mLastFrameTime;

    GLFWwindow* mWindow;

private:
    static void _frameBufferResizeCb(GLFWwindow* window, lgInt width, lgInt height);
    lgInt _createWindow();
    lgInt _setupWindow();
    void _render(std::vector<GameObject*>* objects);
};

}



#endif //PLATFORMER_GAME_RENDERMANAGER_H
