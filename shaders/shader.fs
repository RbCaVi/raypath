#version 130
uniform float time;

uniform float width;
uniform float height;

varying vec2 pos;

float rand(vec2 v) {
    return fract(sin(dot(v ,vec2(12.9898,78.233))) * 43758.5453) - 0.5;
}

vec3 get_color(vec3 raypos, vec3 raydir) {

    // plane intersection
    vec3 planecenter = vec3(0.0, 0.0, 3.0);
    vec3 planev1 = vec3(1.0, 0.0, 0.0);
    vec3 planev2 = vec3(0.0, 1.0, 0.0);

    vec3 n = cross(planev1, planev2);
    vec3 normal = normalize(n);
    vec3 Q = planecenter;
    float D = dot(normal, Q);

    float denom = dot(normal, raydir);

    if (abs(denom) < 1e-8) {
        return vec3(vec2(0.5, 0.5) + pos * vec2(0.5, -0.5), mod(time, 1.0));
    }

    float t = (D - dot(normal, raypos)) / denom;

    if (t < 0) {
        return vec3(vec2(0.5, 0.5) + pos * vec2(0.5, -0.5), mod(time, 1.0));
    }

    vec3 intersection = raypos + raydir * t;
    vec3 w = n / dot(n,n);

    vec3 P = intersection;

    float a = dot(w, cross(P, planev2));
    float b = dot(w, cross(planev1, P));

    return vec3(mod(a, 1.0), mod(b, 1.0), 1-mod(time, 1.0));
}

void main()
{
    float pi = 3.14159;
    vec3 raypos = vec3(0.0, 0.0, 0.0);

    vec3 color = vec3(0.0);

    int samples = 5;
    float invsamples = 1.0 / samples;
    for(int i = 0; i < samples; i++) {
        vec2 tpos = pos + vec2(rand(pos + vec2(time, i)), rand(pos + vec2(time + 1, i * 2))) / vec2(width, height) * 2;
        vec3 raydir = vec3(tpos, 1.0) * mat3(
            cos(time), sin(time), 0.0,
            -sin(time), cos(time), 0.0,
            0.0, 0.0, 1.0
        ) * mat3(
            cos(pi/3), 0.0, sin(pi/3),
            0.0, 1.0, 0.0,
            -sin(pi/3), 0.0, cos(pi/3)
        );
        color += get_color(raypos, raydir);
    }

    gl_FragColor = vec4(color * invsamples, 0.5);
}
