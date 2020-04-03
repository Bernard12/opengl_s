#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "GL/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"

#include <random>

const int WIDTH = 400, HEIGHT = 400;
const int FPS = 10;
const float delta = 1. / FPS;
const float toRadians = 3.14159265f / 180.f;

GLuint VAO, VBO, GLOBAL_SHADER;

// Vertex GLOBAL_SHADER apply on verticies
static const char* vShader = "#version 330\n"
                             "layout (location = 0) in vec3 pos;\n"
                             "uniform mat4 transform;"
                             "out vec4 bruh;"
                             "void main() {\n"
                             "    gl_Position = transform * vec4(pos, 1.0f);\n"
                             "    bruh = vec4(pos, 1.0f);"
                             "}";

// fragment GLOBAL_SHADER
static const char* fShader = "#version 330\n"
                             "out vec4 colour;"
                             "in vec4 bruh;"
                             "uniform float u_time;"
                             "void main() {"
                             "    vec2 coord = gl_FragCoord.xy;"
                             "    vec2 ct = vec2(1.0, 1.0);"
                             "    float dt = distance(coord, ct);"
                             "    colour = vec4(sin(dt), sin(dt), sin(dt), 1.0);"
                             "}";


void createTriangle() {
    GLfloat verticies[] = {
            -1.f, -1.f, 0.f,
            0.f, 1.f, 0.f,
            1.f, -1.f, 0.f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void addShader(GLuint program, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint results = 0;
    GLchar eLog[1014] = {0};
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &results);
    if (results == GL_FALSE) {
        glGetShaderInfoLog(theShader, sizeof(eLog), 0, eLog);
        printf("[Shader compilation error]: type=%d %s\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, theShader);
}

void compileShaders() {
    GLOBAL_SHADER = glCreateProgram();

    if (!GLOBAL_SHADER) {
        printf("Failed to create GLOBAL_SHADER");
        return;
    }

    addShader(GLOBAL_SHADER, vShader, GL_VERTEX_SHADER);
    addShader(GLOBAL_SHADER, fShader, GL_FRAGMENT_SHADER);

    GLint results = 0;
    GLchar eLog[2014] = {0};

    glLinkProgram(GLOBAL_SHADER);
    glGetProgramiv(GLOBAL_SHADER, GL_LINK_STATUS, &results);

    if (!results) {
        glGetProgramInfoLog(GLOBAL_SHADER, sizeof(eLog), NULL, eLog);
        printf("[Link error]: %s\n", eLog);
        return;
    }

    glBindVertexArray(VAO);
    glValidateProgram(GLOBAL_SHADER);
    glBindVertexArray(0);

    glGetProgramiv(GLOBAL_SHADER, GL_VALIDATE_STATUS, &results);

    if (!results) {
        glGetProgramInfoLog(GLOBAL_SHADER, sizeof(eLog), NULL, eLog);
        printf("[Validating error]: %s\n", eLog);
        return;
    }
}

int main() {
    printf("FPS[%d], delta[%f]\n", FPS, delta);
    //glfw init
    if (!glfwInit()) {
        printf("Cannot initialize glfw");
        glfwTerminate();
        return 1;
    }
    //glfw setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile == no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "My cool Windows", NULL, NULL);
    if (!window) {
        printf("Failed to create glfw windows");
        glfwTerminate();
        return 2;
    }

    // buffer size
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // set context for glew
    glfwMakeContextCurrent(window);

    // allow modern extensions
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialise glew");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }
    //setup viewport
    glViewport(0, 0, WIDTH, HEIGHT);
    createTriangle();
    compileShaders();

    glm::mat4 transform(1.f);
//    transform = glm::scale(transform, glm::vec3(0.6f, 0.6f, 0.6f));
    float value = 0;
    float angleDelta = 0.1f;
    // main loop
    while (!glfwWindowShouldClose(window)) {
        // get
        glfwPollEvents();

        // clear windows
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(GLOBAL_SHADER);
        glBindVertexArray(VAO);
//        transform = glm::rotate(transform, angleDelta * toRadians, glm::vec3(0,0,1));
        int id = glGetUniformLocation(GLOBAL_SHADER, "transform");
        glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(transform));

//        id = glGetUniformLocation(GLOBAL_SHADER, "u_time");
//        value+=0.05f;
//        glUniform1f(id, value);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        sleep(delta);
    }

    return 0;
}
