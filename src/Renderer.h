#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glad/glad.h>

#define ASSERT(x)                                                              \
    do {                                                                       \
        if (!(x))                                                              \
            __builtin_debugtrap();                                             \
    } while (0)

#define GLCall(x)                                                              \
    GLClearError();                                                            \
    x;                                                                         \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);

class Renderer {
  public:
    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib,
              const Shader &shader) const;
};
