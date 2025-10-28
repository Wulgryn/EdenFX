#include "shader.hpp"
#include "../windowManager.hpp"
#include "../IWindow.hpp"

#include "PandoraDebug/console.hpp"

#include "glad/gl.h"

using namespace PandoraDebug;
using namespace PandoraUI;
using namespace PandoraEX;

/**=======================================================================================================================*
 **                                                  REGION STATICSHADERSOURCES
 *========================================================================================================================*/
#pragma region STATICSHADERSOURCES

const char *StaticShaderSource::ui_element_vs = R"(#version 330 core
layout(location=0) in vec2 aUV;   // [0..1] unit quad

uniform vec2 uPos;                 // bal-felső px
uniform vec2 uSize;                // méret px
uniform vec2 uScreen;              // framebuffer (w,h) px

out vec2 vLocal;                   // -size/2 .. +size/2

void main() {
    vec2 px = uPos + aUV * uSize; // képernyő px
    vLocal  = (aUV - 0.5) * uSize;

    vec2 ndc;
    ndc.x = (px.x / uScreen.x) * 2.0 - 1.0;
    ndc.y = 1.0 - (px.y / uScreen.y) * 2.0;
    gl_Position = vec4(ndc, 0.0, 1.0);
}
)";

const char *StaticShaderSource::ui_element_fs = R"(#version 330 core
in vec2 vLocal;
out vec4 FragColor;

uniform vec2  uSize;
uniform float uRadius;      // px
uniform float uBorder;      // px
uniform float uFeather;     // px (AA)
uniform vec4  uFillColor;   // RGBA
uniform vec4  uBorderColor; // RGBA

float sdRoundRect(vec2 p, vec2 halfSize, float r) {
    r = min(r, min(halfSize.x, halfSize.y));
    vec2 q = abs(p) - (halfSize - vec2(r));
    return length(max(q, 0.0)) - r;
}

void main() {
    vec2 halfSize = 0.5 * uSize;

    float dOuter = sdRoundRect(vLocal, halfSize, uRadius);

    float innerRadius = max(uRadius - uBorder, 0.0);
    vec2  innerHalf   = max(halfSize - vec2(uBorder), vec2(0.0));
    float dInner = sdRoundRect(vLocal, innerHalf, innerRadius);

    float aOuter = 1.0 - smoothstep(0.0, uFeather, dOuter);
    float aInner = 1.0 - smoothstep(0.0, uFeather, dInner);

    float borderAlpha = clamp(aOuter - aInner, 0.0, 1.0);
    float fillAlpha   = aInner;

    vec4 color = vec4(0.0);
    color = mix(color, uBorderColor, borderAlpha);
    color = mix(color, uFillColor,   fillAlpha);
    color.a = clamp(borderAlpha * uBorderColor.a + fillAlpha * uFillColor.a, 0.0, 1.0);

    if (color.a <= 0.001) discard;
    FragColor = color;
}
)";

const char *StaticShaderSource::ui_image_element_vs = R"(#version 330 core
layout(location=0) in vec2 aPosPx;  // képpont pozíció (bal-felső origó)
layout(location=1) in vec2 aUV;     // 0..1

uniform vec2 uScreen;               // framebuffer méret (w,h) px
uniform int  uFlipV;                // 0 = ne tükrözz, 1 = V tengely tükrözés

out vec2 vUV;
out vec2 vPosPx;                    // clippinghez

void main() {
    vUV   = (uFlipV != 0) ? vec2(aUV.x, 1.0 - aUV.y) : aUV;
    vPosPx = aPosPx;

    vec2 ndc;
    ndc.x = (aPosPx.x / uScreen.x) * 2.0 - 1.0;
    ndc.y = 1.0 - (aPosPx.y / uScreen.y) * 2.0;
    gl_Position = vec4(ndc, 0.0, 1.0);
}
)";

const char *StaticShaderSource::ui_image_element_fs = R"(#version 330 core
in vec2 vUV;
in vec2 vPosPx;
out vec4 FragColor;

uniform sampler2D uTex;
uniform int   uHasTex;        // 0=nincs textúra → csak uTint
uniform int   uIsSDF;         // 0=bitmap (kép/font), 1=SDF font
uniform vec4  uTint;          // kép: színezés; szöveg: betűszín

