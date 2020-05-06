#version 410 core

in vec3 normal;
in vec4 fragmentPositionEyeSpace;
in vec4 fragmentPositionLightSpace;
in vec2 fragmentTextureCoordinates;
in vec4 fragPos;

uniform vec3 lightPosition;
uniform vec3 lightPosition2;
uniform vec3 viewPos;
uniform	mat3 normalMatrix;
uniform mat3 lightDirMatrix;
uniform	vec3 lightColor;
uniform	vec3 lightDir;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform int dark;
uniform	vec3 visibleFog;


vec3 o;
vec3 lightDir2 = lightDir;

float constant = 1.0f;
float pointLinear = 0.0001f;
float pointQuadratic = 0.00001f;
float pointSpecularStrenght = 0.1f;

float constant2 = 0.001f;
float pointLinear2 = 0.01f;
float pointQuadratic2 = 0.001f;
float pointSpecularStrenght2 = 10.0f;
float specularAttenuation = 0.5f;

vec3 ambient;
float ambientStrength = 0.2f;
float specularStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float shininess = 100.0f;

out vec4 fColor;

float computeShadow()
{	
    vec3 normalizedCoords = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    if(normalizedCoords.z > 1.0f)
        return 0.0f;
    normalizedCoords = normalizedCoords * 0.5f + 0.5f;
    float closestDepth = texture(shadowMap, normalizedCoords.xy).r;    
    float currentDepth = normalizedCoords.z;
    float bias = 0.0035f;
    float shadow = currentDepth - bias> closestDepth  ? 1.0f : 0.0f;

    return shadow;	
}


void handleLampLight()
{
	vec3 norm = normalize(normalMatrix * normal);
	vec3 lightDir = normalize(lightPosition2 - fragPos.xyz);

    	float diff = max(dot(norm, lightDir), 0.0);
	vec3 viewDir = normalize(viewPos - fragPos.xyz);
    	vec3 reflectDir = reflect(-lightDir, norm);
   	 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    	float distance    = length(lightPosition2 - fragPos.xyz);
   	 float attenuation = 1.0 / (constant2 + pointLinear2 * distance + 
  			     pointQuadratic2 * (distance * distance));  

	float attenuation2 = 1.0 / (constant2 + pointLinear2 * distance + 
  			     pointQuadratic2 * (distance * distance) + 
				specularAttenuation * (distance * distance));    
 
	if(lightPosition2.y >= 0){
   		 ambient  += lightColor * vec3(texture(diffuseTexture, fragmentTextureCoordinates)) * attenuation;
  		  diffuse  += lightColor * diff * vec3(texture(diffuseTexture, fragmentTextureCoordinates)) * attenuation;
   	 	specular += lightColor * spec * pointSpecularStrenght2 * attenuation2;
	}
}

void handleSunLight()
{
	vec3 norm = normalize(normalMatrix * normal);
	vec3 lightDir = normalize(lightPosition - fragPos.xyz);
    	float diff = max(dot(norm, lightDir), 0.0);
	vec3 viewDir = normalize(viewPos - fragPos.xyz);
   	 vec3 reflectDir = reflect(-lightDir, norm);
    	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   	 float distance    = length(lightPosition - fragPos.xyz);
    	float attenuation = 1.0 / (constant + pointLinear * distance + 
  			     pointQuadratic * (distance * distance));    

	if(lightPosition.y >= 0){
    		ambient  += lightColor * vec3(texture(diffuseTexture, fragmentTextureCoordinates)) * attenuation;
    		diffuse  += lightColor * diff * vec3(texture(diffuseTexture, fragmentTextureCoordinates)) * attenuation;
    		specular += lightColor * spec * pointSpecularStrenght * attenuation;
	}
}

float computeFog()
{
 	float fogDensity = 0.10f;
 	float fragmentDistance = length(fragmentPositionEyeSpace);
 	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

	 return clamp(fogFactor, 0.0f, 1.0f);
}

void main() 
{
	//ambient = vec3(0.1f, 0.1f, 0.1f);
	//diffuse = vec3(0.2f, 0.2f, 0.2f);
	//specular = vec3(0.0f, 0.0f, 0.0f);

	vec3 cameraPosEye = vec3(0.0f);
	vec3 normalEye = normalize(normalMatrix * normal);	
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	
	vec3 viewDirN = normalize(cameraPosEye - fragmentPositionEyeSpace.xyz);
	vec3 halfVector = normalize(lightDirN + viewDirN);

	ambient = ambientStrength * lightColor;
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
	handleSunLight();
	handleLampLight();

	if(dark == 0){

	float shadow;
	if(lightPosition.y >= 0){
		shadow = computeShadow();
	}else shadow = 0.0f;

	ambient *= vec3(texture(diffuseTexture, fragmentTextureCoordinates));
	diffuse *= vec3(texture(diffuseTexture, fragmentTextureCoordinates));
	specular *= vec3(texture(specularTexture, fragmentTextureCoordinates));

	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
    	vec3 colorFinal = min(color, 1.0f);
	float fogFactor = computeFog();
	vec4 fogColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);

	if (visibleFog.x == 1.0f){
		fColor = fogColor*(1-fogFactor)+ vec4(colorFinal*fogFactor, 1.0f);
	}else{
		fColor = vec4(colorFinal, 1.0f);
	}
	}
	else fColor = vec4(vec3 (texture(diffuseTexture, fragmentTextureCoordinates)), 1.0f);
}
