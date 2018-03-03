///This program is a simple scene with Modern OpenGL to exemplify light
///There is a scene with directional light, point light and spot light.
///And a 4th scene with all lights combined, a general purpose Phong Shader.
///Author: Jorge Andres Gonzalez Borboa
///Following: https://learnopengl.com/Lighting/Multiple-lights

///You need GLFW||GLUT||freeGLUT and GLEW||GLAD to compile this example.

///Also stb_image.h to do the texture loading:
///stb_image-v2.16 - public domain image loader-http://nothings.org/stb_image.h

///As well as GLM to do all the linear algebra
///https://glm.g-truc.net/0.9.8/index.html

///Write this define before including GLEW
#define GLEW_STATIC

///Library for loading files, (jgp, jpeg, png, etc)
#define STB_IMAGE_IMPLEMENTATION

/// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <stdexcept>

///Use this if the header file is in the include folder
#include <stb_image.h>
///Use this if the header file is in the project
//#include "stb_image.h"

#include <iostream>

using namespace std;
using namespace glm;

#include "Shader.h"
#include "Camera.h"


///\/////////////////////Data for the cube//////////////////////////////////////

///THE CUBE ISN"T GENERATED IN THE LOCAL CENTER, WE'LL FIX THIS LATER WITH A
///SELF DEFINE LOCAL MATRIX TRANSFORMATION

GLfloat verticesLight[] = {

    ///Front
    //Position
	1.0,	1.0,	1.0,    //Top Right [0]
	0.0f,	1.0,	1.0,    //Top Left [1]
	1.0,	0.0f,	1.0,    //Bottom Right [2]
	0.0f,	0.0f,	1.0,    //Bottom Left [3]

	///Back
	//Position
	1.0,	1.0,	0.0f,  //Top Right [4]
	0.0f,	1.0,	0.0f,  //Top Left  [5]
    0.0f,	0.0f,	0.0f,  //Bottom Left [6]
	1.0f,	0.0f,	0.0f   //Bottom Right   [7]

};

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

// Declares the Elements Array, where the indices to be drawn are stored
unsigned int indices [] = {
	3, 2, 6, 7, 4, 2, 0,
	3, 1, 6, 5, 4, 1, 0
};
///\////////////////////////////////////////////////////////////////////////////

///\//////////////////////////DECLARATIONS//////////////////////////////////////

///Declare Window
GLFWwindow* window;
///Declare Window aspect ratio
float fScreenWidth = 800.0f;
float fScreenHeight = 600.0f;

///Declare VAO, VBO and EBO for the cube
//VAO-Vertex Array Object
//VBO-Vertex Buffer Object
//EBO-Element Buffer Object
unsigned int VBO;
unsigned int VAO;

unsigned int iDiffuseMap;
unsigned int iSpecularMap;

///Declare VAO, VBO, and EBO for the light
unsigned int lightVBO;
unsigned int lightVAO;
unsigned int lightEBO;

///Camera Initial Values
vec3 camPos = vec3(0.0f,0.0f,3.0f);
vec3 camFront = vec3(0.0f,0.0f,-1.0f);
vec3 camRight = vec3(1.0f,0.0f,0.0f);
vec3 camUp = vec3(0.0f,1.0f,0.0f);

///Camera Object
///This is a free roam camera
Camera camera(camPos,camFront,camUp,PERSPECTIVE);

///This camera is locked looking at the center of the world
//Camera camera(camPos,vec3(0,0,0),camUp,PERSPECTIVE,0);

///Time between current and last frame
float deltaTime = 0.0f;

///TimeStamp of last Frame
float lastFrame = 0.0f;

///\////////////////////////////////////////////////////////////////////////////

///Declare the model matrix
mat4 modelMat;

///Declare the view matrix
mat4 viewMat;

///Declare the projection matrix
mat4 projMat;

