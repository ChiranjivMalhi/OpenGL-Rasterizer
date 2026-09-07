#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"
#include <cstdio>
#include <cassert>

Image::~Image()
{
    Unload();
}

void Image::Load(const char* pFilename)
{
    if (m_imageData) {
        Unload();
    }

    stbi_set_flip_vertically_on_load(1);

    m_imageData = stbi_load(pFilename, &m_width, &m_height, &m_bpp, 0);

    if (!m_imageData) {
        printf("Can't load texture from '%s' - %s\n", pFilename, stbi_failure_reason());
        exit(0);
    }

    printf("Loaded '%s' - width %d, height %d, bpp %d\n", pFilename, m_width, m_height, m_bpp);
}

void Image::Unload()
{
    printf("Unloading STB image\n");
    if (!m_imageData) {
        printf("Trying to unload a NULL image\n");
        exit(0);
    }

    stbi_image_free(m_imageData);
    m_imageData = nullptr;
    m_width = 0;
    m_height = 0;
    m_bpp = 0;
}