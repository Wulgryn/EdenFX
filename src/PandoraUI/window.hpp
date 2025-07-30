#ifndef PANDORAUI_WINDOW_HPP
#define PANDORAUI_WINDOW_HPP

#include "PandoraEX/string.hpp"
#include "PandoraEX/size2.hpp"
#include "windowOpts.hpp"
#include "PandoraEX/event.hpp"
#include "PandoraEX/color.hpp"
#include "PandoraEX/asyncList.hpp"
#include "UIElements/UIElement.hpp"

class GLFWwindow;
namespace std
{
    class thread;
}

namespace PandoraUI
{

    class Window
    {
        PandoraEX::String _title;
        PandoraEX::Size2 _size;
        bool _isInitialized = false;

        GLFWwindow *_window = NULL;

        int _fps = 60;
        std::thread *_thread = NULL;
        long _lastFrameTime = 0;

        void renderGUI();
    public:
        GPU_API gpu_api = GPU_API::OPENGL;
        UsecaseType usecase = UsecaseType::GUI_WINDOW;
        bool GPUAcceleration = true;
        PandoraEX::Color backgroundColor = PandoraEX::Color(0, 0, 0, 1);

        PandoraEX::AsyncList<UIElement> guiLayer;

        PandoraEX::Event<Window*> onRender;
        PandoraEX::Event<Window*> onLateUpdate;

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