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

class thing {
public:
	material *mat;

	thing(material *mat);

	virtual std::string render() = 0;
};

#define MATERIAL(name, constructors) \
class name : public material { \
public: \
    constructors \
	std::string render() override; \
};

#define THING(name, constructors) \
class name : public thing { \
public: \
    constructors \
	std::string render() override; \
};

MATERIAL(defaultnaterial, 
    defaultmaterial();
)

THING(plane, 
    plane(material *mat);
)