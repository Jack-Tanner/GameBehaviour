#pragma once
#include "BaseTexture.h"

#include <glm/glm.hpp>
#include <vector>

struct AnimClip
{
	const char*				m_pszName;
	float					m_fAnimDuration;
	unsigned int			m_uStartFrame, m_uEndFrame;
};

class AnimatedTexture : public BaseTexture
{
public:
	AnimatedTexture();
	virtual ~AnimatedTexture();

	virtual bool Initialise( std::string& strFilePath );
	virtual bool Initialise( std::string& strFilePath, glm::vec2 vFrameSize );

	bool		AddAnim( const AnimClip& animClip );
	AnimClip	GetAnim( const char* pszName ) const;
	AnimClip	GetAnim( unsigned int uIndex ) const;

	unsigned int GetNumFrames() const
	{
		return m_uNumFrames;
	}

	unsigned int GetNumCols() const
	{
		return m_uNumCols;
	}

	unsigned int GetNumRows() const
	{
		return m_uNumRows;
	}

	const glm::vec2& GetFrameSize()
	{
		return m_FrameSize;
	}

private:

	glm::vec2				m_FrameSize;
	std::vector<AnimClip>	m_Clips;
	unsigned int			m_uNumFrames;
	unsigned int			m_uNumRows, m_uNumCols;
};

