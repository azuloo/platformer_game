//
// Created by azuloo on 28.11.21.
//

#ifndef PLATFORMER_GAME_TEXTURE2D_H
#define PLATFORMER_GAME_TEXTURE2D_H

#include "stb_image.h"
#include "../../Types.h"

namespace lg::graphics {

class Texture2D {
public:
    Texture2D()=default;
    Texture2D(const char* imagePath, lgInt width, lgInt height, lgInt nrChannels);
    ~Texture2D()=default;

    void loadImageData(bool withMipmap);

    [[nodiscard]] inline lgUInt getTextureHandle() const { return mTexture; }
private:
    lgUInt mTexture;
    lgInt mWidth, mHeight;
    lgInt mNrChannels;

    const char* mImagePath;
    unsigned char* mImageData;
};

}

#endif //PLATFORMER_GAME_TEXTURE2D_H
