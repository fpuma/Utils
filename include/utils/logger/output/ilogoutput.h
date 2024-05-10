#pragma once

#include <utils/logger/logdefinitions.h>

namespace puma
{
    class ILogOutput
    {
    public:
        virtual ~ILogOutput() {}

        virtual void print( const CategoryInfo& _categoryInfo, LogType _logType, const char* _report ) const = 0;
    };
}