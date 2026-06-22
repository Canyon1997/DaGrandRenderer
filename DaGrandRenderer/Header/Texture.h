#ifndef DAGRANDRENDERER_TEXTURE_H
#define DAGRANDRENDERER_TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <string>

class Texture
{

private:
    std::string filePath;
    unsigned int texBuf;
    bool bInitialized;


public:

    Texture(const std::string& texPath) : bInitialized(false)
    {
        filePath = std::string(TEXTURE_ROOT) + texPath;
    }

    void generate2DTexture_RGB();
    void generate2DTexture_RGBA();

    const unsigned int getBuffer() const
    {
        if (!bInitialized)
        {
            std::cout << "Buffer: " << filePath << " is not initialized" << std::endl;
            return 0;
        }

        return texBuf;
    }

    void bind2D() const
    {
        glBindTexture(GL_TEXTURE_2D, getBuffer());
    }

private:

    // internal function for setting the (s,t,r) values of textures. Global across all types of texture for now
    void setTextureSettings();

};

#endif //DAGRANDRENDERER_TEXTURE_H