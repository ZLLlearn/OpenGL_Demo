#include <iostream>
#include "displayWindow.h"
#include "shader.h"
#include "drawManager.h"
#include "camera.h"

int main() 
{
    displayWindowFactory windowFactory;
    auto mainWindow = windowFactory.createDisplayWindow("main window", 1980, 1080);
    mainWindow->activateContext();

    Shader tshader("resources/shaderCodes/triangle_vs.glsl", "resources/shaderCodes/triangle_fs.glsl");
    Shader cshader("resources/shaderCodes/cube_vs.glsl", "resources/shaderCodes/cube_vs.glsl");

    auto& drawManager = DrawManager::getDrawManager();
    auto trangleVao = drawManager.initVAO();
    auto cubeVao = drawManager.initVAO();
    
    Camera camera;

    while (mainWindow->paint()) {
        mainWindow->setState(StateType::BackColor, 0x00ff00ff);
        //mainWindow->processInput();

        //tshader.activate();
        //drawManager.drawTriangle(trangleVao);

        cshader.activate();
        cshader.setMat("model", glm::mat4(1.0f));
        cshader.setMat("view", camera.getViewMatrix());
        cshader.setMat("proj", camera.getProjMatrix());
        drawManager.drawCube(cubeVao);
    }

    return 0;
}