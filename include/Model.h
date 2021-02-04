
class Model{
    public:
        Model(GLchar* path);
        void draw(ShaderProgram & shader);
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processModel(aiNode* node, const aiScene* scene);
        Mesh processModel(aiMesh* mesh, const aiScene* scene);
        vector<TextureSampler> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
}
