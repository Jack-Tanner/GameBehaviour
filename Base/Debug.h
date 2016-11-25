#pragma once

#if _DEBUG
#include <sstream>
#include <Windows.h>

#define GLOG( s ) { \
	std::ostringstream os_; \
	os_ << s << "\n"; \
	OutputDebugString(os_.str().c_str());\
} \

#define GASSERT( b ) { \
	if( !(b) ) { \
		GLOG( __FILE__ << "\nLine: " << __LINE__ << "\nGASSERT( " << #b << " )" ) \
		DebugBreak(); \
	} \
} \

#define GL_CHECK() {  \
	GLenum glError = glGetError(); \
	if ( glError != GL_NO_ERROR ) \
	{ \
		GLOG( "glError == " << glError << "\n" << glewGetErrorString( glError ) ); \
		GASSERT( false ); \
	} \
} \

#else 
#define GLOG( x )
#define GASSERT( b )
#define GL_CHECK()
#endif