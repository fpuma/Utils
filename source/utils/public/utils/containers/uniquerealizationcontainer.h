#pragma once

#include <assert.h>
#include <functional>
#include <map>
#include <memory>
#include <typeindex>

namespace puma
{

    template<class BaseClass>
    class UniqueRealizationContainer
    {
    public:

        UniqueRealizationContainer() = default;

        UniqueRealizationContainer( const UniqueRealizationContainer<BaseClass>& _container )
            : m_elements( _container.m_elements)
            , m_registeredClasses( _container.m_registeredClasses )
            , m_factories( _container.m_factories )
        {}

        UniqueRealizationContainer( UniqueRealizationContainer<BaseClass>&& _container ) noexcept
            : m_elements( std::move( _container.m_elements ) )
            , m_registeredClasses( std::move( _container.m_registeredClasses ) )
            , m_factories( std::move( _container.m_factories ) )
        {}

        UniqueRealizationContainer& operator =( const UniqueRealizationContainer<BaseClass>& _container )
        {
            m_elements = _container.m_elements;
            m_registeredClasses = _container.m_registeredClasses;
            m_factories = _container.m_factories;
            return *this;
        }
        
        UniqueRealizationContainer& operator =( const UniqueRealizationContainer<BaseClass>&& _container )
        {
            m_elements = std::move(_container.m_elements);
            m_registeredClasses = std::move(_container.m_registeredClasses);
            m_factories = std::move(_container.m_factories);
            return *this;
        }

        UniqueRealizationContainer<BaseClass> cloneRegistriesOnly()
        {
            return UniqueRealizationContainer<BaseClass>( *this, true );
        }

        template<class T>
        T* add()
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            auto typeIndex = std::type_index( typeid(T) );

            auto itElement = getElement<T>();
            assert( itElement == m_elements.end() ); //An element of that type already exists

            auto itRegisteredClass = m_registeredClasses.find( typeIndex );
            assert( itRegisteredClass != m_registeredClasses.end() ); //The class or interface you are trying to add has not been registered

            auto itFactory = m_factories.find( itRegisteredClass->second );
            assert( itFactory != m_factories.end() ); //Could not find the factory

            auto emplaceResult = m_elements.emplace( itRegisteredClass->second, itFactory->second() );
            assert( emplaceResult.second );

            itElement = emplaceResult.first;
            assert( m_elements.end() != itElement );

            onAdded( itElement->first, itElement->second.get() );

            return static_cast<T*>(itElement->second.get());
        }

        template<class T>
        T* get()
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            auto typeIndex = std::type_index( typeid(T) );
            
            auto itRegisteredClass = m_registeredClasses.find( typeIndex );
            assert( itRegisteredClass != m_registeredClasses.end() ); // The type has not been registered

            auto itElement = m_elements.find( itRegisteredClass->second );
            assert( itElement != m_elements.end() ); //There is no element of that type

            return static_cast<T*>(itElement->second.get());
        }

        template<class T>
        const T* get() const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            auto typeIndex = std::type_index(typeid(T));

            auto itRegisteredClass = m_registeredClasses.find(typeIndex);
            assert(itRegisteredClass != m_registeredClasses.end()); // The type has not been registered

            auto itElement = m_elements.find(itRegisteredClass->second);
            assert(itElement != m_elements.end()); //There is no element of that type

            return static_cast<T*>(itElement->second.get());
        }

        template<class T>
        void remove()
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            
            auto itElement = getElement<T>();
            assert( itElement != m_elements.end() ); //The element to be removed does not exist 

            onRemoved( itElement->first, itElement->second.get() );

            m_elements.erase( itElement );
        }

        template<class T>
        bool contains() const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            
            bool result = false;

            auto typeIndex = std::type_index( typeid(T) );
            auto itRegisteredClass = m_registeredClasses.find( typeIndex );
            
            if ( itRegisteredClass != m_registeredClasses.end() )
            {
                result = m_elements.contains( itRegisteredClass->second );
            }

            return result;
        }

        template<class InterfaceClass, class RealizedClass>
        void registerInterface()
        {
            static_assert(std::is_base_of<BaseClass, InterfaceClass>::value);
            static_assert(std::is_base_of<InterfaceClass, RealizedClass>::value);

            auto interfaceType = std::type_index( typeid(InterfaceClass) );
            auto realizedType = std::type_index( typeid(RealizedClass) );

#ifdef _DEBUG
            auto itRegisteredClass = m_registeredClasses.find( interfaceType );
            assert( itRegisteredClass == m_registeredClasses.end() ); //That interface has already been registered
            itRegisteredClass = m_registeredClasses.find( realizedType );
            assert( itRegisteredClass == m_registeredClasses.end() ); //That realization has already been registered
#endif

            m_registeredClasses.emplace( interfaceType, realizedType );
            m_registeredClasses.emplace( realizedType, realizedType );
            m_factories.emplace( realizedType, []() { return std::make_unique<RealizedClass>(); } );
        }

        template<class RealizedClass>
        void registerClass()
        {
            static_assert(std::is_base_of<BaseClass, RealizedClass>::value);

            auto realizationType = std::type_index( typeid(RealizedClass) );
#ifdef _DEBUG
            auto itRegisteredClass = m_registeredClasses.find( realizationType );
            assert( itRegisteredClass == m_registeredClasses.end() ); //That interface has already been registered
#endif
            m_registeredClasses.emplace( realizationType, realizationType );
            m_factories.emplace( realizationType, []() { return std::make_unique<RealizedClass>(); } );
        }

        template<class ClassToCheck>
        bool isRegistered()
        {
            auto classType = std::type_index( typeid(ClassToCheck) );
            
            auto itRegisteredClass = m_registeredClasses.find( classType );
            return m_registeredClasses.end() != itRegisteredClass;
        }

        void traverse( std::function<void( BaseClass* )> _function )
        {
            for ( auto itElement = m_elements.begin(); itElement != m_elements.end(); ++itElement )
            {
                _function( itElement->second.get() );
            }
        }

        void clear()
        {
            m_elements.clear();
        }

        size_t size()
        {
            return m_elements.size();
        }

    protected:

        using Key = std::type_index;

        virtual void onAdded( Key _key, BaseClass* _system ) {}
        virtual void onRemoved( Key _key, BaseClass* _system ) {}

    private:

        using InterfaceID = std::type_index;
        using RealizationID = std::type_index;

        using Elements = std::map<RealizationID, std::unique_ptr<BaseClass>>;
        using RegisteredClassesMap = std::map<InterfaceID, RealizationID>;
        using Factories = std::map<RealizationID, std::function<std::unique_ptr<BaseClass>()>>;

        UniqueRealizationContainer( const UniqueRealizationContainer<BaseClass>& _container, bool _cloneRegistriesOnly )
            : m_elements()
            , m_registeredClasses( _container.m_registeredClasses )
            , m_factories( _container.m_factories )
        {}

        template<class T>
        typename Elements::iterator getElement()
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            auto typeIndex = std::type_index( typeid(T) );
            auto itRegisteredClass = m_registeredClasses.find( typeIndex );
            return m_registeredClasses.end() == itRegisteredClass ? m_elements.end() : m_elements.find( itRegisteredClass->second );
        }

        Elements m_elements;
        RegisteredClassesMap m_registeredClasses;
        Factories m_factories;
    };
}