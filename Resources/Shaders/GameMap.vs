#version 300 es

//Vertex position attribute
in vec2 a_posL;

//Texture coordinate attribute
in vec2 a_texCoord;

uniform mat4 uniWorld;

out vec2 texCoord;

void main()
{
    //Process texCoord
    texCoord = a_texCoord;

    //Process vertex
    gl_Position = uniWorld * vec4( a_posL, 1.0, 1.0 );
}
