#include "shader.h"

#include <glad/glad.h>
#include <iostream>

//constructer to build & read the shader
unsigned int compileShader(const char* vertexSource, const char* fragmentSource) {
	// compile and link a shader from vertex and fragment sources
	int success; // did the phase complete successfully (compile vertex, compile fragment, link)

	// compile the vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	
	// check for errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED" << std::endl;
		int loglength;
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			char *infolog = (char*)malloc(loglength);
			glGetShaderInfoLog(vertex, loglength, NULL, infolog);
			std::cout << infolog << std::endl;
			free(infolog);
		}
	}

	// compile the fragment shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);

	// check for errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
		int loglength;
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			char *infolog = (char*)malloc(loglength);
			glGetShaderInfoLog(fragment, loglength, NULL, infolog);
			std::cout << infolog << std::endl;
			free(infolog);
		}
	}

	// link the shader program
	unsigned int ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// check for errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
		int loglength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			char *infolog = (char*)malloc(loglength);
			glGetProgramInfoLog(ID, loglength, NULL, infolog);
			std::cout << infolog << std::endl;
			free(infolog);
		}
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return ID;
}
//use/activate the shader
void Shader::use()
{
	glUseProgram(ID);
}

// utility uniform functions
void setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
