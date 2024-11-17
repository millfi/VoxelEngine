#include <chrono>
#include "settings.hpp"
#include <iostream>

#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

GLuint ShaderProgram(fs::path vertShPath, fs::path fragShPath)
{
    std::ifstream vertIfs(vertShPath, std::ios::binary);
    if (!vertIfs) {
        std::cerr << "Can't read vertex shader source : " << vertShPath << std::endl;
    }
    
    std::string vertShCode;
    vertShCode = std::string(std::istreambuf_iterator<char>(vertIfs), std::istreambuf_iterator<char>());
    auto vsrc = vertShCode.c_str();

    
    const GLuint program = glCreateProgram();


    GLuint vsobj = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsobj, 1, &vsrc, nullptr);
    glCompileShader(vsobj);

    GLint vs_status;
    glGetShaderiv(vsobj, GL_COMPILE_STATUS, &vs_status);
    if (vs_status == GL_FALSE) {
        std::cerr << "Compile error in vertex shader." << std::endl;
        GLsizei logsize;
        glGetShaderiv(vsobj, GL_INFO_LOG_LENGTH, &logsize);
        std::vector<GLchar> errorLog(logsize);
        glGetShaderInfoLog(vsobj, logsize, nullptr, errorLog.data());
        std::cerr << errorLog.data() << std::endl;
    }

    glAttachShader(program, vsobj);
    glDeleteShader(vsobj);

    std::ifstream fragIfs(fragShPath, std::ios::binary);
    if (!fragIfs) {
        std::cerr << "Can't read fragment shader source : " << fragShPath << std::endl;
    }
    std::string fragShCode = std::string(std::istreambuf_iterator<char>(fragIfs), std::istreambuf_iterator<char>());

    auto fsrc = fragShCode.c_str();
    GLuint fsobj = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsobj, 1, &fsrc, nullptr);
    glCompileShader(fsobj);

    GLint fs_status;
    glGetShaderiv(fsobj, GL_COMPILE_STATUS, &fs_status);
    if (fs_status == GL_FALSE) {
        std::cerr << "Compile error in fragment shader." << std::endl;
        GLsizei logsize;
        glGetShaderiv(fsobj, GL_INFO_LOG_LENGTH, &logsize);
        std::vector<GLchar> errorLog(logsize);
        glGetShaderInfoLog(fsobj, logsize, nullptr, errorLog.data());
        std::cerr << errorLog.data() << std::endl;
    }

    glAttachShader(program, fsobj);
    glDeleteShader(fsobj);


    glLinkProgram(program);
    GLint link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        std::cerr << "Link Error." << std::endl;
        GLsizei logsize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);
        std::vector<GLchar> errorLog(logsize);
        glGetProgramInfoLog(program, logsize, nullptr, errorLog.data());
        std::cerr << errorLog.data() << std::endl;
    }

    return program;
}
class VoxelEngine {
    using chrono_time = std::chrono::system_clock;
    chrono_time::time_point clock;
    chrono_time::time_point delta_time;
    bool is_running;
    GLFWwindow* window;


    GLuint program;
    GLuint vao;
    GLuint verticesVbo;
    GLuint colorVbo;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> color;
public:
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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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
        

        vertices = {
           glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3(-1.0f, -1.0f, 0.0f),
           glm::vec3(1.0f, -1.0f, 0.0f)
        };
        color = {
           glm::vec3(1.0f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3(0.0f, 0.0f, 1.0f)
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        glGenBuffers(1, &verticesVbo);
        glBindBuffer(GL_ARRAY_BUFFER, verticesVbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &colorVbo);
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
        glEnableVertexAttribArray(1);

        program = ShaderProgram("./shader.vert", "./shader.frag");
    }

    void update() {
        delta_time = chrono_time::now();
    }
    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
    }
    void handle_events() {
        if (glfwWindowShouldClose(window) == GL_TRUE || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            is_running = false;
        }
    }
    void run(){
        while (is_running) {
            handle_events();
            update();
            render();
            glfwPollEvents();
        }
        glfwTerminate();
    }
};  
int main() {
    VoxelEngine game;
    game.run();
}