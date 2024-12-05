#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>

unsigned int compileShader(const char* vertexSource, const char* fragmentSource);

// utility uniform functions
void setBool(unsigned int shader, const std::string& name, bool value);
void setInt(unsigned int shader, const std::string& name, int value);
void setFloat(unsigned int shader, const std::string& name, float value);
void setVec2(unsigned int shader, const std::string& name, const glm::vec2& value);
void setVec2(unsigned int shader, const std::string& name, float x, float y);
void setVec3(unsigned int shader, const std::string& name, const glm::vec3& value);
void setVec3(unsigned int shader, const std::string& name, float x, float y, float z);
void setVec4(unsigned int shader, const std::string& name, const glm::vec4& value);
void setVec4(unsigned int shader, const std::string& name, float x, float y, float z, float w);
void setMat2(unsigned int shader, const std::string& name, const glm::mat2& mat);
void setMat3(unsigned int shader, const std::string& name, const glm::mat3& mat);
void setMat4(unsigned int shader, const std::string& name, const glm::mat4& mat);

#endif // !SHADER_H
