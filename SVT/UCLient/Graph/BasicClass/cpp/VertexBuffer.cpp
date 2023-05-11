#include "../lib/VertexBuffer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned size, bool dynamic)
{
	glCall(glGenBuffers(1, &m_RendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	if(dynamic)	{ glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW)); }
	else		{ glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); }
}
VertexBuffer::~VertexBuffer()
{
	glCall(glDeleteBuffers(1, &m_RendererID));
}
void VertexBuffer::Bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::Unbind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
