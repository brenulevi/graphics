#include "modelloader.h"

std::shared_ptr<Model> ModelLoader::loadFromFile(const std::string &filepath)
{
    auto importer = Assimp::Importer();

    const aiScene* scene = importer.ReadFile(
        filepath,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::runtime_error("Failed to load model: " + std::string(importer.GetErrorString()));
    }

    std::string directory = filepath.substr(0, filepath.find_last_of('/'));

    auto model = std::make_shared<Model>();
    model->rootNode = processNode(scene->mRootNode, scene, *model, directory);
    return model;
}

ModelNode ModelLoader::processNode(const aiNode *node, const aiScene *scene, Model &model, std::string& directory)
{
    ModelNode modelNode;

    modelNode.name = node->mName.C_Str();

    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D translation;

    node->mTransformation.Decompose(scaling, rotation, translation);

    modelNode.position = glm::vec3(translation.x, translation.y, translation.z);
    modelNode.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
    modelNode.scale = glm::vec3(scaling.x, scaling.y, scaling.z);
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[i]];

        ModelMesh modelMesh = processMesh(assimpMesh, scene, directory);

        unsigned int meshIndex = model.meshes.size();
        model.meshes.push_back(modelMesh);

        modelNode.meshIndices.push_back(meshIndex);
    }

    // Process child nodes recursively
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        modelNode.children.push_back(
            std::make_unique<ModelNode>(processNode(node->mChildren[i], scene, model, directory))
        );
    }

    return modelNode;
}

ModelMesh ModelLoader::processMesh(const aiMesh *mesh, const aiScene *scene, std::string& directory)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(mesh->mNumVertices);

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = 
        {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        if (mesh->mNormals)
        {
            vertex.normal =
            {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }
        else
        {
            vertex.normal = { 0.0f, 0.0f, 0.0f };
        }

        if (mesh->mTextureCoords[0]) // Check if texture coordinates are present
        {
            vertex.texCoords =
            {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }
        else
        {
            vertex.texCoords = { 0.0f, 0.0f };
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    ModelMesh modelMesh;
    modelMesh.mesh = std::make_shared<Mesh>(vertices, indices);

    // Process material
    unsigned int materialIndex = mesh->mMaterialIndex;
    aiMaterial* material = scene->mMaterials[materialIndex];

    modelMesh.material = processMaterial(material, directory);

    return modelMesh;
}

std::shared_ptr<Material> ModelLoader::processMaterial(const aiMaterial *material, std::string& directory)
{
    auto mat = std::make_shared<Material>();

    // Shader
    mat->setShader(AssetManager::getShader("default"));

    // Shininess
    float shininess = 32.0f; // Default shininess
    material->Get(AI_MATKEY_SHININESS, shininess);
    mat->setShininess(shininess);

    // Diffuse texture
    aiString diffusePath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath) == AI_SUCCESS)
    {
        std::string fullPath = directory + "/" + diffusePath.C_Str();

        auto diffuseTexture = AssetManager::loadTexture(diffusePath.C_Str(), fullPath);

        mat->setDiffuse(diffuseTexture);
    }

    // Specular texture
    aiString specularPath;
    if (material->GetTexture(aiTextureType_SPECULAR, 0, &specularPath) == AI_SUCCESS)
    {
        std::string fullPath = directory + "/" + specularPath.C_Str();

        auto specularTexture = AssetManager::loadTexture(specularPath.C_Str(), fullPath);

        mat->setSpecular(specularTexture);
    }

    return mat;
}
