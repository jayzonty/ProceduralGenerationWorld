#include "Engine/SceneGraph.hpp"

#include "Engine/ECS/Components/SceneComponent.hpp"
#include "Engine/ECS/ECS.hpp"
#include <queue>

namespace Engine
{
/**
 * @brief Constructor
 */
SceneGraph::SceneGraph()
    : m_root(nullptr)
    , m_allNodes()
{
    m_root = new Node();
}

/**
 * @brief Destructor
 */
SceneGraph::~SceneGraph()
{
    Clear();

    if (m_root != nullptr)
    {
        delete m_root;
        m_root = nullptr;
    }
}

/**
 * @brief Adds the specified entity to the scene graph
 * @param[in] entity Entity to add
 * @note This function adds a SceneComponent to the entity if the entity does not have it yet
 */
void SceneGraph::Add(const ECS::entity_t &entity)
{
    AddInternal(entity, ECS::NULL_ENTITY);
}

/**
 * @brief Adds the specified entity to the scene graph, and sets the entity's parent as specified
 * @param[in] entity Entity to add
 * @param[in] parent Parent of the entity to add
 * @note This function adds a SceneComponent to the entity if the entity does not have it yet
 */
void SceneGraph::Add(const ECS::entity_t &entity, const ECS::entity_t &parent)
{
    AddInternal(entity, parent);
}

/**
 * @brief Removes the specified entity from the scene graph
 * @param[in] entity Entity to remove from the scene graph
 */
void SceneGraph::Remove(const ECS::entity_t &entity)
{
    RemoveInternal(entity, true);
}

/**
 * @brief Clears the scene graph
 */
void SceneGraph::Clear()
{
    for (auto& it : m_allNodes)
    {
        delete it.second;
    }
    m_allNodes.clear();
}

/**
 * @brief Refreshes the scene graph
 * @note This also updates the indices in the SceneComponent's of each entity
 */
void SceneGraph::Refresh()
{
    uint32_t index = 0;

    std::queue<Node*> q;
    q.push(m_root);

    while (!q.empty())
    {
        Node *node = q.front();
        q.pop();

        if (node->entityId != m_root->entityId)
        {
            SceneComponent *sceneComponent = ECS::Registry::GetComponent<SceneComponent>(node->entityId);
            if (sceneComponent != nullptr)
            {
                sceneComponent->parent = node->parent->entityId;
                sceneComponent->indexInHierarchy = index;
            }
        }
        ++index;

        for (size_t i = 0; i < node->children.size(); ++i)
        {
            q.push(node->children[i]);
        }
    }
}

/**
 * @brief Internal method for adding an entity to the scene graph
 * @param[in] entity Entity to add
 * @param[in] parent Parent of the entity to add
 * @param[in] refresh Flag indicating whether to refresh the scene graph immediately
 */
void SceneGraph::AddInternal(const ECS::entity_t &entity, const ECS::entity_t &parent, bool refresh)
{
    // Do not allow duplicate entries within the scene graph
    if (m_allNodes.find(entity) != m_allNodes.end())
    {
        return;
    }

    Node *newNode = new Node();
    newNode->entityId = entity;
    m_allNodes[entity] = newNode;

    Node *parentNode = m_root;
    if (m_allNodes.find(parent) != m_allNodes.end())
    {
        parentNode = m_allNodes[parent];
    }
    
    newNode->parent = parentNode;
    parentNode->children.push_back(newNode);

    if (!ECS::Registry::HasComponent<SceneComponent>(entity))
    {
        SceneComponent *sceneComponent = ECS::Registry::AddComponent<SceneComponent>(entity);
        std::cout << entity << ": " << std::hex << sceneComponent << std::endl;
        sceneComponent->parent = parent;
    }

    if (refresh)
    {
        Refresh();
    }
}

/**
 * @brief Internal method for removing an entity from the scene graph
 * @param[in] entity Entity to remove
 * @param[in] refresh Flag indicating whether to refresh the scene graph immediately
 */
void SceneGraph::RemoveInternal(const ECS::entity_t &entity, bool refresh)
{
    if (m_allNodes.find(entity) == m_allNodes.end())
    {
        return;
    }

    delete m_allNodes[entity];
    m_allNodes.erase(entity);

    ECS::Registry::RemoveComponent<SceneComponent>(entity);

    if (refresh)
    {
        Refresh();
    }
}
}
