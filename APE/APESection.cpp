#include "APESection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//构建与实现
//////////////////////////////////////////////////////////////////////////

#define ZALIGN(x,a)(((x-1)/a+1)*a)

APESection::APESection()
{

}

APESection::~APESection()
{

}


void APESection::operator=(APEFile& lPeFile)
{
	SetInfo(lPeFile.GetInfo());
}

int APESection::RvaToSectionIndex(DWORD dwRva)
{
	int iSectionIndex = -1;
	int iSectionCount = GetSectionCout();
	for (int i=0;i<iSectionCount;i++)
	{
		PIMAGE_SECTION_HEADER pTempSectionHeader = GetSection(i);
		if (pTempSectionHeader->VirtualAddress<=dwRva)
		{
			if (dwRva <= pTempSectionHeader->Misc.VirtualSize+pTempSectionHeader->VirtualAddress)
			{
				iSectionIndex=i;
				break;
			}
		}
	}
	return iSectionIndex;
}

int APESection::OffsetToSectionIndex(DWORD dwOffset)
{
	int iSectionIndex;
	int iSectionCount = GetSectionCount();
	for (int i=0;i<=iSectionCount;i++)
	{
		PIMAGE_SECTION_HEADER pTempSectionHeader = GetSection(i);
		if (pTempSectionHeader->PointerToRawData < dwOffset)
		{
			if (dwOffset <= (pTempSectionHeader->PointerToRawData + pTempSectionHeader->SizeOfRawData))
			{
				iSectionIndex = i;
				break;
			}
		}

	}
	return iSectionIndex;
}


//节区的RVA地址-在文件中的偏移=?VRr?,两者差值吧.用来OffsetToRva.
DWORD APESection::GetVRK(int nSectionIndex)
{
	PIMAGE_SECTION_HEADER pTempSectionHeader = GetSection(nSectionIndex);
	DWORD dwVRK = pTempSectionHeader->VirtualAddress-pTempSectionHeader->PointerToRawData;
	return dwVRK;
}

DWORD APESection::OffsetToRva(DWORD dwOffset)
{
	int nSectionIndex = OffsetToSectionIndex(dwOffset);
	DWORD dwVRK=GetVRK(nSectionIndex);
	return dwVRK+dwOffset;
}

BOOL APESection::AddSection(char *pSectionName,DWORD &dwSectionSize,DWORD dwSectionStact)
{
	LPVOID	lPointer = NULL;
	PBYTE	pData =NULL;
	DWORD	newSectionAddr = 0;

	if (!IsPEFile())
	{
		return FALSE;
	}

	PIMAGE_NT_HEADERS pNTHdr = GetNtHeader();

	//是否有足够空间容纳节头
	//增加后的大小与所有头＋节表的大小比较.不对吧?
	//SizeOfHeaders是MS-DOS头,PE头,区块表的尺寸之和,外加对齐.
	//我们这里占用的空间是对齐缝隙的空间.SizeOfHeaders>=新区块表的结尾.
	if ((pNTHdr->FileHeader.NumberOfSections+1)*sizeof(IMAGE_SECTION_HEADER)+前面部分的大小>pNTHDr->OptionalHeader.SizeOfHeaders)
	{
		return FALSE;
	}
	//对齐数据
	DWORD uCodeDelta=ZALIGN(dwSectionSize,pNTHdr->OptionalHeader.SectionAlignment);
	DWORD uFileDelta=ZALIGN(dwSectionSize,pNTHdr->OptionalHeader.FileAlignment);
	
	PIMAGE_SECTION_HEADER pNewSec=(PIMAGE_SECTION_HEADER)(pNTHdr+1)+pNTHdr->FileHeader.NumberOfSections;




}

















