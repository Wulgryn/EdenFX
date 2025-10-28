#ifndef PANDORAUI_IWINDOW_HPP
#define PANDORAUI_IWINDOW_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/property.hpp"
#include "PandoraEX/color.hpp"
#include "PandoraEX/observableSize2.hpp"
#include "PandoraUI/windowManager.hpp"


class GLFWwindow;
namespace std
{
    class thread;
}

namespace PandoraUI
{
    Class(IWindow)
    {
    private:
        friend void WindowManager::startUpdateLoop();
        static std::atomic<unsigned long long> s_nextID;
        
        void _update();
        void _render();
        void _close();
        unsigned long long _id;
    protected:
        bool _isInitialized = false;

        GLFWwindow* _glfWindow = nullptr;
        std::thread* _updateThread = nullptr;
        double _lastFrameTime = 0;

        virtual void render() {}
    public:
        PandoraEX::Color backgroundColor = PandoraEX::Color(0.1, 0.1, 0.1, 1.0);

        PandoraEX::Property<bool> isVSyncEnabled = true;
        PandoraEX::Property<PandoraEX::String> title = PandoraEX::String("EdenFX Window");
        PandoraEX::ObservableSize2 size = PandoraEX::ObservableSize2(800, 600);
        PandoraEX::Property<int> fps = 60;


        PandoraEX::ControlledEvent<IWindow, true, IWindow&> onRender;
        PandoraEX::ControlledEvent<IWindow, true, IWindow&> onLateRender;
        PandoraEX::ControlledEvent<IWindow, true, IWindow&> onUpdate;
        PandoraEX::ControlledEvent<IWindow, true, IWindow&> onLateUpdate;

        PandoraEX::ControlledEvent<IWindow, true, IWindow&> onGladInitialize;


        IWindow();
        unsigned long long id() const { return _id; }

        virtual void initialize();

        virtual void show();
        virtual void close();
        virtual void waitForExit();


    };
}

#endif // PANDORAUI_IWINDOW_HPP