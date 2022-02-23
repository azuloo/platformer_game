//
// Created by azuloo on 27.10.21.
//

#ifndef PLATFORMER_GAME_SHADER_H
#define PLATFORMER_GAME_SHADER_H

#include "../../Types.h"

namespace lg::graphics {

class Shader {
public:
    Shader(
        float* vertices,
        long verticesSize,
        unsigned int* indices,
        long indicesSize,
        const char* vertexSource,
        const char* fragmentSource
    );
    ~Shader();

    void init();

    void addVertexAttrPointer(lgUInt index, lgInt size, lgEnum type, bool normalized, lgInt stride, const void* ptr);

    [[nodiscard]] inline unsigned int getVertexShader() const noexcept { return mVertexShader; }
    [[nodiscard]] inline unsigned int getFragmentShader() const noexcept { return mFragmentShader; }
    [[nodiscard]] inline unsigned int getProgram() const noexcept { return mShaderProgram; }
    [[nodiscard]] inline unsigned int getVAO() const noexcept { return vao; }
    [[nodiscard]] inline unsigned int getVBO() const noexcept { return vbo; }
    [[nodiscard]] inline unsigned int getEBO() const noexcept { return ebo; }

protected:
    void initShaders();
    void initBuffers();

private:
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;

    const char *mVertexSource;
    const char *mFragmentSource;

    float* mVertices;
    long mVerticesSize;
    unsigned int* mIndices;
    long mIndicesSize;

    unsigned int mVertexShader;
    unsigned int mFragmentShader;
    unsigned int mShaderProgram;

    static bool shaderIsCompiled(unsigned int shader);
    static bool programIsLinked(unsigned int program);
};

}

#endif //PLATFORMER_GAME_SHADER_H
