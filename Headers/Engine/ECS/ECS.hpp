#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

/**
 * Implementation based on the article:
 * https://austinmorlan.com/posts/entity_component_system/
 */

namespace Engine
{
namespace ECS
{
/**
 * Hard limit on the number of entities in the ENTIRE SYSTEM
 */
const uint32_t MAX_ENTITIES = 1000;

/**
 * Entity is just an index/ID.
 */
using entity_t = uint32_t;

/**
 * Null entity identifier
 */
const entity_t NULL_ENTITY = MAX_ENTITIES;

/**
 * Registry class containing the data necessary to 
 * handle the ECS system.
 */
class Registry
{
private:
    /**
     * Base class for the generic array that will contain
     * the component data for each entity.
     */
    class IComponentArray
    {
    public:
        /**
         * @brief Destructor
         */
        virtual ~IComponentArray() = default;

        /**
         * @brief Handler function for when the specified entity is destroyed.
         * @param[in] entity Entity ID
         */
        virtual void EntityDestroyed(const entity_t& entity) = 0;
    };

    /**
     * Generic array type that will contain the component data
     * for each entity. The array is designed to be constantly
     * tightly-packed.
     */
    template <typename T>
    class ComponentArray : public IComponentArray
    {
    private:
        /**
         * Data entry for an element in the component array
         */
        struct ComponentDataEntry
        {
            /**
             * Entity ID of the entity that owns this data
             */
            entity_t entity;

            /**
             * Actual data
             */
            T data;
        };

    private:
        /**
         * List of component data
         */
        std::vector<ComponentDataEntry> m_data;

        /**
         * Mapping between an entity ID and the index in the array
         * that contains its data.
         */
        std::unordered_map<entity_t, size_t> m_entityToDataIndexMap;

        /**
         * Number of occupied entries in the array.
         */
        size_t m_actualSize;

    public:
        /**
         * @brief Constructor
         */
        ComponentArray()
            : m_data(MAX_ENTITIES)
            , m_entityToDataIndexMap()
            , m_actualSize(0)
        {}

        /**
         * @brief Destructor
         */
        ~ComponentArray()
        {}

        /**
         * @brief Gets a list of all entities that this component array is keeping track of
         * @return LIst of entities that this component array is keeping track of
         */
        std::vector<entity_t> GetAllEntities()
        {
            std::vector<entity_t> ret;
            for (auto& it : m_entityToDataIndexMap)
            {
                ret.push_back(it.first);
            }
            return ret;
        }

        /**
         * @brief Inserts the component data of an entity into the array.
         * @param[in] entity Entity ID
         * @param[in] componentData Component data
         */
        void Insert(const entity_t& entity, const T& componentData)
        {
            if (m_entityToDataIndexMap.find(entity) != m_entityToDataIndexMap.end())
            {
                return;
            }

            if (entity < m_data.size())
            {
                m_data[m_actualSize].entity = entity;
                m_data[m_actualSize].data = componentData;

                m_entityToDataIndexMap.insert({entity, m_actualSize});
                ++m_actualSize;
            }
        }

        /**
         * @brief Removes the component data for an entity.
         * @param[in] entity Entity ID
         */
        void Remove(const entity_t& entity)
        {
            if (m_entityToDataIndexMap.find(entity) == m_entityToDataIndexMap.end())
            {
                // Entity is not mapped to a data
                return;
            }

            if (entity < m_data.size())
            {
                size_t lastIndex = m_actualSize - 1;
                size_t dataIndexOfRemovedEntity = m_entityToDataIndexMap[entity];

                // Place the last element in the array to the spot that just freed up
                m_data[dataIndexOfRemovedEntity] = m_data[lastIndex];

                // Update the data index of the entity that was just swapped to its new location
                entity_t entityToMove = m_data[dataIndexOfRemovedEntity].entity;
                m_entityToDataIndexMap[entityToMove] = dataIndexOfRemovedEntity;

                --m_actualSize;
            }
        }

        /**
         * @brief Tells whether this component array contains data for the specified entity
         * @param[in] entity Entity to check
         * @return Returns true if this component array contains data for the specified entity
         */
        bool HasDataForEntity(const entity_t& entity)
        {
            return m_entityToDataIndexMap.find(entity) != m_entityToDataIndexMap.end();
        }

        /**
         * @brief Gets the component data for the entity.
         * @param[in] entity Entity ID
         */
        T& Get(const entity_t& entity)
        {
            size_t dataIndex = m_entityToDataIndexMap[entity];
            return m_data[dataIndex].data;
        }

