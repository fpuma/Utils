#pragma once

#include <time/timers/syncedtimer.h>

#include <memory>

namespace puma
{
    class SyncedCountdownTimer
    {
    public:

        SyncedCountdownTimer( const std::shared_ptr<float>& _currentExecutionTime )
            : m_timeLimit( 1.0f )
            , m_syncedTimer( _currentExecutionTime )
        {}

        virtual ~SyncedCountdownTimer() = default;

        void play()
        {
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

        bool isFinished() const
        {
            float currentTime = m_syncedTimer.getCurrentTime();
            return currentTime > m_timeLimit;
        }

        float getCurrentTime() const
        {
            float currentTime = m_syncedTimer.getCurrentTime();
            currentTime = currentTime > m_timeLimit ? m_timeLimit : currentTime;
            return currentTime;
        }

        float getRemainingTime() const
        {
            return m_timeLimit - getCurrentTime();
        }

        void setTimeLimit( float _timeLimit ) { m_timeLimit = _timeLimit; }
        float getTimeLimit() const { return m_timeLimit; }

    private:

        float m_timeLimit;
        SyncedTimer m_syncedTimer;
    };
}
