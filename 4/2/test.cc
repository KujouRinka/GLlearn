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
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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

glm::vec3 pointLightPositions[] = {
  glm::vec3(0.7f, 0.2f, 2.0f),
  glm::vec3(2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f, 2.0f, -12.0f),
  glm::vec3(0.0f, 0.0f, -3.0f)
};

GLint WIDTH = 800;
GLint HEIGHT = 600;

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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  GLint width, height, nr_channel;
  unsigned char *data = stbi_load(
    "./assets/container2.png",
    &width, &height, &nr_channel, STBI_rgb
  );
  if (!data) {
    std::cout << "Failed to load texture" << std::endl;
    return -1;
  }
  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  data = stbi_load(
    "./assets/container2_specular.png",
    &width, &height, &nr_channel, STBI_rgb
  );
  if (!data) {
    std::cout << "Failed to load texture" << std::endl;
    return -1;
  }
  GLuint texture1;
  glGenTextures(1, &texture1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  data = stbi_load(
    "./assets/matrix.jpg",
    &width, &height, &nr_channel, 0
  );
  if (!data) {
    std::cout << "Failed to load texture" << std::endl;
    return -1;
  }
  GLuint texture2;
  glGenTextures(1, &texture2);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);


  Shader shader("./vert2.vert", "./frag2.frag");
  Shader lightShader("./vert2.vert", "./light_frag.frag");
  Shader edgeShader("./vert2.vert", "./shaderSingleColor.frag");

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  // loc
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) 0);
  // normal vector
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) (sizeof(GLfloat) * 3));
  // tex coord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) (sizeof(GLfloat) * 6));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glm::vec3 lightColor{1.0f, 1.0f, 1.0f};

  shader.use();
  shader.setInt("material.diffuse", 0);
  shader.setInt("material.specular", 1);

  // directional light
  shader.setVec3("dirLight.direction", {-0.2f, -1.0f, -0.3f});
  shader.setVec3("dirLight.ambient", glm::vec3(0.00f) * lightColor);
  shader.setVec3("dirLight.diffuse", glm::vec3(0.0f) * lightColor);
  shader.setVec3("dirLight.specular", glm::vec3(0.0f) * lightColor);
  // point light 1
  shader.setVec3("pointLights[0].position", pointLightPositions[0]);
  shader.setVec3("pointLights[0].ambient", {0.05f, 0.05f, 0.05f});
  shader.setVec3("pointLights[0].diffuse", {0.8f, 0.8f, 0.8f});
  shader.setVec3("pointLights[0].specular", {1.0f, 1.0f, 1.0f});
  shader.setFloat("pointLights[0].constant", 1.0f);
  shader.setFloat("pointLights[0].linear", 0.09f);
  shader.setFloat("pointLights[0].quadratic", 0.032f);
  // point light 2
  shader.setVec3("pointLights[1].position", pointLightPositions[1]);
  shader.setVec3("pointLights[1].ambient", {0.05f, 0.05f, 0.05f});
  shader.setVec3("pointLights[1].diffuse", {0.8f, 0.8f, 0.8f});
  shader.setVec3("pointLights[1].specular", {1.0f, 1.0f, 1.0f});
  shader.setFloat("pointLights[1].constant", 1.0f);
  shader.setFloat("pointLights[1].linear", 0.09f);
  shader.setFloat("pointLights[1].quadratic", 0.032f);
  // point light 3
  shader.setVec3("pointLights[2].position", pointLightPositions[2]);
  shader.setVec3("pointLights[2].ambient", {0.05f, 0.05f, 0.05f});
  shader.setVec3("pointLights[2].diffuse", {0.8f, 0.8f, 0.8f});
  shader.setVec3("pointLights[2].specular", {1.0f, 1.0f, 1.0f});
  shader.setFloat("pointLights[2].constant", 1.0f);
  shader.setFloat("pointLights[2].linear", 0.09f);
  shader.setFloat("pointLights[2].quadratic", 0.032f);
  // point light 4
  shader.setVec3("pointLights[3].position", pointLightPositions[3]);
  shader.setVec3("pointLights[3].ambient", {0.05f, 0.05f, 0.05f});
  shader.setVec3("pointLights[3].diffuse", {0.8f, 0.8f, 0.8f});
  shader.setVec3("pointLights[3].specular", {1.0f, 1.0f, 1.0f});
  shader.setFloat("pointLights[3].constant", 1.0f);
  shader.setFloat("pointLights[3].linear", 0.09f);
  shader.setFloat("pointLights[3].quadratic", 0.032f);

  shader.setInt("material.diffuse", 0);
  shader.setInt("material.specular", 1);
  shader.setFloat("material.shininess", 32.0f);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    process_input(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.WorldUp);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<GLfloat>(WIDTH) / HEIGHT, 0.1f, 100.0f);

    glStencilMask(0x00);
    lightShader.use();
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    for (int i = 0; i < std::size(pointLightPositions); ++i) {
      lightShader.setVec3("lightColor", lightColor);
      glm::mat4 model{1.0f};
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, {0.2f, 0.2f, 0.2f});

      lightShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);
    shader.use();
    shader.setVec3("viewPos", camera.Position);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    for (int i = 0; i < std::size(cubePositions); ++i) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // edge
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);
    edgeShader.use();
    edgeShader.setMat4("view", view);
    edgeShader.setMat4("projection", projection);
    for (int i = 0; i < std::size(cubePositions); ++i) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      model = glm::scale(model, glm::vec3{1.1f});
      edgeShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glStencilMask(0xff);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
