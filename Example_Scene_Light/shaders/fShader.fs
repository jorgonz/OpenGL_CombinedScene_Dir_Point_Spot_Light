//FRAGMENT SHADER
#version 330 core

//MATERIAL STRUCT

//The amount of color this object reflects under each type of light
struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shine;
};

uniform Material material;
////////////////////////////////////////////////////////////////////////////////

//LIGHT STRUCT

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

out vec4 fragColor;

uniform vec3 lightPos;

uniform vec3 camPos;

in vec3 fragNormal;
in vec3 fragPos;
in vec2 textCoord;

void main()
{
    //DIFUSE//

    //Calculate Diffuse Light
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 normalizedFragNormal = normalize(fragNormal);

    //Value varies form 0 to 1
    float diffuseOffset = max(dot(normalizedFragNormal,lightDir),0.0f);

    vec3 diffuseLight = diffuseOffset * light.diffuse * vec3(texture(material.diffuseMap,textCoord));

    ////////////////////////////////////////////////////////////////////////////

    //AMBIENT//

    //Calculate Ambient Light
    vec3 ambientLight = light.ambient * vec3(texture(material.diffuseMap,textCoord));

    ////////////////////////////////////////////////////////////////////////////

    //SPECULAR//

    //Calculate a vector that extends from the fragment towards the viewer
    vec3 viewDir = normalize(camPos - fragPos);

    //Calculate the vector of reflected light, from the fragment to the
    //viewer
    vec3 reflectLightDirection = reflect(-lightDir,normalizedFragNormal);

    //Calculate the impact of reflected light
    float specularOffset = pow(max(dot(reflectLightDirection,viewDir),0.0f),
    material.shine);

    //Calculate specularLight
    vec3 specularLight = specularOffset * light.specular *
    vec3(texture(material.specularMap,textCoord));

    ////////////////////////////////////////////////////////////////////////////

    //Compute final color
    vec3 resultColor = diffuseLight + ambientLight + specularLight;

    //Apply color to each fragment
    fragColor = vec4(resultColor , 1.0);
}
