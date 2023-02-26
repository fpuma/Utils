#pragma once

namespace puma
{
    class NonCopyable
    {
    public:
        NonCopyable( const NonCopyable& _other ) = delete;
        NonCopyable& operator = ( const NonCopyable& _other ) = delete;

    protected:
        NonCopyable(){}
        virtual ~NonCopyable(){}
    };
}
