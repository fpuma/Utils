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

        static MappedRealizationContainer<Key, BaseClass> createWithExistingRegistries( const MappedRealizationContainer<Key, BaseClass>& _container )
        {
            return MappedRealizationContainer<BaseClass>( _container );
        }

        MappedRealizationContainer() = default;

        MappedRealizationContainer( MappedRealizationContainer<Key, BaseClass>&& _container ) noexcept
            : m_elements( std::move( _container ) )
            , m_containerRegistryTemplate( std::move( _container ) )
        {}

        MappedRealizationContainer& operator =( const MappedRealizationContainer& _services ) = delete;

        template<class T>
        T* add( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            
            if ( m_elements.end() == itElement )
            {
                auto emplaceResult = m_elements.emplace( _key, std::move( UniqueRealizationContainer<BaseClass>::createWithExistingRegistries( m_containerRegistryTemplate ) ) );
                assert( emplaceResult.second );

                itElement = emplaceResult.first;
                assert( m_elements.end() != itElement );
            }

            return itElement->second.add<T>();
        }

        template<class T>
        T* get( Key _key ) const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key

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
        bool exists( Key _key ) const
        {
            bool result = false;

            auto itElement = m_elements.find( _key );

            if ( m_elements.end() != itElement )
            {
                result = itElement->second.exists<T>();
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

        MappedRealizationContainer( const MappedRealizationContainer<Key, BaseClass>& _container )
            : m_elements()
            , m_containerRegistryTemplate( _container.m_containerRegistryTemplate )
        {}

        std::map<Key, UniqueRealizationContainer<BaseClass>> m_elements;
        UniqueRealizationContainer<BaseClass> m_containerRegistryTemplate;

    };
}