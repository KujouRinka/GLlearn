#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hh"
#include "camera.hh"
#include "model.hh"

GLint WIDTH = 800;
GLint HEIGHT = 600;

float lastFrame = 0.0f;
float deltaTime = 0.0f;

float lastX = 400;
float lastY = 300;

Camera camera({0.0f, 0.0f, 3.0f});

void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, WIDTH, HEIGHT);
}

bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float x_offset = xpos - lastX;
  float y_offset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(x_offset * 2, y_offset * 2);
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
  camera.ProcessMouseScroll(y_offset);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    // cameraPos += cameraSpeed * direction;
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    // cameraPos -= cameraSpeed * glm::normalize(glm::cross(direction, cameraUp));
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    // cameraPos -= cameraSpeed * direction;
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    // cameraPos += cameraSpeed * glm::normalize(glm::cross(direction, cameraUp));
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    // cameraPos += cameraSpeed * cameraUp;
    camera.ProcessKeyboard(UP, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    // cameraPos -= cameraSpeed * cameraUp;
    camera.ProcessKeyboard(DOWN, deltaTime);
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
    glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  stbi_set_flip_vertically_on_load(true);

  glEnable(GL_DEPTH_TEST);

  Shader ourShader("./vert2.vert", "./frag2.frag");
  // Shader lightShader("./vert2.vert", "./light_frag.frag");

  Model ourModel("./assets/backpack/backpack.obj");

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    process_input(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    ourShader.setMat4("model", model);
    ourModel.Draw(ourShader);


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
