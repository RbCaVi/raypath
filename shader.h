#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader
{
public:
	//the programs ID
	unsigned int ID;

	//constructer to build & read the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	//use/activate the shader
	void use();

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	
	bool getBool(const std::string& name);
	int getInt(const std::string& name);
	float getFloat(const std::string& name);
	glm::vec2 getVec2(const std::string& name);
	glm::vec3 getVec3(const std::string& name);
	glm::vec4 getVec4(const std::string& name);
	glm::mat2 getMat2(const std::string& name);
	glm::mat3 getMat3(const std::string& name);
	glm::mat4 getMat4(const std::string& name);
};

#endif // !SHADER_H
