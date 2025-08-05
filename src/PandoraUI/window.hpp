#ifndef PANDORAUI_WINDOW_HPP
#define PANDORAUI_WINDOW_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/string.hpp"
#include "PandoraEX/size2.hpp"
#include "windowOpts.hpp"
#include "PandoraEX/color.hpp"
#include "PandoraEX/asyncList.hpp"
#include "PandoraEX/property.hpp"
#include "PandoraEX/event.hpp"
#include "UIElements/UIElement.hpp"

class GLFWwindow;
namespace std
{
    class thread;
}

namespace PandoraUI
{

    Class(Window)
    {
        bool _isInitialized = false;

        GLFWwindow *_window = NULL;
        std::thread *_thread = NULL;
        long _lastFrameTime = 0;

        void constructClass();

        void renderGUI();


    public:
        GPU_API gpu_api = GPU_API::OPENGL;
        UsecaseType usecase = UsecaseType::GUI_WINDOW;
        bool GPUAcceleration = true;
        PandoraEX::Property<PandoraEX::Color> backgroundColor = PandoraEX::Color(0,0,0,1);

        PandoraEX::Property<PandoraEX::String> title = PandoraEX::String("EdenFX Window");
        PandoraEX::Property<PandoraEX::Size2> size = PandoraEX::Size2(800, 600);
        PandoraEX::Property<int> fps = 60;


        PandoraEX::AsyncList<UIElement> guiLayer;

        PandoraEX::ControlledEvent<Window, true, Window*> onRender;
        PandoraEX::ControlledEvent<Window, true, Window*> onLateUpdate;

        Window(PandoraEX::String title = "EdenFX Window");
        Window(int width, int height, PandoraEX::String title = "EdenFX Window");

        Window(const Window &other);
        Window &operator=(const Window &other);

        ~Window();

        void initialize();

        void setFPS(int fps);
        
        void open();
        void close();
        void waitForExit();

        void render();
        void update();

    };
}

#endif // PANDORAUI_WINDOW_HPP