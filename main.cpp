#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "settings.hpp"

#include <iostream>
struct VoxelEngine {
    using chrono_time = std::chrono::system_clock;
    chrono_time::time_point clock;
    chrono_time::time_point delta_time;
    bool is_running;
    GLFWwindow* window;
    VoxelEngine() :
        clock(chrono_time::now()), delta_time(VoxelEngine::clock - VoxelEngine::clock), is_running(true)
    {
        init();
    }

    void init() {
        if (!glfwInit()) {
            exit(1);
        }
        // OpenGL Version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // リサイズ不可
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, "Game", nullptr, nullptr);
        if (window == nullptr) {
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        } else {
            std::cout << "success create GLFW window." << std::endl;
        }
        glfwMakeContextCurrent(window);

        // GLEWの初期化
        if (glewInit() != GLEW_OK) {
            std::cerr << "Can't initialize GLEW." << std::endl;
            exit(1);
        }
        else {
            std::cout << "success init GLEW." << std::endl;
        }
#if defined(_DEBUG)
        // GLFW エラーのコールバック
        glfwSetErrorCallback(
            [](auto id, auto description) { std::cerr << description << std::endl; });
        // OpenGL エラーのコールバック
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(
            [](auto source, auto type, auto id, auto severity, auto length,
                const auto* message, const void* userParam) {
                    auto t = type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "";
                    std::cerr << "GL CALLBACK: " << t << " type = " << type
                        << ", severity = " << severity << ", message = " << message
                        << std::endl;
            },
            0);
#endif
        // VSyncを設定
        glfwSwapInterval(1);
    }

    void update() {
        this->delta_time = chrono_time::now();
    }
    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(this->window);
    }
    void handle_events() {
        if (glfwWindowShouldClose(window) == GL_TRUE || glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            this->is_running = false;
        }
    }
    void run(){
        while (this->is_running) {
            this->handle_events();
            this->update();
            this->render();
        }
        glfwTerminate();
    }
};  
int main() {
    VoxelEngine game;
    game.run();
}