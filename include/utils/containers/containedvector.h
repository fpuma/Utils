#pragma once

#include <array>
#include <assert.h>
#include <iterator>

namespace puma
{
    template<class ElementType, std::size_t ContainerSize>
    class ContainedVector
    {
    public:

        using iterator = typename std::array<ElementType, ContainerSize>::iterator;
        using const_iterator = typename std::array<ElementType, ContainerSize>::const_iterator;
        using value_type = typename ElementType;

        ElementType& operator [] ( size_t _index )
        {
            assert( _index < size() );
            return m_container[_index];
        }

        const ElementType& operator [] ( size_t _index ) const
        {
            assert( _index < size() );
            return m_container[_index];
        }

        ElementType* data() { return m_container.begin(); }
        const ElementType* data() const { return m_container.begin(); }

        void push_back( const ElementType& _element )
        {
            assert( m_currentSize < ContainerSize ); //max size of container has been reached
            m_container[m_currentSize] = _element;
            ++m_currentSize;
        }

        void push_back( ElementType&& _element )
        {
            push_back( _element );
        }

        const ElementType& back() const
        {
            return m_container[m_currentSize - 1];
        }

        ElementType& back()
        {
            return m_container[m_currentSize - 1];
        }

        iterator erase(iterator _element) 
        { 
            assert( _element >= begin() ); // Iterator is outside of the container range
            assert( _element < end() ); // Iterator is outside of the container range

            size_t elementDistance = std::distance( m_container.begin(), _element );

            if ( _element < (end() - 1) )
            {
                void* dst = (void*)(m_container.data() + elementDistance);
                void* src = (void*)(m_container.data() + elementDistance + 1);
                size_t copySize = std::distance( _element + 1, end() ) * sizeof(ElementType);
                std::memcpy( dst, src, copySize );
            }
            --m_currentSize;

            return m_container.begin() + elementDistance;
        }

        iterator begin() { return m_container.begin(); }
        const_iterator begin() const { return m_container.begin(); }

        iterator end() { return m_container.begin() + m_currentSize; }
        const_iterator end() const { return m_container.begin() + m_currentSize; }

        size_t size() const { return m_currentSize; }
        
        void clear() { m_currentSize = 0; }

    private:

        std::array<ElementType, ContainerSize> m_container;
        size_t m_currentSize = 0;

    };
}