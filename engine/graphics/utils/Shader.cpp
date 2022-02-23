//
// Created by azuloo on 27.10.21.
//

#include <iostream>
#include "Shader.h"
#include <glad/glad.h>
#include <stdexcept>

using lg::graphics::Shader;

/// -------------------------------------------------
Shader::Shader(
    float *vertices, long verticesSize, unsigned int *indices,
    long indicesSize, const char* vertexSource, const char* fragmentSource
) :
    mVertices(vertices), mVerticesSize(verticesSize),
    mIndices(indices), mIndicesSize(indicesSize), mVertexSource(vertexSource),
    mFragmentSource(fragmentSource), vao(0), vbo(0),ebo(0),
    mVertexShader(0), mFragmentShader(0), mShaderProgram(0)
{}

/// -------------------------------------------------
Shader::~Shader()
{
//    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
}

/// -------------------------------------------------
void Shader::init()
{
    initBuffers();
    initShaders();
}

/// -------------------------------------------------
void Shader::initBuffers()
{
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);

    /// VAO is bound
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    auto verticesTotalSize = static_cast<long>(mVerticesSize * sizeof(float));
    auto indicesTotalSize = static_cast<long>(mIndicesSize * sizeof(float));

    glBufferData(GL_ARRAY_BUFFER, verticesTotalSize, mVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesTotalSize, mIndices, GL_STATIC_DRAW);

    /// The call to glVertexAttribPointer (section above) registered VBO as the vertex attribute's bound
    /// vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    /// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    /// VAO is unbound
}

/// -------------------------------------------------
bool Shader::shaderIsCompiled(unsigned int shader)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return !!success;
}

/// -------------------------------------------------
bool Shader::programIsLinked(unsigned int program)
{
    int success;
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    return !!success;
}

/// -------------------------------------------------
void Shader::initShaders()
{
    /// Vertex shader section
    mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVertexShader, 1, &mVertexSource, nullptr);
    glCompileShader(mVertexShader);

    if (!Shader::shaderIsCompiled(mVertexShader)) {
        throw std::runtime_error("failed to compile vertex shader");
    }
    /// Vertex shader section end

    /// Fragment shader section
    mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFragmentShader, 1, &mFragmentSource, nullptr);
    glCompileShader(mFragmentShader);

    if (!Shader::shaderIsCompiled(mFragmentShader)) {
        throw std::runtime_error("failed to compile fragment shader");
    }
    /// Fragment shader section end

    /// Shader program section
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragmentShader);
    glLinkProgram(mShaderProgram);

    if (!Shader::programIsLinked(mShaderProgram)) {
        throw std::runtime_error("failed to link shader program");
    }
    /// Shader program section end

    glDeleteShader(mVertexShader);
    glDeleteShader(mFragmentShader);
}

/// -------------------------------------------------
void Shader::addVertexAttrPointer(lgUInt index, lgInt size, lgEnum type, bool normalized, lgInt stride,
                                  const void *ptr)
{
    /// Binding vao & vbo, adding new attribute pointer, unbinding vao & vbo
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(index, size, type, normalized, stride, ptr);
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
