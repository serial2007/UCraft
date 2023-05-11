#include "../lib/VertexArray.h"

VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_RenderID));
}
VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_RenderID));
}
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	this->Bind();
	vb.Bind();
	const std::vector<VertexBufferElement> elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned i = 0; i < elements.size(); ++i)
	{
		const VertexBufferElement element = elements[i];
		glCall(glEnableVertexAttribArray(i));
		glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
void VertexArray::Bind() const
{
	glCall(glBindVertexArray(m_RenderID));
}
void VertexArray::Unbind() const
{
	glCall(glBindVertexArray(0));
}