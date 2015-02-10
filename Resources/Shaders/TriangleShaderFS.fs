#version 300 es
precision mediump float;

out vec4 oColor;
uniform vec4 unifColor;

void main()
{
	oColor = unifColor;
}
