#include "glm/ext/vector_float2.hpp"
#include "imgui.h"
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required on macOS

    glm::ivec2 windowResolution(640, 480);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowResolution.x, windowResolution.y,
                              "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float positions[] = {
        -0.5f, -0.5f, // 0
        0.5f,  -0.5f, // 1
        0.5f,  0.5f,  // 2
        -0.5f, 0.5f   // 3
    };

    // Index Buffer = transform any form into a triangle
    unsigned int indices[] = {
        0, 1, 2, //
        2, 3, 0  //
    };

    VertexArray va;

    /* Create and fill the VBO (=Vertex Buffer Object), stores the actual vertex
     * data */
    VertexBuffer vb(positions, sizeof(positions));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* Index Buffer Object, abstraction to reuse vertex data */
    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader shader("./res/shaders/Basic.shader");
    shader.Bind();
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the current buffer

    glm::ivec2 iResolution;
    glfwGetFramebufferSize(window, &iResolution.x, &iResolution.y);

    shader.SetUniform2i("iResolution", iResolution.x, iResolution.y);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    glm::vec3 translationA(0, 0, 0);
    glm::vec3 translationB(1, 1, 0);

    float r = 0.0f;
    float increment = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment = 0.05f;
        }

        r += increment;

        {

            ImGui::Begin("Debugger"); // Create a window called "Hello,
                                      // world!" and append into it.

            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 1.0f);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 1.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
