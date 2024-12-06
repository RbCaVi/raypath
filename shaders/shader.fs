#version 130

uniform float width;
uniform float height;

uniform float fps;
uniform mat3 camera;

varying vec2 pos;

float rand(vec2 v) {
    return fract(sin(dot(v ,vec2(12.9898,78.233))) * 43758.5453) - 0.5;
}

vec3 bg() {
    return vec3(vec2(0.5, 0.5) + pos * vec2(0.5, -0.5), mod(time, 1.0));
}

struct hit {
    float dist;
    vec3 color;
};

struct ray {
    vec3 pos;
    vec3 dir;
};

vec3 at(ray r, float t) {
    return r.pos + r.dir * t;
}

hit pathtrace(ray r) {
    // plane intersection
    vec3 planecenter = vec3(0.0, 0.0, 3.0);
    vec3 planev1 = vec3(1.0, 0.0, 0.0);
    vec3 planev2 = vec3(1.0, 1.0, 0.0);

    vec3 n = cross(planev1, planev2);
    vec3 normal = normalize(n);
    float D = dot(normal, planecenter);

    float denom = dot(normal, r.dir);

    if (abs(denom) < 1e-8) {
        return hit(-1, vec3(0.0));
    }

    float t = (D - dot(normal, r.pos)) / denom;

    if (t < 0) {
        return hit(-1, vec3(0.0));
    }

    vec3 intersection = at(r, t);
    vec3 w = n / dot(n,n);

    vec3 P = intersection;

    float a = dot(w, cross(P, planev2));
    float b = dot(w, cross(planev1, P));

    return hit(t, vec3(1-mod(time, 1.0), mod(a, 1.0), mod(b, 1.0)));
}

hit pathtrace2(ray r) {
    // plane intersection
    vec3 planecenter = vec3(3.0, 3.0, 3.0);
    vec3 planev1 = vec3(0.0, -1.0, 1.0);
    vec3 planev2 = vec3(1.0, 0.0, 0.0);

    vec3 n = cross(planev1, planev2);
    vec3 normal = normalize(n);
    float D = dot(normal, planecenter);

    float denom = dot(normal, r.dir);

    if (abs(denom) < 1e-8) {
        return hit(-1, vec3(0.0));
    }

    float t = (D - dot(normal, r.pos)) / denom;

    if (t < 0) {
        return hit(-1, vec3(0.0));
    }

    vec3 intersection = at(r, t);
    vec3 w = n / dot(n,n);

    vec3 P = intersection;

    float a = dot(w, cross(P, planev2));
    float b = dot(w, cross(planev1, P));

    return hit(t, vec3(1-mod(time, 1.0), mod(a, 1.0), mod(b, 1.0)));
}

hit pathtrace3(ray r) {
    vec3 center = vec3(0.0, 0.0, 2.0);
    float radius = 0.5;
    
    vec3 oc = center - r.pos;
    auto a = dot(r.dir, r.dir);
    auto h = dot(r.dir, oc);
    auto c = dot(oc, oc) - radius*radius;
    auto d = h*h - a*c;

    if (d < 0) {
        return hit(-1.0, vec3(0.0));
    }

    float ainv = 1.0 / a;
    float sqrtd_a = sqrt(d) * ainv;
    float h_a = h * ainv;

    if (h_a < -sqrtd_a) {
        // neither the h - sqrtd nor the h + sqrtd solutions will work
        return hit(-1.0, vec3(0.0));
    }

    float t;
    if (h_a < sqrtd_a) {
        // the "close" solution is behind the ray origin
        t = h_a + sqrtd_a;
    } else {
        t = h_a - sqrtd_a;
    }

    return hit(t, ((center - at(r, t)) / radius) * 0.5 + 0.5);
}

vec3 get_color(hit p) {
    if (p.dist < 0.0) {
        return bg();
    }
    return p.color;
}

hit merge_hits(hit h1, hit h2) {
    if (h1.dist < 0) {
        return h2;
    }
    if (h2.dist < 0) {
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

void main()
{
    if (pos.x > 0.9) {
        float top = map(fps, 0.0, 120.0, -1.0, 1.0);
        if (pos.y < top) {
            if (pos.y < -0.5) {
                gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            } else if (pos.y < 0.0) {
                gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
            } else {
                gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
            }
            return;
        }
    }
    float pi = 3.14159;
    vec3 campos = vec3(0.0, 0.0, 0.0);

    vec3 color = vec3(0.0);

    int samples = 10;
    float invsamples = 1.0 / samples;
    for(int i = 0; i < samples; i++) {
        vec2 tpos = pos + vec2(rand(pos + vec2(time, i)), rand(pos + vec2(time + 1, i * 2))) / vec2(width, height) * 4; // * 2 makes less blurring but maybe more aliasing? (not sure)
        vec3 raydir = vec3(tpos, 1.0) * camera;
        ray r = ray(campos, raydir);
        color += get_color(merge_hits(pathtrace(r), merge_hits(pathtrace2(r), pathtrace3(r))));
    }

    gl_FragColor = vec4(color * invsamples, 1.0);
}
