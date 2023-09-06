#include <iostream>
#include "displayWindow.h"

int main() 
{
    displayWindowFactory windowFactory;
    auto mainWindow = windowFactory.createDisplayWindow("main window", 800, 600);
    mainWindow->activateContext();

    while (mainWindow->paint()) {
        mainWindow->setState(StateType::BackColor, 0x00ff00ff);
        mainWindow->processInput();
    }

    return 0;
}