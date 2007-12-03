#include "CFileSystem.h"

bool CFile::Read( void * p_address, u32 length, u32 * p_nbytes_read = NULL ) const;
bool CFile::Write( const void * p_address, u32 length, u32 * p_nbytes_written = NULL ) const;
bool CFile::IsEOF() const;
u32  CFile::GetLength() const;
u32  CFile::Tell() const;
bool CFile::Seek( const u32 offset, const u32 origin ) const;
char CFile::FGetC() const;
string CFile::GetExtension() const;
bool CFile::IsReadOnly() const;
bool CFile::IsWritable() const;
CFile::operator FILE * () const;
CFile::CFile( const CString & filename, FILE * const p_handle );
CFile::~CFile();