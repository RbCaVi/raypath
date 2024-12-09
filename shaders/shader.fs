#version 130

uniform float width;
uniform float height;

uniform float fps;
uniform mat3 camera;
uniform vec3 campos;
uniform float time;

uniform float fov;

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
        return miss();
    }

    float t = (D - dot(normal, r.pos)) / denom;

    if (t < 0) {
        return miss();
    }

    vec3 intersection = at(r, t);
    vec3 w = n / dot(n,n);

    vec3 P = intersection;

    float a = dot(w, cross(P, planev2));
    float b = dot(w, cross(planev1, P));

    return noreflect(t, vec3(1-mod(time, 1.0), mod(a, 1.0), mod(b, 1.0)));
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
        return miss();
    }

    float t = (D - dot(normal, r.pos)) / denom;

    if (t < 0) {
        return miss();
    }

    vec3 intersection = at(r, t);
    vec3 w = n / dot(n,n);

    vec3 P = intersection;

    float a = dot(w, cross(P, planev2));
    float b = dot(w, cross(planev1, P));

    return noreflect(t, vec3(1-mod(time, 1.0), mod(a, 1.0), mod(b, 1.0)));
}

hit pathtrace3(ray r) {
    vec3 center = vec3(3.0, 0.0, 1.0);
    float radius = 0.5;
    
    vec3 oc = center - r.pos;
    float a = dot(r.dir, r.dir);
    float h = dot(r.dir, oc);
    float c = dot(oc, oc) - radius*radius;
    float d = h*h - a*c;

    if (d < 0) {
        return miss();
    }

    float ainv = 1.0 / a;
    float sqrtd_a = sqrt(d) * ainv;
    float h_a = h * ainv;

    if (h_a < -sqrtd_a) {
        // neither the h - sqrtd nor the h + sqrtd solutions will work
        return miss();
    }

    float t;
    if (h_a < sqrtd_a) {
        // the "close" solution is behind the ray origin
        t = h_a + sqrtd_a;
    } else {
        t = h_a - sqrtd_a;
    }

    return noreflect(t, ((center - at(r, t)) / radius) * 0.5 + 0.5);
}

hit pathtrace4(ray r) {
    vec3 center = vec3(2.0, 0.0, 2.8);
    float radius = 0.5;
    
    vec3 oc = center - r.pos;
    float a = dot(r.dir, r.dir);
    float h = dot(r.dir, oc);
    float c = dot(oc, oc) - radius*radius;
    float d = h*h - a*c;

    if (d < 0) {
        return miss();
    }

    float ainv = 1.0 / a;
    float sqrtd_a = sqrt(d) * ainv;
    float h_a = h * ainv;

    if (h_a < -sqrtd_a) {
        // neither the h - sqrtd nor the h + sqrtd solutions will work
        return miss();
    }

    float t;
    if (h_a < sqrtd_a) {
        // the "close" solution is behind the ray origin
        t = h_a + sqrtd_a;
    } else {
        t = h_a - sqrtd_a;
    }

    vec3 normal = (center - at(r, t)) / radius;

    vec3 ref = r.dir - 2 * dot(r.dir, normal) * normal;

    return hit(t, vec3(0.2), vec3(0.8), ref);
}

hit pathtrace5(ray r) {
    vec3 bmin = vec3(-2.0, -2.0, -2.0);
    vec3 bmax = vec3(2.0, -1.5, -1.5);

    vec3 adinv = 1.0 / r.dir;

    vec3 t0 = (bmin - r.pos) * adinv;
    vec3 t1 = (bmax - r.pos) * adinv;

    float tmin = max(
        max(
            min(t0.x, t1.x),
            min(t0.y, t1.y)
        ),
        min(t0.z, t1.z)
    );

    float tmax = min(
        min(
            max(t0.x, t1.x),
            max(t0.y, t1.y)
        ),
        max(t0.z, t1.z)
    );

    if (tmax <= tmin) {
        return miss();
    }

    if (tmin > 0) {
        return noreflect(tmin, vec3(0.0, 1.0, 0.0));
    }

    if (tmax > 0) {
        return noreflect(tmax, vec3(0.0, 1.0, 0.0));
    }
    
    return miss();
}

ray march(ray r, float dist) {
    return ray(at(r, dist), r.dir);
}

float sdf(vec3 pos) {
    vec3 r = pos - vec3(0.0, 0.0, 0.0);
    return sqrt(dot(r, r)) - 1.0;
}

hit raymarch(ray r, float mindist, float maxdist) {
    float totaldist = mindist;
    float ilen = 1 / sqrt(dot(r.dir, r.dir));
    r = march(r, mindist);
    r.dir = r.dir * ilen;
    for (int i = 0; i < 100; i++) {
        float dist = sdf(r.pos);
        if (dist > 10000) {
            return miss();
        }
        if (dist < 0.01) {
            return noreflect(totaldist, abs(r.pos));
        }
        totaldist += dist * ilen;
        if (totaldist > maxdist) {
            return miss();
        }
        r = march(r, dist);
    }
    return miss();
}

