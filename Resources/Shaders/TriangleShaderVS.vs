#version 300 es

in vec2 a_posL;

void main()
{
    gl_Position = vec4(a_posL, 1.0, 1.0);
    // gl_Position = posL;
}
