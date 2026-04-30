#include "shader.hh"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;

  try {
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertexFile.open(vertexPath, std::ios_base::in);
    fragmentFile.open(fragmentPath, std::ios_base::in);

    vertexCode.assign(std::istreambuf_iterator<char>(vertexFile), std::istreambuf_iterator<char>());
    fragmentCode.assign(std::istreambuf_iterator<char>(fragmentFile), std::istreambuf_iterator<char>());

    vertexFile.close();
    fragmentFile.close();
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  int success;
  char err_msg[512];
  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, err_msg);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << err_msg << std::endl;
  };

  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, err_msg);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << err_msg << std::endl;
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, nullptr, err_msg);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << err_msg << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  glDeleteProgram(ID);
}

void Shader::use() {
  glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, GLboolean value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(value));
}

void Shader::setInt(const std::string &name, GLint value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, GLfloat value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
