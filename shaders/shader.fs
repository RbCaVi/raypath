#version 130
uniform float time;

varying vec2 posout;

void main()
{
    gl_FragColor = vec4(posout, mod(time, 1.0), 1.0);
}