///Declare the position of the cubes
vec3 cubePositions[] = {
  vec3( 0.0f,  0.0f,  0.0f),
  vec3( 2.0f,  5.0f, -15.0f),
  vec3(-1.5f, -2.2f, -2.5f),
  vec3(-3.8f, -2.0f, -12.3f),
  vec3( 2.4f, -0.4f, -3.5f),
  vec3(-1.7f,  3.0f, -7.5f),
  vec3( 1.3f, -2.0f, -2.5f),
  vec3( 1.5f,  2.0f, -2.5f),
  vec3( 1.5f,  0.2f, -1.5f),
  vec3(-1.3f,  1.0f, -1.5f)
};

///Position of the point light, (Lamp)
vec3 lightPos;

///Types of light
enum lightType {dirLight, pLight, spLight, combinedLight};
lightType ltCurrentLight = dirLight;

///Flag for the enter key
bool isEnterDown = false;

///\////////////////////////////////////////////////////////////////////////////

///\//////////////////////////CALLBACKS/////////////////////////////////////////

///This is the callback function for when the window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///Just adjust the viewport to the new window size
    glViewport(0, 0, width, height);

}

///This is the callback function for input data, keyboard, mouse etc
void processInput(GLFWwindow *window)
{
    ///If the 'esc' key was pressed
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ///Close the window
        glfwSetWindowShouldClose(window, true);
    }

    ///If the 'enter' key was pressed
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !isEnterDown)
    {
        switch(ltCurrentLight)
        {
            case dirLight:
                ltCurrentLight = pLight;
                break;
            case pLight:
                ltCurrentLight = spLight;
                break;
            case spLight:
                ltCurrentLight = combinedLight;
                break;
            case combinedLight:
                ltCurrentLight = dirLight;
                break;
            default:
                cout << "Error: There is no defined light type" << endl;
                break;
        }
        isEnterDown = true;
    }

    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
    {
        isEnterDown = false;
    }

///\/////////////////////////CAMERA CONTROLS////////////////////////////////////

    ///\//////////////////
    ///CAMERA POSITION////
    ///\//////////////////

    ///Move front and back
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.MoveCamera(FORWARD,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.MoveCamera(BACKWARD,deltaTime);
    }

    ///Move left and right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.MoveCamera(LEFT,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.MoveCamera(RIGHT,deltaTime);
    }

    ///Move Up and Down
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        camera.MoveCamera(UP,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        camera.MoveCamera(DOWN,deltaTime);
    }

///\////////////////////////////////////////////////////////////////////////////

    ///\/////////////////
    ///CAMERA ROTATION///
    ///\/////////////////

    ///Update new Yaw angle
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
       camera.MoveCamera(RIGHT_SPIN,deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.MoveCamera(LEFT_SPIN,deltaTime);
    }

    ///Update new Pitch angle
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.MoveCamera(UP_SPIN,deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {

        camera.MoveCamera(DOWN_SPIN,deltaTime);
    }

}

///\////////////////////////////////////////////////////////////////////////////

///\///////////////////INITIALIZE ALL THE FRAMEWORKS////////////////////////////
void initialize()
{
    ///Initialize GLFW
    if(glfwInit() == GLFW_TRUE)
    {
        cout << "GLFW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLFW" << endl;
    }

    ///Set GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ///Uncomment this function for MAC-OS
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ///Create a window
    window = glfwCreateWindow(fScreenWidth, fScreenHeight, "ModernOpenGL", NULL,
                              NULL);
    if (window == NULL)
    {
        ///If it fails, end the program
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }

    ///Set the current context, the window we are currently working in.
    glfwMakeContextCurrent(window);

    ///Register 'framebuffer_size_callback' as the resize window callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ///Set the initial viewport
    glViewport(0, 0, fScreenWidth, fScreenHeight);

    ///Initialize GLEW
    if(glewInit() == GLEW_OK)
    {
        cout << "GLEW initialized successfully" << endl;
    }
    else
    {
        cout << "Unable to initialize GLEW" << endl;
    }


}
///\////////////////////////////////////////////////////////////////////////////

