//
// Created by azuloo on 06.12.21.
//

#ifndef PLATFORMER_GAME_CAMERA_H
#define PLATFORMER_GAME_CAMERA_H

#include "../../Types.h"

namespace lg::graphics {

class Camera {
public:
    Camera()=default;
    ~Camera()=default;

    static lgMat4 orthographic(lgFloat left, lgFloat right, lgFloat bottom, lgFloat top, lgFloat near, lgFloat far);
    static lgMat4 perspective(lgFloat fov, lgFloat ar, lgFloat near, lgFloat far);
};

}

#endif //PLATFORMER_GAME_CAMERA_H
