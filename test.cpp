#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "cylinder.h"
#include <thread>
#include <chrono>
#include <GL/gl.h>
#include <iostream>
#include "stb_image.h"
#include "cube.h"
#include "SpotLight.h"
using namespace std;
GLfloat a = 0, b = 0, c = 0, d = 0, e = 0;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

enum GameState {
    SPLASH_SCREEN,
    MAIN_GAME
};
int mainGameState = SPLASH_SCREEN;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void beach(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void sea(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void tree(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);

glm::mat4 transforamtion(float tx, float ty, float tz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix;
    return model;
}


void update(int value)
{
    a += 20.0; //Plane position takeoff on x axis
    b -= 10.0; //Road Strip backward movement
    c += 15; //take off at certain angle on y axis
    if (b <= -78.0)// moving of run way
        b = 0.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //glfwSwapBuffers(window);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

// settings

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float flor = 0.0f;
float block1 = -20.0f;
float block2 = -40.0f;

int score = 3;
bool game = false;

float plane_x = 0.0f;
bool pLight = false;
bool dLight = false;
bool sLight = false;
bool sphere_true = true;


// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);




// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.0f,  2.00f,  0.0f),
    glm::vec3(1.5f,  -1.5f,  0.0f),
    glm::vec3(-2.0f,  2.0f,  0.0f),
    glm::vec3(-1.5f,  -1.5f,  0.0f)
};
//glm::vec3(-0.5, 1, -0.5)
PointLight pointlight1(
    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient dark grey
    1.0f, 1.0f, 1.0f,     // diffuse white
    1.0f, 1.0f, 1.0f,        // specular white
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
glm::vec3 spotLightPositions[] = {
    glm::vec3(-1,  1.50f,  0.0f),

};
SpotLight spotlight1(
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,  // position
    .2f, .2f, .2f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1,       // light number
    glm::cos(glm::radians(30.5f)),
    glm::cos(glm::radians(60.0f)),
    .5f, -1, 0
);

// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool point = false;

float point_z = 0.0f;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float point_x, point_x2, point_x3;
int cnt = 1;
int num = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO st
    // ays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Sphere sphere = Sphere();
    Cylinder cylinder = Cylinder();

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    //Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    string diffuseMapPath = "untit1.png";
    string specularMapPath = "untit1.png";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    
    
    string dgameover = "gameover.jpg";
    string sgameover = "gameover.jpg";
    unsigned int diff1 = loadTexture(dgameover.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec1 = loadTexture(sgameover.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube gameover = Cube(diff1, spec1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string dgrass = "grass.png";
    string sgrass = "grass.png";
    unsigned int diff2 = loadTexture(dgrass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec2 = loadTexture(sgrass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass = Cube(diff2, spec2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string dbuild = "dbuilding1.jpg";
    string sbuild = "dbuilding1.jpg";
    unsigned int diff3 = loadTexture(dbuild.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec3 = loadTexture(sbuild.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube building = Cube(diff3, spec3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string dbuild1 = "sbuilding1.jpg";
    string sbuild1 = "sbuilding1.jpg";
    unsigned int diff4 = loadTexture(dbuild1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec4 = loadTexture(sbuild1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube building1 = Cube(diff4, spec4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    string dsphere = "wall.jpg";
    string ssphere = "wall.jpg";
    unsigned int diff5 = loadTexture(dsphere.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec5 = loadTexture(ssphere.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere scoreboard=Sphere(0.3f, 36, 18, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diff5, spec5, 0.0f, 0.0f, 1.0f, 1.0f, diff5, spec5);
    scoreboard.setRadius(0.5);

    string dsphere1 = "golok.jpg";
    string ssphere1 = "golok.jpg";
    unsigned int diff6 = loadTexture(dsphere1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec6 = loadTexture(ssphere1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere sphere = Sphere(0.3f, 36, 18, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diff5, spec5, 0.0f, 0.0f, 1.0f, 1.0f, diff6, spec6);
    
    
    while (!glfwWindowShouldClose(window) && mainGameState == SPLASH_SCREEN) {
 glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShaderWithTexture.use();

        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        
        identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4  alTogether;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        alTogether = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        float baseHeight = 8.1; float width = 3;  float length = 2;
        model = glm::mat4(1.0f);
        //Ground
        model = transforamtion(-6, -3, -10, width*4 , baseHeight, length);
        model = alTogether * model;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glfwSwapBuffers(window); glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            mainGameState = MAIN_GAME; // Transition to the main game
        }
    }



    std::random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(-3.0f, 3.0f);

    float randomVal = dis(gen);
    point_x = randomVal;
    point_x2 = dis(gen);
    point_x3 = dis(gen);
    while (!glfwWindowShouldClose(window) && mainGameState == MAIN_GAME)
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        lightingShader.setVec3("diectionalLight.direction", 0.0f, 3.0f, 0.0f);
        lightingShader.setVec3("diectionalLight.ambient", .2, .2, .2);
        lightingShader.setVec3("diectionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("diectionalLight.specular", 1.0f, 1.0f, 1.0f);

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            lightingShader.setVec3("diectionalLight.diffuse", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotlight.diffuse", 0.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            lightingShader.setVec3("diectionalLight.specular", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotlight.specular", 0.0f, 0.0f, 0.0f);
        }


        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            lightingShader.setBool("dlighton", false);
        //if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        lightingShader.setBool("pointlighton", true);

        lightingShader.setVec3("spotlight.position", -0.5, 1, -0.5);
        lightingShader.setVec3("spotlight.direction", 0, -1, 0);
        lightingShader.setVec3("spotlight.ambient", .2, .2, .2);
        lightingShader.setVec3("spotlight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotlight.k_c", 1.0f);
        lightingShader.setFloat("spotlight.k_l", 0.09);
        lightingShader.setFloat("spotlight.k_q", 0.032);
        lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            lightingShader.setBool("spotlighton", false);


        //pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
        //    1.0f, 1.0f, 1.0f,     // ambient
        //    1.0f, 1.0f, 1.0f,      // diffuse
        //    1.0f, 1.0f, 1.0f,        // specular
        //    1.0f,   //k_c
        //    0.09f,  //k_l
        //    0.032f, //k_q
        //    1,       // light number
        //    glm::cos(glm::radians(20.5f)),
        //    glm::cos(glm::radians(25.0f)),
        //    0, -1, 0
        // activate shader
        lightingShader.use();


        // pass projection matrix to shader (note that in this case it could change every frame)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        //camera/view transformation
        view = camera.GetViewMatrix();
        // glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
         lightingShaderWithTexture.use();







        if (score == 0)
        {
            identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            glm::mat4  alTogether;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, -0.5f, -4.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 3.0, 3.0));
            model = glm::mat4(1.0f);
            model = translateMatrix * scaleMatrix;
            gameover.drawCubeWithTexture(lightingShaderWithTexture, model);
        }
        if(score>0)
        {
            float baseHeight = 4.0f;
            float width = 2.0f;
            float length = 2.0f;
            float b1_x = point_x + 1.0f;
            float b1_z = block1 + flor + 1.0f;
            float b2_x = point_x2 - 1.0f;
            float b2_z = block2 + flor + 2.0f;
            float b3_x = point_x3 - 3.5f;
            float b3_z = block2 + flor - 5.0f;

            //texture
            glm::mat4 modelforgrass1 = model * 
                glm::translate(model, glm::vec3(-7.0, -0.95, block1 + flor)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(15.0, 0.001, 20.5));
        glm::mat4 modelforgrass2=  
            glm::translate(model, glm::vec3(-7.0, -0.95, block2 + flor)) * 
            glm::scale(glm::mat4(1.0f), glm::vec3(15.0, 0.001, 20.5));
            bed(cubeVAO, lightingShader, model);
             //building texture

           glm::mat4 modelforbuilding1=glm::translate(glm::mat4(1.0f)
               , glm::vec3(b1_x, -1.0f,b1_z)) *
               glm::scale(glm::mat4(1.0f), glm::vec3(width, baseHeight, length));
          glm::mat4 modelforbuilding2 = glm::translate(glm::mat4(1.0f),
              glm::vec3(b2_x, -1.0f, b2_z)) *
              glm::scale(glm::mat4(1.0f), glm::vec3(width, baseHeight, length));
          //float b3_x = point_x3 - 3.5f;
          //float b3_z = block2 + flor - 1.0f;
          glm::mat4 modelforbuilding3 = 
 glm::translate(glm::mat4(1.0f), glm::vec3(b3_x, -1.0f, b3_z))
    * glm::scale(glm::mat4(1.0f), glm::vec3(width, baseHeight, length));
         beach(cubeVAO, lightingShader, model);
          grass.drawCubeWithTexture(lightingShaderWithTexture, modelforgrass1);
          grass.drawCubeWithTexture(lightingShaderWithTexture, modelforgrass2);

          building.drawCubeWithTexture(lightingShaderWithTexture, modelforbuilding1);
          building1.drawCubeWithTexture(lightingShaderWithTexture, modelforbuilding2);
          building.drawCubeWithTexture(lightingShaderWithTexture, modelforbuilding3);

            if (point)
            {
                std::random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<float> dis(-3.0f, 3.0f);

                randomVal = dis(gen);
                point = false;
            }

            point_z += 0.1f;
            float zz = point_z - 40.5f;
            if (zz > 4.0f)
            {
                point_z = 0.0f;
                point = true;
            }

            if (zz >= -6.5f && randomVal <= plane_x + 0.5f && randomVal >= plane_x - 0.5f)
            {
                point_z = 0.0f;
                point = true;
                if (score < 5)
                    score++;
            }

            //sphere
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            modelForSphere = glm::translate(model, glm::vec3(randomVal, 0.0f, zz));
            sphere.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);
            float xx = 0.0;
            for (int i = 0; i < score; i++)
            {
                xx += 0.7;
                glm::mat4 modelForSphere2 = glm::mat4(1.0f);
                modelForSphere2 = glm::translate(model, glm::vec3(xx + 4.5f, 5.0f, -10.0));
                
                scoreboard.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);
            }
        }

        //sea(cubeVAO, lightingShader, model); tree(cubeVAO, lightingShader, model);



        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);
        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        glm::mat4 trns = glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, -5.48f));

        glm::mat4 scl = glm::scale(identityMatrix, glm::vec3(1.9f, 2.5f, 1.5f));
        modelMatrixForContainer = trns * scl;
        // cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 0.784f, float g = 0.784f, float b = 0.784f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(r, g, b));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model)
{
    glm::mat4 translate, scale, rotate, identityMatrix = glm::mat4(1.0f);
    //glm::mat4 alltogether = (glm::translate(glm::mat4(1.0f), glm::vec3(translate_X, 0.0f, 0.0f)));
    model = identityMatrix;

    //cylinder for aeroplane
    Cylinder cylinder = Cylinder();
    glm::mat4 modelForcylinder = glm::mat4(1.0f);
    modelForcylinder = (glm::translate(model, glm::vec3(plane_x + 0.0f, 0.0f, 0.6f))) *
        (glm::scale(model, glm::vec3(0.24f, 0.24f, 0.4f)))
        * (glm::rotate(identityMatrix, glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)));
    cylinder.drawSphere(lightingShader, modelForcylinder);
    //pakha for plane
    glm::mat4 modelForcylinder2 = glm::mat4(1.0f);
    glm::mat4 scaleforpakha = glm::scale(model, glm::vec3(1.0f, 0.05f, 0.5f)),
        translateforpakha = glm::translate(model, glm::vec3(plane_x - 1.0, 0.0, -1.5)),
        rotationforpakha = glm::rotate(identityMatrix, glm::radians(45.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    modelForcylinder2 = translateforpakha * rotationforpakha * scaleforpakha;
    drawCube(cubeVAO, lightingShader, modelForcylinder2, 0.784, 0.784, 0.784);

    glm::mat4 modelForcylinder3 = glm::mat4(1.0f);
    glm::mat4 scaleforpakha2 = glm::scale(model, glm::vec3(1.2f, 0.05f, 0.5f)),
        translateforpakha2 = glm::translate(model, glm::vec3(plane_x + 0.7, 0.0, -1.5)),
        rotationforpakha2 = glm::rotate(identityMatrix, glm::radians(135.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    modelForcylinder3 = translateforpakha2 * rotationforpakha2 * scaleforpakha2;
    drawCube(cubeVAO, lightingShader, modelForcylinder3, 0.784, 0.784, 0.784);
    Cylinder cylinder2 = Cylinder();
    glm::mat4 modelForcylinder4 = glm::mat4(1.0f);
    modelForcylinder4 = (glm::translate(model, glm::vec3(plane_x - 0.5f, -0.1f, -1.0f))) *
        (glm::scale(model, glm::vec3(0.09f, 0.09f, 0.1f)))
        * (glm::rotate(identityMatrix, glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)));
    cylinder2.drawSphere(lightingShader, modelForcylinder4);
    glm::mat4 modelForcylinder5 = glm::mat4(1.0f);
    modelForcylinder5 = (glm::translate(model, glm::vec3(plane_x + 0.5f, -0.1f, -1.0f))) *
        (glm::scale(model, glm::vec3(0.09f, 0.09f, 0.1f)))
        * (glm::rotate(identityMatrix, glm::radians(90.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)));
    cylinder2.drawSphere(lightingShader, modelForcylinder5);

}
void beach(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    flor += 0.03f;
    if (flor >= 20.0f)
    {
        if (block1 == -20.f)
        {
            std::random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<float> dis(-3.0f, 3.0f);
            point_x = dis(gen);
            block1 = -40.0f;
            block2 = -20.0f;
        }
        else
        {
            std::random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<float> dis(-3.0f, 3.0f);
            point_x2 = dis(gen);
            block1 = -20.0f;
            block2 = -40.0f;
        }
        cnt++;
        num = cnt - 1;
        flor = 0.0f;

    }

    float baseHeight = 0.001;
    float width = 15.0;
    float length = 20.5;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
     translate = glm::translate(model, glm::vec3(-7.0, -1.0, block1 + flor));
    model = alTogether * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0); 
    
    model = glm::mat4(1.0f);
    translate = glm::translate(model, glm::vec3(-7.0, -1.0, block2 + flor));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0);
    

    //building
    baseHeight = 4.0f;
    width = 2.0f;
    length = 2.0f;
    model = glm::mat4(1.0f);
    float b1_x = point_x + 1.0f;
    float b1_z = block1 + flor + 1.0f;


    if (b1_x <= plane_x && b1_x + 2.0f >= plane_x)
    {

        if (b1_z >= -8.5f)
        {
            printf("lagse 1\n");
            if (num != cnt)
            {
                if (score>0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }

    }
    else if (b1_x <= plane_x - 0.5f && b1_x + 2.0f >= plane_x - 0.5)
    {
        if (b1_z >= -6.0f)
        {
            printf("lagse 1\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b1_x <= plane_x - 1.5f && b1_x + 2.0f >= plane_x - 1.5)
    {
        if (b1_z >= -6.0f)
        {
            printf("lagse 1\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b1_x <= plane_x + 0.5f && b1_x + 2.0f >= plane_x + 0.5)
    {
        if (b1_z >= -6.0f)
        {
            printf("lagse 1\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b1_x <= plane_x + 1.5f && b1_x + 2.0f >= plane_x + 1.5)
    {
        if (b1_z >= -6.0f)
        {
            printf("lagse 1\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }

    float b2_x = point_x2 - 1.0f;
    float b2_z = block2 + flor + 2.0f;
    if (b2_x <= plane_x && b2_x + 2.0f >= plane_x)
    {

        if (b2_z >= -8.5f)
        {
            printf("lagse 2\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }

    }
    else if (b2_x <= plane_x - 0.5f && b2_x + 2.0f >= plane_x - 0.5)
    {
        if (b2_z >= -6.0f)
        {
            printf("lagse 2\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b2_x <= plane_x - 1.5f && b2_x + 2.0f >= plane_x - 1.5)
    {
        if (b2_z >= -6.0f)
        {
            printf("lagse 2\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b2_x <= plane_x + 0.5f && b2_x + 2.0f >= plane_x + 0.5)
    {
        if (b2_z >= -6.0f)
        {
            printf("lagse 2\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b2_x <= plane_x + 1.5f && b2_x + 2.0f >= plane_x + 1.5)
    {
        if (b2_z >= -6.0f)
        {
            printf("lagse 2\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    




    float b3_x = point_x3 - 3.5f;
    float b3_z = block2 + flor - 5.0f;
    if (b3_x <= plane_x && b3_x + 2.0f >= plane_x)
    {

        if (b3_z >= -8.5f)
        {
            printf("lagse 3\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }

    }
    else if (b3_x <= plane_x - 0.5f && b3_x + 2.0f >= plane_x - 0.5)
    {
        if (b3_z >= -6.0f)
        {
            printf("lagse 3\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b3_x <= plane_x - 1.5f && b3_x + 2.0f >= plane_x - 1.5)
    {
        if (b3_z >= -6.0f)
        {
            printf("lagse 3\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b3_x <= plane_x + 0.5f && b3_x + 2.0f >= plane_x + 0.5)
    {
        if (b3_z >= -6.0f)
        {
            printf("lagse 3\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }
    else if (b3_x <= plane_x + 1.5f && b3_x + 2.0f >= plane_x + 1.5)
    {
        if (b3_z >= -6.0f)
        {
            printf("lagse 3\n");
            if (num != cnt)
            {
                if (score > 0)
                {
                    score--;
                }
                num = cnt;
            }

            if (plane_x >= 0.0)
                plane_x -= 5.0f;
            else
                plane_x += 5.0f;
            printf("%d\n", score);
        }
    }

   /*
    scale = glm::scale(model, glm::vec3(0.1f, 5.1f, 0.1f));
    translate = glm::translate(model, glm::vec3(plane_x-1.5f, -2.0f, -6.5f));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1, 0.2, 0.4f);
    /*
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(b2_x, -1.0f, b2_z));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1, 0.2, 0.4f);

    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(b3_x, -1.0f, b3_z));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1, 0.2, 0.4f);
    */

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        plane_x -= 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        plane_x += 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        pointlight2.turnOff();
        pointlight3.turnOff();
        pointlight4.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        pointlight2.turnOn();
        pointlight3.turnOn();
        pointlight4.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;


        }
        else
        {

            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (specularToggle)
        {

            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {

            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            specularToggle = !specularToggle;
        }



    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        pointlight1.turnAmbientOn();
        pointlight2.turnAmbientOn();
        pointlight3.turnAmbientOn();
        pointlight4.turnAmbientOn();
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        pointlight1.turnDiffuseOn();
        pointlight2.turnDiffuseOn();
        pointlight3.turnDiffuseOn();
        pointlight4.turnDiffuseOn();
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        pointlight1.turnSpecularOn();
        pointlight2.turnSpecularOn();
        pointlight3.turnSpecularOn();
        pointlight4.turnSpecularOn();
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        pointlight1.ambient = glm::vec3(0.0, 1.0, 0.0);
        pointlight1.diffuse = glm::vec3(0.0, 1.0, 0.0);
        pointlight1.specular = glm::vec3(0.0, 1.0, 0.0);


        pointlight2.ambient = glm::vec3(0.0, 1.0, 0.0);
        pointlight2.diffuse = glm::vec3(0.0, 1.0, 0.0);
        pointlight2.specular = glm::vec3(0.0, 1.0, 0.0);


        pointlight3.ambient = glm::vec3(0.0, 1.0, 0.0);
        pointlight3.diffuse = glm::vec3(0.0, 1.0, 0.0);
        pointlight3.specular = glm::vec3(0.0, 1.0, 0.0);


        pointlight4.ambient = glm::vec3(0.0, 1.0, 0.0);
        pointlight4.diffuse = glm::vec3(0.0, 1.0, 0.0);
        pointlight4.specular = glm::vec3(0.0, 1.0, 0.0);

    }


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


















//
//  sphere.h
//  test
//
//  Created by Nazirul Hasan on 26/9/23.
//

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

class Sphere
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    // glm::vec3 emissive;
     // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess;

    Sphere(float radius, int sectorCount, int stackCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax, unsigned int diffuseTexture, unsigned int specularTexture) : verticesStride(24) {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny, dMap, sMap, textureXmin, textureYmin, textureXmax, textureYmax);
        setUpSphereVertexDataAndConfigureVertexAttribute();
        diffuseMap = diffuseTexture;
        specularMap = specularTexture;
    }
    ~Sphere() {}
    
    void setDefaults() {
        this->radius = 1.0f;
        this->sectorCount = 36;
        this->stackCount = 18;
        this->ambient = glm::vec3(1.0f, 0.0f, 0.0f);
        this->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
        this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
        this->shininess = 32.0f;
        this->diffuseMap = 0;
        this->specularMap = 0;
        this->TXmin = 0.0f;
        this->TXmax = 1.0f;
        this->TYmin = 0.0f;
        this->TYmax = 1.0f;
    }
    void set(float radius, int sectorCount, int stackCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        this->radius = radius;
        this->sectorCount = sectorCount;
        this->stackCount = stackCount;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
        //this->emissive = emis;
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->TXmin = textureXmin;
        this->TXmax = textureXmax;
        this->TYmin = textureYmin;
        this->TYmax = textureYmax;
    }

    void setRadius(float radius)
    {
        if (radius != this->radius)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    }

    void setSectorCount(int sectors)
    {
        if (sectors != this->sectorCount)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    }

    void setStackCount(int stacks)
    {
        if (stacks != this->stackCount)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    }

    // for interleaved vertices
    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;     // # of vertices
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);  // # of bytes
    }

    int getVerticesStride() const
    {
        return verticesStride;   // should be 24 bytes
    }
    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }
    void drawSphereWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f)) {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setVec3("material.ambient", this->ambient);
        lightingShaderWithTexture.setVec3("material.diffuse", this->diffuse);
        lightingShaderWithTexture.setVec3("material.specular", this->specular);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(sphereTexVAO);
        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0);
    }
    // draw in VertexArray mode
    void drawSphere(Shader& lightingShader, glm::mat4 model) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        //lightingShader.setVec3("material.emissive", this->emissive);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        // draw a sphere with VAO
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }

private:
    unsigned int sphereTexVAO;
    unsigned int sphereVBO;
    unsigned int sphereEBO;
    vector<float> textureCoordinates, texCoords;

    unsigned int textureID;
    // member functions
    void buildCoordinatesAndIndices()
    {
        float x, y, z, xz;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle = -sectorStep;
        float stackAngle = PI / 2 + stackStep;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle -= stackStep;        // starting from pi/2 to -pi/2
            xz = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);
            // add (sectorCount+1) vertices per stack
            // first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle += sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                z = xz * cosf(sectorAngle);
                x = xz * sinf(sectorAngle);
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
        }

        // generate index list of sphere triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                if (i != 0 && i != (stackCount - 1))
                {
                    // k1 => k2 => k1+1
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);

                    // k1+1 => k2 => k2+1
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
                // 2 triangles per sector excluding first and last stacks
                else if (i == 0)
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);

                }

                else if (i == (stackCount - 1))
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
            }
        }
    }
    void buildTextureCoordinates()
    {
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;

        for (int i = 0; i <= stackCount; ++i)
        {
            float stackAngle = PI / 2 - i * stackStep;

            for (int j = 0; j <= sectorCount; ++j)
            {
                float sectorAngle = j * sectorStep;

                float x = cosf(sectorAngle) * sinf(stackAngle);
                float y = cosf(stackAngle);
                float z = sinf(sectorAngle) * sinf(stackAngle);

                textureCoordinates.push_back(x);
                textureCoordinates.push_back(y);
            }
        }
    }

    void setUpSphereVertexDataAndConfigureVertexAttribute() {
        //generateSphereData();
        generatepoints();
        buildVertices();
        glGenVertexArrays(1, &sphereTexVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);

        glBindVertexArray(sphereTexVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        int stride = this->getVerticesStride();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    void generatepoints()
    {
        //std::vector<float>().swap(coordinates);
        //std::vector<float>().swap(normals);
        //std::vector<float>().swap(texCoords);
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back(s);
                texCoords.push_back(t);
            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                if (i != 0 && i != (stackCount - 1))
                {
                    // k1 => k2 => k1+1
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);

                    // k1+1 => k2 => k2+1
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
                // 2 triangles per sector excluding first and last stacks
                else if (i == 0)
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);

                }

                else if (i == (stackCount - 1))
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
            }
        }
    }
    void buildVertices()
    {
        size_t i, j;
        size_t count = coordinates.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            if (i < normals.size())
                vertices.push_back(normals[i]);
            if (i + 1 < normals.size())
                vertices.push_back(normals[i + 1]);
            if (i + 2 < normals.size())
                vertices.push_back(normals[i + 2]);

            // Add texture coordinates
            if (j < textureCoordinates.size())
                vertices.push_back(textureCoordinates[j]);
            if (j + 1 < textureCoordinates.size())
                vertices.push_back(textureCoordinates[j + 1]);
        }
    }

    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
    {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    vector<float> computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        const float EPSILON = 0.000001f;

        vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }

    // memeber vars
    unsigned int sphereVAO;
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride;                 // # of bytes to hop to the next vertex (should be 24 bytes)

};










