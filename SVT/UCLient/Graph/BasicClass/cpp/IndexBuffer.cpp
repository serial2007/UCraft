#include "../lib/IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned count, bool Dynamic) :
	m_Count(count)
{
	ASSERT(sizeof(unsigned) == sizeof(GLuint));

	glCall(glGenBuffers(1, &m_RenderID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
	if(Dynamic)		{ glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), data, GL_DYNAMIC_DRAW)); }
	else			{ glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), data, GL_STATIC_DRAW)); }
}
IndexBuffer::~IndexBuffer()
{
	glCall(glDeleteBuffers(1, &m_RenderID));
}
void IndexBuffer::Bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}
void IndexBuffer::Unbind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}