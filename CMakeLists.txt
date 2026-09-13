cmake_minimum_required(VERSION 4.4)

project(cpp-opengl)

find_package(OpenGL REQUIRED)

add_subdirectory(vendor/glfw-3.5.1)

add_executable(Application
  src/Application.cpp
  src/Shader.cpp
  src/Texture.cpp
  src/IndexBuffer.cpp
  src/VertexBuffer.cpp
  src/VertexArray.cpp
  src/Renderer.cpp
  vendor/glad/src/glad.c
  vendor/stb_image/stb_image.cpp

  vendor/imgui/backends/imgui_impl_glfw.cpp
  vendor/imgui/backends/imgui_impl_opengl3.cpp
  vendor/imgui/imgui.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_tables.cpp
  vendor/imgui/imgui_widgets.cpp
  vendor/imgui/imgui_demo.cpp
)

target_include_directories(Application PRIVATE
  vendor/glad/include
  vendor/stb_image
  vendor/glm
  vendor/imgui
)

target_link_libraries(Application PRIVATE
  glfw
  OpenGL::GL
)
