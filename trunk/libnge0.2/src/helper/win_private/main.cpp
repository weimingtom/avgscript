#include "CFileSystem_win.h"


int main(int argc,char* argv[])
{
	CFileList * dir_files = new CFileList;
	CFileSystem::GetDirFiles("J:\\test\\*",dir_files);
	printf("%d\n",dir_files->size());
	for (int i =0 ;i<dir_files->size();i++) {
		if(!(*dir_files)[i]->IsDirectory())
		printf("%s\n",(*dir_files)[i]->m_filename.c_str());
	}
	getchar();
	return 1;
}