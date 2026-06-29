#ifndef GL_SHADER_HH
#define GL_SHADER_HH

#include <glad/glad.h>

#include <string>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"


class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  void use();

  // uniform
  void setBool(const std::string &name, GLboolean value) const;
  void setInt(const std::string &name, GLint value) const;
  void setFloat(const std::string &name, GLfloat value) const;
  void setVec3(const std::string &name, const glm::vec3 &vec3) const;
  void setMat4(const std::string &name, const glm::mat4 &mat4) const;

  GLuint getHandler() const;

public:
  GLuint ID;
};


#endif //GL_SHADER_HH
