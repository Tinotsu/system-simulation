#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

VertexArray::VertexArray() {

    GLCall(glGenVertexArrays(1, &m_RendererID)); // Initialize the VAO
}

VertexArray::~VertexArray() {

    GLCall(
        glad_glDeleteVertexArrays(1, &m_RendererID)); // Active/Select the VAO
}

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout) {
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        /* Define the descrption of how OpenGL should interpret data (then it's
         * stored by VAO) */
        GLCall(glVertexAttribPointer(i, element.count, element.type,
                                     element.normalized, layout.GetStride(),
                                     (const void *)offset));
        offset +=
            element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const { GLCall(glad_glBindVertexArray(m_RendererID)); }

void VertexArray::UnBind() const { GLCall(glad_glBindVertexArray(0)); }
