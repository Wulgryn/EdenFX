#include "shader.hpp"
#include "PandoraDebug/console.hpp"
#include "../windowManager.hpp"

#include "glad/gl.h"

using namespace PandoraDebug;
using namespace PandoraUI;
using namespace PandoraEX;

GLuint ShaderProgram::__compileShader(GLenum type, const PandoraEX::String &source)
{
    GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        String infoLog(len, '\0');
        glGetShaderInfoLog(shader, len, NULL, infoLog.data());
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logFailure("Failed to compile shader:\n %s", infoLog);
        return 0;
    }
    return shader;
}

GLuint ShaderProgram::__linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        String infoLog(len, '\0');
        glGetProgramInfoLog(program, len, NULL, infoLog.data());
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logFailure("Failed to link program:\n %s", infoLog);
        return 0;
    }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void ShaderProgram::__genObjects()
{
    // GLuint _VAO = 0, _VBO = 0, _EBO = 0;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    float quad[] = {0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f};
    unsigned idx[] = {0, 1, 2, 0, 2, 3};

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glBindVertexArray(0);
}

void ShaderProgram::use()
{
    if (!_isInitialized)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Shader program is not initialized, cannot use it.");
        return;
    }
    // if (s_currentProgram != _program)
    // {
    //     glUseProgram(_program);
    //     s_currentProgram = _program;
    //     glBindVertexArray(_VAO);
    // }
    // onUse.invoke(*this);
    glBindVertexArray(_VAO);
}

void ShaderProgram::initialize(const PandoraEX::String& vertexSource, const PandoraEX::String& fragmentSource)
{
    if (_isInitialized)
        return;

    unsigned int vertexShader = __compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Vertex shader compilation failed, cannot initialize shader program.");
        return;
    }
    DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
    DC::logSuccess("Vertex shader compiled successfully.");
    unsigned int fragmentShader = __compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Fragment shader compilation failed, cannot initialize shader program.");
        glDeleteShader(vertexShader);
        return;
    }
    DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
    DC::logSuccess("Fragment shader compiled successfully.");
    _program = __linkProgram(vertexShader, fragmentShader);
    if (_program == 0)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Shader program linking failed, cannot initialize shader program.");
        return;
    }
    DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
    DC::logSuccess("Shader program linked successfully.");
    __genObjects();
    DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
    DC::logInfo("Generated shader objects.");
    _isInitialized = true;
    // onInitialize.invoke(*this);
}

void ShaderProgram::bindUniform(const PandoraEX::String &name, bool override)
{
    if (!_isInitialized)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Shader program is not initialized, cannot bind uniform '%s'.", name.c_str());
        return;
    }
    if (_uniformLocations.containsKey(name) && !override)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Uniform '%s' is already bound in shader program. Use 'override' to rebind.", name.c_str());
        return;
    }

    int location = glGetUniformLocation(_program, name.c_str());
    if (location == -1)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Uniform '%s' not found in shader program.", name.c_str());
        return;
    }
    _uniformLocations.add(name, location);
    DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
    DC::logInfo("Bound uniform '%s' at location %d in shader program.", name.c_str(), location);
}

unsigned int ShaderProgram::getUniformLocation(const PandoraEX::String &name) const
{
    if (!_isInitialized)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Shader program is not initialized, cannot get uniform location for '%s'.", name.c_str());
        return -1;
    }
    if (!_uniformLocations.containsKey(name))
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Uniform '%s' is not bound in shader program. Use 'BindUniform' to bind it first.", name.c_str());
        return -1;
    }
    return _uniformLocations[name];
}

unsigned int ShaderProgram::getProgram() const
{
    if (!_isInitialized)
    {
        DC::contextInfoOnce = "Shader/Context[" + String(WindowManager::getCurrentWindowContext()) + "]";
        DC::logWarning("Shader program is not initialized, cannot get program ID.");
        return -1;
    }
    return _program;
}