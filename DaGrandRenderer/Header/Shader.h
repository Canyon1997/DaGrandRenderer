//
// Created by canyo on 3/17/2026.
//

#ifndef DAGRANDRENDERER_SHADER_H
#define DAGRANDRENDERER_SHADER_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <sstream>

#include "stb_image.h"


class Shader
{
private:
    unsigned int shaderProgram = -1;

public:

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    void use() const
    {
        glUseProgram(shaderProgram);
    }
    const unsigned int getProgram() const
    {
        return shaderProgram;
    }

    static void flipVerticallyOnLoad(const bool state)
    {
        stbi_set_flip_vertically_on_load(state);
    }
};

#endif //DAGRANDRENDERER_SHADER_H