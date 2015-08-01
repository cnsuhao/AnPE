

#include "APEReLoc.h"
#include "APESection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG


//////////////////////////////////////////////////////////////////////////
//构建与实现
//////////////////////////////////////////////////////////////////////////

APEReLoc::APEReLoc()
{

}

APEReLoc::~APEReLoc()
{

}

void APEReLoc::operator = (APEFile& lPeFile)
{
	SetInfo(lPeFile.GetInfo());
}

//得到首个重定位表
PIMAGE_BASE_RELOCATION APEReLoc::GetFirstReloc()
{
	PIMAGE_BASE_RELOCATION pRelocDesc = NULL;
	pRelocDesc = (PIMAGE_BASE_RELOCATION)GetDirectoryEntryToData(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if (pRelocDesc == NULL)
	{
		return NULL;
	}
	return pRelocDesc;
};

//得到重定位表的个数
int APEReLoc::GetReLocCount()
{
	int i = 0;
	DWORD dwOffset;
	PIMAGE_DATA_DIRECTORY pDataDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	dwOffset = pDataDir->VirtualAddress;
	PIMAGE_BASE_RELOCATION pTempReLoc = GetFirstReloc();
	while(pTempReLoc->VirtualAddress)
	{
		++i;
		dwOffset = dwOffset + (pTempReLoc->SizeOfBlock);
		pTempReLoc = (PIMAGE_BASE_RELOCATION)RvaToPtr(dwOffset);
	}

	return i;
}

PIMAGE_BASE_RELOCATION APEReLoc::GetReLoc(int nIndex, DWORD *rdwOffset)
{
	PIMAGE_BASE_RELOCATION pReLocal = NULL;
	DWORD dwOffset = 0;
	int nCount = GetReLocCount();
	PIMAGE_DATA_DIRECTORY pDataDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	dwOffset = pDataDir->VirtualAddress;

	PIMAGE_BASE_RELOCATION pTempReLoc = GetFirstReloc();
	int j = 0;
	if (nIndex<nCount)
	{
		while (pTempReLoc->VirtualAddress)
		{
			if (nIndex == (j++))
			{
				pReLocal = pTempReLoc;
				*rdwOffset = dwOffset;
				break;
			}
			dwOffset = dwOffset +(pTempReLoc->SizeOfBlock);
			pTempReLoc = (PIMAGE_BASE_RELOCATION)RvaToPtr(dwOffset);

		}
	}

	return pReLocal;
}

int APEReLoc::GetTypeOffsetCount(int nIndex)
{
	int i = 0;
	DWORD dwOffset;
	PIMAGE_BASE_RELOCATION pReLocal = GetReLoc(nIndex,&dwOffset);
	if (pReLocal!=NULL)
	{
		i=(pReLocal->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION));
	}

	return i;
}

WORD APEReLoc::GetTypeOffset(int nRelocIndex,int nOffsetIndex)
{
	DWORD dwRva;
	PIMAGE_BASE_RELOCATION pReLoc = GetReLoc(nRelocIndex,&dwRva);
	APESection lPeSection;
	lPeSection.SetInfo(GetInfo());
	DWORD dwFileOffset = lPeSection.RvaToOffset(dwRva);
	WORD  wTypeOffset = 0;
	if (pReLoc!=NULL)
	{
		memcpy_s(&wTypeOffset,sizeof(wTypeOffset),GetImage()+dwFileOffset+sizeof(IMAGE_BASE_RELOCATION)+\
			sizeof(WORD)*nOffsetIndex,sizeof(WORD));//改用了安全版函数
	}

	return wTypeOffset;
}

WORD APEReLoc::GetReLocType(int nRelocIndex, int nTypeOffsetIndex)
{
	WORD wTypeOffset = GetTypeOffset(nRelocIndex,nTypeOffsetIndex);
	wTypeOffset &= 0xF000;
	wTypeOffset = wTypeOffset>>12;
	return wTypeOffset;
}

DWORD APEReLoc::GetReLocRVA(int nRelocIndex,int nTypeOffsetIndex)
{
	DWORD dwOffset;
	PIMAGE_BASE_RELOCATION pReLocal = GetReLoc(nRelocIndex,&dwOffset);
	WORD wTypeOffset = GetTypeOffset(nRelocIndex,nTypeOffsetIndex);
	wTypeOffset &= 0x0FFF;
	return pReLocal->VirtualAddress+wTypeOffset;
}


BOOL APEReLoc::RelocVA(DWORD dwNewAddress,DWORD dwNewImageBase,DWORD &dwOldVA)
{
	PIMAGE_OPTIONAL_HEADER pOptHeader = GetNtOptionalHeader();
	dwOldVA-=pOptHeader->ImageBase;
	dwOldVA-=0x1000;
	dwOldVA+=dwNewAddress;
	dwOldVA+=dwNewImageBase;
	return TRUE;
}

BOOL APEReLoc::RelocMySelf(DWORD dwNewSecAddress,DWORD dwImageBase)
{
	int iRecCount = GetReLocCount();
	int iTypeOffset=0;
	for (int i=0;i<iRecCount;i++)
	{
		iTypeOffset = GetTypeOffsetCount(i);
		for (int j=0;j<iTypeOffset;j++)
		{
			if (GetReLocType(i,j)==3)
			{
				RelocVA(dwNewSecAddress,dwImageBase,*(DWORD*)RvaToPtr((GetReLocRVA(i,j))));
			}
		}
	}
	return TRUE;
}
