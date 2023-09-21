#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <memory>


struct InputModifiers {
    bool isCtrlPressed = true;
    bool isShiftPressed = false;
    bool isAltPressed = false;
};

struct KeyInput {
    enum class Type {
        None,
        KeyPressed,
        KeyReleased,
    };
    enum class Key {
        None,
        Space,
        Key0,
        Key1,
        Key2,
        Key3,
        Q,
        E,
        W,
        A,
        S,
        D,
    };

    Type type{Type::None};
    Key key{Key::None};
    InputModifiers mode{};
    bool isChange();
    void reset();
};

struct MouseInput {
    enum class Type {
        None,
        ButtonDown,
        ButtonUp,
        ButtonDoubleClick,
        Move,
        Wheel,
        Click,
    };
    enum class Button {
        None,
        Left,
        Middle,
        Right,
    };
    struct Pos {
        double x;
        double y;
    };
    
    Type type{Type::None};
    Button button{Button::None};
    Pos prePos{0, 0};
    Pos pos{0, 0};
    double wheelDelta{0.0};
    InputModifiers mode{};

    bool isChange();
    void reset();
};

class Camera;
class InputManager {
public:
    static InputManager& getInputManager();

    void setCamera(const std::shared_ptr<Camera>& camera);
    void setFrame(float frame);
    void setKeyInput(KeyInput::Type type, KeyInput::Key key, InputModifiers* mode = nullptr);
    void setMouseInput(MouseInput::Type type, MouseInput::Button button, double x, double y, InputModifiers* mode = nullptr);
    void setMouseZoom(double wheelDelta);
    void updateCamera();

private:
    InputManager();
    ~InputManager() = default;

    void keyEvent();
    void mouseEvent();

    double m_frame{0.0};
    double m_preFrame{0.0};
    bool isFirst{true};
    std::unique_ptr<KeyInput> m_keyInput;
    std::unique_ptr<MouseInput> m_mouseInput;
    std::shared_ptr<Camera> m_camera;

};


#endif