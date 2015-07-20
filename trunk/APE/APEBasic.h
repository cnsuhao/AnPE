#pragma once

#include <Windows.h>
#include <tchar.h>


typedef struct _MAP_FILE_STRUCT
{
	HANDLE	hFile;		//���ļ��ľ��
	HANDLE	hMapping;	//ӳ���ļ��ľ��
	LPVOID	ImageBase;  //�ļ����ڴ��е���ʼ��ַ
	DWORD	dwFileSize;	//�ļ���ʵ�ʴ�С
} MAP_FILE_STRUCT,*PMAP_FILE_STRUCT;

enum{
	AFILE_FROM_NORMAL=0,	//�ļ���ʱ�Ĵ�����
	AFILE_FROM_FILE,
	AFILE_FROM_MEMORY

};

//������PE���� ���Եõ�DOSͷ,NTͷ,�ڵ���Ϣ.
class APEFile
{
public:
	BOOL	LoadPEFile(BYTE* lpBuffer,DWORD dwSize);			//����PE�ļ�
	LPVOID	RvaToPtr(DWORD	dwRVA);								//����ƫ��
	LPVOID	GetDirectoryEntryToData(USHORT DirectoryEntry);
	PIMAGE_SECTION_HEADER GetSection(int nIndex);				//�õ�ָ���ڵ���Ϣ
	BOOL	IsPEFile();
	PIMAGE_SECTION_HEADER GetFirstSectionHeader();				//�õ���һ����
	int		GetSectionCout();									//�õ�����
	PIMAGE_DATA_DIRECTORY GetDataDirectory(int nIndex);			//�õ�����ָ��Ŀ¼����Ϣ
	PIMAGE_DATA_DIRECTORY GetDataDirectory();					//�õ�����Ŀ¼��һ��
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
	//���ڱ�ʶ�ļ��Ĵ�״̬
	BYTE bLoadStact;

protected:
	//���ڴ洢�򿪵��ļ����,�ļ�����,�ļ���С
	MAP_FILE_STRUCT *pstMapFile;

};
