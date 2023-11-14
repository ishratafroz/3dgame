#ifndef CURVED_ROAD_H
#define CURVED_ROAD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "shader.h"

// Vertex structure
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};


class CurvedRoad {
public:
    // Constructor
    CurvedRoad(const std::string& texturePath,
        const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
        int numSegments, float roadWidth);

    // Destructor
    ~CurvedRoad();

    // Initialize the road (setup VAO, VBO, etc.)
    void init();

    // Render the road
    void draw(Shader& shader, glm::mat4& model);

private:
    GLuint VAO, VBO, texture;
    std::vector<Vertex> vertices;
    std::string texturePath;
    glm::vec3 p0, p1, p2, p3;
    int numSegments;
    float roadWidth;

    // Load texture
    GLuint loadTexture(const std::string& filename);

    // Create road mesh
    void createMesh();

    // Tessellate Bezier curve
    std::vector<glm::vec3> tessellateBezier();

    // Bezier curve point calculation
    glm::vec3 bezierPoint(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t);
};

#endif // CURVED_ROAD_H
