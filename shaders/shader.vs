#version 130
attribute vec2 posin;

varying vec2 pos;

void main()
{
    gl_Position = vec4(posin, -1.0, 1.0);
    pos = posin;
}
