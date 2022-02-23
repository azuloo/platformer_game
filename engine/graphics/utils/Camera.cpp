//
// Created by azuloo on 06.12.21.
//

#include "Camera.h"
#include "gtc/matrix_transform.hpp"

using lg::graphics::Camera;

/// -------------------------------------------------
lgMat4 Camera::orthographic(lgFloat left, lgFloat right, lgFloat bottom, lgFloat top, lgFloat near, lgFloat far)
{
    return glm::ortho(left, right, bottom, top, near, far);
}

/// -------------------------------------------------
lgMat4 Camera::perspective(lgFloat fov, lgFloat ar, lgFloat near, lgFloat far)
{
    return glm::perspective(fov, ar, near, far);
}
