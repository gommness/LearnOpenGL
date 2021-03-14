#include "VertexShader.h"
#include "Model.h"
#include "Utils.h"
#include <vector>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const GLchar* path){
    loadModel(path);
}

void Model::preDraw(ShaderProgram & shader){
    // TODO: set textureSampler uniforms
}

void Model::draw(ShaderProgram & shader){
    for(GLuint i=0; i < this->meshes.size(); ++i){
        this->meshes[i].draw(shader);
    }
}

void Model::loadModel(const std::string & path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));
    this->directory += "/";

    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
    // process all the node's meshes
    for(GLuint i=0; i < node->mNumMeshes; ++i){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }
    // process each of the node's children
    for(GLuint i=0; i < node->mNumChildren; ++i){
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> vertices;
    std::vector<GLuint> indexes;
    std::vector<TextureSampler*> textures;

    // process vertices
    for(GLuint i=0; i < mesh->mNumVertices; ++i){
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        // TODO we only use 1 texture coordinate per vertex, expand later
        if(mesh->mTextureCoords[0]){
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        // process vertex positions, normals and texture coordinates
        vertices.push_back(vertex); 
    }

    // process indexes
    for(GLuint i=0; i < mesh->mNumFaces; ++i){
        aiFace face = mesh->mFaces[i];
        for(GLuint j=0; j < face.mNumIndices; ++j){
            indexes.push_back(face.mIndices[j]);
        }
    }
    
    // process material
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<TextureSampler*> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<TextureSampler*> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<TextureSampler*> normalMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<TextureSampler*> ambientMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
    }
    
    return Mesh(vertices, indexes, textures);
}

std::vector<TextureSampler*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){
    std::vector<TextureSampler*> textures;

    for(GLuint i=0; i < mat->GetTextureCount(type); ++i){
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string filePath(this->directory);
        filePath += str.C_Str();

        GLboolean skip = false;
        for(GLuint j=0; j < texturesLoaded.size(); ++j){
            if(texturesLoaded[j].getFileName() == filePath){
                textures.push_back(&(texturesLoaded[j]));
                skip = true;
                break;
            }
        }
        if(!skip){
            TextureSampler texture;
            texture.load(filePath, GL_TEXTURE0+texturesLoaded.size());
            texture.setUniformName("material."+typeName);
            texture.setType(type);
            textures.push_back(&(texturesLoaded.emplace_back(texture)));
        }
    }
    return textures;
}

Material Model::loadMaterial(aiMaterial* mat){
    Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    return material;
}
