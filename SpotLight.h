//
//  spotLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <iostream>
#include <string>
using namespace std;

class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    float k_c;
    float k_l;
    float k_q;
    float inner_circle;
    float outer_circle;
    int Number;

    SpotLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num, float in_circle, float out_circle, float dirX, float dirY, float dirZ) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        inner_circle = in_circle;
        outer_circle = out_circle;
        direction = glm::vec3(dirX, dirY, dirZ);
        Number = num-1;
    }
    SpotLight() {
        position = glm::vec3(0, 0, 0);
        ambient = glm::vec3(1.0f, 1.0f, 1.0f);
        diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        specular = glm::vec3(1.0f, 1.0f, 1.0f);
        k_c = 1.0f;
        k_l = 0.09f;
        k_q = 0.032f;
        inner_circle = glm::cos(glm::radians(50.5f));
        outer_circle = glm::cos(glm::radians(80.0f));
        direction = glm::vec3(0, -1, 0);
        Number = 1;
    }
    void setUpspotLight(Shader& lightingShader)
    {
        lightingShader.use();
        
        lightingShader.setVec3("spotLights[0].position", position);
        lightingShader.setVec3("spotLights[0].ambient", ambientOn*ambient);
        lightingShader.setVec3("spotLights[0].diffuse", diffuseOn * diffuse);
        lightingShader.setVec3("spotLights[0].specular", specularOn * specular);
        lightingShader.setFloat("spotLights[0].k_c", k_c);
        lightingShader.setFloat("spotLights[0].k_l", k_l);
        lightingShader.setFloat("spotLights[0].k_q", k_q);
        lightingShader.setFloat("spotLights[0].inner_circle", inner_circle);
        lightingShader.setFloat("spotLights[0].outer_circle", outer_circle);
        lightingShader.setVec3("spotLights[0].direction", direction);
        lightingShader.setBool("slighton", true);
    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnDiffuseOn()
    {
        ambientOn = 0.0;
        diffuseOn = 1.0;
        specularOn = 0.0;
    }
    void turnDiffuseOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnSpecularOn()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* spotLight_h */
