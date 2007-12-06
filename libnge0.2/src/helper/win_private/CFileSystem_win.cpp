#include <stdio.h>
#include <direct.h>
#include <sys/stat.h>
#include <io.h>
#include "CFileSystem_win.h"

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

//修复用户输入的目录字符。
//例如"c://test"和"c://test//"都是合法字符
void CFileSystem::FixPath(char* desstr,const char* srcstr)
{
	int srclen = strlen(srcstr);
	strncpy(desstr,srcstr,srclen);
	if(srcstr[srclen-1] == '\\')
		strcat(desstr,"*");
	else
		strcat(desstr,"\\*");
}

bool	CFileSystem::GetDirFiles( const char* directory,  CFileList* dir_files )
{
	bool ret_code = false,find_code =false;
	FIND_FILE_HANDLE handle;
	char dir_full[256]={0};
	FixPath(dir_full,directory);
	if ( CFileSystem::FindFirstFile( dir_full, handle ) == true )
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
				char buf[256] = {0};
				strncpy(buf,dir_full,strlen(dir_full)-2);
				dir_entry->m_filename = string(buf)+string( "\\" ) + dir_entry->m_filename ;
				dir_files->push_back( dir_entry );
				if ( dir_entry->IsDirectory() == true )
				{
					ret_code = GetDirFiles(  dir_entry->m_filename.c_str(), dir_files );
				}
			}
		}

		CFileSystem::FindCloseFile( handle );
	}
	else
	{
		//printf("FindFirstFile FAILED!");
	}

	return ret_code;
}

