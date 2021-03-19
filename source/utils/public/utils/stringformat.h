#pragma once

#include <string>
#include <stdarg.h>

namespace puma
{
	std::string formatString( const char* _format, ... )
	{
		va_list args;
		va_start( args, _format );
		char buffer[256];
		vsnprintf( buffer, 256, _format, args );
		va_end( args );
		return std::string( buffer );
	}
}