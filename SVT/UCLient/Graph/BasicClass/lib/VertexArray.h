#include "BasicHeaders.h"
#include "VertexBuffer.h"

class VertexArray //ָ�����㻺��������
{
private:
	unsigned int m_RenderID;
public:
	VertexArray(bool m) {}
	VertexArray();
	VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		this->AddBuffer(vb, layout);
	}
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};