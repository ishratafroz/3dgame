#ifndef TUNNEL_H
#define TUNNEL_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "shader.h"
#include "CurvedRoad.h"

// Vertex structure

class Tunnel {
public:
    Tunnel(const std::string& texturePath,
        const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
        int numSegments, float tunnelRadius, int circlePoints);

    ~Tunnel();

    void init();
    void draw(Shader& shader, glm::mat4& model);

private:
    GLuint VAO, VBO, texture;
    std::vector<Vertex> vertices;
    std::string texturePath;
    glm::vec3 p0, p1, p2, p3;
    int numSegments, circlePoints;
    float tunnelRadius;

    GLuint loadTexture(const std::string& filename);
    void createMesh();
    std::vector<glm::vec3> tessellateBezier();
    glm::vec3 bezierPoint(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t);
};

#endif // TUNNEL_H
