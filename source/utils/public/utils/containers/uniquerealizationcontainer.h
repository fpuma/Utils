#pragma once

#include <algorithm>
#include <assert.h>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace puma
{

    template<class BaseClass>
    class UniqueRealizationContainer
    {
    public:

        UniqueRealizationContainer() = default;
        UniqueRealizationContainer( const UniqueRealizationContainer<BaseClass>& _container ) = delete;
        UniqueRealizationContainer& operator =( const UniqueRealizationContainer<BaseClass>& _container ) = delete;

        ~UniqueRealizationContainer()
        {
            clear();
        }

        UniqueRealizationContainer( UniqueRealizationContainer<BaseClass>&& _container ) noexcept
            : m_elements( std::move( _container.m_elements ) )
            , m_registeredClasses( std::move( _container.m_registeredClasses ) )
            , m_factories( std::move( _container.m_factories ) )
        {}

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
        std::shared_ptr<T> add()
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            auto typeIndex = std::type_index( typeid(T) );

            auto itRegisteredClass = m_registeredClasses.find( typeIndex );
            assert( itRegisteredClass != m_registeredClasses.end() ); //The class or interface you are trying to add has not been registered
            if (itRegisteredClass == m_registeredClasses.end()) return nullptr;

            assert( !contains<T>() ); //An element of that type already exists
            if (contains<T>()) return nullptr;

            auto itFactory = m_factories.find( itRegisteredClass->second );
            assert( itFactory != m_factories.end() ); //Could not find the factory
            if (itFactory == m_factories.end()) return nullptr;

            auto emplaceResult = m_elements.emplace( itRegisteredClass->second, itFactory->second() );
            assert( emplaceResult.second ); // Failed to add the element or element already existed
            if (!emplaceResult.second) return nullptr;
            
            auto itElement = emplaceResult.first;
            onAdded( itElement->second );
            return static_pointer_cast<T>(itElement->second);
        }

        template<class T>
        T* get() { return getElement<T>().get(); }

        template<class T>
        const T* get() const { return getElement<T>().get(); }

        template<class T>
        std::shared_ptr<T> getSafely() { return getElement<T>(); }

        template<class T>
        std::shared_ptr<const T> getSafely() const { return getElement<T>(); }

 
        BaseClass* get( std::type_index _typeIndex ) { return getElement( _typeIndex ).get(); }
        const BaseClass* get( std::type_index _typeIndex ) const { return getElement( _typeIndex ).get(); }
        std::shared_ptr<BaseClass> getSafely( std::type_index _typeIndex ) { return getElement( _typeIndex ); }
        std::shared_ptr<const BaseClass> getSafely( std::type_index _typeIndex ) const { return getElement( _typeIndex ); }

        template<class T>
        void remove()
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            
            RegisteredClassesMap::const_iterator itRegisteredClass = m_registeredClasses.find( std::type_index( typeid(T) ) );
            assert( itRegisteredClass != m_registeredClasses.end() ); // The type has not been registered
            if (itRegisteredClass == m_registeredClasses.end()) return;

            std::type_index ti = itRegisteredClass->second;

            auto itElement = m_elements.find( ti );
            assert( itElement != m_elements.end() ); //There is no element of that type
            if (itElement == m_elements.end()) return;

            auto elementPtr = itElement->second;
            if (nullptr == elementPtr) return;
            onRemoved( elementPtr );

            auto typeIndex = std::type_index( typeid(T) );
            m_elements.erase( ti );
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
            m_factories.emplace( realizedType, []() { return std::make_shared<RealizedClass>(); } );
        }

        template<class RealizedClass>
        void registerClass()
        {
            static_assert(std::is_base_of<BaseClass, RealizedClass>::value);

            auto realizationType = std::type_index( typeid(RealizedClass) );
#ifdef _DEBUG
            auto itRegisteredClass = m_registeredClasses.find( realizationType );
            assert( itRegisteredClass == m_registeredClasses.end() ); //That realization has already been registered
#endif
            m_registeredClasses.emplace( realizationType, realizationType );
            m_factories.emplace( realizationType, []() { return std::make_shared<RealizedClass>(); } );
        }

        template<class ClassToCheck>
        bool isRegistered() const
        {
            auto classType = std::type_index( typeid(ClassToCheck) );
            return m_registeredClasses.contains( classType );
        }

        void visit( std::function<void( std::shared_ptr<BaseClass> )> _function )
        {
            for ( auto itElement = m_elements.begin(); itElement != m_elements.end(); ++itElement )
            {
                _function( itElement->second );
            }
        }

        void clear()
        {
            assert( std::all_of( m_elements.begin(), m_elements.end(), []( Elements::value_type elementPtr )
                {
                    return 2 == elementPtr.second.use_count();
                } ) );

            m_elements.clear();
        }

        size_t size() const
        {
            return m_elements.size();
        }

    protected:

        virtual void onAdded( std::shared_ptr<BaseClass> _system ) {}
        virtual void onRemoved( std::shared_ptr<BaseClass> _system ) {}

    private:

        using InterfaceID = std::type_index;
        using RealizationID = std::type_index;

        using Elements = std::unordered_map<RealizationID, std::shared_ptr<BaseClass>>;
        using RegisteredClassesMap = std::unordered_map<InterfaceID, RealizationID>;
        using Factories = std::unordered_map<RealizationID, std::function<std::shared_ptr<BaseClass>()>>;

        UniqueRealizationContainer( const UniqueRealizationContainer<BaseClass>& _container, bool _cloneRegistriesOnly )
            : m_elements()
            , m_registeredClasses( _container.m_registeredClasses )
            , m_factories( _container.m_factories )
        {}

        template<class T>
        std::shared_ptr<T> getElement() const
        {
            static_assert(std::is_base_of<BaseClass, T>::value);
            std::type_index typeIndex = std::type_index( typeid(T) );
            return static_pointer_cast<T>(getElement( typeIndex ));
        }

        std::shared_ptr<BaseClass> getElement( std::type_index _typeIndex ) const
        {
            RegisteredClassesMap::const_iterator itRegisteredClass = m_registeredClasses.find( _typeIndex );
            assert( itRegisteredClass != m_registeredClasses.end() ); // The type has not been registered
            if (itRegisteredClass == m_registeredClasses.end()) return nullptr;

            auto itElement = m_elements.find( itRegisteredClass->second );
            assert( itElement != m_elements.end() ); //There is no element of that type
            if (itElement == m_elements.end()) return nullptr;

            return itElement->second;
        }

        Elements m_elements;
        RegisteredClassesMap m_registeredClasses;
        Factories m_factories;
    };
}