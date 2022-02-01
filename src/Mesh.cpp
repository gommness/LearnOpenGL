#include "Mesh.h"
#include "DebugTools.h"
#include "Model.h"

Mesh::Mesh(Model& owner, std::vector<Vertex> & vertices, std::vector<GLuint> & indexes, std::vector<unsigned int> & textureIds) :
    vertices(vertices), indexes(indexes), textureIds(textureIds), owner(&owner)
{
    setUp();
}

void Mesh::draw(ShaderProgram shader){
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    std::string uniformName;
    for(unsigned int i = 0; i < textureIds.size(); i++){
        switch(owner->getTexture(textureIds[i]).getType()){
            case aiTextureType_DIFFUSE:
                uniformName = owner->getTexture(textureIds[i]).getUniformName()+std::to_string(diffuseNr);
                diffuseNr++;
                break;
            case aiTextureType_SPECULAR:
                uniformName = owner->getTexture(textureIds[i]).getUniformName()+std::to_string(specularNr);
                specularNr++;
                break;
            default:
                uniformName = owner->getTexture(textureIds[i]).getUniformName();
                break;
        }
        shader.setUniform(uniformName,owner->getTexture(textureIds[i]));
        owner->getTexture(textureIds[i]).bind();
    }
    // reset tex units
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::string vertexToString(const Vertex & v){
    std::stringstream ss;
    ss << "position: "<< v.position.x<<", "<<v.position.y<<", "<<v.position.z<<". ";
    ss << "normal: "<< v.normal.x<<", "<<v.normal.y<<", "<<v.normal.z<<". ";
    ss << "texCoords: "<< v.texCoords.x<<", "<<v.texCoords.y<<".";
    return ss.str();
}

std::string Mesh::toString(){
    std::stringstream ss;
    for(unsigned int i=0; i<vertices.size(); ++i){
        ss << vertexToString(vertices[i]) << "\n";
    }
    return ss.str();
}

void Mesh::setUp(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLuint), &indexes[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    // vertex colors
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

    glBindVertexArray(0);
}
