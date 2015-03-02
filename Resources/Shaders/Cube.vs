#version 300 es

//Vertex position attribute
in vec3 v3Position;

//Texture coordinate attribute
in vec2 v2TexCoord;
out vec2 ov2TexCoord;

uniform mat4 um4World;

out vec4 ov4Color;

void main()
{
    //Process texCoord
    // texCoord = a_texCoord;

    //Process vertex
    gl_Position = um4World * vec4( v3Position, 1.0 );
    ov2TexCoord = v2TexCoord;
    // v4Color = vec4(1);
    // ov4Color = vec4(clamp(v3Position, 0.0, 1.0), 1.0);
}