bool	CFileSystem::FindFirstFile( const char* path, FIND_FILE_HANDLE & handle )
{
	//win32的sb函数。
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


//拷贝一个文件
bool	CFileSystem::CopyFile( const char * src_file, const char * dst_file )
{
	if ( IsFileExists( dst_file ) == true )
	{
		if ( DeleteFile( dst_file ) == false )
		{
			return false;
		}
	}
	CFile * const	p_src_file = Open( src_file, "rb" );
	CFile * const	p_dst_file = Open( dst_file, "wb" );
	char *		p_buffer  = NULL;
	if ( p_src_file != NULL && p_dst_file != NULL )
	{
		bool	success =  true ;
		s32		src_length = p_src_file->GetLength();
		if ( src_length > MAX_BUFFER )
		{
			p_buffer = new char[ MAX_BUFFER ];
			int remain = src_length;
			while ( remain >  MAX_BUFFER && success ==true )
			{
				if ( p_src_file->Read( p_buffer, MAX_BUFFER ) == false )
				{
					success = false;					
				}
				if ( p_dst_file->Write( p_buffer, MAX_BUFFER ) == false )
				{
					success = false;
				}
				remain -= MAX_BUFFER;
			}
			if ( p_src_file->Read( p_buffer, remain ) == false )
			{
					success = false;
			}
			if ( p_dst_file->Write( p_buffer, remain ) == false )
			{
					success = false;
			}
			delete [] p_buffer;
			p_buffer = NULL;
		}
		else{
			p_buffer = new char[ src_length ];
			if ( p_src_file->Read( p_buffer, src_length ) == false )
			{
					success = false;
			}
			if ( p_dst_file->Write( p_buffer, src_length ) == false )
			{
					success = false;
			}
			delete [] p_buffer;
			p_buffer = NULL;
		}
		Close( p_dst_file );
		Close( p_src_file );
		return success;
	}
	return false;
}

bool	CFileSystem::MakeWritable( const char* filename )
{
	return ( _chmod(filename, _S_IWRITE ) >= 0 );
}

bool	CFileSystem::MakeReadOnly( const char* filename )
{
	return ( _chmod(filename, _S_IREAD  ) >= 0 );
}

bool	CFileSystem::DeleteFile( const char* filename )
{
	//改变文件为非只读并删除文件。
	if ( MakeWritable( filename ) == true )
	{
		return ( remove( filename ) >= 0 );
	}
	return false;
}


bool	CFileSystem::CopyDir( const char* src_dir, const char* dst_dir )
{
	//	Gather the files in this directory
	CFileList	dir_files;
	int i = 0;
	if ( GetDirFiles( src_dir, &dir_files ) == false )
	{
		return false;
	}
	//	Strip off the src path from the filenames
	const u32	src_path_len = strlen(src_dir);
	for ( i = 0; i < dir_files.size(); i++)
	{
		dir_files[i]->m_filename = dir_files[i]->m_filename.substr(src_path_len,dir_files[i]->m_filename.length()-src_path_len);//dir_entry.m_szFileName[ src_path_len ];
	}
	//	Make sure the destination folder exists
	if ( MakeDir( dst_dir ) == false )
	{
		return false;
	}
	//	Make sure all the destination directorys exist
	for ( i = 0 ;i < dir_files.size() ; i++)
	{
		if ( dir_files[i]->IsDirectory() == true )
		{
			const string directory = dst_dir + dir_files[i]->m_filename;

			if ( MakeDir( directory.c_str() ) == false )
			{
				return false;
			}
		}
	}
	//	Copy all the files
	for ( i = 0; i < dir_files.size(); i++  )
	{
		if ( dir_files[i]->IsFile() == true )
		{
			const string	src_file = src_dir + dir_files[i]->m_filename ;
			const string	dst_file = dst_dir + dir_files[i]->m_filename ;

			if ( CopyFile( src_file.c_str(), dst_file.c_str()) == false )
			{
				return false;
			}
		}
	}

	for ( i = 0; i < dir_files.size(); i++  ){
		if(dir_files[i]){
			delete dir_files[i];
			dir_files[i] = NULL;
		}
	}

	return true;
}

bool	CFileSystem::MakeDir( const char* directory )
{
	if ( IsDirExists( directory ) == false )
	{
		return ( _mkdir( directory ) == 0 );
	}
	return true;
}


bool	CFileSystem::DeleteDir( const char* directory )
{

	//	Get a list of all the files and folders to delete
	CFileList	dir_files;
	int  i = 0;
	bool	no_files = false ;
	bool	no_directory = false ;

	if ( GetDirFiles( directory, &dir_files ) == false )
	{
		return false;
	}
	//	Delete all the files first
	while ( no_files == false && dir_files.empty() == false )
	{
		no_files = true;
		for ( i = 0; i < dir_files.size(); i++ )
		{
			if ( dir_files[i]->IsFile() == true )
			{
				if ( DeleteFile( dir_files[i]->m_filename.c_str() ) == false )
				{
					return false;
				}
				no_files = false;
				delete dir_files[i];
				dir_files.erase( dir_files.begin()+ i );
				break;
			}
		}
	}
	//	Delete all the directorys last

	while ( no_directory == false && dir_files.empty() == false )
	{
		no_directory = true;

		for ( i = 0 ;i < dir_files.size();i++ )
		{
			if ( dir_files[i]->IsDirectory() == true )
			{
				no_directory = false;

				if ( _rmdir( dir_files[i]->m_filename.c_str()) == 0 )
				{
					delete dir_files[i];
					dir_files.erase(dir_files.begin()+ i );
					break;
				}
			}
		}
	}
	//	Finally remove the parent directory
	if ( _rmdir( directory ) != 0 )
	{
		return false;
	}

	return true;

}




bool	CFileSystem::Rename( const char* old_name, const char* new_name )
{
	if ( MakeWritable( old_name ) == true )
	{
		return ( rename( old_name, new_name ) == 0 );
	}

	return false;
}

void	CFileSystem::SplitPath( const char* path, char* p_drive, char* p_dir, char* p_fname, char* p_ext )
{
	_splitpath(path,p_drive,p_dir,p_fname,p_ext);
}

string	CFileSystem::GetFileExtension( const char* filename )
{
	char ext[256];
	SplitPath(filename,NULL,NULL,NULL,ext);
	return string(ext);
}


string	CFileSystem::MakeFullPath( const char* filename )
{
	string name =  filename;
	if ( name.find( m_workdir ) >= 0 )
	{
		return name;
	}
	else
	{
		return ( m_workdir + name );
	}
}
	
/*
*/


