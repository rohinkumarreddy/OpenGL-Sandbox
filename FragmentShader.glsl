#version 430

/* Vars passed from vertex shader */
in vec3 p_normal;				//vertex normal|no translation only rotation|world space 
in vec3 p_pos;					//vertex position|world space

/* Vars passed to next shader */
out vec4 color;

/* Uniforms */
uniform vec4 u_ambientLight;	//ambient light|brightness
uniform vec3 u_lightPos;		//light source postion|world space
uniform vec3 u_eyePos;			//eye postion|world space

void main()
{
	/* Diffused light calculation */
	vec3 lightVec = normalize(u_lightPos - p_pos);//light vector|vertex to light source|world
	float brightness = dot(lightVec, normalize(p_normal));//light component along normal
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);//color from diffused light

	/* Specular light calculation */
	vec3 refLightVec = reflect(-lightVec, p_normal);//reflect reverse light vector by normal|world space
	vec3 eyeVec = normalize(u_eyePos - p_pos);//eye vector from eye to vertex position|world space
	float specularity = dot(refLightVec, eyeVec);//relected light vector component along eye vector|world space
	specularity = pow(specularity, 50);//specular exponent
	vec4 specularLight = vec4(0, 0, specularity, 1.0);//color from specular light

	color = u_ambientLight + clamp(diffuseLight, 0, 1) + clamp(specularLight, 0, 1);
}

/* //Diffusion + ambient light
color = vec4(brightness + u_ambientLight.x,
brightness + u_ambientLight.y,
brightness + u_ambientLight.z, 1.0);	//color with light*/

//color = vec4(p_color, 1.0);