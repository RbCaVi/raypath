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
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);

	// check for errors
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
		int loglength;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &loglength);
		if (loglength > 0) {
			char *infolog = (char*)malloc(loglength);
			glGetProgramInfoLog(shader, loglength, NULL, infolog);
			std::cout << infolog << std::endl;
			free(infolog);
		}
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shader;
}

// utility uniform functions
void setBool(unsigned int shader, const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value);
}

void setInt(unsigned int shader, const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
}

void setFloat(unsigned int shader, const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
}

void setVec2(unsigned int shader, const std::string& name, const glm::vec2& value) {
	glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void setVec2(unsigned int shader, const std::string& name, float x, float y) {
	glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);
}

void setVec3(unsigned int shader, const std::string& name, const glm::vec3& value) {
	glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void setVec3(unsigned int shader, const std::string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(shader, name.c_str()), x, y, z);
}

void setVec4(unsigned int shader, const std::string& name, const glm::vec4& value) {
	glUniform4fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
}

void setVec4(unsigned int shader, const std::string& name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(shader, name.c_str()), x, y, z, w);
}

void setMat2(unsigned int shader, const std::string& name, const glm::mat2& mat) {
	glUniformMatrix2fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void setMat3(unsigned int shader, const std::string& name, const glm::mat3& mat) {
	glUniformMatrix3fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void setMat4(unsigned int shader, const std::string& name, const glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
