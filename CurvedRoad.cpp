#include "CurvedRoad.h"
#include "stb_image.h"
#include <iostream>

CurvedRoad::CurvedRoad(const std::string& texturePath,
    const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
    int numSegments, float roadWidth)
    : texturePath(texturePath),
    p0(p0), p1(p1), p2(p2), p3(p3),
    numSegments(numSegments), roadWidth(roadWidth) {
    init();
}

CurvedRoad::~CurvedRoad() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Handle texture deletion if necessary
}

void CurvedRoad::init() {
    // Load texture
    texture = loadTexture(texturePath);

    // Create road mesh
    createMesh();

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
// Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);
}

void CurvedRoad::draw(Shader& shader, glm::mat4& model) {
    shader.use();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindVertexArray(0);
}

GLuint CurvedRoad::loadTexture(const std::string& filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

void CurvedRoad::createMesh() {
    auto curvePoints = tessellateBezier();

    for (size_t i = 0; i < curvePoints.size() - 1; ++i) {
        glm::vec3 dir = glm::normalize(curvePoints[i + 1] - curvePoints[i]);
        glm::vec3 normal = glm::normalize(glm::vec3(-dir.y, dir.x, 0.0f)); // Assuming flat road surface
        glm::vec3 left = curvePoints[i] + normal * roadWidth;
        glm::vec3 right = curvePoints[i] - normal * roadWidth;
        glm::vec3 leftNext = curvePoints[i + 1] + normal * roadWidth;
        glm::vec3 rightNext = curvePoints[i + 1] - normal * roadWidth;

        float texCoordY = i / static_cast<float>(curvePoints.size() - 1);

        vertices.push_back({ left, normal, {0.0f, texCoordY} });
        vertices.push_back({ right, normal, {1.0f, texCoordY} });
        vertices.push_back({ leftNext, normal, {0.0f, texCoordY + 1.0f / (curvePoints.size() - 1)} });

        vertices.push_back({ right, normal, {1.0f, texCoordY} });
        vertices.push_back({ rightNext, normal, {1.0f, texCoordY + 1.0f / (curvePoints.size() - 1)} });
        vertices.push_back({ leftNext, normal, {0.0f, texCoordY + 1.0f / (curvePoints.size() - 1)} });
    }
}


std::vector<glm::vec3> CurvedRoad::tessellateBezier() {
    std::vector<glm::vec3> curvePoints;
    for (int i = 0; i <= numSegments; ++i) {
        float t = i / static_cast<float>(numSegments);
        curvePoints.push_back(bezierPoint(p0, p1, p2, p3, t));
    }
    return curvePoints;
}

glm::vec3 CurvedRoad::bezierPoint(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec3 point = uuu * p0;
    point += 3 * uu * t * p1;
    point += 3 * u * tt * p2;
    point += ttt * p3;

    return point;
}
