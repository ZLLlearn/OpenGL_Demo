#include <iostream>
#include <math.h>
#include "stb_image.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "displayWindow.h"
#include "shader.h"
#include "drawManager.h"
#include "inputManager.h"
#include "camera.h"
#include "framebuffer.h"
#include "texture.h"

int main() 
{
    const uint32_t windowWidth = 1200, windowHeight = 900;
    displayWindowFactory windowFactory;
    auto mainWindow = windowFactory.createDisplayWindow("main window", windowWidth, windowHeight);
    mainWindow->activateContext();

    Shader base_2DShader("resources/shaderCodes/base_2D_vs.glsl", "resources/shaderCodes/base_2D_fs.glsl");
    Shader base_3DShader("resources/shaderCodes/base_3D_vs.glsl", "resources/shaderCodes/base_3D_fs.glsl");
    Shader texture_2DShader("resources/shaderCodes/texture_2D_vs.glsl", "resources/shaderCodes/texture_2D_fs.glsl");
    Shader base_pbrShader("resources/shaderCodes/base_pbr_vs.glsl", "resources/shaderCodes/base_pbr_fs.glsl");
    Shader IBL_pbrShader("resources/shaderCodes/base_pbr_vs.glsl", "resources/shaderCodes/IBL_pbr_fs.glsl");
    Shader IBLTexture_pbrShader("resources/shaderCodes/base_pbr_vs.glsl", "resources/shaderCodes/IBL_texture_pbr_fs.glsl");
    Shader equirectanglarToCubemapShader("resources/shaderCodes/cubemap_vs.glsl", "resources/shaderCodes/equirectangularToCubemap_fs.glsl");
    Shader irradianceShader("resources/shaderCodes/cubemap_vs.glsl", "resources/shaderCodes/irradianceConvolution_fs.glsl");
    Shader prefilterShader("resources/shaderCodes/cubemap_vs.glsl", "resources/shaderCodes/prefilter_fs.glsl");
    Shader brdfShader("resources/shaderCodes/brdf_vs.glsl", "resources/shaderCodes/brdf_fs.glsl");
    Shader backgroundShader("resources/shaderCodes/background_vs.glsl", "resources/shaderCodes/background_fs.glsl");

    auto& drawManager = DrawManager::getDrawManager();
    auto trangleVao = drawManager.initVAO();
    auto quadVao = drawManager.initVAO();
    auto cubeVao = drawManager.initVAO();
    auto sphereVao = drawManager.initVAO();
    
    auto& inputManager = InputManager::getInputManager();
    auto mCamera = std::make_shared<Camera>();
    inputManager.setCamera(mCamera);
    mCamera->fitView(windowWidth, windowHeight);

    int rows = 5, columns = 5;
    float spacing = 2.5f;

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    texture_2DShader.setInt("texture2D", 0);

    base_pbrShader.activate();
    base_pbrShader.setVec("albedo", 0.5f, 0.0f, 0.0f);
    base_pbrShader.setFloat("ao", 1.0f);
    IBL_pbrShader.activate();
    IBL_pbrShader.setVec("albedo", 0.5f, 0.0f, 0.0f);
    IBL_pbrShader.setFloat("ao", 1.0f);
    IBL_pbrShader.setInt("irradianceMap", 0);
    IBL_pbrShader.setInt("prefilterMap", 1);
    IBL_pbrShader.setInt("brdfLUT", 2);
    IBLTexture_pbrShader.activate();
    //IBLTexture_pbrShader.setVec("albedo", 0.5f, 0.0f, 0.0f);
    //IBLTexture_pbrShader.setFloat("ao", 1.0f);
    IBLTexture_pbrShader.setInt("irradianceMap", 0);
    IBLTexture_pbrShader.setInt("prefilterMap", 1);
    IBLTexture_pbrShader.setInt("brdfLUT", 2);
    IBLTexture_pbrShader.setInt("albedoMap", 3);
    IBLTexture_pbrShader.setInt("normalMap", 4);
    IBLTexture_pbrShader.setInt("metallicMap", 5);
    IBLTexture_pbrShader.setInt("roughnessMap", 6);
    IBLTexture_pbrShader.setInt("aoMap", 7);

    backgroundShader.activate();
    backgroundShader.setInt("environmentMap", 0);

    Framebuffer captureFbo;
    uint32_t rbo = captureFbo.genRenderbuffer();
    captureFbo.activate();
    captureFbo.setRenderbufferSize(rbo, 512, 512);
    captureFbo.genAttachmentR(rbo);

    Texture hdrTexture;
    hdrTexture.loadTexture("resources/textures/industrial_sunset_02_16k.hdr");

    Texture albedo_titanium;
    albedo_titanium.loadTexture("resources/textures/Titanium-Scuffed_basecolor.png");
    Texture normal_titanium;
    normal_titanium.loadTexture("resources/textures/Titanium-Scuffed_normal.png");
    Texture metallic_titanium;
    metallic_titanium.loadTexture("resources/textures/Titanium-Scuffed_metallic.png");
    Texture roughness_titanium;
    roughness_titanium.loadTexture("resources/textures/Titanium-Scuffed_roughness.png");
    Texture ao_titanium;
    ao_titanium.loadTexture("resources/textures/worn-shiny-metal-ao.png");

    Texture envCubemap(TextureType::Cube);
    envCubemap.genTexture(512, 512, nullptr, true, true);

    equirectanglarToCubemapShader.activate();
    equirectanglarToCubemapShader.setInt("equirectangularMap", 0);
    equirectanglarToCubemapShader.setMat("proj", captureProjection);
    hdrTexture.activate(0);

    mainWindow->setWindowSize(512, 512);

    captureFbo.activate();
    for (int i = 0; i < 6; ++i) {
        equirectanglarToCubemapShader.setMat("view", captureViews[i]);
        captureFbo.genAttachmentT(TextureType::Cube, envCubemap.getId(), i);
        mainWindow->setState(StateType::Buffer, 0);

        drawManager.drawCube(cubeVao);
    }

    Texture irradianceMap(TextureType::Cube);
    irradianceMap.genTexture(32, 32);

    captureFbo.activate();
    captureFbo.setRenderbufferSize(rbo, 32, 32);

    irradianceShader.activate();
    irradianceShader.setInt("environmentMap", 0);
    irradianceShader.setMat("proj", captureProjection);
    envCubemap.activate(0);

    mainWindow->setWindowSize(32, 32);

    for (int i = 0; i < 6; ++i) {
        irradianceShader.setMat("view", captureViews[i]);
        captureFbo.genAttachmentT(TextureType::Cube, irradianceMap.getId(), i);
        mainWindow->setState(StateType::Buffer, 0);

        drawManager.drawCube(cubeVao);
    }

    Texture prefilterMap(TextureType::Cube);
    prefilterMap.genTexture(128, 128, nullptr, true, true);

    prefilterShader.activate();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat("proj", captureProjection);
    envCubemap.activate(0);

    captureFbo.activate();
    for (int mip = 0, maxMipLevels = 5; mip < maxMipLevels; ++mip) {
        uint32_t mipWidth = static_cast<uint32_t>(128 * std::pow(0.5, mip));
        uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));
        captureFbo.setRenderbufferSize(rbo, mipWidth, mipHeight);

        mainWindow->setWindowSize(mipWidth, mipHeight);

        float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
        prefilterShader.setFloat("roughness", roughness);
        for (int i = 0; i < 6; ++i) {
            prefilterShader.setMat("view", captureViews[i]);
            captureFbo.genAttachmentT(TextureType::Cube, prefilterMap.getId(), i, mip);
            mainWindow->setState(StateType::Buffer, 0);

            drawManager.drawCube(cubeVao);
        }
    }

    Texture brdfLUTTexture;
    brdfLUTTexture.genTexture(512, 512, nullptr, false, false);

    captureFbo.activate();
    captureFbo.setRenderbufferSize(rbo, 512, 512);
    captureFbo.genAttachmentT(TextureType::_2D, brdfLUTTexture.getId());

    mainWindow->setWindowSize(512, 512);
    
    brdfShader.activate();
    mainWindow->setState(StateType::Buffer, 0);

    drawManager.drawQuad(quadVao);

    Framebuffer::reset();

    backgroundShader.activate();
    backgroundShader.setMat("proj", mCamera->getProjMatrix());

    mainWindow->setWindowSize(windowWidth, windowHeight);

    while (mainWindow->paint()) {
        mainWindow->processInput(inputManager);
        mainWindow->setState(StateType::Buffer, 0);
        mainWindow->setState(StateType::BackColor, 0x00ff00ff);

        //base_2DShader.activate();
        //drawManager.drawTriangle(trangleVao);

        //base_3DShader.activate();
        //base_3DShader.setMat("model", glm::mat4(1.0f));
        //base_3DShader.setMat("view", mCamera->getViewMatrix());
        //base_3DShader.setMat("proj", mCamera->getProjMatrix());
        //base_3DShader.setVec("objectColor", 0.8f, 0.0f, 0.0f);
        //drawManager.drawCube(cubeVao);

        //texture_2DShader.activate();
        //brdfLUTTexture.activate(0);
        //drawManager.drawQuad(quadVao);

        auto& view = mCamera->getViewMatrix();
        auto& proj = mCamera->getProjMatrix();
        IBL_pbrShader.activate();
        IBL_pbrShader.setMat("view", view);
        IBL_pbrShader.setMat("proj", proj);
        IBL_pbrShader.setVec("cameraPos", mCamera->getPosition());

        irradianceMap.activate(0);
        prefilterMap.activate(1);
        brdfLUTTexture.activate(2);
        albedo_titanium.activate(3);
        normal_titanium.activate(4);
        metallic_titanium.activate(5);
        roughness_titanium.activate(6);
        ao_titanium.activate(7);

        auto model = glm::mat4(1.0f);
        for (int row = 0; row < rows; ++row) {
            IBL_pbrShader.setFloat("metallic", static_cast<float>(row) / static_cast<float>(rows));
            for (int column = 0; column < columns; ++column) {
                IBL_pbrShader.setFloat("roughness", glm::clamp(static_cast<float>(column) / static_cast<float>(columns), 0.05f, 1.0f));

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((column - (columns / 2)) * spacing, (row - (rows / 2)) * spacing, 0.0f));
                IBL_pbrShader.setMat("model", model);
                IBL_pbrShader.setMat("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
                drawManager.drawSphere(sphereVao);
            }
        }

        auto timeFactor = inputManager.getCurrentFrame();
        glm::vec3 newPos = glm::vec3(sin(timeFactor) * 5.0f, sin(timeFactor / 2.0f) * 5.0f, 10.0f);
        IBL_pbrShader.setVec("lightPositions", newPos);
        IBL_pbrShader.setVec("lightColors", 300.0f, 300.0f, 300.0f);

        model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.2f));
        IBL_pbrShader.setMat("model", model);
        IBL_pbrShader.setMat("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
        drawManager.drawSphere(sphereVao);

        backgroundShader.activate();
        backgroundShader.setMat("view", view);
        envCubemap.activate(0);
        //irradianceMap.activate(0);
        //prefilterMap.activate(0);
        drawManager.drawCube(cubeVao);
    }

    return 0;
}