#pragma once
#include "BasicHeaders.h"
#include "lib/stb_image.h"
#include <string>

class Texture
{
private:
	unsigned m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture() {};
	Texture(const std::string path);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind();
	inline int GetWidth()  const { return m_Width;  }
	inline int GetHeight() const { return m_Height; }
};

static GLuint LoadTexture(const std::string& path);