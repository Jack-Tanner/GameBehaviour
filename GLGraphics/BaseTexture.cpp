#include "BaseTexture.h"
#include <SOIL.h>

#include "Helpers.h"
#include "Debug.h"


BaseTexture::BaseTexture() : m_bInitialised( false ), m_TexHandle( 0 ), m_Width( 0 ), m_Height( 0 )
{
}


BaseTexture::~BaseTexture()
{
	if ( m_bInitialised )
		Destroy();
}

bool BaseTexture::Initialise( const std::string& strFilePath )
{
	// Load image data.
	unsigned char* pData = nullptr;
	pData = SOIL_load_image( strFilePath.c_str(), &m_Width, &m_Height, 0, SOIL_LOAD_RGBA );

	if ( pData == nullptr )
	{
		GASSERT( false && "Failed to load texture file! " );
		return false;
	}
	
	m_FileName = Helpers::ExtractFileName( strFilePath );

	// Generate and upload texture to GPU.
	GL_CHECK();
	glGenTextures( 1, &m_TexHandle );
	GL_CHECK();
	glBindTexture( GL_TEXTURE_2D, m_TexHandle );
	GL_CHECK();
	GASSERT( m_TexHandle != 0 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData );
	GL_CHECK();

	// Free the data.
	if ( pData )
	{
		SOIL_free_image_data( pData );
	}


	glBindTexture( GL_TEXTURE_2D, 0 );

	m_bInitialised = true;
	return true;
}

void BaseTexture::BindTexture( GLuint uLocation )
{
	GL_CHECK();

	// Activate and bind texture.
	GLint glLocation = GL_TEXTURE0 + uLocation;
	glActiveTexture( glLocation );
	glBindTexture( GL_TEXTURE_2D, m_TexHandle );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	GL_CHECK();
}

void BaseTexture::Destroy()
{
	glDeleteTextures( 1, &m_TexHandle );
	GL_CHECK();
	m_bInitialised = false;
}
