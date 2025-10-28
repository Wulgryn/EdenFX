#include "button.hpp"

#include "glad/gl.h"

using namespace PandoraUI::GUI;

void Button::initialize()
{
    IElement::initialize();
    shaders.add(&StaticShaders::uiElementShader());
    StaticShaders::uiElementShader().getEvents().onUse += [this](ShaderProgram& shader) {
        glUniform2f(shader.getUniformLocation("uScreen"), (float)__getViewPort().width, (float)__getViewPort().height);

        glUniform2f(shader.getUniformLocation("uPos"), (float)position.x, (float)position.y);
        glUniform2f(shader.getUniformLocation("uSize"), (float)size.width, (float)size.height);
        glUniform1f(shader.getUniformLocation("uRadius"), 0.0f);
        glUniform1f(shader.getUniformLocation("uBorder"), 3.0f);
        glUniform1f(shader.getUniformLocation("uFeather"), 0.0f);
        // glUniform4f(shader.getUniformLocation("uFillColor"), 0.2f, 0.6f, 0.9f, 1.0f);
        glUniform4f(shader.getUniformLocation("uFillColor"), 0.3f, 0.3f, 0.3f, 0.0f);
        glUniform4f(shader.getUniformLocation("uBorderColor"), 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };
    
}

void Button::update()
{
    
}
