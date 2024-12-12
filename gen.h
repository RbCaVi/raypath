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

class number {
public:
	virtual std::string render() = 0;
};

class number_const : public number {
public:
	float value;

	number_const(float value);

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

MATERIAL(defaultmaterial, 
    defaultmaterial();
)

MATERIAL(showuvmaterial, 
    showuvmaterial();
)

MATERIAL(shownormalmaterial, 
    shownormalmaterial();
)

MATERIAL(showposmaterial, 
    showposmaterial();
)

MATERIAL(colormaterial, 
    colormaterial(vec3 *color);
)

MATERIAL(raymarchmaterial, 
    // the object you put this on is the bounding shape of the raymarched object - needs tmin and tmax instead of just t - right now, just box has this
    // mat is the material of the raymarched object
    raymarchmaterial(sdf *shape, material *mat);
)

THING(plane, 
    plane(material *mat);
)

THING(sphere, 
    sphere(material *mat, vec3 *center, number *radius);
)

THING(box, 
    box(material *mat, vec3 *corner1, vec3 *corner2);
)