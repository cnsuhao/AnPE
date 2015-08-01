#include "APEImport.h"
#include "APESection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//构建与实现
//////////////////////////////////////////////////////////////////////////

APEImport::APEImport()
{

}

APEImport::~APEImport()
{

}

void APEImport::operator =(APEFile& lPeFile)
{
	SetInfo(lPeFile.GetInfo());
}

//得到整个首个导入表
PIMAGE_IMPORT_DESCRIPTOR APEImport::GetFirstImportDesc()
{
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	pImportDesc =(PIMAGE_IMPORT_DESCRIPTOR)GetDirectoryEntryToData(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (pImportDesc)
	{
		return NULL;
	}
	return pImportDesc;
}

//得到共有多少个dll
int APEImport::GetImportDesCount()
{
	int i=0;
	PIMAGE_IMPORT_DESCRIPTOR pTempImport=GetFirstImportDesc();
	while (pTempImport->FirstThunk)
	{
		++i;
		++pTempImport;
	}
	return i;
}

//得到某个API 函数的名字
PIMAGE_IMPORT_BY_NAME APEImport::GetImportFucByName(DWORD RavThunk)
{
	PIMAGE_NT_HEADERS		pNtH=NULL;
	PIMAGE_IMPORT_BY_NAME	pApiName=NULL;

	pApiName=(PIMAGE_IMPORT_BY_NAME)RvaToPtr(RavThunk);

	return pApiName;
}

DWORD APEImport::GetImportThunk(PIMAGE_IMPORT_DESCRIPTOR pImportDesc)
{
	return pImportDesc->OriginalFirstThunk?pImportDesc->OriginalFirstThunk:pImportDesc->FirstThunk;
}

BOOL APEImport::ReLocalImport(DWORD dwRVA)
{
	int iCount=GetImportDesCount();
	APESection lPeSection;
	lPeSection.SetInfo(GetInfo());

	for (int i=0;i<iCount;i++)
	{
		PIMAGE_IMPORT_DESCRIPTOR pImport=GetImportDesc(i);
		DWORD dwOrThunk =pImport->OriginalFirstThunk;
		DWORD dwIatThunk=pImport->FirstThunk;

		//哪门子硬编码?
		pImport->OriginalFirstThunk+=dwRVA-0x2000;
		pImport->Name+=(dwRVA-0x2000);
		pImport->FirstThunk+=(dwRVA-0x2000);

		while (*(DWORD*)RvaToPtr(dwOrThunk)!=0)
		{
			*(DWORD*)RvaToPtr(dwOrThunk)+=(dwRVA-0x2000);
			*(DWORD*)RvaToPtr(dwIatThunk)+=(dwRVA-0x2000);
			dwOrThunk+=sizeof(DWORD);
			dwIatThunk+=sizeof(DWORD);
		}


	}
	return TRUE;
}














