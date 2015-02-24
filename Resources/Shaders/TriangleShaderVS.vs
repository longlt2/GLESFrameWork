#version 300 es

layout(location = 0) in vec2 a_posL;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    gl_Position = uModel * uView * uProj * vec4(a_posL, 1.0, 1.0);
    // gl_Position = posL;
}
