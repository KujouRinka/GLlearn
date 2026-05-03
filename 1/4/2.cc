#include <iostream>
#include <numbers>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hh"

float vertices[] = {
  // pos              // color
  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // rd
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // ld
  0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};

const char *fragmentShaderYellowSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
  "}\n\0";

int w = 600;
int h = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  w = width;
  h = height;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
    glfwCreateWindow(w, h, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader ourShader("./vert_shader.vert", "./frag_shader.frag");

  GLuint VBO[2], VAO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);

  glBindVertexArray(VAO[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  struct {
    int v = -100;
    int f = -1;
    float next() {
      if (v == 100 || v == -100) {
        f *= -1;
      }
      v += f;
      return float(v) / 100;
    }
  } val;
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float xbase = 1.0f, ybase = 1.0f;
    if (w > h) {
      xbase = float(h) / w;
    } else {
      ybase = float(w) / h;
    }
    xbase *= 0.5;
    ybase *= 0.5;

    float timeValue = glfwGetTime();
    float x1 = cos(timeValue), x2 = cos(timeValue + 2 * std::numbers::pi / 3), x3 = cos(timeValue + 4 * std::numbers::pi / 3);
    float y1 = sin(timeValue), y2 = sin(timeValue + 2 * std::numbers::pi / 3), y3 = sin(timeValue + 4 * std::numbers::pi / 3);
    vertices[0] = xbase * x1;
    vertices[1] = ybase * y1;
    vertices[6] = xbase * x2;
    vertices[7] = ybase * y2;
    vertices[12] = xbase * x3;
    vertices[13] = ybase * y3;
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ourShader.use();
    ourShader.setFloat("h_offset", val.next());
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
    // glfwWaitEvents();
  }
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);

  glfwTerminate();

  return 0;
}
