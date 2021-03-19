#pragma once

#include <string>
#include <stdarg.h>

namespace puma
{
	///For now it will only print up to 256 characters
	std::string formatString( const char* _format, ... )
	{
		va_list args;
		va_start( args, _format );
		char buffer[256];
		vsnprintf( buffer, 256, _format, args );
		va_end( args );
		return std::string( buffer );
	}

	std::string formatString( const char* _format, const va_list& _args )
	{
		char buffer[256];
		vsnprintf( buffer, 256, _format, _args );
		return std::string( buffer );
	}
}