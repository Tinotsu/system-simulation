#include "glm/ext/vector_float2.hpp"
#include "imgui.h"
#include <vector>
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include "IndexBuffer.h"
#include "Physics.h"
#include "PhysicsConstant.h"
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

float speedTime = 0;

// Initial State

std::vector<Object> objects{
    {"Sun",
     {0, 0},
     {0, 0},
     {0, 0},
     1989100,
     695700.0 / 100,
     {255, 255, 0, 1}}, // radius divided by 10

    {"Mercury", {57910006, 0}, {0, 48}, {0, 0}, 0.33, 2439, {160, 160, 160, 1}},

    {"Venus",
     {108199995, 0},
     {1, 35},
     {0, 0},
     4.87,
     6051.85,
     {255, 153, 51, 1}},

    {"Earth", {149599951, 0}, {1, 30}, {0, 0}, 5.87, 6378.15, {0, 0, 255, 1}},

    {"Mars", {227939920, 0}, {1, 24}, {0, 0}, 0.64, 3396, {255, 0, 0, 1}},

    {"Jupiter",
     {778330257, 0},
     {1, 13},
     {0, 0},
     1898.9,
     69911,
     {244, 160, 25, 1}},

    {"Saturn",
     {1429400028, 0},
     {1, 9.7},
     {0, 0},
     568.46,
     58296,
     {234, 123, 70, 1}},

    {"Uranus", {2870989228, 0}, {1, 6.8}, {0, 0}, 86.62, 25559, {0, 0, 167, 1}},

    {"Neptune",
     {4504299579, 0},
     {1, 5.4},
     {0, 0},
     102.43,
     24764,
     {10, 34, 210, 1}}};

// Normalization

void Normalization() {

    for (int n = 0; n < objects.size(); n++) {
        objects[n].position /= 149597870;
        objects[n].radius /= 149597.870 / 2;
        objects[n].mass /= 1989100;
        objects[n].velocity *= 86400.0 / 149597870;
        objects[n].acceleration *= 86400.0 / 149597870;
    }
}
const float simulationTime0 = 0.0f;
float simulationTime = simulationTime0;

std::vector<Object> ObjectConstants;

void SimulationReset() { objects = ObjectConstants; }
void TimeReset() { simulationTime = simulationTime0; }

int main(void) {

    ObjectConstants = objects;
    Normalization();

    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE); // required on macOS

    glm::ivec2 windowResolution(1280, 960);
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

    // glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

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

    Shader shader("./res/shaders/Circle.shader");
    shader.Bind();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

        glm::ivec2 iResolution;
        glfwGetFramebufferSize(window, &iResolution.x, &iResolution.y);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 proj =
            glm::ortho(-float(iResolution.x) / 200, float(iResolution.x) / 200,
                       -float(iResolution.y) / 200, float(iResolution.y) / 200,
                       -1.0f, 1.0f);
        shader.SetUniform2i("iResolution", iResolution.x, iResolution.y);
        /* TIME */

        auto current = std::chrono::steady_clock::now();

        float frameTime = std::chrono::duration<float>(current - previous)
                              .count(); // time recorded since the last frame

        previous = current;
        accumulator += frameTime * speedTime;
        simulationTime += FIXED_DT * speedTime;

        while (accumulator >= FIXED_DT) {
            RunPhysic(FIXED_DT, objects);

            simulationTime += FIXED_DT;
            accumulator -= FIXED_DT;
        }

        for (int n = 0; n < objects.size(); n++) {
            glm::vec3 translation(objects[n].position.x, objects[n].position.y,
                                  0);
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                shader.SetUniform1f("radius", objects[n].radius);
                shader.SetUniform4f("color", objects[n].color.r / 255.0,
                                    objects[n].color.g / 255.0,
                                    objects[n].color.b / 255.0,
                                    objects[n].color.a);
                renderer.Draw(va, ib, shader);
            }
        };

        {
            ImGui::Begin("Debugger");

            ImGui::SliderInt("Frequency", &frequency, 20, 120);
            FIXED_DT = 1.0f / frequency;
            ImGui::Text("FIXED_DT: %.3f", FIXED_DT);
            ImGui::SliderFloat("Simulation Speed", &speedTime, 0.0f, 50.0f);

            if (ImGui::SliderFloat("simulationTime", &simulationTime, 0.0f,
                                   20.0f)) {

                float newSimulationTime = simulationTime;
                SimulationReset();

                for (float t = 0.0f; t <= newSimulationTime; t += FIXED_DT) {
                    RunPhysic(FIXED_DT, objects);
                }
            }

            ImGui::Text("simulation time: %.3f", simulationTime);
            if (ImGui::Button("START"))
                speedTime = speedTime == 0.0f ? 1.0f : 0.0f;

            if (ImGui::Button("Reset")) {
                SimulationReset();
                TimeReset();
            }

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
