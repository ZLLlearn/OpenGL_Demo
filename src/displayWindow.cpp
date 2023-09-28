#include <iostream>
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "inputManager.h"
#include "displayWindow.h"

BaseWindow::BaseWindow(const std::string& name) : m_name(name) {}

BaseWindow::~BaseWindow() {}


class OpenGLWindow : public BaseWindow {
public:
    OpenGLWindow(const std::string& name, uint16_t width, uint16_t height);
    ~OpenGLWindow();

    void setWindowSize(uint16_t width, uint16_t height) override;
    uint32_t getWindowWidth() override;
    uint32_t getWindowHeight() override;
    void activateContext() override;
    void processInput(InputManager&) override;
    bool paint() override;
    void setState(StateType type, uint32_t targetVal) override;

private:
    void setColor(uint32_t color);
    void setBuffer();

    static int s_width, s_height;
    static bool s_hasMouseInput, s_hasWheelInput;
    static double s_xPos, s_yPos;
    static double s_wheelDelta;
    GLFWwindow* m_window;
    bool closeWindow = false;

};

int OpenGLWindow::s_width = 0;
int OpenGLWindow::s_height = 0;
bool OpenGLWindow::s_hasMouseInput = false;
bool OpenGLWindow::s_hasWheelInput = false;
double OpenGLWindow::s_xPos = 0.0;
double OpenGLWindow::s_yPos = 0.0;
double OpenGLWindow::s_wheelDelta = 0.0;

OpenGLWindow::OpenGLWindow(const std::string& name, uint16_t width, uint16_t height) : BaseWindow(name) {
    m_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        s_width = width;
        s_height = height;
        glViewport(0, 0, width, height);});
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
       s_hasMouseInput = true;
       s_xPos = x, s_yPos = y;
       });
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        s_hasWheelInput = true;
        s_wheelDelta = yOffset;
    });
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

OpenGLWindow::~OpenGLWindow() {
    glfwTerminate();
}

void OpenGLWindow::setWindowSize(uint16_t width, uint16_t height) {
    glViewport(0, 0, width, height);
}

uint32_t OpenGLWindow::getWindowWidth() {
    return s_width;
}

uint32_t OpenGLWindow::getWindowHeight() {
    return s_height;
}

void OpenGLWindow::activateContext() {
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void OpenGLWindow::processInput(InputManager& inputManager) {
    InputModifiers* ptr1 = nullptr;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        InputModifiers mode1 {};
        ptr1 = &mode1;
        mode1.isShiftPressed = true;
    }
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        closeWindow = true;
    }
    else if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::Q, ptr1);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::E, ptr1);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::W, ptr1);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::A, ptr1);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::S, ptr1);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::D, ptr1);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        inputManager.setKeyInput(KeyInput::Type::KeyPressed, KeyInput::Key::Space);
    }

    if (s_hasMouseInput) {
        InputModifiers* ptr2 = nullptr;
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            InputModifiers mode2 {};
            mode2.isCtrlPressed = true;
            ptr2 = &mode2;
        }
        inputManager.setMouseInput(MouseInput::Type::ButtonDown, MouseInput::Button::None, s_xPos, s_yPos, ptr2);
        s_wheelDelta = 0.0;
        s_hasMouseInput = false;
    }

    if (s_hasWheelInput) {
        inputManager.setMouseZoom(s_wheelDelta);
        s_wheelDelta = 0.0;
        s_hasWheelInput = false;
    }

    inputManager.setFrame(glfwGetTime());
    inputManager.updateCamera();
}

bool OpenGLWindow::paint() {
    if (closeWindow || glfwWindowShouldClose(m_window)) {
        return false;
    }
    glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(m_window);
    glfwPollEvents();

    return true;
}

void OpenGLWindow::setState(StateType type, uint32_t targetVal) {
    switch (type)
    {
    case StateType::Buffer:
        setBuffer();
        break;
    case StateType::BackColor:
        setColor(targetVal);
        break;
    default:
        break;
    }
}

void OpenGLWindow::setBuffer() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLWindow::setColor(uint32_t color) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

