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

class mask {
public:
	virtual std::string render() = 0;
};

class sdf {
public:
	virtual std::string render() = 0;
};

class material {
public:
	virtual std::string render() = 0;
};

class thing {
public:
	material *mat;
	int id;

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
    // black
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
    // the object you put this on is the bounding shape of the raymarched object - needs tmin and tmax instead of just t - right now, only box has this
    // mat is the material of the raymarched object
    raymarchmaterial(sdf *shape, material *mat);
)

MATERIAL(passthroughmaterial, 
    // lets a ray pass through it - by returning a hit with reflection
    // "only" useful with maskmaterial
    passthroughmaterial();
)

MATERIAL(missmaterial, 
    // lets a ray pass through it - by returning a miss - no seeing the inside
    // "only" useful with maskmaterial
    missmaterial();
)

MATERIAL(maskmaterial, 
    // return m1 if the hit is inside the mask (or various other boolean conditions)
    maskmaterial(mask *shape, material *m1, material *m2);
)

THING(plane, 
    plane(material *mat, vec3 *center, vec3 *u, vec3 *v);
)

THING(sphere, 
    vec3 *center;
    number *radius;

    sphere(material *mat, vec3 *center, number *radius);
)

THING(box, 
    vec3 *corner1;
    vec3 *corner2;

    box(material *mat, vec3 *corner1, vec3 *corner2);
)
