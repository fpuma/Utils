#pragma once

#include <mutex>

namespace puma
{
    template<class BufferType>
    class MultiThreadBuffer
    {
    public:

        MultiThreadBuffer()
            : m_toRead( &m_buffer0 )
            , m_nextToRead( nullptr )
            , m_toWrite( &m_buffer1 )
            , m_nextToWrite( &m_buffer2 )
        {

        }

        bool updateWriteBuffer( bool _force = false )
        {
            std::lock_guard<std::mutex> guard( m_bufferSyncMutex );
            bool update = nullptr != m_nextToWrite;
            if (update)
            {
                assert( nullptr == m_nextToRead ); //If nextToWrite is available, then nextToRead should be nullptr
                std::swap( m_toWrite, m_nextToRead );
                std::swap( m_toWrite, m_nextToWrite );
            }
            else if (_force)
            {
                std::swap( m_toWrite, m_nextToRead );
                update = true;
            }
            return update;
        }

        bool updateReadBuffer()
        {
            std::lock_guard<std::mutex> guard( m_bufferSyncMutex );
            bool update = nullptr != m_nextToRead;
            if (update)
            {
                assert( nullptr == m_nextToWrite ); //If nextToRead is available, nextToWrite must be nullptr
                std::swap( m_toRead, m_nextToWrite );
                std::swap( m_toRead, m_nextToRead );
            }
            return update;
        }

        const BufferType* read() const { return m_toRead; }
        BufferType* write() { return m_toWrite; }

    private:

        BufferType* m_toRead = nullptr;
        BufferType* m_nextToRead = nullptr;
        BufferType* m_toWrite = nullptr;
        BufferType* m_nextToWrite = nullptr;

        BufferType m_buffer0;
        BufferType m_buffer1;
        BufferType m_buffer2;

        std::mutex m_bufferSyncMutex;

    };
}