#version 130
attribute vec2 pos;

varying vec2 posout;

void main()
{
    gl_Position = vec4(pos, -1.0, 1.0);
    posout = pos;
}
