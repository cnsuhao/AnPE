#include "APEImport.h"
#include "APESection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//������ʵ��
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

//�õ������׸������
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

//�õ����ж��ٸ�dll
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

//�õ�ĳ��API ����������
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

		//������Ӳ����?
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














