#pragma once

namespace puma
{
    template <class IDType, IDType InvalidValue >
    class GenericID
    {
    public:
        GenericID() {}
        explicit GenericID( IDType _id ) : m_value( _id ) {}
        GenericID( const GenericID& _other ) : m_value( _other.m_value ) {}
        GenericID( const GenericID&& _other ) noexcept : m_value( _other.m_value ) {}
        
        GenericID&  operator =  ( const GenericID& _other ) { m_value = _other.m_value; return *this; }
        bool        operator == ( const GenericID& _other ) const { return m_value == _other.m_value; }
        bool        operator != ( const GenericID& _other ) const { return m_value != _other.m_value; }
        bool        operator <  ( const GenericID& _other ) const { return m_value < _other.m_value; }

        IDType value() const { return m_value; }
        bool isValid() const { return InvalidValue != m_value; }
        void invalidate()    { m_value = InvalidValue; }

    private:
        IDType m_value = InvalidValue;
    };
}

#define DECLARE_GENERIC_ID(ID_NAME, ID_TYPE, INVALID_ID_VALUE)     class ID_NAME : public puma::GenericID<ID_TYPE, INVALID_ID_VALUE>\
{\
public:\
    ID_NAME() {}\
    ID_NAME( ID_TYPE _id )\
        : GenericID( _id )\
    {}\
};