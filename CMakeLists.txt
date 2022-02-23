cmake_minimum_required(VERSION 3.20)
project(platformer_game)

set(CMAKE_CXX_STANDARD 14)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(platformer_game
        src/main.cpp
        glad/src/glad.c
        src/scenes/MainScene.h
        src/scenes/MainScene.cpp
        engine/graphics/abstracts/GameObject.h
        engine/graphics/abstracts/GameScene.h
        engine/graphics/utils/Shader.cpp
        engine/graphics/utils/Shader.h
        engine/graphics/objects/Rectangle.hpp
        engine/graphics/utils/Texture2D.cpp
        engine/graphics/utils/Texture2D.h
        stb_image/stb_image.h stb_image/stb_image.cpp
        engine/graphics/utils/Camera.cpp
        engine/graphics/utils/Camera.h
        engine/State.cpp engine/State.h
        engine/SimulationManager.cpp
        engine/SimulationManager.h
        engine/physics/rigid_body/RigidBody2D.cpp engine/physics/rigid_body/RigidBody2D.h
        engine/physics/shapes/Box2D.cpp engine/physics/shapes/Box2D.h
        engine/physics/arbiter/Arbiter.cpp
        engine/physics/arbiter/Arbiter.h
        engine/physics/arbiter/ContactPoint.h
        engine/physics/Math.h
        engine/physics/Box2DCommons.h
        engine/physics/Commons.h
        engine/graphics/RenderManager.cpp
        engine/graphics/RenderManager.h
        engine/physics/PhysicsManager.cpp
        engine/physics/PhysicsManager.h
        engine/abstracts/AbstractManager.h)

target_include_directories(platformer_game PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glad/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glm>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb_image>
)

target_link_libraries(platformer_game glfw OpenGL::GL)