hit pathtrace6(ray r) {
    vec3 bmin = vec3(-1.5, -1.5, -1.5);
    vec3 bmax = vec3(1.5, 1.5, 1.5);

    vec3 adinv = 1.0 / r.dir;

    vec3 t0 = (bmin - r.pos) * adinv;
    vec3 t1 = (bmax - r.pos) * adinv;

    float tmin = max(
        max(
            min(t0.x, t1.x),
            min(t0.y, t1.y)
        ),
        min(t0.z, t1.z)
    );

    float tmax = min(
        min(
            max(t0.x, t1.x),
            max(t0.y, t1.y)
        ),
        max(t0.z, t1.z)
    );

    if (tmax <= tmin) {
        return miss();
    }
    
    if (rand(vec2(time, tmin)) > -0.3) {
        return raymarch(r, max(tmin, 0), tmax);
    } else {
        return noreflect(tmax, vec3(0.0, 0.0, 1.0));
    }
}

float sdf2(vec3 pos) {
    vec3 r = mod(pos, 0.5) - vec3(0.25, 0.25, 0.25);
    return sqrt(dot(r, r)) - 0.1;
}

hit raymarch2(ray r, float mindist, float maxdist) {
    float totaldist = mindist;
    float ilen = 1 / sqrt(dot(r.dir, r.dir));
    r = march(r, mindist);
    r.dir = r.dir * ilen;
    for (int i = 0; i < 100; i++) {
        float dist = sdf2(r.pos);
        if (dist > 10000) {
            return miss();
        }
        if (dist < 0.01) {
            return noreflect(totaldist, abs(r.pos));
        }
        totaldist += dist * ilen;
        if (totaldist > maxdist) {
            return miss();
        }
        r = march(r, dist);
    }
    return miss();
}

hit pathtrace7(ray r) {
    vec3 bmin = vec3(-1.5, -1.5, -10.0 - 1.5);
    vec3 bmax = vec3(1.5, 1.5, -10.0 + 1.5);

    vec3 adinv = 1.0 / r.dir;

    vec3 t0 = (bmin - r.pos) * adinv;
    vec3 t1 = (bmax - r.pos) * adinv;

    float tmin = max(
        max(
            min(t0.x, t1.x),
            min(t0.y, t1.y)
        ),
        min(t0.z, t1.z)
    );

    float tmax = min(
        min(
            max(t0.x, t1.x),
            max(t0.y, t1.y)
        ),
        max(t0.z, t1.z)
    );

    if (tmax <= tmin) {
        return miss();
    }
    
    if (rand(vec2(time, tmin)) > -0.3) {
        return raymarch2(r, max(tmin, 0), tmax);
    } else {
        return noreflect(tmax, vec3(1.0, 1.0, 1.0));
    }
}

vec3 castray(ray r) {
    vec3 color = vec3(0.0);
    vec3 factor = vec3(1.0);
    for (int i = 0; i < 10; i++) {
        hit h = merge_hits(
            merge_hits(
                merge_hits(
                    pathtrace(r),
                    pathtrace2(r)
                ),
                merge_hits(
                    pathtrace3(r),
                    pathtrace4(r)
                )
            ),
            merge_hits(
                merge_hits(
                    pathtrace5(r),
                    pathtrace6(r)
                ),
                pathtrace7(r)
            )
        );
        if (false) {
            color = vec3(mod(h.dist * sqrt(dot(r.dir, r.dir)), 0.2) * 5);
            break;
        } else {
            color += get_color(h) * factor;
        }
        factor *= h.reflected;
        r = ray(at(r, h.dist - 0.01), h.dir);
        if (dot(factor, factor) < 0.1) {
            break;
        }
    }
    return color;
}

void main() {
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

    if (pos.x < -0.8 && pos.y > 0.8) {
        int xi = int(map(pos.x, -1.0, -0.8, 0, 3));
        int yi = int(map(pos.y, 1.0, 0.8, 0, 3));
        float value = camera[xi][yi];
        if (value > 0.0) {
            gl_FragColor = vec4(0.0, value, 0.0, 1.0);
        } else {
            gl_FragColor = vec4(-value, 0.0, 0.0, 1.0);
        }
        return;
    }

    vec3 color = vec3(0.0);

    int samples = 10;
    float invsamples = 1.0 / samples;
    for(int i = 0; i < samples; i++) {
        vec2 tpos = pos + vec2(rand(pos + vec2(time, i)), rand(pos + vec2(time + 1, i * 2))) / vec2(width, height) * 4; // * 2 makes less blurring but maybe more aliasing? (not sure)
        vec3 raydir = vec3(tpos * fov, 1.0) * camera;
        ray r = ray(campos + vec3(tpos * (1 - fov), 0.0) * camera, raydir);
        color += castray(r);
    }

    gl_FragColor = vec4(color * invsamples, 1.0);
}
