#include "GuiApp.h"

int main()
{
    GuiApp app;
    if(!app.Initialize())
        return 1;

    app.Run();
    app.Shutdown();
    return 0;
}
