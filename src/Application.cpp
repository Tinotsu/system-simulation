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
#include <chrono>
#include <glad/glad.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/* ================ */
/* =====Physic===== */
/* ================ */

int frequency = 60;
float FIXED_DT = 1.0 / frequency;
float accumulator = 0.0f;
auto previous = std::chrono::steady_clock::now();

float speedTime = 1;

const float g = 9.81f;

// Initial State
const float alt0 = 2.5f;
const float v0 = 0.0f;
const float simulationTime0 = 0.0f;

float alt = alt0;
float v = v0;
float simulationTime = simulationTime0;

void SimulationReset() {
    alt = alt0;
    v = v0;
}

void activePhysic(float dt) {
    v += g * dt;
    alt -= v * dt;

    if (alt <= -2.5) {
        alt = -2.5;
        v = -v * 0.85f;
    }
}

int main(void) {
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required on macOS

    glm::ivec2 windowResolution(640, 480);
    window = glfwCreateWindow(windowResolution.x, windowResolution.y,
                              "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

    float positions[] = {
        -0.5f, -0.5f, // 0
        0.5f,  -0.5f, // 1
        0.5f,  0.5f,  // 2
        -0.5f, 0.5f   // 3
    };

    unsigned int indices[] = {
        0, 1, 2, //
        2, 3, 0  //
    };

    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);
    glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader shader("./res/shaders/Circle.shader");
    shader.Bind();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        renderer.Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.Bind();

        /* TIME */

        auto current = std::chrono::steady_clock::now();

        float frameTime = std::chrono::duration<float>(current - previous)
                              .count(); // time recorded since the last frame

        previous = current;
        accumulator += frameTime * speedTime;

        simulationTime += FIXED_DT * speedTime;

        while (accumulator >= FIXED_DT) {
            activePhysic(FIXED_DT);

            accumulator -= FIXED_DT;
        }

        glm::vec3 translationA(0, alt, 0);
        glm::vec3 translationB(2, 1, 0);

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        {
            ImGui::Begin("Debugger");

            ImGui::SliderInt("Frequency", &frequency, 20, 120);
            FIXED_DT = 1.0f / frequency;
            ImGui::Text("FIXED_DT: %.3f", FIXED_DT);
            ImGui::SliderFloat("Simulation Speed", &speedTime, 0.0f, 3.0f);

            if (ImGui::SliderFloat("simulationTime", &simulationTime, 0.0f,
                                   20.0f)) {
                SimulationReset();
                for (float i = FIXED_DT; i < simulationTime; i += FIXED_DT) {
                    activePhysic(FIXED_DT);
                }
            }

            ImGui::Text("simulation time: %.3f", simulationTime);
            if (ImGui::Button("Pause"))
                speedTime = speedTime == 0.0f ? 1.0f : 0.0f;

            if (ImGui::Button("Reset"))
                SimulationReset();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
