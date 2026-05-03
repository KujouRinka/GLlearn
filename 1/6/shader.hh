#ifndef GL_SHADER_HH
#define GL_SHADER_HH

#include <glad/glad.h>

#include <string>

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  void use();

  // uniform
  void setBool(const std::string &name, GLboolean value) const;
  void setInt(const std::string &name, GLint value) const;
  void setFloat(const std::string &name, GLfloat value) const;

public:
  GLuint ID;
};


#endif //GL_SHADER_HH