        /**
         * @brief Handler function for when an entity is destroyed.
         * @param[in] entity Entity ID
         */
        void EntityDestroyed(const entity_t& entity) override
        {
            Remove(entity);
        }
    };

    /**
     * Manager for all component arrays.
     */
    class ComponentManager
    {
    private:
        /**
         * Mapping between a component name to its ID. In
         * the context of the signature, the ID corresponds to
         * which bit this component is in the bitset.
         */
        std::unordered_map<std::string, IComponentArray*> m_componentNameToArrayMap;

    public:
        /**
         * @brief Constructor
         */
        ComponentManager()
            : m_componentNameToArrayMap()
        {}

        /**
         * @brief Destructor
         */
        ~ComponentManager()
        {
            for (auto &it : m_componentNameToArrayMap)
            {
                delete it.second;
            }
            m_componentNameToArrayMap.clear();
        }

        /**
         * @brief Gets the component array associated with the specified type
         * @return Component array associated with the specified type
         */
        template <typename T>
        ComponentArray<T>* GetComponentArray()
        {
            std::string typeName = typeid(T).name();
            if (m_componentNameToArrayMap.find(typeName) == m_componentNameToArrayMap.end())
            {
                return nullptr;
            }

            ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T>*>(m_componentNameToArrayMap[typeName]);
            return componentArray;
        }

        /**
         * @brief Adds the specified component to the specified entity.
         * @param[in] entity Entity ID
         * @param[in] initialData Initial data of the component
         */
        template <typename T>
        void AddComponentToEntity(const entity_t& entity, const T& initialData)
        {
            std::string typeName = typeid(T).name();
            if (m_componentNameToArrayMap.find(typeName) == m_componentNameToArrayMap.end())
            {
                ComponentArray<T> *array = new ComponentArray<T>();
                m_componentNameToArrayMap[typeName] = array;
            }

            ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T>*>(m_componentNameToArrayMap[typeName]);
            componentArray->Insert(entity, initialData);
        }

        /**
         * @brief Checks whether there is component data associated with the specified entity
         * @param[in] entity Entity to check
         * @return Returns true if there is component data associated with the specified entity
         */
        template <typename T>
        bool HasComponentData(const entity_t& entity)
        {
            std::string typeName = typeid(T).name();
            if (m_componentNameToArrayMap.find(typeName) == m_componentNameToArrayMap.end())
            {
                return false;
            }

            ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T>*>(m_componentNameToArrayMap[typeName]);
            return componentArray->HasDataForEntity(entity);
        }

        /**
         * @brief Gets the component data attached to the entity.
         * @param[in] entity Entity ID
         * @return Data for the component that is attached to the entity.
         */
        template <typename T>
        T& GetComponentData(const entity_t& entity)
        {
            std::string typeName = typeid(T).name();
            assert(m_componentNameToArrayMap.find(typeName) != m_componentNameToArrayMap.end());

            ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T>*>(m_componentNameToArrayMap[typeName]);
            return componentArray->Get(entity);
        }

        /**
         * @brief Removes/de-attaches the specified component type from the specified entity.
         * @param[in] entity Entity ID
         */
        template <typename T>
        void RemoveComponentFromEntity(const entity_t& entity)
        {
            std::string typeName = typeid(T).name();
            if (m_componentNameToArrayMap.find(typeName) == m_componentNameToArrayMap.end())
            {
                return;
            }

            ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T>*>(m_componentNameToArrayMap[typeName]);
            componentArray->Remove(entity);
        }

        /**
         * @brief Handler function for when an entity is destroyed.
         * @param[in] entity Entity ID
         */
        void OnEntityDestroyed(const entity_t& entity)
        {
            for (auto &it : m_componentNameToArrayMap)
            {
                it.second->EntityDestroyed(entity);
            }
        }
    };

private:
    /**
     * Queue containing all the available entity IDs.
     */
    std::queue<entity_t> m_availableEntities;

    /**
     * Component manager
     */
    ComponentManager m_componentManager;

private:
    /**
     * @brief Constructor.
     */
    Registry()
        : m_availableEntities()
        , m_componentManager()
    {
        for (uint32_t i = 0; i < MAX_ENTITIES; ++i)
        {
            m_availableEntities.push(i);
        }
    }

    /**
     * @brief Handler function for when an entity was destroyed.
     * @param[in] entity Entity ID
     */
    void OnEntityDestroyed(const entity_t& entity)
    {
        m_componentManager.OnEntityDestroyed(entity);
    }

