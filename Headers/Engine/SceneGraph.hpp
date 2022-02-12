#pragma once

#include "Engine/ECS/Components/SceneComponent.hpp"
#include "Engine/ECS/ECS.hpp"

#include <cassert>
#include <map>

namespace Engine
{
/**
 * Scene graph
 */
class SceneGraph
{
private:
    /**
     * Scene graph node
     */
    struct Node
    {
        /**
         * Parent node
         */
        Node *parent;

        /**
         * Entity ID of the current node
         */
        ECS::entity_t entityId;

        /**
         * List of children
         */
        std::vector<Node*> children;

        /**
         * @brief Constructor
         */
        Node()
            : parent(nullptr)
            , entityId(ECS::NULL_ENTITY)
            , children()
        {
        }

        /**
         * @brief Adds the specified node as a child
         * @param[in] nodeToAdd Node to add
         */
        void AddChild(Node* nodeToAdd)
        {
            if (nodeToAdd == nullptr)
            {
                return;
            }

            for (size_t i = 0; i < children.size(); ++i)
            {
                if (children[i]->entityId == nodeToAdd->entityId)
                {
                    return;
                }
            }

            children.push_back(nodeToAdd);
        }

        /**
         * @brief Removes the specified ndoe as a child
         * @param[in] nodeToRemove Node to remove
         */
        void RemoveChild(const Node* nodeToRemove)
        {
            if (nodeToRemove == nullptr)
            {
                return;
            }

            for (size_t i = children.size(); i > 0; --i)
            {
                if (children[i]->entityId == nodeToRemove->entityId)
                {
                    children.erase(children.begin() + i);
                }
            }
        }
    };

private:
    /**
     * Scene graph root node
     */
    Node *m_root;

    /**
     * Map to store the mapping between an entity and its node
     */
    std::map<ECS::entity_t, Node*> m_allNodes;

public:
    /**
     * @brief Constructor
     */
    SceneGraph();

    /**
     * @brief Destructor
     */
    ~SceneGraph();

    /**
     * @brief Adds the specified entity to the scene graph
     * @param[in] entity Entity to add
     * @note This function adds a SceneComponent to the entity if the entity does not have it yet
     */
    void Add(const ECS::entity_t &entity);

    /**
     * @brief Adds the specified entity to the scene graph, and sets the entity's parent as specified
     * @param[in] entity Entity to add
     * @param[in] parent Parent of the entity to add
     * @note This function adds a SceneComponent to the entity if the entity does not have it yet
     */
    void Add(const ECS::entity_t &entity, const ECS::entity_t &parent);

    /**
     * @brief Removes the specified entity from the scene graph
     * @param[in] entity Entity to remove from the scene graph
     */
    void Remove(const ECS::entity_t &entity);

    /**
     * @brief Clears the scene graph
     */
    void Clear();

private:
    /**
     * @brief Refreshes the scene graph
     * @note This also updates the indices in the SceneComponent's of each entity
     */
    void Refresh();

    /**
     * @brief Internal method for adding an entity to the scene graph
     * @param[in] entity Entity to add
     * @param[in] parent Parent of the entity to add
     * @param[in] refresh Flag indicating whether to refresh the scene graph immediately
     */
    void AddInternal(const ECS::entity_t &entity, const ECS::entity_t &parent, bool refresh = true);

    /**
     * @brief Internal method for removing an entity from the scene graph
     * @param[in] entity Entity to remove
     * @param[in] refresh Flag indicating whether to refresh the scene graph immediately
     */
    void RemoveInternal(const ECS::entity_t &entity, bool refresh = true);
};
}
