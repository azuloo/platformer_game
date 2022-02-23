//
// Created by azuloo on 28.11.21.
//

#include "Texture2D.h"
#include "stb_image.h"
#include <glad/glad.h>

using lg::graphics::Texture2D;

/// -------------------------------------------------
Texture2D::Texture2D(const char* imagePath, lgInt width, lgInt height, lgInt nrChannels)
    : mWidth(width), mHeight(height), mNrChannels(nrChannels), mImagePath(imagePath), mTexture(0), mImageData(nullptr)
{
    /// TODO: MOVE IT FROM HERE (OpenGL crashes if called before window init)
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // TODO: Move these to setters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/// -------------------------------------------------
void Texture2D::loadImageData(bool withMipmap)
{
    if (!mImagePath) return;
    stbi_set_flip_vertically_on_load(true);
    mImageData = stbi_load(mImagePath, &mWidth, &mHeight, &mNrChannels, 0);
    if (mImageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImageData);
       if (withMipmap)
           glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(mImageData);
}
