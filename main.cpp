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
#include "sphere2.h"
#include "cylinder.h"
#include <thread>
#include <chrono>
#include <GL/gl.h>
#include <iostream>
#include "stb_image.h"
#include "cube.h"
#include "SpotLight.h"
#include "Cylinder1.h"
#include "Pyramid.h"
#include "CurvedRoad.h"
//#include <SFML/Audio.hpp>
#include <map>

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
void scsToWcs(float sx, float sy, float wcsv[3]);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void beach(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void sea(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void tree(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);

long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L);
unsigned int loadCubemap(vector<std::string> faces);

glm::mat4 transforamtion(float tx, float ty, float tz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix;
    return model;
}


GLint viewport[4];
//float wcsClkDn[3];
float wcsClkUp[12 * 3] = {
-0.0700, -6.1400, -5.1000,
-0.1500, -6.1450, -5.1000,
-0.2400, -6.1250, -5.1000,
-0.2550, -6.0200, -5.1000,
-0.3000, -6.9050, -5.1000,
-0.3500, -7.8150, -5.1000,
-0.4050, -7.6750, -5.1000,
-0.4500, -7.6000, -5.1000,
-0.4750, -7.5050, -5.1000,
-0.4050, -7.4250, -5.1000,
-0.3050, -7.3750, -5.1000,
-0.3050, -7.3750, -5.1000
};
vector <float> cntrlPoints;
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;

class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];
int mouseButtonFlag = 0;
FILE* fp;
const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
bool showControlPoints = true;
bool loadBezierCurvePoints = false;
bool showHollowBezier = false;
bool lineMode = false;
unsigned int bezierVAO;
//sound

