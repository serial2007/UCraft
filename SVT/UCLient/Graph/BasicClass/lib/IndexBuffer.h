#include "BasicHeaders.h"

class IndexBuffer
{
private:
	unsigned int m_RenderID;
	unsigned int m_Count;
public:
	IndexBuffer() {};
	IndexBuffer(bool) { return; };
	IndexBuffer(const unsigned int* data, unsigned count, bool dynamic = 0);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
};