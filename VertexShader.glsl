#version 430

/* Attributes */
in layout(location=0) vec3 a_position;	//vertex position|model space
in layout(location=1) vec3 a_color;		//vertex color
in layout(location=2) vec3 a_normal;	//surface normals|model space

/* Uniforms */	
uniform mat4 u_MVPMtx;			//full transform matrix|model to projection
uniform mat4 u_MWMtx;			//model to world matrix

/* Vars passed to fragment shader */
out vec3 p_normal;				//vertex normal for light calculations
out vec3 p_pos;					//vertex position for light calculations|world space

void main()
{
	vec4 pos = vec4(a_position, 1.0);
	gl_Position = u_MVPMtx * pos;					//model to projection|projection
	p_normal = vec3(u_MWMtx * vec4(a_normal, 0));	//normals with only rotation|world space
	p_pos = vec3(u_MWMtx * pos);					//model to world|truncate to vec3
}