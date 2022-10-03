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
	float epsilon;
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
uniform vec3 u_eyePos;			//eye postion|world space

uniform int u_pointLightCount;	//number of active point lights
uniform int u_spotLightCount;	//number of active spot lights

uniform DirectionalLight u_directionalLight;			//directional light|sky-light
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];		//collection of point lights
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];		//collection of spot lights

uniform sampler2D texSample;	//texture sampler
uniform Material u_material;	//Material description

//CalcLightByDirection: Calculates light by direction at the point of illumination
//Input: light source, direction: vector from light source to point of illumination
//Output: light at the point of illumination in color
vec4 CalcLightByDirection(Light light, vec3 direction)
{
	//Ambient light
	vec4 ambientLight = vec4(light.color, 1.0f) * light.ambientIntensity;
	ambientLight = clamp(ambientLight, 0, 1);

	//vector from point of illumiation to the light source
	vec3 lightVec = -direction;
	
	//Diffuse factor is the component of the light vector
	//along the normal at the point of illumination
	float diffuseFactor = max(dot(normalize(p_normal), normalize(lightVec)), 0.0f);
	//Diffused light
	vec4 diffusedLight = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
	diffusedLight = clamp(diffusedLight, 0, 1);
	
	vec4 specularLight = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		//vector from point of illumination to the eye
		vec3 eyeVec = normalize(u_eyePos - p_pos);
		//reflect light direction by the normal at the point of illumination
		vec3 reflectedLightVec = normalize(reflect(normalize(direction), normalize(p_normal)));
		//Specular factor is the component of the reflected light vector
		//along the eye vector at the point of illumination
		float specularFactor = max(dot(eyeVec, reflectedLightVec), 0);
		{
			specularFactor = pow(specularFactor, u_material.shininess);
			specularLight = vec4(light.color *
								  u_material.specularIntensity *
								  specularFactor, 1.0f);
			specularLight = clamp(specularLight, 0, 1);
		}
	}

	return (ambientLight + diffusedLight + specularLight);
}

//CalcDirectionalLight: Calculates light at the point of illumination using
//light direction, a vector from light source to point of illumination
//Output: light at the point of illumination in color
vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(u_directionalLight.base,
								u_directionalLight.direction);
}

//CalcPointLight: Calculates light at the point of illumination using
//light direction, a vector from light source to point of illumination
//light attenuation, light propagation factor based on distance from source
//Output: light at the point of illumination in color
vec4 CalcPointLight(PointLight pLight)
{
	//vector from light source to the point of illumination
	vec3 direction = p_pos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	//calculate light based on the direction(directional light)
	vec4 color = CalcLightByDirection(pLight.base, direction);
	//calculate the attenuation based on distance from source
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (color / attenuation);
}

//CalcSpotLight: Calculates light at the point of illumination using
//light direction, a vector from light source to the focus-point of illumination
//light attenuation, light propagation factor based on distance from source
//within a given radius around the point of illumination
//Output: light at the point of illumination in color
vec4 CalcSpotLight(SpotLight sLight)
{
	//vector from light source to the point of illumination
	vec3 direction = normalize(p_pos - sLight.base.position);
	//vector from intended focus-point of illumination to the light source
	vec3 lightFocusVec = sLight.direction;
	//calculate the component of light focus vector along light direction
	float slFactor = dot(direction, normalize(lightFocusVec));
	
	//light edge is the component of the 
	//edge vector(radius limit) along
	//the focus direction
	vec4 light = CalcPointLight(sLight.base);
	float intensity = clamp((slFactor - (sLight.edge - sLight.epsilon))/sLight.epsilon, 0, 1);
	return light * intensity;
}

vec4 CalcPointLights()
{
	vec4 totalLight = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_pointLightCount; i++)
	{		
		totalLight += CalcPointLight(u_pointLights[i]);
	}
	
	return clamp(totalLight, 0, 1);
}

vec4 CalcSpotLights()
{
	vec4 totalLight = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_spotLightCount; i++)
	{		
		totalLight += CalcSpotLight(u_spotLights[i]);
	}
	
	return clamp(totalLight, 0, 1);;
}

void main()
{
	vec4 illumination = CalcDirectionalLight() + CalcPointLights() + CalcSpotLights();
	illumination = clamp(illumination, 0, 1);

	color = clamp(texture(texSample, p_texCoords) * illumination, 0, 1);
}

//	if(slFactor > sLight.edge)
//	{
//		vec4 light = CalcPointLight(sLight.base);
//		
////		light * ( 1.0f - ( (1.0f - slFactor)/(1.0f - sLight.edge) ) );
//		return clamp(light, 0, 1);
//	}
//	else
//		return vec4(0, 0, 0, 1);