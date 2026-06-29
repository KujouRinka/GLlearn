#ifndef GL_MESH_HH
#define GL_MESH_HH

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

#include "shader.hh"

using namespace std;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  GLuint id;
  string type;
  string path;
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

  Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

  void Draw(Shader &shader);

private:
  GLuint VAO, VBO, EBO;
  void setupMesh();
};

#endif //GL_MESH_HH
