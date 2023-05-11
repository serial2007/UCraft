#include "BasicHeaders.h"

struct VertexBufferElement
{
	unsigned type;
	unsigned count;
	unsigned char normalized;
	static unsigned GetSizeOfType(unsigned type)
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		case GL_INT:			return sizeof(int);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() : m_Stride(0) {};

	template<typename T> void push(unsigned count)
	{
		ASSERT(false);
	}
	template<> void push<float>(unsigned count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}
	template<> void push<int>(unsigned count)
	{
		m_Elements.push_back({ GL_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_INT) * count;
	}
	template<> void push<unsigned int>(unsigned count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	template<> void push<unsigned char>(unsigned count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const
	{
		return m_Elements;
	}
	inline unsigned GetStride() const
	{
		return m_Stride;
	}
};

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer() { return; };
	VertexBuffer(bool) { return; };
	VertexBuffer(const void* data, unsigned size, bool dynamic = 0);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
};

