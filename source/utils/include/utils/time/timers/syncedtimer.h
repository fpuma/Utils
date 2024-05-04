#pragma once

#include <assert.h>
#include <memory>

namespace puma
{
    class SyncedTimer
    {
    public:
        SyncedTimer( const std::shared_ptr<float>& _currentExecutionTime )
            : m_currentTimeBuffer( 0.0f )
            , m_initialTime( 0.0f )
            , m_timerState( State::Stopped )
            , m_currentExecutionTime(_currentExecutionTime)
        {}

        virtual ~SyncedTimer() = default;

        void play()
        {
            if ( m_timerState != State::Ticking )
            {
                m_initialTime = *m_currentExecutionTime - m_currentTimeBuffer;
                m_timerState = State::Ticking;
            }
        }

        void stop()
        {
            m_timerState = State::Stopped;
            m_initialTime = 0.0f;
            m_currentTimeBuffer = 0.0f;
        }

        void pause()
        {
            if ( m_timerState == State::Ticking )
            {
                m_currentTimeBuffer = getCurrentTime();
                m_timerState = State::Paused;
            }
        }

        float getCurrentTime() const
        {
            float currentTime = 0.0f;

            switch ( m_timerState )
            {
            case State::Stopped:
                currentTime = 0.0f;
                break;

            case State::Ticking:
                currentTime = *m_currentExecutionTime - m_initialTime;;
                break;

            case State::Paused:
                currentTime = m_currentTimeBuffer;
                break;

            default:
                assert( false ); //Invalid Timer state
                break;
            }

            return currentTime;
        }

    private:

        enum class State
        {
            Stopped = 0,
            Ticking = 1,
            Paused = 2,
        };

        float m_currentTimeBuffer;
        float m_initialTime;
        State m_timerState;
        const std::shared_ptr<float> m_currentExecutionTime;
    };
}