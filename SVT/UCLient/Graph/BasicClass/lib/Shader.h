#pragma once
#include "BasicHeaders.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned RendererID;
	bool CompileShader();
	std::string m_FilePath;
	int GetUniformLocation(const std::string& name);

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned CompileShader(unsigned int type, const std::string& source);
	std::unordered_map<std::string, int> m_LocationCache;

public:
	Shader() {};
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniform1i(const std::string name, int value);
	void SetUniform1iv(const std::string name, int len, const int* value);
	void SetUniform1f(const std::string name, float value);
	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string name, const glm::mat4& val);
};
;;;;;;;;;;;;;;;;;;;