uniform int   uUseClip;       // 1=clip kerek téglalapba
uniform vec2  uClipPos;       // clip bal-felső px
uniform vec2  uClipSize;      // clip méret px
uniform float uClipRadius;    // clip rádiusz px
uniform float uClipFeather;   // AA px (pl. 1.0)

float sdRoundRect(vec2 p, vec2 halfSize, float r) {
    r = min(r, min(halfSize.x, halfSize.y));
    vec2 q = abs(p) - (halfSize - vec2(r));
    return length(max(q, 0.0)) - r;
}

void main() {
    vec4 color = uTint;

    if (uHasTex != 0) {
        vec4 texel = texture(uTex, vUV);
        if (uIsSDF != 0) {
            float sd = texel.r;
            float w  = fwidth(sd);
            float alpha = smoothstep(0.5 - w, 0.5 + w, sd);
            color = vec4(uTint.rgb, uTint.a * alpha);
        } else {
            color = texel * uTint;
        }
    }

    if (uUseClip != 0) {
        vec2 center   = uClipPos + 0.5 * uClipSize;
        vec2 halfSize = 0.5 * uClipSize;
        float d = sdRoundRect(vPosPx - center, halfSize, uClipRadius);
        float a = 1.0 - smoothstep(0.0, uClipFeather, d);
        color.a *= a;
    }

    if (color.a <= 0.001) discard;
    FragColor = color;
}
)";

const char *StaticShaderSource::ui_text_element_vs = StaticShaderSource::ui_image_element_vs;
const char *StaticShaderSource::ui_text_element_fs = StaticShaderSource::ui_image_element_fs;

/**=======================================================================================================================*
 **                                           END OF REGION STATICSHADERSOURCES
 *========================================================================================================================*/
#pragma endregion STATICSHADERSOURCES

GLuint Shader::s_currentProgram = -1;
unsigned int Shader::s_currentWindowContext = -1;

Shader::Shader(const PandoraEX::String &vertexSource, const PandoraEX::String &fragmentSource)
    : _vertexSource(vertexSource), _fragmentSource(fragmentSource)
{
}

void Shader::initialize()
{
    // Get current window context ID
    unsigned int windowContext = WindowManager::getCurrentWindowContext();
    if (_programs.containsKey(windowContext))
    {
        DC::contextInfoOnce = "Shader/Context[" + String(windowContext) + "]";
        DC::logWarning("Shader already initialized for this window context.");
        return;
    }

    ShaderProgram *program = new ShaderProgram();
    program->initialize(_vertexSource, _fragmentSource);
    if (!program->isInitialized())
    {
        DC::contextInfoOnce = "Shader/Context[" + String(windowContext) + "]";
        DC::logFailure("Shader initialization failed.");
        delete program;
        return;
    }

    _programs.get(windowContext, *program);
    getEvents().onInitialize.invoke(*program);
}

void Shader::initializeAll()
{
    // Initialize shader for all window contexts
    for (int i = 0; i < WindowManager::getWindowCount(); i++)
    {
        unsigned int id = WindowManager::getWindow(i).id();

        if (_programs.containsKey(id))
            continue;

        ShaderProgram *program = new ShaderProgram();
        program->initialize(_vertexSource, _fragmentSource);
        if (!program->isInitialized())
        {
            DC::contextInfoOnce = "Shader/Context[" + String(id) + "]";
            DC::logFailure("Shader initialization failed for window context %u.", id);
            delete program;
            continue;
        }

        _programs.get(id, *program);
        getEvents().onInitialize.invoke(*program);
        // onInitialize.invoke(*program);
    }
}

bool Shader::isInitialized() const
{
    unsigned int windowContext = WindowManager::getCurrentWindowContext();
    return _programs.containsKey(windowContext);
}

ShaderEvent& Shader::getEvents()
{
    unsigned int windowContext = WindowManager::getCurrentWindowContext();
    return _shaderEvents.get(windowContext, *(new ShaderEvent()));
}

