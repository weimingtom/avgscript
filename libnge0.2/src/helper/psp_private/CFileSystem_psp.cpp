#include "CFileSystem_psp.h"


bool CFile::Read( void * p_address, u32 length, u32 * p_nbytes_read ) const
{
	const u32 bytes_read( fread( p_address, 1, length, m_phandle ) );

	if ( p_nbytes_read != NULL )
	{
		*p_nbytes_read = bytes_read;
	}

	return ( bytes_read == length );
}

bool CFile::Write( const void * p_address, u32 length, u32 * p_nbytes_written ) const
{
	const u32 bytes_written( fwrite( p_address, 1, length, m_phandle ) );

	if ( p_nbytes_written != NULL )
	{
		*p_nbytes_written = bytes_written;
	}

	return ( bytes_written == length );
}

bool CFile::IsEOF() const
{
	return ( feof( m_phandle ) != 0 );
}

u32  CFile::GetLength() const
{
	return m_length;
}

u32  CFile::Tell() const
{
	return ftell( m_phandle );
}

bool CFile::Seek( const u32 offset, const u32 origin ) const
{
	return ( fseek( m_phandle, offset, origin ) == 0 );
}

char CFile::GetChar() const
{
	char	character;

	if ( Read( &character, 1 ) == true )
	{
		return character;
	}

	return EOF;
}


CFile::CFile( const char * filename, SceUID p_handle )
:	m_filename( filename )
,	m_phandle( p_handle )
,	m_length( 0 )
{
	if ( Seek( 0, SEEK_END ) == true )
	{
		m_length = Tell();

		if ( Seek( 0, SEEK_SET ) == false )
		{
			printf( "Failed to seek to beginning of file" );
		}
	}
	else
	{
		printf( "Failed to seek to the end of file" );
	}
}
CFile::~CFile()
{
	fclose( m_phandle );
}



