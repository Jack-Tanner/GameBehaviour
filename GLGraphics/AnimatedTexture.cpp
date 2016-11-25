#include "AnimatedTexture.h"
#include <Debug.h>


AnimatedTexture::AnimatedTexture() : m_FrameSize( 0.0f, 0.0f ), m_uNumFrames( 0 ), m_uNumRows( 0 ), m_uNumCols( 0 )
{
}


AnimatedTexture::~AnimatedTexture()
{
}

bool AnimatedTexture::Initialise( std::string & strFilePath )
{
	return Initialise( strFilePath, glm::vec2( 0.0f, 0.0f ) );
}

bool AnimatedTexture::Initialise( std::string & strFilePath, glm::vec2 vFrameSize )
{
	bool bResult = BaseTexture::Initialise( strFilePath );
	if ( bResult )
	{
		if ( vFrameSize.x == 0 && vFrameSize.y == 0 )
		{
			GASSERT( false && "No frame size set!" );
			vFrameSize.x = (float)GetWidth();
			vFrameSize.y = ( float ) GetHeight();
		}

		m_uNumCols = GetWidth() / ( unsigned int ) vFrameSize.x;
		m_uNumRows = GetHeight() / ( unsigned int ) vFrameSize.y;
		m_uNumFrames = ( m_uNumCols * m_uNumRows ) - 1;
		m_FrameSize = vFrameSize;

		return true;
	}
	return false;
}

bool AnimatedTexture::AddAnim( const AnimClip & animClip )
{
	if ( animClip.m_fAnimDuration >= 0.0f && animClip.m_uStartFrame <= m_uNumFrames && animClip.m_uEndFrame <= m_uNumFrames )
	{
		bool bFound = false;
		for ( auto& clip : m_Clips )
		{
			if ( strcmp( animClip.m_pszName, clip.m_pszName ) == 0 )
			{
				bFound = true;
				break;
			}
		}

		if ( bFound == false )
		{
			m_Clips.push_back( animClip );
		}
		return true;
	}

	return false;
}

AnimClip AnimatedTexture::GetAnim( const char * pszName ) const
{
	for ( auto& clip : m_Clips )
	{
		if ( strcmp( pszName, clip.m_pszName ) == 0 )
		{
			return clip;
		}
	}

	GASSERT( false && "FAILED TO FIND ANIM" );
	return AnimClip();
}

AnimClip AnimatedTexture::GetAnim( unsigned int uIndex ) const
{
	if ( uIndex < m_Clips.size() )
	{
		return m_Clips[uIndex];
	}

	GASSERT( false && "FAILED TO FIND ANIM" );
	return AnimClip();
}
