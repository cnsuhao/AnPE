#pragma once

#include <Windows.h>
#include <tchar.h>


typedef struct _MAP_FILE_STRUCT
{
	HANDLE	hFile;		//打开文件的句柄
	HANDLE	hMapping;	//映射文件的句柄
	LPVOID	ImageBase;  //文件在内存中的起始地址
	DWORD	dwFileSize;	//文件的实际大小
} MAP_FILE_STRUCT,*PMAP_FILE_STRUCT;

enum{
	AFILE_FROM_NORMAL=0,	//文件打开时的处理方法
	AFILE_FROM_FILE,
	AFILE_FROM_MEMORY

};

//基本的PE处理 可以得到DOS头,NT头,节等信息.
class APEFile
{
public:
	BOOL	LoadPEFile(BYTE* lpBuffer,DWORD dwSize);			//载入PE文件
	LPVOID	RvaToPtr(DWORD	dwRVA);								//计算偏移
	LPVOID	GetDirectoryEntryToData(USHORT DirectoryEntry);
	PIMAGE_SECTION_HEADER GetSection(int nIndex);				//得到指定节的信息
	BOOL	IsPEFile();
	PIMAGE_SECTION_HEADER GetFirstSectionHeader();				//得到第一个节
	int		GetSectionCout();									//得到节数
	PIMAGE_DATA_DIRECTORY GetDataDirectory(int nIndex);			//得到数据指定目录的信息
	PIMAGE_DATA_DIRECTORY GetDataDirectory();					//得到数据目录第一项
	PIMAGE_OPTIONAL_HEADER GetNtOptionalHeader();
	PIMAGE_FILE_HEADER GetNtFileHeader();	
	PIMAGE_NT_HEADERS  GetNtHeader();
	PIMAGE_DOS_HEADER  GetDosHeader();
	DWORD	GetImageSize();
	BYTE*	GetImage();
	bool	Free();
	BOOL	LoadPEFile(WCHAR* lpFileName);
	BOOL	SetInfo(MAP_FILE_STRUCT *pMapFileMsg);
	MAP_FILE_STRUCT*	GetInfo();
	void	operator=(APEFile& lPeFile);
	APEFile();
	virtual ~APEFile();

private:
	//用于标识文件的打开状态
	BYTE bLoadStact;

protected:
	//用于存储打开的文件句柄,文件数据,文件大小
	MAP_FILE_STRUCT *pstMapFile;

};
