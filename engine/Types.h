//
// Created by azuloo on 29.11.21.
//

#ifndef PLATFORMER_GAME_TYPES_H
#define PLATFORMER_GAME_TYPES_H

#include "glm.hpp"
#include <glad/glad.h>

// TODO: Different types for different graphics libs (OpenGL, Vulkan etc.)

typedef GLuint lgUInt;
typedef GLint lgInt;
typedef float lgFloat;
typedef double lgDouble;
typedef GLenum lgEnum;
typedef glm::mat4 lgMat4;
typedef glm::mat2x2 lgMat2x2;
typedef glm::vec1 lgVec1;
typedef glm::vec2 lgVec2;
typedef glm::vec3 lgVec3;
typedef glm::vec4 lgVec4;
typedef const char* lgStringLiteral;

#endif //PLATFORMER_GAME_TYPES_H