void Shader::use()
{
    unsigned int windowContext = WindowManager::getCurrentWindowContext();
    if (!_programs.containsKey(windowContext))
    {
        DC::contextInfoOnce = "Shader/Context[" + String(windowContext) + "]";
        DC::logWarning("Shader not initialized for this window context. Call initialize() first.");
        return;
    }

    ShaderProgram &program = _programs[windowContext];
    if (s_currentWindowContext != windowContext)
    {
        glUseProgram(program.getProgram());
        s_currentProgram = program.getProgram();
        s_currentWindowContext = windowContext;
        program.use();
    }
    if (s_currentProgram != program.getProgram())
    {
        glUseProgram(program.getProgram());
        s_currentProgram = program.getProgram();
        program.use();
    }

    getEvents().onUse.invoke(program);
    // onUse.invoke(program);
}

ShaderProgram &Shader::getProgram(bool initOnFail)
{
    unsigned int windowContext = WindowManager::getCurrentWindowContext();
    if (!_programs.containsKey(windowContext))
    {
        if (initOnFail)
        {
            DC::contextInfoOnce = "Shader/Context[" + String(windowContext) + "]";
            DC::logFailure("Shader not initialized for this window context. Call initialize() first.");
            ThrowExceptionF(PandoraEX::Exceptions::InvalidOperationException, "Shader not initialized for this window context.");
        }
        else
        {
            DC::contextInfoOnce = "Shader/Context[" + String(windowContext) + "]";
            DC::logWarning("Shader not initialized for this window context, initializing now.");
            initialize();
        }
    }

    return _programs[windowContext];
}

/**=======================================================================================================================*
 **                                                  REGION STATICSHADERS
 *========================================================================================================================*/
#pragma region STATICSHADERS

Shader &StaticShaders::uiElementShader()
{
    static Shader shader = Shader(StaticShaderSource::ui_element_vs, StaticShaderSource::ui_element_fs);
    shader.getEvents().onInitialize = [](ShaderProgram &program)
    {
        program.bindUniform("uPos");
        program.bindUniform("uSize");
        program.bindUniform("uScreen");
        program.bindUniform("uRadius");
        program.bindUniform("uBorder");
        program.bindUniform("uFeather");
        program.bindUniform("uFillColor");
        program.bindUniform("uBorderColor");
        DC::contextInfoOnce = "StaticShaders::uiElementShader";
        DC::logInfo("Shader initialized and uniforms bound.");
    };
    if (!shader.isInitialized())
    {
        shader.initialize();
    }
    return shader;
}

Shader &StaticShaders::uiImageElementShader()
{
    static Shader shader(StaticShaderSource::ui_image_element_vs, StaticShaderSource::ui_image_element_fs);
    shader.getEvents().onInitialize = [](ShaderProgram &program)
    {
        program.bindUniform("uScreen");
        program.bindUniform("uFlipV");

        program.bindUniform("uTex");
        program.bindUniform("uHasTex");
        program.bindUniform("uIsSDF");
        program.bindUniform("uTint");
        program.bindUniform("uUseClip");
        program.bindUniform("uClipPos");
        program.bindUniform("uClipSize");
        program.bindUniform("uClipRadius");
        program.bindUniform("uClipFeather");
        DC::contextInfoOnce = "StaticShaders::uiImageElementShader";
        DC::logInfo("Shader initialized and uniforms bound.");
    };
    if (!shader.isInitialized())
    {
        shader.initialize();
    }
    return shader;
}

Shader &StaticShaders::uiTextElementShader()
{
    static Shader shader(StaticShaderSource::ui_text_element_vs, StaticShaderSource::ui_text_element_fs);
    shader.getEvents().onInitialize = [](ShaderProgram &program)
    {
        program.bindUniform("uScreen");
        program.bindUniform("uFlipV");

        program.bindUniform("uTex");
        program.bindUniform("uHasTex");
        program.bindUniform("uIsSDF");
        program.bindUniform("uTint");
        program.bindUniform("uUseClip");
        program.bindUniform("uClipPos");
        program.bindUniform("uClipSize");
        program.bindUniform("uClipRadius");
        program.bindUniform("uClipFeather");
        DC::contextInfoOnce = "StaticShaders::uiTextElementShader";
        DC::logInfo("Shader initialized and uniforms bound.");
    };
    if (!shader.isInitialized())
    {
        shader.initialize();
    }
    return shader;
}

/**=======================================================================================================================*
 **                                           END OF REGION STATICSHADERS
 *========================================================================================================================*/
#pragma endregion STATICSHADERS