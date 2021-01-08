#version 430

/* Attributes */
in layout(location=0) vec4 a_position;//vertex position|model space
in layout(location=1) vec3 a_color;	//vertex color

/* Uniforms */	
uniform mat4 u_MVPMtx;//full transform matrix|model to projection

/* Vars passed to fragment shader */
out vec3 p_color;//color passed to next shader

void main()
{
	gl_Position = u_MVPMtx * a_position;//model to projection|projection
	p_color = a_color;//attribute color passed to next shader
}