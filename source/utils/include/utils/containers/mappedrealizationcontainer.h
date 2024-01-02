#pragma once

#include <utils/containers/uniquerealizationcontainer.h>

#include <assert.h>
#include <functional>
#include <map>
#include <memory>
#include <typeindex>


namespace puma
{

    template<class Key, class BaseClass>
    class MappedRealizationContainer
    {
    public:

        MappedRealizationContainer() = default;

        MappedRealizationContainer& operator =( const MappedRealizationContainer& _container ) = delete;
        MappedRealizationContainer( const MappedRealizationContainer<Key, BaseClass>& _container ) = delete;

        MappedRealizationContainer( MappedRealizationContainer<Key, BaseClass>&& _container ) noexcept
            : m_elements( std::move( _container ) )
            , m_containerRegistryTemplate( std::move( _container ) )
        {}

        virtual ~MappedRealizationContainer()
        {
            clear();
        }

        MappedRealizationContainer& operator =( const MappedRealizationContainer&& _container )
        {
            m_elements = std::move(_container.m_elements);
            m_containerRegistryTemplate = std::move(_container.m_containerRegistryTemplate);
            return *this;
        }

        template<class InterfaceClass, class RealizedClass>
        void registerInterface()
        {
            m_containerRegistryTemplate.registerInterface<InterfaceClass, RealizedClass>();

            for (auto itElement = m_elements.begin(); itElement != m_elements.end(); ++itElement)
            {
                itElement->second.registerInterface<InterfaceClass, RealizedClass>();
            }
            onInterfaceRegistered( std::type_index( typeid(InterfaceClass) ), std::type_index( typeid(RealizedClass) ) );
        }

        template<class RealizedClass>
        void registerClass()
        {
            m_containerRegistryTemplate.registerClass<RealizedClass>();

            for (auto itElement = m_elements.begin(); itElement != m_elements.end(); ++itElement)
            {
                itElement->second.registerClass<RealizedClass>();
            }
            onClassRegistered( std::type_index( typeid(RealizedClass) ) );
        }

        template<class ClassToCheck>
        bool isRegistered() const
        {
            return m_containerRegistryTemplate.isRegistered<ClassToCheck>();
        }

        template<class T>
        std::shared_ptr<T> add( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            return std::static_pointer_cast<T>(add( _key, std::type_index( typeid(T) ) ));
        }

        template<class T>
        void remove( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            remove( _key, std::type_index( typeid(T) ) );
        }

        template<class T>
        T* get( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            return static_cast<T*>(get( _key, std::type_index( typeid(T) ) ));
        }

        template<class T>
        const T* get( Key _key ) const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            return static_cast<const T*>(get( _key, std::type_index( typeid(T) ) ));
        }

        template<class T>
        std::shared_ptr<T> getSafely( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            return std::static_pointer_cast<T>(getSafely( _key, std::type_index( typeid(T) ) ));
        }

        template<class T>
        std::shared_ptr<const T> getSafely( Key _key ) const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            return std::static_pointer_cast<const T>(getSafely( _key, std::type_index( typeid(T) ) ));
        }

        bool contains( Key _key ) const
        {
            return m_elements.contains( _key );
        }

        template<class T>
        bool contains( Key _key ) const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            return contains( _key, std::type_index( typeid(T) ) );
        }

        std::shared_ptr<BaseClass> add( Key _key, std::type_index _typeIndex )
        {
            auto itElement = m_elements.find( _key );

            if (m_elements.end() == itElement)
            {
                auto emplaceResult = m_elements.emplace( _key, std::move( m_containerRegistryTemplate.cloneRegistriesOnly() ) );
                assert( emplaceResult.second );
                if (!emplaceResult.second) return nullptr;

                itElement = emplaceResult.first;
            }

            auto result = itElement->second.add( _typeIndex );

            if (nullptr != result)
            {
                onAdded( _key, result, _typeIndex );
            }

            return result;
        }

        void remove( Key _key, std::type_index _typeIndex )
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key

            if (itElement != m_elements.end())
            {
                onRemoved( _key, itElement->second.getSafely( _typeIndex ), _typeIndex );
                itElement->second.remove( _typeIndex );
                if (itElement->second.size() == 0)
                {
                    m_elements.erase( itElement );
                }
            }
        }

        BaseClass* get( Key _key, std::type_index _typeIndex )
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely( _typeIndex ).get();
        }

        const BaseClass* get( Key _key, std::type_index _typeIndex ) const
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely( _typeIndex ).get();
        }

        std::shared_ptr<BaseClass> getSafely( Key _key, std::type_index _typeIndex )
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely( _typeIndex );
        }

        std::shared_ptr<const BaseClass> getSafely( Key _key, std::type_index _typeIndex) const
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely( _typeIndex );
        }

        bool contains( Key _key, std::type_index _typeIndex ) const
        {
            bool result = false;

            auto itElement = m_elements.find( _key );

            if (m_elements.end() != itElement)
            {
                result = itElement->second.contains( _typeIndex );
            }

            return result;
        }

        void visit( Key _key, std::function<void( std::shared_ptr<BaseClass> )> _function )
        {
            auto itElement = m_elements.find( _key );
            if (itElement != m_elements.end())
            {
                itElement->second.visit( _function );
            }
        }

        void clear()
        {
            m_elements.clear();
        }

        size_t size() const
        {
            return m_elements.size();
        }

    protected:

        virtual void onAdded( Key, std::shared_ptr<BaseClass>, std::type_index ) {}
        virtual void onRemoved( Key, std::shared_ptr<BaseClass>, std::type_index ) {}
        virtual void onClassRegistered( std::type_index _typeIndex ) {}
        virtual void onInterfaceRegistered( std::type_index _interfaceTypeIndex, std::type_index _classTypeIndex ) {}

    private:

        using ContainersMap = std::unordered_map<Key, UniqueRealizationContainer<BaseClass>>;
        ContainersMap m_elements;
        UniqueRealizationContainer<BaseClass> m_containerRegistryTemplate;

    };
}