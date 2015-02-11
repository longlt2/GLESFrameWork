#version 300 es

uniform sampler2D uSampler;

//Texture coordinate
in vec2 texCoord;

//Final color
out vec4 gl_FragColor;

void main()
{
    //Set fragment
    gl_FragColor = texture2D( uSampler, texCoord );
}