    /**
     * @brief Filters the specified set of entities such that it only contains entities that
     * contains the specified component type
     * @param[in] entities Set of entities to filter
     */
    template <typename T>
    void FilterEntitiesWithComponent(std::set<entity_t>& entities)
    {
        ComponentArray<T> *array = GetInstance().m_componentManager.GetComponentArray<T>();
        if (array != nullptr)
        {
            std::vector<entity_t> entitiesToRemove;

            for (auto &it : entities)
            {
                if (!array->HasDataForEntity(it))
                {
                    entitiesToRemove.push_back(it);
                }
            }
            for (auto &it : entitiesToRemove)
            {
                entities.erase(it);
            }
        }
    }

public:
    // Delete copy constructor and copy operator
    Registry(const Registry&) = delete;
    void operator=(const Registry&) = delete;

    /**
     * @brief Gets the singleton instance for this class.
     * @return Singleton instance for this class.
     */
    static Registry& GetInstance()
    {
        static Registry instance;
        return instance;
    }

    /**
     * @brief Creates an entity.
     * @return Entity ID of the created entity
     */
    static entity_t CreateEntity()
    {
        Registry& instance = GetInstance();
        
        entity_t ret = instance.m_availableEntities.front();
        instance.m_availableEntities.pop();

        return ret;
    }

    /**
     * @brief Destroys the specified entity.
     * @param[in] entity Entity ID of the entity to destroy
     */
    static void DestroyEntity(const entity_t& entity)
    {
        Registry& instance = GetInstance();

        instance.OnEntityDestroyed(entity);
        instance.m_availableEntities.push(entity);
    }

    /**
     * @brief Adds the specified component type to the specified entity.
     * @param[in] entity Entity ID
     * @param[in] data Initial data of the component
     * @return Reference to the underlying component data
     */
    template <typename T>
    static T* AddComponent(const entity_t& entity, const T& data = {})
    {
        Registry &instance = GetInstance();
        instance.m_componentManager.AddComponentToEntity<T>(entity, data);
        return &instance.m_componentManager.GetComponentData<T>(entity);
    }

    /**
     * @brief Checks whether the specified entity contains the specified component type
     * @param[in] entity Entity to check
     * @return Returns true if the entity contains the specified component type
     */
    template <typename T>
    static bool HasComponent(const entity_t& entity)
    {
        return GetInstance().m_componentManager.HasComponentData<T>(entity);
    }

    /**
     * @brief Gets the reference to the component data of the 
     * specified type that is attached to the entity.
     * @param[in] entity Entity ID
     * @return Reference to the component data
     */
    template <typename T>
    static T* GetComponent(const entity_t& entity)
    {
        if (HasComponent<T>(entity))
        {
            return &GetInstance().m_componentManager.GetComponentData<T>(entity);
        }
        return nullptr;
    }

    /**
     * @brief Removes the specified component type from the entity.
     * @param[in] entity Entity ID
     */
    template <typename T>
    static void RemoveComponent(const entity_t& entity)
    {
        GetInstance().m_componentManager.RemoveComponentFromEntity<T>(entity);
    }

    /**
     * @brief Gets the set of entities that contain the specified component type
     * @return Set of entities that contain the specified component type
     */
    template <typename A>
    static std::set<entity_t> GetEntitiesWithComponents()
    {
        std::set<entity_t> ret;

        ComponentArray<A> *array = GetInstance().m_componentManager.GetComponentArray<A>();
        if (array != nullptr)
        {
            std::vector<entity_t> entities = array->GetAllEntities();
            ret.insert(entities.begin(), entities.end());
        }

        return ret;
    }

    /**
     * @brief Gets the set of entities that contain the specified combination of component types
     * @return Set of entities that contain the specified combination of component types
     */
    template <typename A, typename B>
    static std::set<entity_t> GetEntitiesWithComponents()
    {
        std::set<entity_t> ret = GetEntitiesWithComponents<A>();
        GetInstance().FilterEntitiesWithComponent<B>(ret);
        return ret;
    }

    /**
     * @brief Gets the set of entities that contain the specified combination of component types
     * @return Set of entities that contain the specified combination of component types
     */
    template <typename A, typename B, typename C>
    static std::set<entity_t> GetEntitiesWithComponents()
    {
        std::set<entity_t> ret = GetEntitiesWithComponents<A, B>();
        GetInstance().FilterEntitiesWithComponent<C>(ret);
        return ret;
    }

    /**
     * @brief Gets the set of entities that contain the specified combination of component types
     * @return Set of entities that contain the specified combination of component types
     */
    template <typename A, typename B, typename C, typename D>
    static std::set<entity_t> GetEntitiesWithComponents()
    {
        std::set<entity_t> ret = GetEntitiesWithComponents<A, B, C>();
        GetInstance().FilterEntitiesWithComponent<D>(ret);
        return ret;
    }
};
}
}
