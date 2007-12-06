#include "CFileSystem_win.h"


int main(int argc,char* argv[])
{
	
	CFileList * dir_files = new CFileList;
	CFileSystem::GetDirFiles(".",dir_files);
	printf("1 目录文件访问测试 共%d个目录或文件\n",dir_files->size());
	for (int i =0 ;i<dir_files->size();i++) {
		if(!(*dir_files)[i]->IsDirectory())
			printf("文件: %s\n",(*dir_files)[i]->m_filename.c_str());
		else
			printf("目录: %s\n",(*dir_files)[i]->m_filename.c_str());
	}

	//拷贝测试。
	printf("2 文件拷贝测试\n");
	printf("文件拷贝%s\n",CFileSystem::CopyFile("main.cpp","main.bak")==true?"成功":"失败");
	printf("3 目录拷贝测试\n");
	printf("目录拷贝1%s\n",CFileSystem::CopyDir( "Debug", "Debug1" )==true?"成功":"失败");
	printf("目录拷贝2%s\n",CFileSystem::CopyDir( "Debug", "Debug2" )==true?"成功":"失败");
	printf("4 目录删除测试,请按回车键继续....\n");
	getchar();
	printf("目录删除%s\n",CFileSystem::DeleteDir("Debug2" )==true?"成功":"失败");
	printf("测试结束\n");
	CFileSystem::Rename("main.bak","main.bak2");
	//getchar();
	return 1;
}