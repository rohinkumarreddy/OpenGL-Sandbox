#version 430

/* Constants */
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

/* Class definitions */

/* Simple light source */
struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

/* Direction light source */
struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

/* Point based light source */
struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

/* Spot light|Tappered point light source */
struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

/* Object surface material */
struct Material
{
	float specularIntensity;
	float shininess;
};

/* Vars passed from vertex shader */
in vec3 p_normal;				//vertex normal|no translation only rotation|world space 
in vec3 p_pos;					//vertex position|world space
in vec2 p_texCoords;			//texture coordinates

/* Vars passed to next shader */
out vec4 color;

/* Uniforms */
//uniform vec4 u_ambientLight;	//ambient light|brightness
//uniform vec4 u_diffusedLight;	//diffused light|brightness
//uniform vec3 u_lightPos;		//light source postion|world space
uniform vec3 u_eyePos;			//eye postion|world space
//uniform vec4 u_lAttenuationFac; //light attenuation fators|(kC,kL,kQ,cuttoff)

uniform int u_pointLightCount;	//number of active point lights
uniform int u_spotLightCount;	//number of active spot lights

uniform DirectionalLight u_directionalLight;			//directional light|sky-light
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];		//collection of point lights
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];		//collection of spot lights

uniform sampler2D texSample;	//texture sampler
uniform Material u_material;	//Material description

/* function definitions */

/* Light source attenuation calculation */
//float LightAttenuation(float lightDist)
//{
//	lightDist = max(lightDist, 0);
//
//	/* light attenuation factors */
//	float kc = u_lAttenuationFac.x;		//constant attenuation factor
//	float kl = u_lAttenuationFac.y;		//linear attenuation factor
//	float kq = u_lAttenuationFac.z;		//quadratic attenuation factor
//	float cutoff = u_lAttenuationFac.a; //distance from source at which light attenuation is max
//	
//	float linearComponent = kl*lightDist;
//	float quadraticComponent = kq*pow(lightDist,2);
//	float attenuation = 1.0/(kc+linearComponent+quadraticComponent);
//
//	/* Scale or bias attenuation */
//	// attenuation = 0 at extent of max influence
//	// attenuation = 1 at d = 0
//	attenuation = (attenuation - cutoff)/(1-cutoff);
//	attenuation = clamp(attenuation, 0, 1);
//	
//	return attenuation;
//}

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(p_normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(u_eyePos - p_pos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(p_normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_material.shininess);
			specularColor = vec4(light.color *
								  u_material.specularIntensity *
								  specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(u_directionalLight.base,
								u_directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = p_pos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 color = CalcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(p_pos - sLight.base.position);
	float slFactor = dot(rayDirection, normalize(sLight.direction));
	
	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);
		
		return color * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		
	} else {
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_pointLightCount; i++)
	{		
		totalColor += CalcPointLight(u_pointLights[i]);
	}
	
	return totalColor;
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_spotLightCount; i++)
	{		
		totalColor += CalcSpotLight(u_spotLights[i]);
	}
	
	return totalColor;
}

void main()
{
	/* Diffused light calculation */
//	vec3 lVec = u_lightPos - p_pos;
//	float lightDist = length(lVec);
//	vec3 lightVec = normalize(lVec);//normalized light vector|vertex to light source|world
//	float brightness = dot(lightVec, normalize(p_normal));//light component along normal
//	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);//color from diffused light
//	diffuseLight = clamp(u_diffusedLight*diffuseLight, 0, 1);
//	/* Specular light calculation */
//	vec3 refLightVec = reflect(-lightVec, p_normal);//reflect reverse light vector by normal|world space
//	vec3 eyeVec = normalize(u_eyePos - p_pos);//eye vector from eye to vertex position|world space
//	float specularity = clamp(dot(refLightVec, eyeVec), 0, 1);//relected light vector component along eye vector|world space
//	specularity = pow(specularity, 50);//specular exponent
//	vec4 specularLight = vec4(specularity, specularity, specularity, 1);//color from specular light
//	specularLight = clamp(specularLight, 0, 1);
//	
//	/* light attenuation */
//	float lightAttenuation = LightAttenuation(lightDist);
//	vec4 illumination = lightAttenuation * (u_ambientLight +
//											diffuseLight +
//											specularLight);
//	illumination += CalcDirectionalLight();
//	vec4 illumination = CalcDirectionalLight() + CalcPointLights();
	vec4 illumination = CalcDirectionalLight() + CalcPointLights() + CalcSpotLights();
//	vec4 illumination = CalcSpotLights();
//	vec4 illumination = CalcPointLights();
	illumination = clamp(illumination, 0, 1);

	color = clamp(texture(texSample, p_texCoords) * illumination, 0, 1);
}