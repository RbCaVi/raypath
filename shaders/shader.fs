#version 130
uniform float time;

varying vec2 pos;

void main()
{
    float pi = 3.14159;
    vec3 raypos = vec3(0.0, 0.0, 0.0);
    vec3 raydir = vec3(pos, 1.0) * mat3(
        cos(pi/3), 0.0, sin(pi/3),
        0.0, 1.0, 0.0,
        -sin(pi/3), 0.0, cos(pi/3)
    ) * mat3(
        cos(time), sin(time), 0.0,
        -sin(time), cos(time), 0.0,
        0.0, 0.0, 1.0
    );

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
        gl_FragColor = vec4(vec2(0.5, 0.5) + pos * vec2(0.5, -0.5), mod(time, 1.0), 1.0);
        return;
    }

    float t = (D - dot(normal, raypos)) / denom;

    if (t < 0) {
        gl_FragColor = vec4(vec2(0.5, 0.5) + pos * vec2(0.5, -0.5), mod(time, 1.0), 1.0);
        return;
    }

    vec3 intersection = raypos + raydir * t;
    vec3 w = n / dot(n,n);

    vec3 P = intersection;

    float a = dot(w, cross(P, planev2));
    float b = dot(w, cross(planev1, P));

    gl_FragColor = vec4(mod(a, 1.0), mod(b, 1.0), 1-mod(time, 1.0), 1.0);
}
