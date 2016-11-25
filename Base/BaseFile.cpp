#include "BaseFile.h"

#include "Debug.h"


BaseFile::BaseFile() : m_FileSize( 0 )
{
}


BaseFile::BaseFile( std::string strPath, bool bBinary )
{
	OpenFile( strPath, bBinary );
}

BaseFile::~BaseFile()
{
	if( m_File.is_open() )
		m_File.close();
}

bool BaseFile::OpenFile( std::string strPath, bool bBinary )
{
	if ( m_File.is_open() )
		m_File.close();

	unsigned int flags = std::fstream::in;

	flags |= std::ios::binary;

	m_File = std::fstream( strPath.c_str(), flags );

	if ( m_File.is_open() )
	{
		m_File.seekg( 0, std::ios::end );
		m_FileSize = (unsigned int)m_File.tellg();
		m_File.seekg( 0, std::fstream::beg );

		m_FileName = strPath;
		return true;
	}

	GASSERT( false );

	return false;
}

std::shared_ptr<char> BaseFile::Read()
{
	return Read( m_FileSize );
}

std::shared_ptr<char> BaseFile::Read( unsigned int uAmount )
{
	GASSERT( ( uAmount <= m_FileSize  ) );
	
	if ( uAmount > m_FileSize )
		uAmount = m_FileSize;

	std::shared_ptr<char> pBuffer = std::shared_ptr<char>( new char[m_FileSize + 1], std::default_delete<char[]>() );
	if ( pBuffer )
	{
		m_File.read( pBuffer.get(), uAmount );
		pBuffer.get()[m_FileSize] = '\0';
		return pBuffer;
	}


	return nullptr;
}

std::shared_ptr<char> BaseFile::ReadFrom( unsigned int uAmount, unsigned int uOffset )
{
	if ( IsFileOpen() )
	{
		GASSERT( ( ( uAmount + uOffset ) <= m_FileSize ) );
		m_File.seekg( uOffset );
		return Read( uAmount );

	}

	return nullptr;
}
