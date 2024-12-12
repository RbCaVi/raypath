#version 130

uniform float width;
uniform float height;

uniform float fps;
uniform mat3 camera;
uniform vec3 campos;
uniform float time;

uniform float fov;

uniform bool showdist;

varying vec2 pos;

float rand(vec2 v) {
    return fract(sin(dot(v ,vec2(12.9898,78.233))) * 43758.5453) - 0.5;
}

vec3 bg() {
    return vec3(vec2(0.5, 0.5) + pos * vec2(0.5, -0.5), mod(time, 1.0));
}

struct ray {
    vec3 pos;
    vec3 dir;
};

struct hit {
    float dist;
    vec3 color;
    vec3 reflected;
    vec3 dir;
};

vec3 at(ray r, float t) {
    return r.pos + r.dir * t;
}

vec3 get_color(hit p) {
    if (p.dist < 0.0) {
        return bg();
    }
    return p.color;
}

hit merge_hits(hit h1, hit h2) {
    if (h1.dist < 0.01) {
        return h2;
    }
    if (h2.dist < 0.01) {
        return h1;
    }
    if (h2.dist < h1.dist) {
        return h2;
    }
    return h1;
}

// https://gist.github.com/companje/29408948f1e8be54dd5733a74ca49bb9
float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

hit miss() {
    return hit(-1, vec3(0.0), vec3(0.0), vec3(0.0));
}

hit noreflect(float dist, vec3 color) {
    return hit(dist, color, vec3(0.0), vec3(0.0));
}

ray march(ray r, float dist) {
    return ray(at(r, dist), r.dir);
}
