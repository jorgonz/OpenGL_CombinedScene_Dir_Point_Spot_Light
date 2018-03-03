//FRAGMENT SHADER

#version 330 core

//The amount of color this object reflects under each type of light
struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shine;
};

uniform Material material;

//Directional Light
struct DirLight
{
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

};

#define NR_DIR_LIGHTS 100
uniform DirLight dirLights[NR_DIR_LIGHTS];

//Pointlight
struct PointLight
{
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

#define NR_POINT_LIGHTS 100
uniform PointLight pLights[NR_POINT_LIGHTS];

//SpotLight
struct SpotLight
{
  vec3 position;
  vec3 direction;

  float innerCosAngle;
  float outerCosAngle;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

};

#define NR_SPOT_LIGHTS 100
uniform SpotLight spLights[NR_SPOT_LIGHTS];
////////////////////////////////////////////////////////////////////////////////

//Fragment Data
in vec3 fragNormal;
in vec3 fragPos;
in vec2 textCoord;

//Need it for specular light
uniform vec3 camPos;

//final color of the pixel
out vec4 fragColor;

//Number of Lights per Array
uniform int dirLightSize;
uniform int pLightSize;
uniform int spLightSize;

////////////////////////////////////////////////////////////////////////////////

//Ambient
vec3 CalcAmbient(vec3 ambient)
{
  return ambient * vec3(texture(material.diffuseMap,textCoord));
}

//Diffuse
vec3 CalcDiffuse(vec3 diffuse, vec3 lightDir, vec3 normalizedFragNormal)
{
  float diffuseOffset = max(dot(normalizedFragNormal,lightDir),0.0f);

  return diffuse * diffuseOffset * vec3(texture(material.diffuseMap,textCoord));
}

//Specular
vec3 CalcSpecular(vec3 specular, vec3 lightDir, vec3 normalizedFragNormal)
{
  vec3 viewerDir =  normalize(camPos - fragPos);

  vec3 reflectedLight = reflect(-lightDir,normalizedFragNormal);

  float specularOffset = pow(max(dot(reflectedLight,viewerDir),0.0f),
  material.shine);

  return specular * specularOffset * vec3(texture(material.specularMap,
    textCoord));

}

////////////////////////////////////////////////////////////////////////////////

//Directional Light
vec3 CalcDirLight(DirLight dirLight, vec3 normalizedFragNormal)
{
  //Light Dir from pixel to light source
  vec3 lightDir = normalize(-dirLight.direction);

  //Calculate Phong
  vec3 ambientLight  = CalcAmbient(dirLight.ambient);
  vec3 diffuseLight  = CalcDiffuse(dirLight.diffuse,lightDir,
    normalizedFragNormal);
  vec3 specularLight = CalcSpecular(dirLight.specular,lightDir,
      normalizedFragNormal);

  return ambientLight + diffuseLight + specularLight;
}

//Point Light
vec3 CalcPointLight(PointLight pLight, vec3 normalizedFragNormal)
{
  //Light Dir from pixel to light source
  vec3 lightDirVec = pLight.position - fragPos;

  //Distance from point light to fragment (magnitude)
  float dist = length(lightDirVec);

  //Light Dir from pixel to light source (normalized)
  vec3 lightDir = normalize(lightDirVec);

  //Decaying factor
  float attenuation = 1.0 / (pLight.constant + pLight.linear * dist +
    pLight.quadratic * (dist * dist));

    //Calculate Phong
    vec3 ambientLight  = attenuation * CalcAmbient(pLight.ambient);
    vec3 diffuseLight  = attenuation * CalcDiffuse(pLight.diffuse,lightDir,
      normalizedFragNormal);
    vec3 specularLight = attenuation * CalcSpecular(pLight.specular,lightDir,
        normalizedFragNormal);

    return ambientLight + diffuseLight + specularLight;

}

//Spot Light
vec3 CalcSpotLight(SpotLight spLight, vec3 normalizedFragNormal)
{
  //Light Dir from pixel to light source
  vec3 lightDirVec = spLight.position - fragPos;

  //Distance from point light to fragment (magnitude)
  float dist = length(lightDirVec);

  //Light Dir from pixel to light source (normalized)
  vec3 lightDir = normalize(lightDirVec);

  //Decaying factor (distance wise)
  float attenuation = 1.0 / (spLight.constant + spLight.linear * dist +
    spLight.quadratic * (dist * dist));

  //Cos angle between the direction of the flashlight and the direction the
  // light is hitting the pixel.
  float flashLightCosAngle = dot(lightDir,-normalize(spLight.direction));

  //Decaying factor (angle wise)
  float epsilon = spLight.innerCosAngle - spLight.outerCosAngle;

  float intensity = clamp((flashLightCosAngle - spLight.outerCosAngle) / epsilon
  , 0.0, 1.0);

  //Calculate Phong
  vec3 ambientLight  = attenuation * CalcAmbient(spLight.ambient);
  vec3 diffuseLight  = attenuation * intensity * CalcDiffuse(spLight.diffuse,
    lightDir,normalizedFragNormal);
  vec3 specularLight = attenuation * intensity * CalcSpecular(spLight.specular,
    lightDir,normalizedFragNormal);

  return ambientLight + diffuseLight + specularLight;

}

////////////////////////////////////////////////////////////////////////////////

void main()
{

  //Normal of the fragment (normalized)
  vec3 normalizedFragNormal = normalize(fragNormal);

  //Calculate the color of the fragment
  vec3 resultColor = vec3(0.0f, 0.0f, 0.0f);

  for(int i=0; i<dirLightSize; i++)
  {
    resultColor += CalcDirLight(dirLights[i],normalizedFragNormal);
  }

  for(int i=0; i<pLightSize; i++)
  {
    resultColor += CalcPointLight(pLights[i],normalizedFragNormal);
  }

  for(int i=0; i<spLightSize; i++)
  {
    resultColor += CalcSpotLight(spLights[i],normalizedFragNormal);
  }

  //Apply color to each fragment
  fragColor = vec4(resultColor, 1.0);

}
