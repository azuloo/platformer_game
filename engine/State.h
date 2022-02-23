//
// Created by azuloo on 07.12.21.
//

#ifndef PLATFORMER_GAME_STATE_H
#define PLATFORMER_GAME_STATE_H

#include "./Types.h"
#include <memory>

namespace lg {

class State {
public:
    static State* instance();

    /// Setters
    inline void setDeltaTime(lgDouble dt) noexcept { mDeltaTime = dt; }

    /// Getters
    [[nodiscard]] inline lgDouble getDeltaTime() const noexcept { return mDeltaTime; }

protected:
    State() : mDeltaTime(0.0f) {};

private:
    static std::unique_ptr<State> mInstance;

    lgDouble mDeltaTime;
};

}

#endif //PLATFORMER_GAME_STATE_H
