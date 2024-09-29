#pragma once
#include <vector>
#include <string>
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "shader.h"
#include "mesh.h"



class Model {
public:
    Model(const std::string path){
        loadModel(path);
    }
    
    void draw(Shader &shader);
    unsigned int TextureFromFile(const char *path, const std::string &directory);
private:
    std::vector<Mesh> meshes;
    std::string directory;
    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
    
    

};

