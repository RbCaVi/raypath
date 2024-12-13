#include "gen.h"

#include <sstream>

static int counter = 0;

static int id() {
	return counter++;
}

vec3_const::vec3_const(float x, float y, float z) : x(x), y(y), z(z) {}

std::string vec3_const::render() {
	std::stringstream stream;

	stream << "vec3(" << x << ", " << y << ", " << z << ")";

	return stream.str();
}

thing::thing(material *mat) : mat(mat) {}

std::string defaultmaterial::render() {
	std::stringstream stream;

	return stream.str();
}

plane::plane(material *mat) : thing(mat) {}

std::string plane::render() {
	vec3 *center = new vec3_const(0.0, 0.0, 3.0);
	vec3 *u = new vec3_const(1.0, 0.0, 0.0);
	vec3 *v = new vec3_const(1.0, 1.0, 0.0);

	std::stringstream stream;

	stream
		<< "hit pathtrace" << id() << "(ray r) {\n"
		<< "    // plane intersection\n"
		<< "    vec3 planecenter = " << center->render() << ";\n"
		<< "    vec3 planev1 = " << u->render() << ";\n"
		<< "    vec3 planev2 = " << v->render() << ";\n"
		<< "\n"
		<< "    vec3 n = cross(planev1, planev2);\n"
		<< "    vec3 normal = normalize(n);\n"
		<< "    float D = dot(normal, planecenter);\n"
		<< "\n"
		<< "    float denom = dot(normal, r.dir);\n"

		<< "    if (abs(denom) < 1e-8) {\n"
		<< "        return miss();\n"
		<< "    }\n"
		<< "\n"
		<< "    float t = (D - dot(normal, r.pos)) / denom;\n"
		<< "\n"
		<< "    if (t < 0) {\n"
		<< "        return miss();\n"
		<< "    }\n"
		<< "\n"
		<< "    vec3 intersection = at(r, t);\n"
		<< "    vec3 w = n / dot(n,n);\n"
		<< "\n"
		<< "    vec3 P = intersection;\n"
		<< "\n"
		<< "    float a = dot(w, cross(P, planev2));\n"
		<< "    float b = dot(w, cross(planev1, P));\n"
		<< "\n"
		<< "    return noreflect(t, vec3(1-mod(time, 1.0), mod(a, 1.0), mod(b, 1.0)));\n"
		<< "}\n";

	return stream.str();
}

sphere::sphere(material *mat, vec3 *center, number *radius) : thing(mat), center(center), radius(radius) {}

box::box(material *mat, vec3 *corner1, vec3 *corner2) : thing(mat), corner1(corner1), corner2(corner2) {}
