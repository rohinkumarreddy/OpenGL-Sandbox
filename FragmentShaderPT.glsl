#version 430

/* Vars passed from vertex shader */
in vec3 p_color;//vertex color|interpolated

/* Vars passed to next shader */
out vec4 color;

void main()
{
	color = vec4(p_color, 1);
}