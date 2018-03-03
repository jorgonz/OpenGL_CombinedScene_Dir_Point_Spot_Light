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

uniform DirLight dirLight;

///////////////////////////////////////////////////////////////////////////////

//Fragment Data
in vec3 fragNormal;
in vec3 fragPos;
in vec2 textCoord;

//Need it for specular light
uniform vec3 camPos;

//final color of the pixel
out vec4 fragColor;

//Ambient
vec3 CalcAmbient()
{
  return dirLight.ambient * vec3(texture(material.diffuseMap,textCoord));
}

//Diffuse
vec3 CalcDiffuse(vec3 lightDir, vec3 normalizedFragNormal)
{
  float diffuseOffset = max(dot(normalizedFragNormal,lightDir),0.0f);

  return dirLight.diffuse * diffuseOffset *
  vec3(texture(material.diffuseMap,textCoord));
}

//Specular
vec3 CalcSpecular(vec3 lightDir, vec3 normalizedFragNormal)
{
  vec3 viewerDir =  normalize(camPos - fragPos);

  vec3 reflectedLight = reflect(-lightDir,normalizedFragNormal);

  float specularOffset = pow(max(dot(reflectedLight,viewerDir),0.0f),
  material.shine);

  return dirLight.specular * specularOffset *
  vec3(texture(material.specularMap,textCoord));

}

void main()
{
    //Light Dir from pixel to light source
    vec3 lightDir = normalize(-dirLight.direction);

    //Normalize the normal vec of the fragments
    vec3 normalizedFragNormal = normalize(fragNormal);

    //Calculate Phong
    vec3 ambientLight  = CalcAmbient();
    vec3 diffuseLight  = CalcDiffuse(lightDir,normalizedFragNormal);
    vec3 specularLight = CalcSpecular(lightDir,normalizedFragNormal);

    //Compute final color
    vec3 resultColor = diffuseLight + ambientLight + specularLight;

    //Apply color to each fragment
    fragColor = vec4(resultColor , 1.0);

}
