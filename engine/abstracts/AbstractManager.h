//
// Created by azuloo on 10.01.22.
//

#ifndef PLATFORMER_GAME_ABSTRACTMANAGER_H
#define PLATFORMER_GAME_ABSTRACTMANAGER_H

namespace lg {

class AbstractManager {
public:
    AbstractManager() = default;
    virtual ~AbstractManager() = default;

    virtual void startUp() = 0;
    virtual void shutDown() = 0;
    virtual void run() = 0;
};

}


#endif //PLATFORMER_GAME_ABSTRACTMANAGER_H
