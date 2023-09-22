#include <iostream>
#include <math.h>
#include "displayWindow.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "drawManager.h"
#include "inputManager.h"
#include "camera.h"

int main() 
{
    const uint32_t windowWidth = 1200, windowHeight = 900;
    displayWindowFactory windowFactory;
    auto mainWindow = windowFactory.createDisplayWindow("main window", windowWidth, windowHeight);
    mainWindow->activateContext();

    Shader base_2Dshader("resources/shaderCodes/base_2D_vs.glsl", "resources/shaderCodes/base_2D_fs.glsl");
    Shader base_3Dshader("resources/shaderCodes/base_3D_vs.glsl", "resources/shaderCodes/base_3D_fs.glsl");
    Shader base_pbrshader("resources/shaderCodes/base_pbr_vs.glsl", "resources/shaderCodes/base_pbr_fs.glsl");

    auto& drawManager = DrawManager::getDrawManager();
    auto trangleVao = drawManager.initVAO();
    auto cubeVao = drawManager.initVAO();
    auto sphereVao = drawManager.initVAO();
    
    auto& inputManager = InputManager::getInputManager();
    auto mCamera = std::make_shared<Camera>();
    inputManager.setCamera(mCamera);
    mCamera->fitView(windowWidth, windowHeight);

    int rows = 7, columns = 7;
    float spacing = 2.5f;
    base_pbrshader.activate();
    base_pbrshader.setVec("albedo", 0.5f, 0.0f, 0.0f);
    base_pbrshader.setFloat("ao", 1.0f);

    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
    };

    while (mainWindow->paint()) {
        mainWindow->setState(StateType::BackColor, 0x00ff00ff);
        mainWindow->processInput(inputManager);

        //base_2Dshader.activate();
        //drawManager.drawTriangle(trangleVao);

        //base_3Dshader.activate();
        //base_3Dshader.setMat("model", glm::mat4(1.0f));
        //base_3Dshader.setMat("view", mCamera->getViewMatrix());
        //base_3Dshader.setMat("proj", mCamera->getProjMatrix());
        //base_3Dshader.setVec("objectColor", 0.8f, 0.0f, 0.0f);
        //drawManager.drawCube(cubeVao);

        base_pbrshader.activate();
        base_pbrshader.setMat("view", mCamera->getViewMatrix());
        base_pbrshader.setMat("proj", mCamera->getProjMatrix());
        base_pbrshader.setVec("cameraPos", mCamera->getPosition());
        auto model = glm::mat4(1.0f);
        for (int row = 0; row < rows; ++row) {
            base_pbrshader.setFloat("metallic", static_cast<float>(row) / static_cast<float>(rows));
            for (int column = 0; column < columns; ++column) {
                base_pbrshader.setFloat("roughness", glm::clamp(static_cast<float>(column) / static_cast<float>(columns), 0.05f, 1.0f));

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((column - (columns / 2)) * spacing, (row - (rows / 2)) * spacing, 0.0f));
                base_pbrshader.setMat("model", model);
                base_pbrshader.setMat("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
                drawManager.drawSphere(sphereVao);
            }
        }

        auto timeFactor = inputManager.getCurrentFrame();
        glm::vec3 newPos = glm::vec3(sin(timeFactor) * 5.0f, sin(timeFactor / 2.0f) * 5.0f, 10.0f);
        base_pbrshader.setVec("lightPositions", newPos);
        base_pbrshader.setVec("lightColors", 300.0f, 300.0f, 300.0f);

        model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.2f));
        base_pbrshader.setMat("model", model);
        base_pbrshader.setMat("normalMatrix", glm::transpose(glm::inverse(glm::mat3(1.0f))));
        drawManager.drawSphere(sphereVao);
    }

    return 0;
}