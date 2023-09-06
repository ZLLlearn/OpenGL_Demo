#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H


#include <memory>
#include <string>
#include <vector>
#include "renderingType.h"


class BaseWindow {
public:
    explicit BaseWindow(const std::string& name);
    virtual ~BaseWindow() = 0;

    virtual void setWindowSize(uint16_t width, uint16_t height) = 0;
    virtual void activateContext() = 0;
    virtual void processInput() = 0;
    virtual bool paint() = 0;
    virtual void setState(StateType type, uint32_t targetVal) = 0;

protected:
    std::string m_name;

};
using windowPtr = std::unique_ptr<BaseWindow>;

class displayWindowFactory {
public:
    displayWindowFactory() = default;
    ~displayWindowFactory() = default;
    windowPtr createDisplayWindow(const std::string& name, uint16_t width, uint16_t height, GraphicsAPI api = GraphicsAPI::OpenGL_4_5);

private:
    windowPtr createOpenGLWindow(const std::string& name, uint16_t width, uint16_t height);
    
    std::vector<windowPtr> m_windows;

};


#endif