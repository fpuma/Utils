#pragma once

#include <assert.h>
#include <chrono>
#include <string>
#include <time.h>

namespace puma
{

    enum class Weekday
    {
        Sunday,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday
    };

    class Timestamp
    {
    public:

        Timestamp()
        {
            m_timeStruct.tm_sec = 0;
            m_timeStruct.tm_min = 0;
            m_timeStruct.tm_hour = 0;
            m_timeStruct.tm_mday = 1;
            m_timeStruct.tm_mon = 0;
            m_timeStruct.tm_year = 0;

            m_timeStruct.tm_wday = 0;
            m_timeStruct.tm_yday = 0;
            m_timeStruct.tm_isdst = 0;

            mktime( &m_timeStruct );
        }

        virtual ~Timestamp() = default;

        int getSecond()     const { return m_timeStruct.tm_sec; }
        int getMinute()     const { return m_timeStruct.tm_min; }
        int getHour()       const { return m_timeStruct.tm_hour; }
        int getMonthDay()   const { return m_timeStruct.tm_mday; }
        int getMonth()      const { return m_timeStruct.tm_mon; }
        int getYear()       const { return m_timeStruct.tm_year + 1900; }

        Weekday getWeekday() const
        {
            assert( m_timeStruct.tm_wday >= (int)Weekday::Sunday );
            assert( m_timeStruct.tm_wday <  (int)Weekday::Saturday );
            return (Weekday)m_timeStruct.tm_wday;
        }

        int getYearDay() const          { return m_timeStruct.tm_yday; }
        bool isDaylightSaving() const   { return m_timeStruct.tm_isdst == 1 ? true : false; }

        void set( int _second, int _minute, int _hour, int _monthDay, int _month, int _year )
        {
            m_timeStruct.tm_sec = _second;
            m_timeStruct.tm_min = _minute;
            m_timeStruct.tm_hour = _hour;
            m_timeStruct.tm_mday = _monthDay;
            m_timeStruct.tm_mon = _month;
            m_timeStruct.tm_year = _year;

            mktime( &m_timeStruct );
        }

        void setSecond( int second )
        {
            m_timeStruct.tm_sec = second;
            mktime( &m_timeStruct );
        }

        void setMinute( int minute )
        {
            m_timeStruct.tm_min = minute;
            mktime( &m_timeStruct );
        }

        void setHour( int hour )
        {
            m_timeStruct.tm_hour = hour;
            mktime( &m_timeStruct );
        }

        void setMonthDay( int monthDay )
        {
            m_timeStruct.tm_mday = monthDay;
            mktime( &m_timeStruct );
        }

        void setMonth( int month )
        {
            m_timeStruct.tm_mon = month;
            mktime( &m_timeStruct );
        }

        void setYear( int year )
        {
            m_timeStruct.tm_year = year;
            mktime( &m_timeStruct );
        }

        void setToCurrentLocalTime()
        {
            time_t auxTime = time( nullptr );
            localtime_s( &m_timeStruct, &auxTime );
        }
        void setToCurrentGMTTime()
        {
            time_t auxTime = time( nullptr );
            gmtime_s( &m_timeStruct, &auxTime );
        }

        const std::string toString()
        {
            char buffer[26];

            asctime_s( buffer, &m_timeStruct );

            return std::string( buffer );
        }

    private:

        tm m_timeStruct;
    };
}