#ifndef GUI_APP_H
#define GUI_APP_H

#include <memory>

#include <GLFW/glfw3.h>

#include "GuiState.h"
#include "InputPanel.h"
#include "OutputPanel.h"
#include "ConversionController.h"

class GuiApp
{
public:
    GuiApp();
    ~GuiApp();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void BeginFrame();
    void RenderFrame();

    GLFWwindow* window;
    GuiState state;
    InputPanel inputPanel;
    OutputPanel outputPanel;
    ConversionController conversionController;
};

#endif
