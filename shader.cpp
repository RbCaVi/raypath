#include "shader.h"

#include <glad/glad.h>
#include <iostream>

//constructer to build & read the shader
Shader::Shader(const char* vShaderCode, const char* fShaderCode)
{
	//compile shaders

	unsigned int vertex, fragment;
	int success;
	char *infolog;

	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//vertex compile errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n";
		int loglength;
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			infolog = (char*)malloc(loglength);
			glGetShaderInfoLog(vertex, loglength, NULL, infolog);
			std::cout << infolog;
			free(infolog);
		}
		std::cout << std::endl;
	}

	//frag shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//frag compile errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
		int loglength;
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			infolog = (char*)malloc(loglength);
			glGetShaderInfoLog(fragment, loglength, NULL, infolog);
			std::cout << infolog;
			free(infolog);
		}
		std::cout << std::endl;
	}

	//making the shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED" << std::endl;
		int loglength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			infolog = (char*)malloc(loglength);
			glGetProgramInfoLog(ID, loglength, NULL, infolog);
			std::cout << infolog << std::endl;
			free(infolog);
		}
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
//use/activate the shader
void Shader::use()
{
	glUseProgram(ID);
}

// utility uniform functions
void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool Shader::getBool(const std::string& name) 
{
	bool value;
	glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &(int&)value);
	return value;
}
int Shader::getInt(const std::string& name) 
{
	int value;
	glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &value);
	return value;
}
float Shader::getFloat(const std::string& name) 
{
	float value;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value);
	return value;
}
glm::vec2 Shader::getVec2(const std::string& name) 
{
	glm::vec2 value;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value[0]);
	return value;
}
glm::vec3 Shader::getVec3(const std::string& name) 
{
	glm::vec3 value;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value[0]);
	return value;
}
glm::vec4 Shader::getVec4(const std::string& name) 
{
	glm::vec4 value;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value[0]);
	return value;
}
glm::mat2 Shader::getMat2(const std::string& name) 
{
	glm::mat2 mat;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &mat[0][0]);
	return mat;
}
glm::mat3 Shader::getMat3(const std::string& name) 
{
	glm::mat3 mat;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &mat[0][0]);
	return mat;
}
glm::mat4 Shader::getMat4(const std::string& name) 
{
	glm::mat4 mat;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &mat[0][0]);
	return mat;
}
