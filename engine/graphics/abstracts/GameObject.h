//
// Created by azuloo on 24.10.21.
//

#ifndef PLATFORMER_GAME_GAMEOBJECT_H
#define PLATFORMER_GAME_GAMEOBJECT_H

#include "glm.hpp"
#include "../utils/Texture2D.h"
#include "../engine/physics/rigid_body/RigidBody2D.h"
#include <memory>

namespace lg::physics {
    class RigidBody2D;
}

using lg::graphics::Texture2D;
using lg::physics::RigidBody2D;

namespace lg::graphics {

class GameObject {
public:
    GameObject(lgVec3 position, lgVec2 size)
        : mPosition{ position }, mSize{ size }, mRotation(0.0f), mPhysics(nullptr),
          mCamera{}, mProjection{}, mTexture{}, mColor{}
    {}
    virtual ~GameObject() = default;

    virtual void render() = 0;
    virtual void addPhysics() = 0;

    /// Setters
    inline virtual void setSize(lgVec2 size) noexcept { mSize = size; }
    inline virtual void setPosition(lgVec3 pos) noexcept { mPosition = pos; }
    inline virtual void setRotation(lgFloat rotation) noexcept { mRotation = rotation; }
    inline virtual void setProjection(lgMat4 projection) noexcept { mProjection = projection; }
    inline virtual void setCamera(lgMat4 camera) noexcept { mCamera = camera; }
    virtual void setVertexSource(const char* source) noexcept = 0;
    virtual void setFragmentSource(const char* source) noexcept = 0;
    inline virtual void setTexture(Texture2D* tex) noexcept { mTexture = tex; }
    inline virtual void setColor(lgVec4 color) noexcept { mColor = color; }

    /// Getters
    [[nodiscard]] inline virtual lgVec2 getSize() const noexcept { return mSize; }
    [[nodiscard]] inline virtual lgVec3 getPosition() const noexcept { return mPosition; }
    [[nodiscard]] inline virtual lgVec3 getCenterPos() const { return mPosition + lgVec3{ mSize, 0.0f } * 0.5f; }
    [[nodiscard]] inline virtual lgFloat getRotation() const noexcept { return mRotation; }
    [[nodiscard]] inline virtual lgMat4 getProjection() const noexcept { return mProjection; }
    [[nodiscard]] inline virtual lgMat4 getCamera() const noexcept { return mCamera; }
    [[nodiscard]] inline virtual Texture2D* getTexture() const noexcept { return mTexture; }
    [[nodiscard]] inline virtual lgVec4 getColor() const noexcept { return mColor; }
    [[nodiscard]] inline RigidBody2D* physics() const noexcept { return mPhysics.get(); }

protected:
    lgVec2 mSize;
    lgVec3 mPosition;
    lgFloat mRotation;

    lgMat4 mProjection;
    lgMat4 mCamera;

    Texture2D* mTexture;
    lgVec4 mColor;

    // TODO: ABSTRACT PHYSICS?
    std::unique_ptr<RigidBody2D> mPhysics;
};

}

#endif //PLATFORMER_GAME_GAMEOBJECT_H
