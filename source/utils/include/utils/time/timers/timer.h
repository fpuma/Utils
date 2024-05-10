#pragma once

#include <utils/time/timers/syncedtimer.h>
#include <utils/time/timereference.h>

namespace puma
{

    class Timer
    {
    public:

        Timer()
            : m_timeReference()
            , m_currentExecutionTime( std::make_shared<float>( (float)m_timeReference.getElapsedSeconds() ) )
            , m_syncedTimer( m_currentExecutionTime )
        {}

        virtual ~Timer() = default;

        void play()
        {
            *m_currentExecutionTime = (float)m_timeReference.getElapsedSeconds();
            m_syncedTimer.play();
        }

        void stop()
        {
            m_syncedTimer.stop();
        }
        
        void pause()
        {
            m_syncedTimer.pause();
        }

        float getCurrentTime() const
        {
            *m_currentExecutionTime = (float)m_timeReference.getElapsedSeconds();
            return m_syncedTimer.getCurrentTime();
        }

    private:

        TimeReference m_timeReference;
        std::shared_ptr<float> m_currentExecutionTime;
        SyncedTimer m_syncedTimer;
    };
}