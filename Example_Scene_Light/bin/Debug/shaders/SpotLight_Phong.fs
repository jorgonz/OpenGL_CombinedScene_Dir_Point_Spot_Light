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

uniform SpotLight spLight;

///////////////////////////////////////////////////////////////////////////////

//Fragment Data
in vec3 fragNormal;
in vec3 fragPos;
in vec2 textCoord;

//Need it for specular light
uniform vec3 camPos;

//final color of the pixel
out vec4 fragColor;

////////////////////////////////////////////////////////////////////////////////

//Ambient
vec3 CalcAmbient()
{
  return spLight.ambient * vec3(texture(material.diffuseMap,textCoord));
}

//Diffuse
vec3 CalcDiffuse(vec3 lightDir, vec3 normalizedFragNormal)
{
  float diffuseOffset = max(dot(normalizedFragNormal,lightDir),0.0f);

  return spLight.diffuse * diffuseOffset *
  vec3(texture(material.diffuseMap,textCoord));
}

//Specular
vec3 CalcSpecular(vec3 lightDir, vec3 normalizedFragNormal)
{
  vec3 viewerDir =  normalize(camPos - fragPos);

  vec3 reflectedLight = reflect(-lightDir,normalizedFragNormal);

  float specularOffset = pow(max(dot(reflectedLight,viewerDir),0.0f),
  material.shine);

  return spLight.specular * specularOffset *
  vec3(texture(material.specularMap,textCoord));

}

////////////////////////////////////////////////////////////////////////////////

void main()
{
    //Light Dir from pixel to light source
    vec3 LightDirection = spLight.position - fragPos;

    //Distance from point light to fragment (magnitude)
    float dist = length(LightDirection);

    //Calc attenuation from point lightDir
    float attenuation = 1.0 / (spLight.constant + spLight.linear * dist +
      spLight.quadratic * (dist * dist));

    //Light direction from the pixel to Light source (normalized)
    vec3 lightDir = normalize(LightDirection);

    float flashLightCosAngle = dot(lightDir,-normalize(spLight.direction));

    float epsilon = spLight.innerCosAngle - spLight.outerCosAngle;

    float intensity = clamp((flashLightCosAngle - spLight.outerCosAngle)/epsilon
    , 0.0, 1.0);

    //Normalize the normal vec of the fragments
    vec3 normalizedFragNormal = normalize(fragNormal);

    //Calculate Phong
    vec3 ambientLight  = attenuation * CalcAmbient();
    vec3 diffuseLight  = attenuation * intensity * CalcDiffuse(lightDir,
      normalizedFragNormal);
    vec3 specularLight = attenuation * intensity * CalcSpecular(lightDir,
      normalizedFragNormal);

    //Compute final color
    vec3 resultColor = diffuseLight + ambientLight + specularLight;

    //Apply color to each fragment
    fragColor = vec4(resultColor , 1.0);

}
