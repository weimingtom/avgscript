#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#ifdef WIN32
#else
#endif



/**
 * 文件属性定义，与sony的定义相同。
 */
enum eAttributeFlags
{
	AF_READ_ONLY	= ( 1 << 0 ),
	AF_HIDDEN	= ( 1 << 1 ),
	AF_UNKNOWN1	= ( 1 << 2 ),
	AF_UNKNOWN2	= ( 1 << 3 ),
	AF_DIRECTORY	= ( 1 << 4 ),
	AF_ARCHIVE	= ( 1 << 5 ),
	AF_DOT_DOT	= ( 1 << 30 ),
	AF_DRIVE	= ( 1 << 31 ),
};

#ifdef WIN32

struct SceIoStat
{
	int st_mode;
};
#define SceUID int
#endif


struct sDirEntry
{
	string		m_filename;
	SceIoStat	m_stats;
	/**
	 * 当前类型是否是一个文件。
	 * @param 无
	 * @return bool,如函数名所示。
	 */
	bool		IsFile() const			{ return ( IsDrive() == false && IsDotDot() == false && IsDirectory() == false ); }
	/**
	 * 当前类型是否是一个盘符。
	 * @param 无
	 * @return bool,如函数名所示。
	 */
	bool		IsDrive() const			{ return ( m_stats.st_mode & AF_DRIVE ); }
	/**
	 * 当前类型是否是一个退目录符[..]。
	 * @param 无
	 * @return bool,如函数名所示。
	 */
	bool		IsDotDot() const		{ return ( m_stats.st_mode & AF_DOT_DOT ); }
	/**
	 * 当前类型是否是一个目录。
	 * @param 无
	 * @return bool,如函数名所示。
	 */
	bool		IsDirectory() const		{ return ( ( m_stats.st_mode & FIO_S_IFDIR ) & FIO_S_IFMT ); }
};

typedef list< sDirEntry > CFileList;

typedef SceUID	FIND_FILE_HANDLE;

/**
 * 文件读写类。
 */
class CFile
{
	public:
		/**
		 * 读文件函数。
		 * @param void*,读取文件的位置。
		 * @param int32,读取文件的长度。
		 * @param int*, 实际读取文件的长度。
		 * @return bool,成功为true，失败为false。
		 */
		bool			Read( void * p_address, u32 length, u32 * p_nbytes_read = NULL ) const;
		/**
		 * 写文件函数。
		 * @param void*,写入文件的位置。
		 * @param int32,写入文件的长度。
		 * @param int*, 实际写入文件的长度。
		 * @return bool,成功为true，失败为false。
		 */
		bool			Write( const void * p_address, u32 length, u32 * p_nbytes_written = NULL ) const;
		/**
		 * 是否到达文件尾。
		 * @param 无参数。
		 * @return bool,返回true或者false。
		 */
		bool			IsEOF() const;
		/**
		 * 获得文件长度。
		 * @param 无参数。
		 * @return int,返回文件的长度。
		 */
		u32			GetLength() const;
		/**
		 * 获得文件长度。
		 * @param 无参数。
		 * @return int,返回文件的长度。
		 */
		u32			Tell() const;
		/**
		 * 移动文件指针位置。
		 * @param int,相对于初始位置的偏移。
		 * @param int,同SEEK_END,SEEK_SET,SEEK_CUR.
		 * @return int,返回成功失败标志。
		 */
		bool			Seek( const u32 offset, const u32 origin ) const;
		/**
		 * 读一个字符，同fgetchar。
		 * @param ,无。
		 * @return char,返回字符，文件指针+1（同fgetchar）。
		 */
		char			GetChar() const;
	protected:
		CFile( const char* filename, FILE * const handle );
		~CFile();
	private:
		string			m_filename;
		FILE *			m_phandle;
		u32			m_length;
};

/**
 * 文件访问类。
 */
