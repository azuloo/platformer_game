//
// Created by azuloo on 24.10.21.
//

#ifndef PLATFORMER_GAME_GAMESCENE_H
#define PLATFORMER_GAME_GAMESCENE_H

#include "./GameObject.h"
#include "glm.hpp"

using lg::graphics::GameObject;

namespace lg::graphics {

class GameScene {
public:
    GameScene()=default;
    virtual ~GameScene()=default;

    virtual void addObject(GameObject* object) = 0;
    virtual void removeObject(GameObject* object) = 0;
};

}

#endif //PLATFORMER_GAME_GAMESCENE_H
