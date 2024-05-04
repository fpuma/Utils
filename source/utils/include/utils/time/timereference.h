#pragma once

#include <chrono>

namespace puma
{
    class TimeReference
    {
    public:

        virtual ~TimeReference() = default;

        TimeReference()
        {
            m_secondsRatio = ((double)(std::chrono::high_resolution_clock::period::num) / (double)(std::chrono::high_resolution_clock::period::den));
            reset();
        }

        double getElapsedSeconds()      const   { return (double)getElapsedCount() * m_secondsRatio; }
        double getElapsedMilliseconds() const   { return (double)getElapsedCount() * m_secondsRatio * 1000.0; }
        double getElapsedMicroseconds() const   { return (double)getElapsedCount() * m_secondsRatio * 1000000.0; }
        double getElapsedNanoseconds()  const   { return (double)getElapsedCount() * m_secondsRatio * 1000000000.0; }

        void reset()
        {
            m_epochCountReference = m_highResClock.now().time_since_epoch().count();
        }

    private:

        long long getElapsedCount() const { return m_highResClock.now().time_since_epoch().count() - m_epochCountReference; }

        std::chrono::high_resolution_clock m_highResClock;
        long long m_epochCountReference;
        double m_secondsRatio;
    };
}