///\////////////////CREATION OF VAOs VBOs and EBOs//////////////////////////////
void setBufferObjects()
{

    ///SETUP THE CUBE CONTAINER INFO

    ///Generate VBO
    glGenBuffers(1, &VBO);

    ///Generate VAO
    glGenVertexArrays(1, &VAO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(VAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    /// vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    /// texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

///\////////////////////////////////////////////////////////////////////////////

    ///SET UP LIGHT OBJECT INFO

    ///Generate VBO
    glGenBuffers(1, &lightVBO);

    ///Generate VAO
    glGenVertexArrays(1, &lightVAO);

    ///Generate EBO
    glGenBuffers(1, &lightEBO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(lightVAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight,
                 GL_STATIC_DRAW);

    ///Populate EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

}
///\////////////////////////////////////////////////////////////////////////////

///\//////////////////////////LOADING TEXTURES//////////////////////////////////
void loadTextures(Shader s)
{

    ///\////////////////////LOAD THE DIFFUSE MAP////////////////////////////

    ///Generate the OpenGL texture object
    glGenTextures(1, &iDiffuseMap);

    ///Active the texture unit before binding
    glActiveTexture(GL_TEXTURE0);

    ///Bind our texture
    glBindTexture(GL_TEXTURE_2D, iDiffuseMap);

    ///Set the texture wrapping/filtering options
    ///(on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     ///Load the texture data
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.png", &width, &height,
                                    &nrChannels, 0);

    if(data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        ///Populate the object with data and generate its MipMap
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture data" << endl;
    }

    ///Free resources
    stbi_image_free(data);


    ///\////////////////////LOAD THE SPECULAR MAP////////////////////////////

    ///Generate the OpenGL texture object
    glGenTextures(1, &iSpecularMap);

    ///Active the texture unit before binding
    glActiveTexture(GL_TEXTURE1);

    ///Bind our texture
    glBindTexture(GL_TEXTURE_2D, iSpecularMap);

    ///Set the texture wrapping/filtering options
    ///(on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ///Load the texture data
    data = stbi_load("textures/container_specular.png", &width,
                                    &height, &nrChannels, 0);
    if(data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        ///Populate the object with data and generate its MipMap
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture data" << endl;
    }

    ///Free resources
    stbi_image_free(data);


}

///\///////////////SET MODEL, VIEW AND PROJECTION MATRIX////////////////////////

void setModelMat(Shader s, vec3 vc3Pos, vec3 Scale,int i)
{
    ///Load Identity Matrix
    modelMat = mat4();

    ///*Remember to read the transformations bottom to top.

    ///Translate to the corresponding positions
    modelMat = translate(modelMat, vc3Pos);

    if(i && ltCurrentLight == dirLight)
    {
        modelMat = rotate(modelMat,(float)glfwGetTime(),vec3(1.0f,0.0f,0.0f));
    }

    modelMat = scale(modelMat, Scale);

    ///*Remember to read the transformations bottom to top.

    ///Set Shader
    s.use();

    ///Set uniform - For the Vertex Shader
    s.setMatrix4fv("modelMat",modelMat);
}

void setViewMat (Shader s)
{
    ///Load Identity Matrix
    viewMat = mat4();

    ///Populate the view Matrix
    viewMat = camera.GetViewMatrix();

    ///Set Shader
    s.use();

    ///Set uniform - For the Vertex Shader
    s.setMatrix4fv("viewMat", viewMat);
}

void setProjMat (Shader s)
{
    ///Load Identity Matrix
    projMat = mat4();

    ///Projection.
    ///Type: Perspective
    ///FOV: 45 degrees
    ///Aspect Ratio: 800/600
    ///Near Clipping Plane: 0.1
    ///Far Clipping Plane: 100
    projMat = camera.GetProjectionMatrix();

    ///Set Shader
    s.use();

    ///Set uniform - For the Vertex Shader
    s.setMatrix4fv("projMat", projMat);

}

///\////////////////////////////////////////////////////////////////////////////

void drawCube(Shader s)
{
    ///Set the shader program
    s.use();

    ///Set the VAO
    glBindVertexArray(VAO);

    ///Draw
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawLight(Shader s)
{
    ///Set the shader program
    s.use();

    ///Set the VAO
    glBindVertexArray(lightVAO);

    ///Draw
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, 0);
}

void calcDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

int main ()
{
    ///Initialize all the frameworks
    initialize();

    ///Compile and Link Shaders into Shader Program for the container

    ///DirLight on other objects
    Shader shader("shaders/vShader.vs","shaders/DirLight_Phong.fs");

    ///PointLight on other objects
    Shader shader2("shaders/vShader.vs","shaders/PointLight_Phong.fs");

    ///SpotLight on other objects
    Shader shader3("shaders/vShader.vs","shaders/SpotLight_Phong.fs");

    ///This shader is to draw the PointLight
    //Whitelight
    Shader lightShader("shaders/vShader.vs","shaders/lightShader.fs");

    //RedLight
    Shader lightShaderRed("shaders/vShader.vs","shaders/lightRedShader.fs");

    //Bluelight
    Shader lightShaderBlue("shaders/vShader.vs","shaders/lightBlueShader.fs");

    ///CombinedLight shader
    Shader shader4("shaders/vShader.vs","shaders/PhongShader.fs");

    ///Set all the info regarding buffer objects
    setBufferObjects();

    ///Load Texture data
    loadTextures(shader);

    ///Enable depth testing
    glEnable(GL_DEPTH_TEST);

    ///Draw in Wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ///Draw in Fillmode, this is default.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ///Tell the user to use the arrow to move
    cout << "------------------------------------------------" << endl;
    cout << "W-A-S-D to move the camera"                       << endl;
    cout << "Page Up and Page Down to change camera elevation" << endl;
    cout << "Arrow Keys to rotate the camera"                  << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Press Enter to change scenes"                     << endl;
    cout << "------------------------------------------------" << endl;
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));

    ///Set the color of the lights *White Light*
    shader.use();
    shader.setVec3("dirLight.ambient",  vec3(0.2f, 0.2f, 0.2f));
    shader.setVec3("dirLight.diffuse",  vec3(0.5f, 0.5f, 0.5f));
    shader.setVec3("dirLight.specular", vec3(1.0f, 1.0f, 1.0f));

    shader2.use();
    shader2.setVec3("pLight.ambient",  vec3(0.2f, 0.2f, 0.2f));
    shader2.setVec3("pLight.diffuse",  vec3(0.5f, 0.5f, 0.5f));
    shader2.setVec3("pLight.specular", vec3(1.0f, 1.0f, 1.0f));

    shader3.use();
    shader3.setVec3("spLight.ambient",  vec3(0.2f, 0.2f, 0.2f));
    shader3.setVec3("spLight.diffuse",  vec3(0.5f, 0.5f, 0.5f));
    shader3.setVec3("spLight.specular", vec3(1.0f, 1.0f, 1.0f));

    ///\////////////////////////////////////////////////////////////////////////
    shader4.use();
    ///One directional light
    shader4.setVec3("dirLights[0].ambient",  vec3(0.1f, 0.1f, 0.1f));
    shader4.setVec3("dirLights[0].diffuse",  vec3(0.25f, 0.25f, 0.25f));
    shader4.setVec3("dirLights[0].specular", vec3(1.0f, 1.0f, 1.0f));

    ///Count
    shader4.setInt("dirLightSize", 1);

    ///Two point lights
    //Red
    shader4.setVec3("pLights[0].ambient",  vec3(0.2f, 0.0f, 0.0f));
    shader4.setVec3("pLights[0].diffuse",  vec3(0.5f, 0.0f, 0.0f));
    shader4.setVec3("pLights[0].specular", vec3(1.0f, 0.0f, 0.0f));

    //Blue
    shader4.setVec3("pLights[1].ambient",  vec3(0.0f, 0.0f, 0.2f));
    shader4.setVec3("pLights[1].diffuse",  vec3(0.0f, 0.0f, 0.5f));
    shader4.setVec3("pLights[1].specular", vec3(0.0f, 0.0f, 1.0f));

    ///Count
    shader4.setInt("pLightSize", 2);

    ///One spot light
    shader4.setVec3("spLights[0].ambient",  vec3(0.2f, 0.2f, 0.2f));
    shader4.setVec3("spLights[0].diffuse",  vec3(0.5f, 0.5f, 0.5f));
    shader4.setVec3("spLights[0].specular", vec3(1.0f, 1.0f, 1.0f));

    ///Count
    shader4.setInt("spLightSize", 1);

    ///\////////////////////////////////////////////////////////////////////////

    ///This is the render loop *While the window is open*
    while(!glfwWindowShouldClose(window))
    {
        ///Get time in between frames for camera transformations
        calcDeltaTime();

        ///Set background color

        if(ltCurrentLight == dirLight)
        {
            //Grey Blue
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        }
        else if (ltCurrentLight == pLight)
        {
            //Dark Grey
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        }
        else if (ltCurrentLight == spLight)
        {
            //Black
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else if (ltCurrentLight == combinedLight)
        {
            //Black
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else
        {
            ///This should never happen.
            cout << "Error: Current Light type is not defined\n";
        }

        ///Refresh Color Bit and Z-Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///SET UP DIR LIGHT/////////////////////////////////////////////////////

        ///Static light
        vec3 lightDir = vec3(-0.2f, -1.0f, -0.3f);

        ///Update the position of the camera in the fragment shader
        shader.use();
        shader.setVec3("camPos",camera.GetPos());

        ///Set the direction of the light
        shader.setVec3("dirLight.direction",lightDir);

        ///\////////////////////////////////////////////////////////////////////

        ///SET UP POINT LIGHT///////////////////////////////////////////////////

        ///Moving light
        vec3 lightPos = vec3(0.0f, 3.0f, (8 * cos(glfwGetTime())) - 3 );

        ///Update the position of the camera in the fragment shader
        shader2.use();
        shader2.setVec3("camPos",camera.GetPos());

        ///Set the position of the light
        shader2.setVec3("pLight.position",lightPos);

        ///Set attenuation constants
        shader2.setFloat("pLight.constant",  1.0f);
        shader2.setFloat("pLight.linear",    0.09f);
        shader2.setFloat("pLight.quadratic", 0.032f);

        ///\////////////////////////////////////////////////////////////////////

        ///SET UP SPOT LIGHT////////////////////////////////////////////////////

        ///SpotLight is anchored to the camera

        ///Update the position of the camera in the fragment shader
        shader3.use();
        shader3.setVec3("camPos", camera.GetPos());

        ///Set the position of the light
        ///SpotLight is anchored to the camera
        shader3.setVec3("spLight.position",camera.GetPos());

        ///Set attenuation constants
        shader3.setFloat("spLight.constant",  1.0f);
        shader3.setFloat("spLight.linear",    0.045f);
        shader3.setFloat("spLight.quadratic", 0.0075f);

        ///Set direction of the spotlight
        shader3.setVec3("spLight.direction", camera.GetFront());

        ///Set the angle of the spotlight (as a Cos angle)
        shader3.setFloat("spLight.innerCosAngle", cos(radians(12.5f)));
        shader3.setFloat("spLight.outerCosAngle", cos(radians(17.5f)));

        ///\////////////////////////////////////////////////////////////////////

        ///SET UP COMBINED LIGHT////////////////////////////////////////////////

        ///First setup Directional Lights

        ///Static light
        vec3 lightDirCombined = vec3(-0.2f, -1.0f, -0.3f);

        ///Update the position of the camera in the fragment shader
        shader4.use();
        shader4.setVec3("camPos",camera.GetPos());

        ///Set the direction of the light

        ///One directional Light
        shader4.setVec3("dirLights[0].direction",lightDirCombined);

        /////////////////////////////////////////////////////////////////////

        ///Now Point lights

        ///Moving light
        vec3 lightPositions_combinedScene[100];
        int lightPositions_combinedSceneSize = 2;

        lightPositions_combinedScene[0] = vec3(0.0f, 3.0f, (8 * cos(glfwGetTime())) - 3 );
        lightPositions_combinedScene[1] = vec3((8 * sin(glfwGetTime())) - 3, 3.0f, 0.0f);

        ///Update the position of the camera in the fragment shader
        shader4.setVec3("camPos",camera.GetPos());

        ///Set the position of the lights
        shader4.setVec3("pLights[0].position",lightPositions_combinedScene[0]);
        shader4.setVec3("pLights[1].position",lightPositions_combinedScene[1]);

        ///Set attenuation constants
        //light one
        shader4.setFloat("pLights[0].constant",  1.0f);
        shader4.setFloat("pLights[0].linear",    0.09f);
        shader4.setFloat("pLights[0].quadratic", 0.032f);

        //light two
        shader4.setFloat("pLights[1].constant",  1.0f);
        shader4.setFloat("pLights[1].linear",    0.09f);
        shader4.setFloat("pLights[1].quadratic", 0.032f);

        ///////////////////////////////////////////////////////////////////////

        ///Finally Spot Lights
        ///Update the position of the camera in the fragment shader
        shader4.setVec3("camPos", camera.GetPos());

        ///Set the position of the lights

        ///SpotLight one is anchored to the camera
        shader4.setVec3("spLights[0].position",camera.GetPos());

        ///Set attenuation constants
        //light one
        shader4.setFloat("spLights[0].constant",  1.0f);
        shader4.setFloat("spLights[0].linear",    0.045f);
        shader4.setFloat("spLights[0].quadratic", 0.0075f);

        ///Set direction of the spotlight
        //light one
        shader4.setVec3("spLights[0].direction", camera.GetFront());

        ///Set the angle of the spotlight (as a Cos angle)
        //light one
        shader4.setFloat("spLights[0].innerCosAngle", cos(radians(12.5f)));
        shader4.setFloat("spLights[0].outerCosAngle", cos(radians(17.5f)));

        ///\////////////////////////////////////////////////////////////////////

        if(ltCurrentLight == dirLight)
        {
            ///Draw 10 cubes in different positions
            for(int i=0; i<10; i++)
            {
                ///Retrieve Position for this cube
                vec3 pos = cubePositions[i];

                ///Set up the Model Matrix (World coordinates)
                setModelMat(shader, pos, vec3(1.0f,1.0f,1.0f),1);

                ///Set the diffuse map texture
                shader.setInt("material.diffuseMap",0);

                ///Set the specular map texture
                shader.setInt("material.specularMap",1);

                ///Set how much light is reflected from specular highlights
                shader.setFloat("material.shine", 16.0f);

                ///Draw
                drawCube(shader);
            }

            ///Set the View Matrix (Camera Coordinates)
            setViewMat(shader);

            ///Set the Projection Matrix (the perspective of the camera)
            setProjMat(shader);

        }
        else if (ltCurrentLight == pLight)
        {
            ///Draw 10 cubes in different positions
            for(int i=0; i<10; i++)
            {
                ///Retrieve Position for this cube
                vec3 pos = cubePositions[i];

                ///Set up the Model Matrix (World coordinates)
                setModelMat(shader2, pos, vec3(1.0f,1.0f,1.0f),1);

                ///Set the diffuse map texture
                shader2.setInt("material.diffuseMap",0);

                ///Set the specular map texture
                shader2.setInt("material.specularMap",1);

                ///Set how much light is reflected from specular highlights
                shader2.setFloat("material.shine", 16.0f);

                ///Draw
                drawCube(shader2);
            }

            ///Set the View Matrix (Camera Coordinates)
            setViewMat(shader2);

            ///Set the Projection Matrix (the perspective of the camera)
            setProjMat(shader2);

            ///Draw the Point Light

            ///Set the model Matrix (World Coordinates)
            setModelMat(lightShader,lightPos, vec3(0.5f,0.5f,0.5f),0);

            ///Draw the light object (Lamp)
            drawLight(lightShader);

            ///Set the View Matrix (Camera Coordinates)
            setViewMat(lightShader);

            ///Set the Projection Matrix (The perspective of the camera)
            setProjMat(lightShader);
        }
        else if (ltCurrentLight == spLight)
        {
            ///Draw 10 cubes in different positions
            for(int i=0; i<10; i++)
            {
                ///Retrieve Position for this cube
                vec3 pos = cubePositions[i];

                ///Set up the Model Matrix (World coordinates)
                setModelMat(shader3, pos, vec3(1.0f,1.0f,1.0f),1);

                ///Set the diffuse map texture
                shader3.setInt("material.diffuseMap",0);

                ///Set the specular map texture
                shader3.setInt("material.specularMap",1);

                ///Set how much light is reflected from specular highlights
                shader3.setFloat("material.shine", 16.0f);

                ///Draw
                drawCube(shader3);
            }

            ///Set the View Matrix (Camera Coordinates)
            setViewMat(shader3);

            ///Set the Projection Matrix (the perspective of the camera)
            setProjMat(shader3);
        }
        else if (ltCurrentLight == combinedLight)
        {
            ///Draw 10 cubes in different positions
            for(int i=0; i<10; i++)
            {
                ///Retrieve Position for this cube
                vec3 pos = cubePositions[i];

                ///Set up the Model Matrix (World coordinates)
                setModelMat(shader4, pos, vec3(1.0f,1.0f,1.0f),1);

                ///Set the diffuse map texture
                shader4.setInt("material.diffuseMap",0);

                ///Set the specular map texture
                shader4.setInt("material.specularMap",1);

                ///Set how much light is reflected from specular highlights
                shader4.setFloat("material.shine", 16.0f);

                ///Draw
                drawCube(shader4);
            }

            ///Set the View Matrix (Camera Coordinates)
            setViewMat(shader4);

            ///Set the Projection Matrix (the perspective of the camera)
            setProjMat(shader4);

            ///\/////////////////////////////////
            ///Draw the Point Lights/////////////
            ///\/////////////////////////////////

            for(int i=0; i<lightPositions_combinedSceneSize; i++)
            {
                if(i==0)
                {
                    ///Set the model Matrix (World Coordinates)
                    setModelMat(lightShaderRed,lightPositions_combinedScene[i], vec3(0.5f,0.5f,0.5f),0);

                    ///Draw the light object (Lamp)
                    drawLight(lightShaderRed);

                    ///Set the View Matrix (Camera Coordinates)
                    setViewMat(lightShaderRed);

                    ///Set the Projection Matrix (The perspective of the camera)
                    setProjMat(lightShaderRed);
                }
                else
                {
                    ///Set the model Matrix (World Coordinates)
                    setModelMat(lightShaderBlue,lightPositions_combinedScene[i], vec3(0.5f,0.5f,0.5f),0);

                    ///Draw the light object (Lamp)
                    drawLight(lightShaderBlue);

                    ///Set the View Matrix (Camera Coordinates)
                    setViewMat(lightShaderBlue);

                    ///Set the Projection Matrix (The perspective of the camera)
                    setProjMat(lightShaderBlue);
                }
            }

        }

///\////////////////////////////////////////////////////////////////////////////

///\////////////////////////////////////////////////////////////////////////////

        ///Process user input
        processInput(window);

        ///Swap the Front and Back buffer.
        glfwSwapBuffers(window);

        ///Poll CallBack Events
        glfwPollEvents();

    }

    ///Free resources when application ends.
    glfwTerminate();

    return 0;
}





