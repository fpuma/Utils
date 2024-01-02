#pragma once
#include <assert.h>

namespace puma
{

    template<class className, class classOwner = className>
    class DefaultInstance
    {
    public:

        static void setInstance( className* instance )
        {
            assert( nullptr == m_sInstance ); //The default instance is already initialized

            m_sInstance = instance;
        }
        
        static className* getInstance() 
        { 
            assert( nullptr != m_sInstance ); //The default instance has not been initialized
            return m_sInstance; 
        }

        static bool isValid() { return nullptr != m_sInstance; }
        static void clear() { m_sInstance = nullptr; }

    private:

        static inline className* m_sInstance = nullptr;
    };
}