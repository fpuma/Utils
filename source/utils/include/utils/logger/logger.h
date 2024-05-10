#pragma once

#include <utils/logger/output/ilogoutput.h>
#include <utils/logger/logdefinitions.h>

#include <utils/containers/uniquerealizationcontainer.h>
#include <utils/noncopyable.h>
#include <utils/numerictypes.h>

#include <vector>

namespace puma
{
    class Logger: public NonCopyable
    {
    public:

        void info( const LogCategory& _category, const char* _log)
        {
            addLog( getCategoryInfo(_category), LogType::Info, _log );
        }

        void warning( const LogCategory& _category, const char* _log)
        {
            addLog( getCategoryInfo( _category ), LogType::Warning, _log );
        }

        void error( const LogCategory& _category, const char* _log)
        {
            addLog( getCategoryInfo( _category ), LogType::Error,  _log );
        }

        LogCategory registerCategory( const std::string& _categoryName )
        {
            LogCategory result( (u32)m_categoryNames.size() );
            m_categoryNames.push_back( _categoryName );
            return result;
        }

        template<class OutputClass>
        void addOutput()
        {
            if ( !m_outputs.isRegistered<OutputClass>() )
            {
                m_outputs.registerClass<OutputClass>();
            }

            m_outputs.add<OutputClass>();
        }

        template<class OutputClass>
        void removeOutput()
        {
            m_outputs.remove<OutputClass>();
        }

    private:
        void addLog( const CategoryInfo& _categoryInfo, LogType _type, const std::string& _report )
        {
            m_outputs.visit( [&]( std::shared_ptr<const ILogOutput> output ) 
            {
                output->print( _categoryInfo, _type, _report.c_str() );
            } );
        }

        CategoryInfo getCategoryInfo( const LogCategory _logCategory )
        {
            LogCategory category = _logCategory.value() < m_categoryNames.size() ? _logCategory : LogCategory();
            const char* name = category.value() != kInvalidLogCategory ? m_categoryNames[category.value()].c_str() : "UnregisteredCategory";
            return { category, name };
        }

        UniqueRealizationContainer<ILogOutput> m_outputs;
        std::vector<std::string> m_categoryNames;
    };
}