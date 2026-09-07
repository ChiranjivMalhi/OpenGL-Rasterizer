#pragma once

#include <string>

typedef unsigned char stbi_uc;

class Image
{
public:
	Image() = default;
	~Image();

	void Load(const char* pFilename);
	void Unload();

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	int GetBPP() const { return m_bpp; }
	unsigned char* GetImageData() const { return m_imageData; }

private:
	unsigned char* m_imageData = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_bpp = 0;
};
