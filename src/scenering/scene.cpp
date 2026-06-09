#include "scene.h"

GameObject *Scene::createGameObject(const std::string &name)
{
    auto gameObject = std::make_unique<GameObject>(*this);
    GameObject *gameObjectPtr = gameObject.get();
    gameObject->setName(name);
    m_gameObjects.push_back(std::move(gameObject));
    return gameObjectPtr;
}

void Scene::start()
{
    for (const auto &gameObject : m_gameObjects)
    {
        gameObject->start();
    }
}

void Scene::update(float deltaTime)
{
    for (const auto &gameObject : m_gameObjects)
    {
        gameObject->update(deltaTime);
    }
}

GameObject *Scene::instantiateModel(const std::shared_ptr<Model> &model)
{
    return instantiateModelNode(model->rootNode, model, nullptr);
}

GameObject *Scene::getGameObjectByName(const std::string &name) const
{
    for (const auto &gameObject : m_gameObjects)
    {
        if (gameObject->getName() == name)
        {
            return gameObject.get();
        }
    }
    return nullptr;
}

GameObject *Scene::instantiateModelNode(const ModelNode &node, const std::shared_ptr<Model> &model, GameObject *parent)
{
    auto gameObject = createGameObject();

    gameObject->setName(node.name);

    if (parent)
    {
        gameObject->setParent(parent);
    }

    auto transform = gameObject->addComponent<Transform>();

    transform->setLocalPosition(node.position);
    transform->setLocalRotationQuat(node.rotation);
    transform->setLocalScale(node.scale);

    if(node.meshIndices.size() > 1)
    {
        for (unsigned int meshIndex : node.meshIndices)
        {
            if (meshIndex >= model->meshes.size())
                throw std::runtime_error("Mesh index out of bounds in model.");

            const auto& meshData = model->meshes[meshIndex];
            gameObject->addComponent<MeshRenderer>(meshData.mesh, meshData.material);
        }
    }
    else if (!node.meshIndices.empty())
    {
        const auto& meshData = model->meshes[node.meshIndices[0]];
        gameObject->addComponent<MeshRenderer>(meshData.mesh, meshData.material);
    }

    for (const auto &childNode : node.children)
    {
        instantiateModelNode(*childNode, model, gameObject);
    }

    return gameObject;
}
