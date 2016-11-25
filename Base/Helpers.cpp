#include "Helpers.h"

namespace Helpers
{
	std::string ExtractFileName( const std::string& strFileName )
	{
		std::size_t lastSlash = strFileName.rfind( '\\' );
		if ( lastSlash != std::string::npos )
		{
			std::string newStr = strFileName.substr( lastSlash + 1, strFileName.length() - lastSlash );
			return newStr;
		}

		lastSlash = strFileName.rfind( '/' );
		if ( lastSlash != std::string::npos )
		{
			std::string newStr = strFileName.substr( lastSlash + 1, strFileName.length() - lastSlash);
			return newStr;
		}

		return strFileName;
	}
}