#pragma once

#include <utils/time/timers/syncedcountdowntimer.h>
#include <utils/time/timereference.h>

namespace puma
{

    class CountdownTimer
    {
    public:

        CountdownTimer()
            : m_timeReference()
            , m_currentExecutionTime( std::make_shared<float>( (float)m_timeReference.getElapsedSeconds() ) )
            , m_syncedCountdownTimer( m_currentExecutionTime )
        {}

        virtual ~CountdownTimer() = default;

        void play()
        {
            *m_currentExecutionTime = (float)m_timeReference.getElapsedSeconds();
            m_syncedCountdownTimer.play();
        }

        void stop()
        {
            m_syncedCountdownTimer.stop();
        }

        void pause()
        {
            m_syncedCountdownTimer.pause();
        }

        bool isFinished() const
        {
            *m_currentExecutionTime = (float)m_timeReference.getElapsedSeconds();
            return m_syncedCountdownTimer.isFinished();
        }

        float getCurrentTime() const
        {
            *m_currentExecutionTime = (float)m_timeReference.getElapsedSeconds();
            return m_syncedCountdownTimer.getCurrentTime();
        }

        float getRemainingTime() const
        {
            *m_currentExecutionTime = (float)m_timeReference.getElapsedSeconds();
            return m_syncedCountdownTimer.getRemainingTime();
        }

        void setTimeLimit( float _timeLimit ) { m_syncedCountdownTimer.setTimeLimit( _timeLimit ); }
        float getTimeLimit() const { return m_syncedCountdownTimer.getTimeLimit(); }

    private:

        TimeReference m_timeReference;
        std::shared_ptr<float> m_currentExecutionTime;
        SyncedCountdownTimer m_syncedCountdownTimer;
    };
}