#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

enum eAttributeFlags
{
	AF_READ_ONLY	= ( 1 << 0 ),
	AF_HIDDEN	= ( 1 << 1 ),
	AF_DIRECTORY	= ( 1 << 2 ),
	AF_ARCHIVE	= ( 1 << 3 ),
	AF_DOT_DOT	= ( 1 << 4 ),
	AF_DRIVE	= ( 1 << 5 )
};

struct sDirEntry
{
	string		m_szFileName;
	SceIoStat	m_Stats;

	bool		IsFile() const			{ return ( IsDrive() == false && IsDotDot() == false && IsDirectory() == false ); }
	bool		IsDrive() const			{ return ( m_Stats.st_mode & AF_DRIVE ); }
	bool		IsBackButton() const		{ return ( m_Stats.st_mode & AF_DOT_DOT ); }
	bool		IsDirectory() const		{ return ( ( m_Stats.st_mode & FIO_S_IFDIR ) & FIO_S_IFMT ); }
};

typedef list< sDirEntry > CFileList;

typedef SceUID	FIND_FILE_HANDLE;


class CFile
{
	public:
		bool			Read( void * p_address, u32 length, u32 * p_nbytes_read = NULL ) const;
		bool			Write( const void * p_address, u32 length, u32 * p_nbytes_written = NULL ) const;
		bool			IsEOF() const;
		u32			GetLength() const;
		u32			Tell() const;
		bool			Seek( const u32 offset, const u32 origin ) const;
		char			FGetC() const;
		string			GetExtension() const;
		bool			IsReadOnly() const;
		bool			IsWritable() const;
		operator FILE * () const;
	protected:
		CFile( const CString & filename, FILE * const p_handle );
		~CFile();
	private:
		string			m_szFilename;
		FILE *			m_pHandle;
		u32			m_Length;
};

class CFileSystem
{
	public:
		static bool		SetRoot( const string & directory );
		static CFile *		Open( const string & filename, const char * const p_open_flags );
		static void		Close( CFile * const p_file );
		static bool		FileExists( const string & filename );
		static bool		DirectoryExists( const string & directory );
		static bool		GetDirectoryFiles( const string & directory, CFileList & dir_files );
		static bool		FindFirstFile( const string & path, FIND_FILE_HANDLE & handle );
		static bool		FindNextFile( sDirEntry & dir_entry, FIND_FILE_HANDLE handle );
		static bool		FindCloseFile( FIND_FILE_HANDLE handle );
		static bool		CopyFile( const string & src_file, const string & dst_file, float progress_inc = 0.f );
		static bool		CopyDirectory( const string & src_dir, const string & dst_dir, float progress_inc = 0.f );
		static bool		DeleteFile( const string & filename );
		static bool		DeleteDirectory( const string & directory );
		static bool		MakeDirectory( const string & directory );
		static bool		IsUMDInserted();
		static bool		ActivateUMD();
		static bool		Rename( const string & old_name, const string & new_name );
		static void		SplitPath( const string & path, string * p_drive, string * p_dir, string * p_fname, string * p_ext );
		static bool		MakeReadOnly( const string & filename );
		static bool		MakeWritable( const string & filename );
		static string		MakeFullPath( const string & filename );
		static string		GetFileExtension( const string & filename );
		static bool		HideCorruptFiles();
		static void		SetHideCorruptFiles( bool hide );
	private:
		static SceIoDirent	s_DirEntry;
		static string		s_szRootDirectory;
};



#endif
