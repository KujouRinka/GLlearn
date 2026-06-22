#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hh"
#include "camera.hh"

float vertices[] = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(1.2f, 1.0f, 2.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f, 3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f, 2.0f, -2.5f),
  glm::vec3(1.5f, 0.2f, -1.5f),
  glm::vec3(-1.3f, 1.0f, -1.5f)
};

GLint WIDTH = 800;
GLint HEIGHT = 600;

// glm::vec3 cameraPos{0.0f, 0.0f, 3.0f};
// glm::vec3 cameraFront{0.0f, 0.0f, -1.0f};
// glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};

float lastFrame = 0.0f;
float deltaTime = 0.0f;

float lastX = 400;
float lastY = 300;

float pitch = 0.0f;
float yaw = -90.0f;

float fov = 45.0f;

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
  // float y_offset = ypos - lastY;
  float y_offset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  // float sensitivity = 0.5f;
  // x_offset *= sensitivity;
  // y_offset *= sensitivity;
  camera.ProcessMouseMovement(x_offset * 2, y_offset * 2);

  // pitch += y_offset;
  // yaw += x_offset;
  // if (pitch > 89.0f) {
  //   pitch = 89.0f;
  // }
  // if (pitch < -89.0f) {
  //   pitch = -89.0f;
  // }

  // glm::vec3 front;
  // front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  // front.y = sin(glm::radians(pitch));
  // front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  // cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
  camera.ProcessMouseScroll(y_offset);
  // if (fov >= 1.0f && fov <= 45.0f) {
  //   fov -= y_offset;
  // }
  // if (fov < 1.0f) {
  //   fov = 1.0f;
  // }
  // if (fov > 45.0f) {
  //   fov = 45.0f;
  // }
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // float cameraSpeed = 3.0f * deltaTime;
  // glm::vec3 direction{cameraFront.x, cameraFront.y, cameraFront.z};
  // glm::vec3 direction{cameraFront.x, 0, cameraFront.z};
  // direction = glm::normalize(direction);
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

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader shader("./vert2.vert", "./frag2.frag");
  Shader lightShader("./vert2.vert", "./light_frag.frag");

  GLuint VAO, VBO, lightVAO;
  glGenVertexArrays(1, &VAO);
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  // loc
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) (sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(lightVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
  glEnableVertexAttribArray(0);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glm::vec3 lightPos = cubePositions[1];
    glm::mat4 lightModel{1.0f};
    lightModel = glm::rotate(lightModel, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3{0.0f, 1.0f, 0.0f});
    lightModel = glm::translate(lightModel, cubePositions[1]);
    lightModel = glm::scale(lightModel, glm::vec3{0.2f});
    glm::vec3 lightPos = glm::vec3(lightModel * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});

    shader.use();
    shader.setVec3("objectColor", {1.0f, 0.5f, 0.31f});
    shader.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
    shader.setVec3("lightPos", lightPos);

    glm::mat4 projection;
    // projection = glm::perspective(glm::radians(fov), static_cast<GLfloat>(WIDTH) / HEIGHT, 0.1f, 100.0f);
    projection = glm::perspective(glm::radians(camera.Zoom), static_cast<GLfloat>(WIDTH) / HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    // float radius = 10.0f;
    // float camX = sin(glfwGetTime()) * radius;
    // float camZ = cos(glfwGetTime()) * radius;
    glm::mat4 view;
    // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.WorldUp);
    shader.setMat4("view", view);
    shader.setVec3("viewPos", camera.Position);

    glm::mat4 model{1.0f};

    model = glm::translate(model, cubePositions[0]);
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader.use();
    lightShader.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);

    // lightModel = glm::rotate(lightModel, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3{0.0f, 1.0f, 0.0f});
    // lightModel = glm::translate(lightModel, lightPos);
    // lightModel = glm::scale(lightModel, glm::vec3{0.2f});
    lightShader.setMat4("model", lightModel);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
