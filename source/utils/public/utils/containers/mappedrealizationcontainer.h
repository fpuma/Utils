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

        MappedRealizationContainer( const MappedRealizationContainer<Key, BaseClass>& _container )
            : m_elements( _container.m_elements )
            , m_containerRegistryTemplate( _container.m_containerRegistryTemplate )
        {}

        MappedRealizationContainer( MappedRealizationContainer<Key, BaseClass>&& _container ) noexcept
            : m_elements( std::move( _container ) )
            , m_containerRegistryTemplate( std::move( _container ) )
        {}

        MappedRealizationContainer& operator =( const MappedRealizationContainer& _container )
        {
            m_elements = _container.m_elements;
            m_containerRegistryTemplate = _container.m_containerRegistryTemplate;
            return *this;
        }

        MappedRealizationContainer& operator =( const MappedRealizationContainer&& _container )
        {
            m_elements = std::move(_container.m_elements);
            m_containerRegistryTemplate = std::move(_container.m_containerRegistryTemplate);
            return *this;
        }

        template<class T>
        T* add( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            
            if ( m_elements.end() == itElement )
            {
                auto emplaceResult = m_elements.emplace( _key, std::move( m_containerRegistryTemplate.cloneRegistriesOnly() ) );
                assert( emplaceResult.second );

                itElement = emplaceResult.first;
                assert( m_elements.end() != itElement );
            }

            return itElement->second.add<T>();
        }

        template<class T>
        T* get( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key

            return static_cast<T*>(itElement->second.get<T>());
        }

        template<class T>
        const T* get(Key _key) const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find(_key);
            assert(itElement != m_elements.end()); //There are no elements for _key

            return static_cast<T*>(itElement->second.get<T>());
        }

        template<class T>
        void remove( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            assert( m_elements.end() != itElement ); //The _key has no elements assigned
            
            itElement->second.remove<T>();
        }

        template<class T>
        bool contains( Key _key ) const
        {
            bool result = false;

            auto itElement = m_elements.find( _key );

            if ( m_elements.end() != itElement )
            {
                result = itElement->second.contains<T>();
            }

            return result;
        }

        template<class InterfaceClass, class RealizedClass>
        void registerInterface()
        {
            m_containerRegistryTemplate.registerInterface<InterfaceClass, RealizedClass>();

            for ( auto itElement = m_elements.begin(); itElement != m_elements.end(); ++itElement )
            {
                itElement->second.registerInterface<InterfaceClass, RealizedClass>();
            }
        }

        template<class RealizedClass>
        void registerClass()
        {
            m_containerRegistryTemplate.registerClass<RealizedClass>();

            for ( auto itElement = m_elements.begin(); itElement != m_elements.end(); ++itElement )
            {
                itElement->second.registerClass<RealizedClass>();
            }
        }

        void clear()
        {
            m_elements.clear();
        }

    private:

        std::map<Key, UniqueRealizationContainer<BaseClass>> m_elements;
        UniqueRealizationContainer<BaseClass> m_containerRegistryTemplate;

    };
}