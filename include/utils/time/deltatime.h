#pragma once

#include <utils/time/timereference.h>

#include <array>
#include <chrono>
#include <numeric>
#include <time.h>

namespace puma
{
    class DeltaTime
    {

    public:

        DeltaTime()
        {}

        virtual ~DeltaTime() = default;
     
        float get() const { return static_cast<float>(m_currentDt); }

        float getAverage() const { return static_cast<float>(m_averageDt); }

        void update()
        {
            float elapsedSeconds = (float)m_timeReference.getElapsedSeconds();
            m_currentDt = (float)(elapsedSeconds - m_previousExecutionSeconds);
            m_previousExecutionSeconds = elapsedSeconds;

            ++m_updateCount;
            m_averageDt += (m_currentDt - m_averageDt) / m_updateCount;
        }

        void reset()
        {
            m_timeReference.reset();
            m_currentDt = 0.0f;
            m_previousExecutionSeconds = 0.0;
            m_averageDt = 0.0f;
            m_updateCount = 0;
        }

    private:

        TimeReference m_timeReference;
        double m_currentDt = 0.0f;
        double m_previousExecutionSeconds = 0.0;
        double m_averageDt = 0.0f;
        unsigned int m_updateCount = 0;
    };

}
