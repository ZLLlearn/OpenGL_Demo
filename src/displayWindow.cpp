#include <iostream>
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "displayWindow.h"


BaseWindow::BaseWindow(const std::string& name) : m_name(name) {}

BaseWindow::~BaseWindow() {}


class OpenGLWindow : public BaseWindow {
public:
    OpenGLWindow(const std::string& name, uint16_t width, uint16_t height);
    ~OpenGLWindow();

    void setWindowSize(uint16_t width, uint16_t height) override;
    void activateContext() override;
    void processInput() override;
    bool paint() override;
    void setState(StateType type, uint32_t targetVal) override;

private:
    void setColor(uint32_t color);

    GLFWwindow* m_window;
    bool closeWindow = false;

};


OpenGLWindow::OpenGLWindow(const std::string& name, uint16_t width, uint16_t height) : BaseWindow(name) {
    m_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
}

OpenGLWindow::~OpenGLWindow() {
    glfwTerminate();
}

void OpenGLWindow::setWindowSize(uint16_t width, uint16_t height) {
    glViewport(0, 0, width, height);
}

void OpenGLWindow::activateContext() {
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void OpenGLWindow::processInput() {
    switch (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
    case GLFW_PRESS:
        closeWindow = true;
        break;
    default:
        break;
    }
}

bool OpenGLWindow::paint() {
    if (closeWindow || glfwWindowShouldClose(m_window)) {
        return false;
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();

    return true;
}

void OpenGLWindow::setState(StateType type, uint32_t targetVal) {
    switch (type)
    {
    case StateType::BackColor:
        setColor(targetVal);
        break;
    
    default:
        break;
    }
}

void OpenGLWindow::setColor(uint32_t color) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


windowPtr displayWindowFactory::createDisplayWindow(const std::string& name, uint16_t width, uint16_t height, GraphicsAPI api) {
    switch (api)
    {
    case GraphicsAPI::OpenGL_4_5:
        return createOpenGLWindow(name, width, height);
    default:
        break;
    }
    return nullptr;
}

windowPtr displayWindowFactory::createOpenGLWindow(const std::string& name, uint16_t width, uint16_t height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return std::make_unique<OpenGLWindow>(name, width, height);
}

