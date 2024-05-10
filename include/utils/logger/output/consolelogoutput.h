#pragma once

#include <utils/logger/logdefinitions.h>
#include <utils/logger/output/ilogoutput.h>

#include <assert.h>
#include <iostream>
#include <string>

namespace puma
{
    class ConsoleLogOutput : public ILogOutput
    {

    public:

        void print( const CategoryInfo& _categoryInfo, LogType _logType, const char* _report ) const override
        {
            const char* typeName = "";
            switch ( _logType )
            {
            case LogType::Info:     typeName = "INFORMATION"; break;
            case LogType::Warning:  typeName = "==WARNING=="; break;
            case LogType::Error:    typeName = "***ERROR***"; break;
            default: assert( false ); break;
            }

            std::cout << typeName << " [" << _categoryInfo.name << "] " << _report << std::endl;
        }
    };
}