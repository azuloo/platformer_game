//
// Created by azuloo on 25.11.21.
//

#ifndef PLATFORMER_GAME_RECTANGLE_HPP
#define PLATFORMER_GAME_RECTANGLE_HPP

#include "../abstracts/GameObject.h"
#include "../utils/Shader.h"
#include "../utils/Texture2D.h"
#include "gtc/type_ptr.hpp"
#include "../../Types.h"
#include "../../physics/shapes/Box2D.h"
#include "../../State.h"

#include <vector>
#include <memory>
#include <iostream>

using lg::graphics::GameObject;
using lg::graphics::Shader;
using lg::graphics::Texture2D;
using lg::physics::shapes::Box2D;

namespace lg::graphics::objects {

class Rectangle : public GameObject {
public:
    Rectangle(
        lgVec3 position,
        lgVec2 size,
        lgFloat vertices[],
        lgInt verticesSize,
        lgUInt indices[],
        lgInt indicesSize
    );
    ~Rectangle() override = default;

    void render() override;
    void addPhysics() override;

    inline void setVertexSource(const char* source) noexcept override { mVertexSource = source; }
    inline void setFragmentSource(const char* source) noexcept override { mFragmentSource = source; }

    void initShader();

    [[nodiscard]] Shader* getShader() const { return mShader.get(); }

private:
    std::unique_ptr<Shader> mShader;

    std::vector<lgFloat> mVertices;
    std::vector<lgUInt> mIndices;

    const char* mVertexSource;
    const char* mFragmentSource;

    lgInt mModelLoc;
    lgInt mProjectionLoc;
    lgInt mColorLoc;
};

};

using lg::graphics::objects::Rectangle;

/// -------------------------------------------------
Rectangle::Rectangle(
    lgVec3 position,
    lgVec2 size,
    lgFloat vertices[],
    lgInt verticesSize,
    lgUInt indices[],
    lgInt indicesSize
) : GameObject(position, size), mModelLoc(0), mColorLoc(0), mProjectionLoc(0)
{
    mVertices.reserve(verticesSize);
    mIndices.reserve(indicesSize);

    std::copy(vertices, vertices+verticesSize, std::back_inserter(mVertices));
    std::copy(indices, indices+indicesSize, std::back_inserter(mIndices));

    mVertexSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projection;
uniform vec4 aColor;

out vec4 vertexColor;

void main() {
    gl_Position = projection * model * vec4(aPos, 1.0f);
    vertexColor = aColor;
}
)";
    mFragmentSource = R"(
#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}
)";
}

/// -------------------------------------------------
void Rectangle::render()
{
    glUseProgram(mShader->getProgram());

    glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mProjection));

    /// These should be in reverse order (scaling -> rotation -> translation)
    lgMat4 model = lgMat4{1.0f };
    model = glm::translate(model, mPosition);

    model = glm::translate(model, glm::vec3{ mSize.x * 0.5f, mSize.y * 0.5f, 0.0f });
    model = glm::rotate(model, mRotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
    model = glm::translate(model, glm::vec3{ -0.5f * mSize.x, -0.5f * mSize.y, 0.0f });
    model = glm::scale(model, lgVec3{mSize, 1.0f });

    glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4fv(mColorLoc, 1, glm::value_ptr(mColor));

    /// VAO is bound
    glBindVertexArray(mShader->getVAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    /// VAO is unbound
}

/// -------------------------------------------------
void Rectangle::initShader()
{
    mShader = std::make_unique<Shader>(
        mVertices.data(), mVertices.size(), mIndices.data(), mIndices.size(),
        mVertexSource, mFragmentSource
    );
    mShader->init();
    mShader->addVertexAttrPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(lgFloat), nullptr);

    mModelLoc = glGetUniformLocation(mShader->getProgram(), "model");
    mProjectionLoc = glGetUniformLocation(mShader->getProgram(), "projection");
    mColorLoc = glGetUniformLocation(mShader->getProgram(), "aColor");
}

/// -------------------------------------------------
void Rectangle::addPhysics()
{
    mPhysics = std::make_unique<Box2D>(this, mSize.x, mSize.y);
}

#endif //PLATFORMER_GAME_RECTANGLE_HPP
