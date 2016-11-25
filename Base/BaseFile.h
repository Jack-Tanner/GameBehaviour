#pragma once

#include <fstream>
#include <memory>

class BaseFile
{
public:
	BaseFile();
	BaseFile( std::string strPath, bool bBinary = false );
	virtual ~BaseFile();

	bool	OpenFile( std::string strPath, bool bBinary = false );

	bool	IsFileOpen() const
	{
		return m_File.is_open();
	}

	std::shared_ptr<char>	Read();
	std::shared_ptr<char>	Read( unsigned int uAmount );
	std::shared_ptr<char>	ReadFrom( unsigned int uAmount, unsigned int uOffset );

private:
	std::fstream		m_File;
	std::string			m_FileName;
	unsigned int		m_FileSize;
};

