#include "model.hh"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directorya);

void Model::Draw(Shader &shader) {
  for (auto &mesh: meshes) {
    mesh.Draw(shader);
  }
}

void Model::loadModel(const string &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
    path,
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
  );
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned i = 0; i < node->mNumChildren; ++i) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

  // vertex
  for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;

    vertex.Position.x = mesh->mVertices[i].x;
    vertex.Position.y = mesh->mVertices[i].y;
    vertex.Position.z = mesh->mVertices[i].z;

    vertex.Normal.x = mesh->mNormals[i].x;
    vertex.Normal.y = mesh->mNormals[i].y;
    vertex.Normal.z = mesh->mNormals[i].z;

    if (mesh->mTextureCoords[0]) {
      vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
    } else {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }
  // indices
  for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // material
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }
  return {std::move(vertices), std::move(indices), std::move(textures)};
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
  vector<Texture> textures;
  for (unsigned i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (auto &j: textures_loaded) {
      if (j.path == str.C_Str()) {
        textures.push_back(j);
        skip = true;
        break;
      }
    }
    if (skip) {
      continue;
    }

    Texture texture;
    texture.id = TextureFromFile(str.C_Str(), directory);
    texture.type = typeName;
    texture.path = str.C_Str();
    textures.push_back(texture);
    textures_loaded.push_back(texture);
  }
  return std::move(textures);
}

unsigned int TextureFromFile(const char *path, const string &directory) {
  string filename = string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