//sf::SoundBuffer buffer;
//sf::Sound sound;

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
int countt = 0;
int score = 3;
bool game = false;
int level2 = 0;
float plane_x = 0.0f;
bool pLight = false;
bool dLight = false;
bool sLight = false;
bool sphere_true = true;
float spare_angle = 10.0f;


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
    glm::vec3(2.5f,  2.50f,  0.0f),
    glm::vec3(1.5f,  -1.5f,  0.0f),
    glm::vec3(-2.50f,  2.5f,  0.0f),
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
    for (int i = 0; i < 36; i++) {
        cntrlPoints.push_back(wcsClkUp[i]);

    }
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1);

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



    string dcscore = "white.png";
    string scscore = "white1.png";
    unsigned int diff7 = loadTexture(dcscore.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec7 = loadTexture(scscore.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube scorept = Cube(diff7, diff7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube scorept1 = Cube(spec7, spec7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    string dsphere = "humann.jpg";
    string ssphere = "humann.jpg";
    unsigned int diff5 = loadTexture(dsphere.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec5 = loadTexture(ssphere.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 scoreboard = Sphere2(0.3f, 36, 18, glm::vec3(1.0f, 0.75f, 0.79f), glm::vec3(1.0f, 0.75f, 0.79), glm::vec3(0.8f, 0.6f, 0.63), 32.0f, diff5, spec5, 0.0f, 0.0f, 1.0f, 1.0f);

    string sky = "sky.jpeg";
    unsigned int skyspec = loadTexture(sky.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2  sky1 = Sphere2(1.0f, 144, 72, glm::vec3(0, 0.4, 0.9), glm::vec3(0, 0.4, 0.9), glm::vec3(0, 0.4, 0.9), 32.0f, skyspec, skyspec, 0.0f, 0.0f, 1.0f, 1.0f);


    string dsphere1 = "lavender.jfif";
    string ssphere1 = "lavender.jfif";
    unsigned int diff6 = loadTexture(dsphere1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int spec6 = loadTexture(ssphere1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    Sphere2 sphere = Sphere2(0.3f, 36, 18, glm::vec3(1.0f, 0.75f, 0.79f), glm::vec3(1.0f, 0.75f, 0.79), glm::vec3(0.8f, 0.6f, 0.63), 32.0f, diff6, spec6, 0.0f, 0.0f, 1.0f, 1.0f);

    Pyramid pyramid("tree.jpg");
    Cylinder1 cylinder1(.1, .1, 1, 16, 20, "treebase.png");
    glm::vec3 p0(0, 5, 0);
    glm::vec3 p1(0, 2, 0);
    glm::vec3 p2(7, 0, 0);
    glm::vec3 p3(7, -10, 0);
    int numSegments = 1000;
    float roadWidth = 1.0f;
    CurvedRoad cr("road.png", p0, p1, p2, p3, numSegments, roadWidth);
   
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
        model = transforamtion(-6, -3, -10, width * 4, baseHeight, length);
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


        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.0f, 1.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.0f, 1.0f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("material.shininess", 32.0f);

        

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
        if (score > 0)
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
            glm::mat4 modelforgrass2 =
                glm::translate(model, glm::vec3(-7.0, -0.95, block2 + flor)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(15.0, 0.001, 20.5));
           bed(cubeVAO, lightingShader, model);
            //road
           glm::mat4  modelforroad = transforamtion(-1, -.94, -2, 1, 1, 1);
           modelforroad = modelforroad * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            cr.draw(lightingShaderWithTexture, modelforroad);
            //building texture

            glm::mat4 modelforbuilding1 = glm::translate(glm::mat4(1.0f)
                , glm::vec3(b1_x, -1.0f, b1_z)) *
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

            glm::mat4 modelforscore = glm::translate(glm::mat4(1.0f)
                , glm::vec3(5.0, 5.4f, -10.0)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(2.5, 0.8, 0));
   
            building1.drawCubeWithTexture(lightingShaderWithTexture, modelforbuilding2);
            if (level2 == 1)
                building.drawCubeWithTexture(lightingShaderWithTexture, modelforbuilding3);

            if (score >= 4)
            {
                level2 = 1;
                glm::mat4 modelforscore = glm::translate(glm::mat4(1.0f)
                    , glm::vec3(3.2, 4.8f, -8.0)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(2.5, 0.8, 0.1));
                scorept.drawCubeWithTexture(lightingShaderWithTexture, modelforscore);
            }
            if (score < 4) {
                level2 = 0;
                glm::mat4 modelforscore = glm::translate(glm::mat4(1.0f)
                    , glm::vec3(3.2, 4.8f, -8.0)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(2.5, 0.8, 0.1));
                scorept1.drawCubeWithTexture(lightingShaderWithTexture, modelforscore);


            }
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
                countt++;
            }
            //sky

            spare_angle = spare_angle + 0.0f;
            glm::mat4 modelforsky = glm::mat4(1.0f);
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(17.0f, 15.0f, 15.0f));
            modelforsky = glm::translate(identityMatrix, glm::vec3(0.0f, 5.0f, -66.0f)) * glm::rotate(identityMatrix, glm::radians(spare_angle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(50.0f, 40.0f, 30.0f));
            sky1.drawSphereWithTexture(lightingShaderWithTexture, modelforsky);

            //sphere
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            modelForSphere = glm::translate(model, glm::vec3(randomVal, 0.0f, zz));
            sphere.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);
            float xx = 0.0;
            for (int i = 0; i < score; i++)
            {
                xx += 0.7;
                glm::mat4 modelForSphere2 = glm::mat4(1.0f);
                modelForSphere2 = glm::translate(model, glm::vec3(xx + 2.5f, 4.15f, -7.0));

                scoreboard.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);
            }
            //tree
           glm::mat4 modelfortree = transforamtion(5.8, 0.5, -12, 2, 2, 1);
            pyramid.draw(lightingShaderWithTexture, modelfortree);
           // modelfortree = transforamtion(0, 0, 0, 1, 1, 1);
            cylinder1.Draw(lightingShaderWithTexture, modelfortree);

             modelfortree = transforamtion(6.8, 0, -20, 2, 2, 1);
            pyramid.draw(lightingShaderWithTexture, modelfortree);
           // modelfortree = transforamtion(0, 0, 0, 1, 1, 1);
            cylinder1.Draw(lightingShaderWithTexture, modelfortree);

            modelfortree = transforamtion(-6.5, 0, -20, 2, 2, 1);
            pyramid.draw(lightingShaderWithTexture, modelfortree);
            // modelfortree = transforamtion(0, 0, 0, 1, 1, 1);
            cylinder1.Draw(lightingShaderWithTexture, modelfortree);

            modelfortree = transforamtion(-6.5, 0, -10, 2, 2, 1);
            pyramid.draw(lightingShaderWithTexture, modelfortree);
            // modelfortree = transforamtion(0, 0, 0, 1, 1, 1);
            cylinder1.Draw(lightingShaderWithTexture, modelfortree);

            //printf("%d",score);
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

        //glBindVertexArray(bezierVAO);
        //glDrawElements(GL_TRIANGLES,                    // primitive type
        //    (unsigned int)indices.size(),          // # of indices
        //    GL_UNSIGNED_INT,                 // data type
        //    (void*)0);

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
    //drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0); 

    model = glm::mat4(1.0f);
    translate = glm::translate(model, glm::vec3(-7.0, -1.0, block2 + flor));
    model = translate * scale;
    //  drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0);
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
                if (score > 0)
                {
                    score--;
                    //if (!buffer.loadFromFile("planecrash.wav"))
                    //{
                    //    std::cerr << "Failed to load sound file!" << std::endl;
                    //}
                    //else
                    //{
                    //    //sound.setBuffer(buffer);
                    //   // sound.play(); //soundClock.restart();
                    //}

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


long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal
    for (int i = 0; i < sizeof(ctrlpoints); i++)
    {
        cout << ctrlpoints[i] << endl;
    }

    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
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
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);
        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
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
