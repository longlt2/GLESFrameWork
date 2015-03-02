#version 300 es

uniform sampler2D uiSampler;

in vec2 ov2TexCoord;

//Texture coordinate
// in vec4 ov4Color;

//Final color
out vec4 gl_FragColor;

void main()
{
    //Set fragment
    gl_FragColor = texture2D( uiSampler, ov2TexCoord );
    // gl_FragColor = v4Color;
}
