//
// Created by azuloo on 07.12.21.
//

#include "State.h"

using lg::State;

std::unique_ptr<State> State::mInstance = nullptr;

/// -------------------------------------------------
State* State::instance()
{
    if (mInstance == nullptr) {
        mInstance = std::unique_ptr<State>(new State());
    }

    return mInstance.get();
}


