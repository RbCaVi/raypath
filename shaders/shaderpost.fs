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