#version 430

/* Vars passed from vertex shader */
in vec3 p_normal;				//vertex normal|no translation only rotation 
in vec3 p_pos;					//vertex position|world space

/* Vars passed to next shader */
out vec4 color;

/* Uniforms */
uniform vec4 u_ambientLight;	//ambient light|brightness
uniform vec3 u_lightPos;		//light source postion|world space

void main()
{
	/* Diffused light calculation */
	vec3 lightVec = normalize(u_lightPos - p_pos);			//light vector|vertex to light source
	float brightness = dot(lightVec, normalize(p_normal));				//light component along normal
	vec4 diffusion_light = vec4(brightness, brightness, brightness, 1.0);	//color with light
	 color = u_ambientLight + clamp(diffusion_light, 0, 1);
	/* //Diffusion + ambient light
	color = vec4(brightness + u_ambientLight.x,
	brightness + u_ambientLight.y,
	brightness + u_ambientLight.z, 1.0);	//color with light*/

	//color = vec4(p_color, 1.0);
}