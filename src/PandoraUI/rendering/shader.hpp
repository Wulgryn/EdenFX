#ifndef PANDORAUI_SHADER_HPP
#define PANDORAUI_SHADER_HPP

#include "PandoraEX/string.hpp"
#include "PandoraEX/object.hpp"
#include "PandoraEX/event.hpp"
#include "PandoraEX/dictionary.hpp"

namespace PandoraUI
{
    namespace StaticShaderSource
    {
        extern const char *ui_element_vs;
        extern const char *ui_element_fs;
        extern const char *ui_image_element_vs;
        extern const char *ui_image_element_fs;
        extern const char *ui_text_element_vs;
        extern const char *ui_text_element_fs;
    }

    Class(ShaderProgram)
    {
        // friend void Shader::initialize();
        // friend void Shader::initializeAll();

        unsigned int __compileShader(unsigned int type, const PandoraEX::String &source);
        unsigned int __linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
        void __genObjects();
        unsigned int _program, _VAO, _VBO, _EBO = -1;
        bool _isInitialized = false;
        PandoraEX::Dictionary<PandoraEX::String, int> _uniformLocations;

    public:
        void initialize(const PandoraEX::String &vertexSource, const PandoraEX::String &fragmentSource);
        void use();

        unsigned int getProgram() const;
        bool isInitialized() const { return _isInitialized; }

        void bindUniform(const PandoraEX::String &name, bool override = false);
        unsigned int getUniformLocation(const PandoraEX::String &name) const;
    };

    class Shader;

    Class(ShaderEvent)
    {
    public:
        /// @brief Event that is triggered when the shader is used.
        /// @param shader& The shader that is being used.
        PandoraEX::ControlledEvent<Shader, true, ShaderProgram &> onUse;
        /// @brief Event that is triggered when the shader is initialized.
        /// @param shader The shader that has been initialized.
        PandoraEX::ControlledEvent<Shader, true, ShaderProgram &> onInitialize;
    };

    Class(Shader)
    {
        static unsigned int s_currentProgram;
        static unsigned int s_currentWindowContext;

        PandoraEX::String _vertexSource;
        PandoraEX::String _fragmentSource;

        PandoraEX::Dictionary<unsigned int, std::reference_wrapper<ShaderProgram>> _programs;
        PandoraEX::Dictionary<unsigned int, ShaderEvent> _shaderEvents;

    public:
        Shader(const PandoraEX::String &vertexSource, const PandoraEX::String &fragmentSource);

        void initialize();
        void initializeAll();

        /// @brief Checks if the shader is initialized for the current window context.
        /// @return True if the shader is initialized, false otherwise.
        bool isInitialized() const;

        /// @brief Gets the shader events. According to the current window context.
        /// @return A reference to the shader events.
        ShaderEvent& getEvents();

        void use();

        ShaderProgram &getProgram(bool initOnFail = false);
        // void bindUniform(const PandoraEX::String& name, bool override = false);
        // unsigned int getUniformLocation(const PandoraEX::String& name) const;
    };

    namespace StaticShaders
    {
        Shader &uiElementShader();
        Shader &uiImageElementShader();
        Shader &uiTextElementShader();
    }
}

#endif // PANDORAUI_SHADER_HPP