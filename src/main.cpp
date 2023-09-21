#include <iostream>
#include "displayWindow.h"
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

    Shader base_2Dshader("resources/shaderCodes/base_2D_vs.glsl", "resources/shaderCodes/base_2D_vs.glsl");
    Shader base_3Dshader("resources/shaderCodes/base_3D_vs.glsl", "resources/shaderCodes/base_3D_fs.glsl");

    auto& drawManager = DrawManager::getDrawManager();
    auto trangleVao = drawManager.initVAO();
    auto cubeVao = drawManager.initVAO();
    auto sphereVao = drawManager.initVAO();
    
    auto& inputManager = InputManager::getInputManager();
    auto mCamera = std::make_shared<Camera>();
    inputManager.setCamera(mCamera);
    mCamera->fitView(windowWidth, windowHeight);

    while (mainWindow->paint()) {
        mainWindow->setState(StateType::BackColor, 0x00ff00ff);
        mainWindow->processInput(inputManager);

        //base_2Dshader.activate();
        //drawManager.drawTriangle(trangleVao);

        base_3Dshader.activate();
        base_3Dshader.setMat("model", glm::mat4(1.0f));
        base_3Dshader.setMat("view", mCamera->getViewMatrix());
        base_3Dshader.setMat("proj", mCamera->getProjMatrix());
        base_3Dshader.setVec("objectColor", 0.8f, 0.0f, 0.0f);
        //drawManager.drawCube(cubeVao);
        drawManager.drawSphere(sphereVao);
    }

    return 0;
}