#ifndef GL_MODEL_HH
#define GL_MODEL_HH

#include <vector>
#include <string>

#include <assimp/scene.h>

#include "shader.hh"
#include "mesh.hh"

using namespace std;

class Model {
public:
  Model(const char *path) {
    loadModel(path);
  }

  void Draw(Shader &shader);

private:
  vector<Mesh> meshes;
  string directory;

  vector<Texture> textures_loaded;

  void loadModel(const string &path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif //GL_MODEL_HH
