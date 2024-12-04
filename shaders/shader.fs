#version 130

varying vec2 posout;

void main()
{
    gl_FragColor = vec4(posout, 1.0, 1.0);
}
