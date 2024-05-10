#pragma once

#include <utils/time/timereference.h>
#include <utils/time/timers/syncedcountdowntimer.h>
#include <utils/time/timers/syncedtimer.h>

#include <memory>

namespace puma
{
    class SyncedTimersReference
    {
    public:

        SyncedTimersReference()
            : m_timeReference()
            , m_currentTime( std::make_shared<float>( (float)m_timeReference.getElapsedSeconds() ) )
        {}

        void update() { *m_currentTime = (float)m_timeReference.getElapsedSeconds(); }

        SyncedTimer getTimer() const
        {
            return SyncedTimer( m_currentTime );
        }

        SyncedCountdownTimer getCountdownTimer() const
        {
            return SyncedCountdownTimer( m_currentTime );
        }

    private:

        TimeReference m_timeReference;
        std::shared_ptr<float> m_currentTime;
    };
}