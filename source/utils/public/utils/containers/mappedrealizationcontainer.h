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

            auto result = itElement->second.add<T>();

            onAdded( _key, result, std::type_index( typeid(T) ) );
            return result;
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
            assert( itElement != m_elements.end() ); //There are no elements for _key
            
            if (itElement != m_elements.end())
            {
                onRemoved( _key, itElement->second.getSafely<T>(), std::type_index(typeid(T)));
                itElement->second.remove<T>();
                if (itElement->second.size() == 0)
                {
                    m_elements.erase( itElement );
                }
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
        bool isRegistered() const
        {
            return m_containerRegistryTemplate.isRegistered<ClassToCheck>();
        }

        void visit( Key _key, std::function<void( std::shared_ptr<BaseClass> )> _function )
        {
            auto itElement = m_elements.find( _key );
            assert( itElement != m_elements.end() ); //There are no elements for _key

            itElement->second.visit( _function );
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

    private:

        using ContainersMap = std::map<Key, UniqueRealizationContainer<BaseClass>>;

        ContainersMap m_elements;
        UniqueRealizationContainer<BaseClass> m_containerRegistryTemplate;

    };
}