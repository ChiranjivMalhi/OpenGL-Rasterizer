#include <iostream>
#include <filesystem>

#include "texture.h"
#include "stb_image.h"
#include "tinyexr.h"

using namespace std;

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName = FileName;
}

bool Texture::Load()
{
    std::filesystem::path path(m_fileName);
    bool isEXR = (path.extension() == ".exr");

    int width = 0;
    int height = 0;
    int bpp = 0;

    unsigned char* image_data = nullptr;
    float* exr_data = nullptr;

    if (isEXR)
    {
        const char* err = nullptr;

        int ret = LoadEXR(&exr_data, &width, &height, m_fileName.c_str(), &err);

        if (ret != TINYEXR_SUCCESS)
        {
            std::cout << "Failed loading EXR: " << m_fileName << std::endl;

            if (err)
            {
                std::cout << err << std::endl;
                FreeEXRErrorMessage(err);
            }

            return false;
        }

        std::cout << "Loaded EXR: " << width << " x " << height << std::endl;
    }
    else
    {
        stbi_set_flip_vertically_on_load(true);

        image_data = stbi_load(m_fileName.c_str(), &width, &height, &bpp, 0);

        if (!image_data)
        {
            std::cout << "Failed loading texture: " << m_fileName << std::endl;
            std::cout << stbi_failure_reason() << std::endl;
            return false;
        }

        std::cout << "Loaded texture: " << width << " x " << height << "  Channels = " << bpp << std::endl;
    }

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);

    if (isEXR)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, exr_data);
    }
    else
    {
        GLenum format = GL_RGB;

        if (bpp == 1)
            format = GL_RED;
        else if (bpp == 3)
            format = GL_RGB;
        else if (bpp == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (image_data)
        stbi_image_free(image_data);

    if (exr_data)
        free(exr_data);

    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}