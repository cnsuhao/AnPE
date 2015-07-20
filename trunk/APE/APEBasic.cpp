

#include "APEBasic.h"
#include <imageHlp.h>  
#pragma comment(lib,"Dbghelp.lib")



APEFile::APEFile()
{
	bLoadStact=AFILE_FROM_NORMAL;
	pstMapFile=NULL;
}

APEFile::~APEFile()
{
	if (Free())
	{
		delete pstMapFile;
		pstMapFile=NULL;
	}
}

//�õ��ļ��򿪵�����

MAP_FILE_STRUCT* APEFile::GetInfo()
{
	return pstMapFile;
}

//�ô򿪵���Ϣ����һ��PE
BOOL APEFile::SetInfo(MAP_FILE_STRUCT* pMapFileMsg)
{
		this->pstMapFile=pMapFileMsg;
	return TRUE;
}

void APEFile::operator =(APEFile& lPeFile)
{
	SetInfo(lPeFile.GetInfo());
}

//���ڽ��ļ������ڴ�

BOOL APEFile::LoadPEFile(WCHAR *lpFileName)
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
	pstMapFile=new MAP_FILE_STRUCT;
	bLoadStact=AFILE_FROM_FILE;

	hFile=CreateFile(lpFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile==NULL)
	{
		OutputDebugString(_T("hFile==NULL"));
		return FALSE;
	}
	pstMapFile->dwFileSize=GetFileSize(hFile,NULL);
	hMapping=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,pstMapFile->dwFileSize,NULL);
	if (hMapping==NULL)
	{
		CloseHandle(hFile);
		OutputDebugString(_T("hMappint==NULL"));
		return FALSE;
	}
	
	ImageBase=MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,pstMapFile->dwFileSize);
	if (ImageBase==NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMapping);
		OutputDebugString(_T("ImageBase==NULL"));
		return FALSE;
	}
	pstMapFile->hFile=hFile;
	pstMapFile->hMapping=hMapping;
	pstMapFile->ImageBase=ImageBase;
	return TRUE;
}

//ֱ�Ӵ��ڴ��м����Ѿ���ϵͳ���ص�PE?

BOOL APEFile::LoadPEFile(BYTE *lpBuffer, DWORD dwSize)
{
	pstMapFile	= new MAP_FILE_STRUCT;
	bLoadStact	= AFILE_FROM_MEMORY;

	pstMapFile->hFile = pstMapFile->hMapping=NULL;
	pstMapFile->ImageBase = lpBuffer;
	pstMapFile->dwFileSize=dwSize;
	
	return TRUE;

}

//���ļ����ڴ����ͷ�
//���ͷ�һ��ImageBase?
bool APEFile::Free()
{
	if (bLoadStact == AFILE_FROM_FILE)
	{
		if (pstMapFile->hMapping != NULL)
		{
			UnmapViewOfFile(pstMapFile->hMapping);
			CloseHandle(pstMapFile->hMapping);
			pstMapFile->hMapping = NULL;
			if (pstMapFile->hFile != NULL)
			{
				CloseHandle(pstMapFile->hFile);
				pstMapFile->hFile=NULL;
			}
				return true;
		}
	}else if(bLoadStact == AFILE_FROM_MEMORY)
	{
		LocalFree(pstMapFile->ImageBase);
		return true;
	}
	return false;
}


BYTE* APEFile::GetImage()
{
	if (pstMapFile != NULL)
	{
		return (BYTE*)pstMapFile->ImageBase;
	}
	return NULL;
}

