#include "../lib/Shader.h"

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;
	std::string line;
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}
unsigned Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
unsigned Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader!\n";
		std::cout << "Type = " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "shader\n";
		std::cout << message << '\n';
		glDeleteShader(id);
		return 0;
	}
	return id;
}
bool Shader::CompileShader()
{
	return 0;
}
Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}
Shader::~Shader()
{
	glDeleteProgram(RendererID);
}
void Shader::Bind() const
{
	glUseProgram(RendererID);
}
void Shader::Unbind() const
{
	glUseProgram(0);
}
void Shader::SetUniform1i(const std::string name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform1iv(const std::string name, int len, const int* value)
{
	glUniform1iv(GetUniformLocation(name), len, (const GLint*)value);
}
void Shader::SetUniform1f(const std::string name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::SetUniformMat4f(const std::string name, const glm::mat4& val)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, 0, &val[0][0]);
}
int Shader::GetUniformLocation(const std::string& name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end())
		return m_LocationCache[name];

	int location = glGetUniformLocation(RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";
	m_LocationCache[name] = location;
	return location;

}
;;;;;;;;;;;;