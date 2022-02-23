#include "glad/glad.h"
#include <memory>

#include "./scenes/MainScene.h"
#include "glm.hpp"
#include "../engine/graphics/objects/Rectangle.hpp"
#include "../engine/graphics/utils/Camera.h"
#include "../engine/physics/Commons.h"
#include "../engine/graphics/RenderManager.h"

using lg::graphics::objects::Rectangle;
using lg::SimulationManager;
using lg::graphics::RenderManager;
using lg::graphics::Texture2D;
using lg::graphics::Camera;
using app::MainScene;

#define APP_WINDOW_WIDTH 1280
#define APP_WINDOW_HEIGHT 720

#define DEFAULT_AR_X 1280.0f
#define DEFAULT_AR_Y 720.0f

#define MAIN_RECT_VERTICES_SIZE 16
#define MAIN_RECT_INDICES_SIZE 6

#define RECT_VERTICES_SIZE 8
#define RECT_INDICES_SIZE 6

std::array<float, MAIN_RECT_VERTICES_SIZE> mVertices = {
    // Vertices   // Texture2D
    1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f,   0.0f, 1.0f,
    0.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 1.0f,   1.0f, 1.0f
};
std::array<unsigned int, MAIN_RECT_INDICES_SIZE> mIndices = {
    0, 1, 2,
    0, 1, 3
};

std::array<float, RECT_VERTICES_SIZE> rectVertices = {
    // Vertices
    1.0f,   0.0f,
    0.0f,   1.0f,
    0.0f,   0.0f,
    1.0f,   1.0f
};

// TODO: Move sharders to files once AssetsManager is up
const char* mainRectVertexSource = R"(
#version 330 core

layout (location = 0) in vec4 vertexData;

uniform mat4 model;
uniform mat4 projection;
uniform vec4 aColor;

out vec4 vertexColor;
out vec2 textCoord;

void main() {
    gl_Position = projection * model * vec4(vertexData.xy, 0.0f, 1.0f);
    textCoord = vertexData.zw;
    vertexColor = aColor;
}
)";
const char* mainRectFragmentSource = R"(
#version 330 core

in vec4 vertexColor;
in vec2 textCoord;

out vec4 FragColor;
uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, textCoord);
}
)";

const char* rectVertexSource = R"(
#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;
uniform vec4 aColor;

out vec4 vertexColor;

void main() {
    gl_Position = projection * model * vec4(aPos.xy, 0.0f, 1.0f);
    vertexColor = aColor;
}
)";
const char* rectFragmentSource = R"(
#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}
)";

// TODO: Consider using static memory instead of heap
// TODO: Assets Manager

/// -------------------------------------------------
std::unique_ptr<Rectangle> getMainRect(Texture2D* tex)
{
    std::unique_ptr<Rectangle> mainRect{std::make_unique<Rectangle>(
            glm::vec3{ 0.0f, 0.0f, -0.5f },
            glm::vec2{ DEFAULT_AR_X, DEFAULT_AR_Y },
            mVertices.data(),
            MAIN_RECT_VERTICES_SIZE,
            mIndices.data(),
            MAIN_RECT_INDICES_SIZE
    )};
    mainRect->setVertexSource(mainRectVertexSource);
    mainRect->setFragmentSource(mainRectFragmentSource);
    mainRect->setTexture(tex);
    // TODO: Is there a better way of initializing shader after setVertexSource()? (default init)
    mainRect->getShader()->addVertexAttrPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    return mainRect;
}

/// -------------------------------------------------
std::unique_ptr<Rectangle> getRect(glm::vec3 position, glm::vec2 size)
{
    std::unique_ptr<Rectangle> rect{ std::make_unique<Rectangle>(
        position,
        size,
        rectVertices.data(),
        RECT_VERTICES_SIZE,
        mIndices.data(),
        RECT_INDICES_SIZE
    ) };

    rect->setVertexSource(rectVertexSource);
    rect->setFragmentSource(rectFragmentSource);
    rect->initShader();
    rect->getShader()->addVertexAttrPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    return rect;
}

/// -------------------------------------------------
int main()
{
    auto renderer = RenderManager::instance();
    renderer->setWidth(APP_WINDOW_WIDTH);
    renderer->setHeight(APP_WINDOW_HEIGHT);
    renderer->setWindowName("Platformer");

    auto simulation = SimulationManager::instance();
    simulation->startUp();

    std::shared_ptr<Texture2D> rectTex { std::make_unique<Texture2D>("back3.png", 1920, 1080, 0) };
    rectTex->loadImageData(true);

    std::unique_ptr<Rectangle> player = getRect(glm::vec3{ 150.f, 400.f, 0.f }, glm::vec2{ 50.f, 50.f });
    std::unique_ptr<Rectangle> rect2 = getRect(glm::vec3{ 2.f, 50.f, 0.f }, glm::vec2{ DEFAULT_AR_X - 2.f, 100.f });

    player->addPhysics();
    player->physics()->setMass(200.f);
    player->physics()->setAngularVelocity(-1.f);

    rect2->addPhysics();
    rect2->physics()->setMass(STATIC_OBJECT_MASS);

    player->setColor(lgVec4{1.0f });
    rect2->setColor(lgVec4{1.0f });

    simulation->setPlayer(player.get());

    simulation->addObject(rect2.get());
    simulation->addObject(player.get());

    simulation->run();

    simulation->shutDown();

    return 0;
}
