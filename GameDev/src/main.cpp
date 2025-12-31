#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <cmath>

// to define the struct for moving the shape
struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};
Vec2 offset;
// declaring keycallbackFunction
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
                std::cout << "GLFW_KEY_UP" << std::endl;
                offset.y += 0.01f;
                break;
        case GLFW_KEY_DOWN:
            std::cout << "GLFW_KEY_DOWN" << std::endl;
            offset.y -= 0.01f;
            break;
        case GLFW_KEY_RIGHT:
            std::cout << "GLFW_KEY_RIGHT" << std::endl;
            offset.x += 0.01f;
            break;
        case GLFW_KEY_LEFT:
            std::cout << "GLFW_KEY_LEFT" << std::endl;
            offset.x -= 0.01f;
            break;
        default:
            break;
        }
    }
}

int main()
{
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Uniform Fix", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // to capture what we press in keyboard
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    // Vertex Shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform vec2 uOffset;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x,position.y + uOffset.y,position.z, 1.0);
        }
    )";

    // Fragment Shader - FIXED: Added semicolon after uniform ucolor
    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 vColor;
        uniform vec4 ucolor;

        void main()
        {
            // Multiply vertex color by uniform color
            FragColor = vec4(vColor, 1.0) * ucolor;
        }
    )";

    // Shader Compilation Helper
    auto compileShader = [](unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0u;
        }
        return id;
        };

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);



    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // FIXED: Removed hidden characters from the vertices array
    std::vector<float> vertices =
    {
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Top Right
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Top Left
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Bottom Left
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // Bottom Right
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Get uniform location
    GLint uColorLoc = glGetUniformLocation(shaderProgram, "ucolor");

    GLint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Update the uniform (using time to make it pulse)
        float timeValue = (float)glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        glUniform4f(uColorLoc, 1.0f, greenValue, 1.0f, 1.0f);
        glUniform2f(uOffsetLoc, offset.x ,offset.y);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}