class CFileSystem
{
	public:
		/**
		 * 设置工作路径，同chdir。
		 * @param const char*,工作目录名。
		 * @return bool,true/false。
		 */
		static bool		SetWorkDir( const char * dir );
		/**
		 * 打开一个文件，返回文件读写类。
		 * @param const char*,打开的文件名。
		 * @param const char*,打开的标志，同fopen。
		 * @return CFile*,文件读写类的指针。
		 */
		static CFile *		Open( const char * filename, const char * const open_flags );
		/**
		 * 关闭一个文件。
		 * @param CFile * const,文件读写类的指针。
		 * @return bool,true/false。
		 */
		static void		Close( CFile * const pfile );
		/**
		 * 是否文件存在。
		 * @param const char*,文件名。
		 * @return bool,true/false。
		 */
		static bool		IsFileExists( const char * filename );
		/**
		 * 是否目录存在。
		 * @param const char*,目录名。
		 * @return bool,true/false。
		 */
		static bool		IsDirExists( const char * dir );
		/**
		 * 得到dir下所有文件的列表，保存在第二个参数CFileList指针中。
		 * @param const char*,目录名。
		 * @param const CFlieList*,获取的文件列表。
		 * @return bool,true/false。
		 */
		static bool		GetDirFiles( const char * dir,const CFileList * dir_files );
		
		static bool		FindFirstFile( const char * path, const FIND_FILE_HANDLE * handle );
		static bool		FindNextFile( const sDirEntry * dir_entry, FIND_FILE_HANDLE handle );
		static bool		FindCloseFile( FIND_FILE_HANDLE handle );
		/**
		 * 拷贝一个文件。
		 * @param const char*,要拷贝源文件名。
		 * @param const char*,要拷贝到的目标文件名。
		 * @return bool,true/false。
		 */
		static bool		CopyFile( const char * src_file, const char * dst_file );
		/**
		 * 拷贝一个目录。
		 * @param const char*,要拷贝源目录名。
		 * @param const char*,要拷贝到的目的目录名。
		 * @return bool,true/false。
		 */
		static bool		CopyDir( const char * src_dir, const char * dst_dir );
		/**
		 * 删除一个文件
		 * @param const char*,要删除的文件名。
		 * @return bool,true/false。
		 */
		static bool		DeleteFile( const char * filename );
		/**
		 * 删除一个目录
		 * @param const char*,要删除的目录名。
		 * @return bool,true/false。
		 */
		static bool		DeleteDir( const char * dir );
		/**
		 * 创建一个目录
		 * @param const char*,要创建的目录名。
		 * @return bool,true/false。
		 */
		static bool		MakeDir( const char * dir );
		/**
		 * 将老的文件名改名为新的文件名。
		 * @param const char*,要修改的旧的文件名。
		 * @param const char*,改变成的新的文件名。
		 * @return bool,true/false。
		 */
		static bool		Rename( const char * old_name, const char * new_name );
		/**
		 * 将文件属性设置为只读。
		 * @param const char*,要设置只读的文件名。
		 * @return bool,true/false。
		 */
		static bool		MakeReadOnly( const char * filename );
		/**
		 * 将文件属性设置为可写。
		 * @param const char*,要设置可写的文件名。
		 * @return bool,true/false。
		 */
		static bool		MakeWritable( const char * filename );
		/**
		 * 生成一个文件的全路径（绝对路径）。
		 * @param const char*,要生成全路径的文件名。
		 * @return string,文件的全路径（绝对路径）。
		 */
		static string		MakeFullPath( const char * filename );
		/**
		 * 获得一个文件的扩展名，例如c://dev/test.mp3就获得字符串mp3。
		 * @param const char*,文件名。
		 * @return string,文件的扩展名。
		 */
		static string		GetFileExtension( const char * filename );
		/**
		 * 解析一个路径，分别填入相应串中。
		 * @param const char*,待解析的路径。
		 * @param const char*,盘符。
		 * @param const char*,目录名。
		 * @param const char*,文件名。
		 * @param const char*,扩展名。
		 * @return 无返回值。
		 */
		static void		SplitPath( const char * path, const char * drive, const char * dir, const char * fname, const char * ext );
	private:
		#ifdef WIN32
		CFileFind       m_filefind;
		#else
		static SceIoDirent	m_direntry;
		#endif
		static string		m_workdir;
};



#endif
