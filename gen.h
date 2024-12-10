#include <string>

class vec3 {
public:
	virtual std::string render() = 0;
};

class vec3_const : public vec3 {
public:
	float x, y, z;

	vec3_const(float x, float y, float z);

	std::string render() override;
};

class material {
public:
	virtual std::string render() = 0;
};

class defaultmaterial : public material {
public:
	std::string render() override;
};

class thing {
public:
	material *mat;

	thing(material *mat);

	virtual std::string render() = 0;
};

class plane : public thing {
public:
	plane(material *mat);

	std::string render() override;
};