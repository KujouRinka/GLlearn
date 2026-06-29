#include <utility>

#include "mesh.hh"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
  this->vertices = std::move(vertices);
  this->indices = std::move(indices);
  this->textures = std::move(textures);

  setupMesh();
}

void Mesh::Draw(Shader &shader) {
  GLuint diffuseNr = 1;
  GLuint specularNr = 1;
  for (unsigned i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    string number;
    string name = textures[i].type;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuseNr++);
    } else if (name == "texture_specular") {
      number = std::to_string(specularNr++);
    }
    shader.setInt(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
}
