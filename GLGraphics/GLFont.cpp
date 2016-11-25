#include "GLFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Debug.h>

GLFont::GLFont() : m_uSize( 0 ), m_uHeight( 0 )
{
}


GLFont::~GLFont()
{
}

bool GLFont::Initialise( const char * pszFontName, unsigned int uFontSize )
{
	FT_Library ftLibrary;
	if ( FT_Init_FreeType( &ftLibrary ) )
	{
		GASSERT( false && "Failed to load font library" );
		return false;
	}

	FT_Face fontFace;
	if ( FT_New_Face( ftLibrary, pszFontName, 0, &fontFace ) )
	{
		GASSERT( false && "Failed to load font!" );
		return false;
	}

	FT_Set_Pixel_Sizes( fontFace, 0, uFontSize );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	for ( GLubyte c = 0; c < 128; c++ )
	{
		if ( FT_Load_Char( fontFace, c, FT_LOAD_RENDER ) )
		{
			GLOG( "Failed to load character: " << c );
			continue;
		}

		GLuint uTexture = 0;
		glGenTextures( 1, &uTexture );
		glBindTexture( GL_TEXTURE_2D, uTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		GLCharacter glCharacter;
		glCharacter.uTexHandle = uTexture;
		glCharacter.uAdvance = fontFace->glyph->advance.x;
		glCharacter.vSize = glm::ivec2( fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows );
		glCharacter.vBearing = glm::ivec2( fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top);
		m_Characters[c] = glCharacter;

		if ( fontFace->glyph->bitmap.rows > m_uHeight )
			m_uHeight = fontFace->glyph->bitmap.rows;

		GL_CHECK();
	}

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	FT_Done_Face( fontFace );
	FT_Done_FreeType( ftLibrary );
	
	m_FontName = pszFontName;
	m_uSize = uFontSize;


	return true;
}

const GLCharacter * GLFont::GetCharacter( GLubyte uCharacter ) const
{
	GASSERT( uCharacter < 128 );
	return &m_Characters.at( uCharacter );
}
