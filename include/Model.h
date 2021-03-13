#ifndef _MODEL
#define _MODEL

#include "ShaderProgram.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float shininess;
};

class Model{
    public:
        Model(const GLchar* path);
        void draw(ShaderProgram & shader);
    private:
        std::vector<Mesh> meshes;
        std::vector<TextureSampler> texturesLoaded;
        std::string directory;

        void loadModel(const std::string & path);
        void preDraw(ShaderProgram & shader);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<TextureSampler*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        Material loadMaterial(aiMaterial* mat);      
};

#endif
