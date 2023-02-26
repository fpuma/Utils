#pragma once

#include <functional>

namespace puma
{
    template <class IDType, IDType InvalidValue >
    class GenericId
    {
    public:
        GenericId() {}
        explicit GenericId( IDType _id ) : m_value( _id ) {}
        explicit GenericId( const GenericId& _other ) : m_value( _other.m_value ) {}
        explicit GenericId( const GenericId&& _other ) noexcept : m_value( _other.m_value ) {}
        
        template<typename T>
        GenericId( T ) = delete;

        GenericId&  operator =  ( const GenericId& _other ) { m_value = _other.m_value; return *this; }
        bool        operator == ( const GenericId& _other ) const { return m_value == _other.m_value; }
        bool        operator != ( const GenericId& _other ) const { return m_value != _other.m_value; }
        bool        operator <  ( const GenericId& _other ) const { return m_value < _other.m_value; }

        IDType value() const { return m_value; }
        bool isValid() const { return InvalidValue != m_value; }
        void invalidate()    { m_value = InvalidValue; }

    private:
        IDType m_value = InvalidValue;
    };
}

#define DECLARE_GENERIC_ID(ID_NAME, ID_TYPE, INVALID_ID_VALUE)\
class ID_NAME : public puma::GenericId<ID_TYPE, INVALID_ID_VALUE>\
{\
public:\
    ID_NAME() {}\
    explicit ID_NAME( ID_TYPE _id )\
        : GenericId( _id )\
    {}\
};\
\
template<>\
struct std::hash<ID_NAME>\
{\
    size_t operator()(const ID_NAME& id) const\
    {\
        return std::hash<ID_TYPE>()(id.value());\
    }\
};