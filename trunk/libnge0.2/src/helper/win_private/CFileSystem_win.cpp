#include "CFileSystem_win.h"
#include <stdio.h>
#include <direct.h>
#include <sys/stat.h>

string	CFileSystem::m_workdir ="";
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


CFile::CFile( const char * filename, FILE * const p_handle )
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


//
// cfilesystem
//
bool CFileSystem::SetWorkDir( const char * dir )
{
	if ( _chdir( dir ) >= 0 )
	{
		m_workdir = dir;

		return true;
	}

	return false;
}

CFile*	CFileSystem::Open( const char * filename, const char * const p_open_flags )
{
	FILE * const	p_handle =fopen( filename, p_open_flags ) ;

	if ( p_handle == NULL )
	{
		return NULL;
	}

	return new CFile( filename, p_handle );
}

void	CFileSystem::Close( CFile * const p_file )
{
	delete p_file;
}


bool	CFileSystem::IsFileExists( const char * filename )
{
	CFile * const	p_file = Open( filename, "rb" ) ;
	if ( p_file == NULL )
	{
		return false;
	}
	Close( p_file );
	return true;
}

bool	CFileSystem::IsDirExists( const char * dir )
{
	struct _stat filestat;
	if ( ::_stat(dir, &filestat) != 0 )
	{
		return false;
	}
	return true;
}

bool	CFileSystem::GetDirFiles( const char* directory,  CFileList* dir_files )
{
	bool ret_code = false,find_code =false;
	FIND_FILE_HANDLE handle;

	if ( CFileSystem::FindFirstFile( directory, handle ) == true )
	{
		ret_code = true;
		find_code = true;
		while ( find_code&& ret_code == true )
		{
			sDirEntry*	dir_entry = new sDirEntry;
			find_code = (CFileSystem::FindNextFile( dir_entry, handle ) == true);
			if(find_code == false)
				continue;
			if ( dir_entry->m_filename != "." && dir_entry->m_filename != ".." )
			{
				//directory[strlen(directory)-2]='\0';
				char buf[256] = {0};
				strncpy(buf,directory,strlen(directory)-2);
				//dir_entry->m_filename = directory[strlen(directory)-2]='\0';
				dir_entry->m_filename = string(buf)+string( "\\" ) + dir_entry->m_filename ;
				dir_files->push_back( dir_entry );
				if ( dir_entry->IsDirectory() == true )
				{
					string format = dir_entry->m_filename+string("\\*");
					//dir_entry->m_filename  = dir_entry->m_filename+string("\\*");
					//dir_entry->m_filename  ="J:\\test\\libevent\\*";
					ret_code = GetDirFiles(  format.c_str(), dir_files );
				}
			}
		}

		CFileSystem::FindCloseFile( handle );
	}
	else
	{
		printf("FindFirstFile FAILED!");
	}

	return ret_code;
}

bool	CFileSystem::FindFirstFile( const char* path, FIND_FILE_HANDLE & handle )
{
	//win32µÄsbº¯Êý¡£
	WIN32_FIND_DATA FindFileData;
  	handle = ::FindFirstFile(path, &FindFileData);
	return (handle != INVALID_HANDLE_VALUE);
}

bool	CFileSystem::FindNextFile( sDirEntry * dir_entry, FIND_FILE_HANDLE handle )
{
	WIN32_FIND_DATA FindFileData;
	const s32 error_code = ::FindNextFile(handle,&FindFileData);

	if ( error_code != 0 )
	{
		//dir_entry.m_states = FindFileData.d_stat;
		if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
			dir_entry->m_stats.st_mode = FIO_S_IFDIR;
		}
		else{
			//
		}

		dir_entry->m_filename = FindFileData.cFileName;

		return true;
	}

	return false;
}
bool	CFileSystem::FindCloseFile( FIND_FILE_HANDLE handle )
{
	return (::FindClose(handle)!=0);
	
}
/*
bool	CFileSystem::CopyFile( const string & src_file, const string & dst_file, float progress_inc )
{
}
bool	CFileSystem::CopyDirectory( const string & src_dir, const string & dst_dir, float progress_inc )
{
}
bool	CFileSystem::DeleteFile( const string & filename )
{
}
bool	CFileSystem::DeleteDirectory( const string & directory )
{
}
bool	CFileSystem::MakeDirectory( const string & directory )
{
}
bool	CFileSystem::IsUMDInserted()
{
}
bool	CFileSystem::ActivateUMD()
{
}
bool	CFileSystem::Rename( const string & old_name, const string & new_name )
{
}
void	CFileSystem::SplitPath( const string & path, string * p_drive, string * p_dir, string * p_fname, string * p_ext )
{
}
bool	CFileSystem::MakeReadOnly( const string & filename )
{
}
bool	CFileSystem::MakeWritable( const string & filename )
{
}
string	CFileSystem::MakeFullPath( const string & filename )
{
}
string	CFileSystem::GetFileExtension( const string & filename )
{
}	
bool	CFileSystem::HideCorruptFiles()
{
}
void	CFileSystem::SetHideCorruptFiles( bool hide )
{
}
*/


