#ifndef _MODEL
#define _MODEL

#include "ShaderProgram.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model{
    public:
        Model(GLchar* path);
        void draw(ShaderProgram & shader);
    private:
        std::vector<Mesh> meshes;
        std::vector<TextureSampler> texturesLoaded;
        std::string directory;

        void loadModel(const std::string & path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<TextureSampler> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
