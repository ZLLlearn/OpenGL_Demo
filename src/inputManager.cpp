#include <iostream>
#include "camera.h"
#include "inputManager.h"


bool KeyInput::isChange() {
    return type != Type::None || key != Key::None;
}

void KeyInput::reset() {
    type = Type::None;
    key = Key::None;
    mode = {};
}

bool MouseInput::isChange() {
    return type != Type::None || button != Button::None || wheelDelta != 0.0;
}

void MouseInput::reset() {
    type = Type::None;
    button = Button::None;
    wheelDelta = 0.0;
    //mode = {};
}

void InputManager::setCamera(const std::shared_ptr<Camera>& camera) {
    m_camera = camera;
}

InputManager& InputManager::getInputManager() {
    static InputManager inputManager;
    return inputManager;
}

InputManager::InputManager() : m_keyInput(std::make_unique<KeyInput>()), m_mouseInput(std::make_unique<MouseInput>()), m_camera(nullptr) {}

void InputManager::setFrame(float frame) {
    m_preFrame = m_frame;
    m_frame = frame;
    if (m_frame > std::pow(10, 9)) {
        m_preFrame = m_frame = 0.0;
        std::cout << "frame is too big" << std::endl;
    }
}

void InputManager::setKeyInput(KeyInput::Type type, KeyInput::Key key, InputModifiers* mode) {
    m_keyInput->type = type;
    m_keyInput->key = key;
    if (mode) {
        m_keyInput->mode = *mode;
    }
}

void InputManager::setMouseInput(MouseInput::Type type, MouseInput::Button button, double x, double y, InputModifiers* mode) {
    m_mouseInput->type = type;
    m_mouseInput->button = button;
    m_mouseInput->prePos = m_mouseInput->pos;
    m_mouseInput->pos.x = x;
    m_mouseInput->pos.y = y;
    if (mode) {
        //m_mouseInput->mode = *mode;
        m_mouseInput->mode.isCtrlPressed = !m_mouseInput->mode.isCtrlPressed;
    }
}

void InputManager::setMouseZoom(double wheelDelta) {
    m_mouseInput->wheelDelta = wheelDelta;
}

void InputManager::updateCamera() {
    if (m_keyInput->isChange()) {
        keyEvent();
        m_keyInput->reset();
    }
    if (m_mouseInput->isChange()) {
        mouseEvent();
        m_mouseInput->reset();
    }
}

void InputManager::keyEvent() {
    float speed = 2.0;
    double deltaTime = m_frame - m_preFrame;
    if (m_keyInput->mode.isShiftPressed) {
        speed *= 10;
    }
    switch (m_keyInput->key)
    {
    case KeyInput::Key::Q:
        m_camera->updatePosition(MoveDirection::Upward, speed * deltaTime);
        break;
    case KeyInput::Key::E:
        m_camera->updatePosition(MoveDirection::Downward, speed * deltaTime);
        break;
    case KeyInput::Key::W:
        m_camera->updatePosition(MoveDirection::Forward, speed * deltaTime);
        break;
    case KeyInput::Key::A:
        m_camera->updatePosition(MoveDirection::Leftward, speed * deltaTime);
        break;
    case KeyInput::Key::S:
        m_camera->updatePosition(MoveDirection::Backward, speed * deltaTime);
        break;
    case KeyInput::Key::D:
        m_camera->updatePosition(MoveDirection::Rightward, speed * deltaTime);
        break;
    case KeyInput::Key::Space:
        m_camera->updatePosition(MoveDirection::Reset, 0);
        break;
    default:
        break;
    }
}

void InputManager::mouseEvent() {
    if (m_mouseInput->wheelDelta != 0.0) {
        m_camera->updateZoomScale(m_mouseInput->wheelDelta);
    }

    if (isFirst) {
        m_mouseInput->prePos.x = m_mouseInput->pos.x;
        m_mouseInput->prePos.y = m_mouseInput->pos.y;
        isFirst = false;
        return;
    }
    double xOffset = m_mouseInput->pos.x - m_mouseInput->prePos.x;
    double yOffset = m_mouseInput->prePos.y - m_mouseInput->pos.y;
    m_mouseInput->prePos.x = m_mouseInput->pos.x;
    m_mouseInput->prePos.y = m_mouseInput->pos.y;

    if (!m_mouseInput->mode.isCtrlPressed) {
        m_camera->updateEuler(xOffset, yOffset);
    }
}

const double InputManager::getCurrentFrame() const {
    return m_frame;
}