DWORD APEFile::GetImageSize()
{
	if (pstMapFile!=NULL)
	{
		return pstMapFile->dwFileSize;
	}
	return 0;
}
//�õ��ļ���dosͷ
PIMAGE_DOS_HEADER APEFile::GetDosHeader()
{
	return (PIMAGE_DOS_HEADER)GetImage();
}
//�õ��ļ���NTͷ
PIMAGE_NT_HEADERS APEFile::GetNtHeader()
{
	PIMAGE_DOS_HEADER pDH=GetDosHeader();
	if (pDH!=NULL)
	{
		return (PIMAGE_NT_HEADERS32)((DWORD)GetImage()+pDH->e_lfanew);
	}
	return NULL;
}
//�õ��ļ�ͷ
PIMAGE_FILE_HEADER APEFile::GetNtFileHeader()
{
	PIMAGE_NT_HEADERS pNth=GetNtHeader();
	if (pNth!=NULL)
	{
		return (PIMAGE_FILE_HEADER)&pNth->FileHeader;
	}
}
//�õ���ѡͷ
PIMAGE_OPTIONAL_HEADER APEFile::GetNtOptionalHeader()
{
	PIMAGE_NT_HEADERS pNth=GetNtHeader();
	if (pNth!=NULL)
	{
		return (PIMAGE_OPTIONAL_HEADER32)&pNth->OptionalHeader;
	}
	return NULL;
}
//�õ�����Ŀ¼
PIMAGE_DATA_DIRECTORY APEFile::GetDataDirectory()
{
	PIMAGE_OPTIONAL_HEADER pOptionHeader =GetNtOptionalHeader();
	if (pOptionHeader!=NULL)
	{
		return (PIMAGE_DATA_DIRECTORY) pOptionHeader->DataDirectory;
	}
	return NULL;
}

//ע������ֻ����16��
PIMAGE_DATA_DIRECTORY APEFile::GetDataDirectory(int nIndex)

{
	if (0<=nIndex<16)
	{
		PIMAGE_DATA_DIRECTORY pDataDir=GetDataDirectory();
		if (pDataDir!=NULL)
		{
				return &pDataDir[nIndex];
		}
	}
	return NULL;
}

int APEFile::GetSectionCout()
{
	PIMAGE_FILE_HEADER pNtFileHeader=GetNtFileHeader();
	if (pNtFileHeader!=NULL)
	{
		return pNtFileHeader->NumberOfSections;
	}
	return 0;
}

PIMAGE_SECTION_HEADER APEFile::GetFirstSectionHeader()
{
	PIMAGE_NT_HEADERS pNtH=GetNtHeader();
	if (pNtH!=NULL)
	{
		return IMAGE_FIRST_SECTION(pNtH);
	}
	return NULL;
}

PIMAGE_SECTION_HEADER APEFile::GetSection(int nIndex)
{
	int nCount=GetSectionCout();
	if (nIndex>=nCount||nIndex<0)
	{
		return NULL;
	}
	PIMAGE_SECTION_HEADER pFirstSectionHeader=GetFirstSectionHeader();
	for (int i=0;i<nCount;i++,pFirstSectionHeader++)
	{
		if (i==nIndex)
		{
			return pFirstSectionHeader;
		}
	}

	return NULL;
	
}
BOOL APEFile::IsPEFile()
{
	PIMAGE_DOS_HEADER pDosHeader=GetDosHeader();
	if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE)
	{
		PIMAGE_NT_HEADERS pNtHeader = GetNtHeader();
		if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
		{
			return TRUE;
		}
	}
	return FALSE;
}


LPVOID APEFile::GetDirectoryEntryToData(USHORT DirectoryEntry)
{
	DWORD dwDataStartRVA;
	LPVOID pDirData=NULL;
	PIMAGE_NT_HEADERS		pNtH=NULL;
	PIMAGE_OPTIONAL_HEADER	pOH =NULL;

	pNtH=GetNtHeader();
	if (!pNtH)
	{
		return NULL;
	}

	dwDataStartRVA=GetDataDirectory(DirectoryEntry)->VirtualAddress;
	if (!dwDataStartRVA)
	{
		return NULL;
	}

	pDirData=RvaToPtr(dwDataStartRVA);
	if (!pDirData)
	{
		return NULL;
	}

	return pDirData;

}

LPVOID APEFile::RvaToPtr(DWORD dwRVA)
{
	PIMAGE_NT_HEADERS pNtH=GetNtHeader();
	return ImageRvaToVa(pNtH,pstMapFile->ImageBase,dwRVA,NULL);
}













