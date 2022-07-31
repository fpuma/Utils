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

        ~MappedRealizationContainer()
        {
            clear();
        }

        MappedRealizationContainer& operator =( const MappedRealizationContainer&& _container )
        {
            m_elements = std::move(_container.m_elements);
            m_containerRegistryTemplate = std::move(_container.m_containerRegistryTemplate);
            return *this;
        }

        template<class T>
        std::shared_ptr<T> add( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            
            if ( m_elements.end() == itElement )
            {
                auto emplaceResult = m_elements.emplace( _key, std::move( m_containerRegistryTemplate.cloneRegistriesOnly() ) );
                assert( emplaceResult.second );
                if (!emplaceResult.second) return nullptr;

                itElement = emplaceResult.first;
            }

            return itElement->second.add<T>();
        }
        template<class T>
        T* get( Key _key )
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely<T>().get();
        }
        template<class T>
        const T* get( Key _key) const
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely<T>().get();
        }

        template<class T>
        std::shared_ptr<T> getSafely( Key _key )
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely<T>();
        }

        template<class T>
        std::shared_ptr<const T> getSafely( Key _key ) const
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second.getSafely<T>();
        }

        template<class T>
        void remove( Key _key )
        {
            static_assert(std::is_base_of<BaseClass, T>::value);

            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() );
            
            if (itElement != m_elements.end())
            {
                itElement->second.remove<T>();
            }
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

        template<class ClassToCheck>
        bool isRegistered()
        {
            return m_containerRegistryTemplate.isRegistered<ClassToCheck>();
        }

        void clear()
        {
            m_elements.clear();
        }

    private:

        using ContainersMap = std::map<Key, UniqueRealizationContainer<BaseClass>>;

        ContainersMap m_elements;
        UniqueRealizationContainer<BaseClass> m_containerRegistryTemplate;

    };
}