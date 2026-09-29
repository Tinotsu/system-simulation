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

const float g = 6.7; // e-11

// Initial State
const glm::vec2 r1_init = {0, 0};
const glm::vec2 r2_init = {2, 2};
glm::vec2 v1 = {1.09, -1.09};
glm::vec2 v2 = {-1.09, 1.09};
const float simulationTime0 = 0.0f;

glm::vec2 r1 = r1_init;
float m1 = 2;

glm::vec2 r2 = r2_init;
float m2 = 2;

float simulationTime = simulationTime0;

void SimulationReset() {
    r1 = r1_init;
    r2 = r2_init;
    v1 = {1, -1};
    v2 = {-1, 1};
}

void activePhysic(float dt) {

    // TODO: Two Body problem here

    float dx = r1.x - r2.x;
    float dy = r1.y - r2.y;
    float distance = sqrt(dx * dx + dy * dy);
    glm::vec2 direction1 = {dx / distance, dy / distance};
    glm::vec2 direction2 = {dx / distance, dy / distance};

    float GForce = (g * m1 * m2) / (distance * distance);
    float acc = GForce / m1;
    glm::vec2 accCoor1 = {acc * direction1.x, acc * direction1.y};
    glm::vec2 accCoor2 = {-acc * direction2.x, -acc * direction2.y};

    // Accelerate

    v1 -= accCoor1 * dt;
    v2 -= accCoor2 * dt;

    r1.x += v1.x * dt;
    r1.y += v1.y * dt;
    r2.x += v2.x * dt;
    r2.y += v2.y * dt;
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

        glm::vec3 translationA(r1.x, r1.y, 0);
        glm::vec3 translationB(r2.x, r2.y, 0);

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
