#version 430

/* Vars passed from vertex shader */
in vec3 p_normal;				//vertex normal|no translation only rotation|world space 
in vec3 p_pos;					//vertex position|world space
in vec2 p_texCoords;			//texture coordinates

/* Vars passed to next shader */
out vec4 color;

/* Uniforms */
uniform vec4 u_ambientLight;	//ambient light|brightness
uniform vec3 u_lightPos;		//light source postion|world space
uniform vec3 u_eyePos;			//eye postion|world space
uniform vec4 u_lAttenuationFac; //light attenuation fators|(kC,kL,kQ,cuttoff)

uniform sampler2D texSample;

/* light attenuation factors */
//float kc = 0.5;					//constant attenuation factor
//float kl = 2.0;					//linear attenuation factor
//float kq = 0.2;					//quadratic attenuation factor

float LightAttenuation(float lightDist, float lightRad)
{
	lightDist = max(lightDist - lightRad, 0);

	/* Basic attenuation */
	float lRatio = lightDist/lightRad+1;//d/r

	/* light attenuation factors */
	float kc = u_lAttenuationFac.x;		//constant attenuation factor
	float kl = u_lAttenuationFac.y;		//linear attenuation factor
	float kq = u_lAttenuationFac.z;		//quadratic attenuation factor
	float cutoff = u_lAttenuationFac.a; //distance from source at which light attenuation is max
	
	float linearComponent = kl*lRatio;
	float quadraticComponent = kq*pow(lRatio,2);
	float attenuation = 1.0/(kc+linearComponent+quadraticComponent);

	/* Scale or bias attenuation */
	// attenuation = 0 at extent of max influence
	// attenuation = 1 at d = 0
	attenuation = (attenuation - cutoff)/(1-cutoff);
	attenuation = max(attenuation, 0);
	
	return attenuation;
}

void main()
{
	/* Diffused light calculation */
	vec3 lVec = u_lightPos - p_pos;
	float lightDist = length(lVec);
	vec3 lightVec = normalize(lVec);//normalized light vector|vertex to light source|world
	float brightness = dot(lightVec, normalize(p_normal));//light component along normal
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);//color from diffused light

	/* Specular light calculation */
	vec3 refLightVec = reflect(-lightVec, p_normal);//reflect reverse light vector by normal|world space
	vec3 eyeVec = normalize(u_eyePos - p_pos);//eye vector from eye to vertex position|world space
	float specularity = clamp(dot(refLightVec, eyeVec), 0, 1);//relected light vector component along eye vector|world space
	specularity = pow(specularity, 50);//specular exponent
	vec4 specularLight = vec4(specularity, specularity, specularity, 1);//color from specular light
	
//	/* light attenuation */
//	float linearComponent = kl*lightDist;
//	float quadraticComponent = kq*pow(lightDist,2);
//	float lightAttenuation = 1.0/(kc+linearComponent+quadraticComponent);
	float lightAttenuation = LightAttenuation(lightDist, 0.25);

//	color = (u_ambientLight + lightAttenuation * clamp(diffuseLight, 0, 1)) * texture(texSample, p_texCoords) +
//			 lightAttenuation * specularLight;
	color = (u_ambientLight + lightAttenuation * clamp(diffuseLight, 0, 1) +
	lightAttenuation * specularLight)* texture(texSample, p_texCoords);
}

/* //Diffusion + ambient light
color = vec4(brightness + u_ambientLight.x,
brightness + u_ambientLight.y,
brightness + u_ambientLight.z, 1.0);	//color with light*/

//color = vec4(p_color, 1.0);