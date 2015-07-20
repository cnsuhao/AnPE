//APExport.cpp:APExport类的实现
//
//////////////////////////////////////////////////////////////////////////

#include "APExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//构建与实现
//////////////////////////////////////////////////////////////////////////

APExplort::APExplort()
{

}

APExplort::~APExplort()
{

}

void APExplort::operator=(APEFile& lPeFile)
{
	SetInfo(lPeFile.GetInfo());

}

PIMAGE_EXPORT_DIRECTORY APExplort::GetExportTab()
{
	PIMAGE_EXPORT_DIRECTORY pExportDesc;
	pExportDesc =(PIMAGE_EXPORT_DIRECTORY)GetDirectoryEntryToData(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (pExportDesc==NULL)
	{
		return NULL;
	}
	return pExportDesc;
}

int APExplort::GetFuncCout()
{
	PIMAGE_EXPORT_DIRECTORY pExportDesc = GetExportTab();
	if (pExportDesc)
	{
		return pExportDesc->NumberOfFunctions;
	}
	return -1;
}

int APExplort::GetNameCout()
{
	PIMAGE_EXPORT_DIRECTORY pExplortDesc = GetExportTab();
	if (pExplortDesc)
	{
		return pExplortDesc->NumberOfNames;
	}
	return -1;
}

DWORD APExplort::GetFirstFunc()
{
	PIMAGE_EXPORT_DIRECTORY pExplortDesc = GetExportTab();
	if (pExplortDesc)
	{
		return pExplortDesc->AddressOfFunctions;
	}
	return 0;
}

DWORD APExplort::GetFunc(int nIndex)
{
	if (nIndex<GetFuncCout())
	{
		DWORD dwFistFunc=GetFirstFunc();
		return dwFistFunc+nIndex*4;//地址是连在一起的.代表后移动4个字节即一个DWORD.
	}
	return 0;
}

DWORD APExplort::GetFuncValue(int nIndex)
{
	DWORD dwFuncRva=GetFunc(nIndex);
	if (dwFuncRva)
	{
		return *(DWORD*)RvaToPtr(dwFuncRva);
	}
}

DWORD APExplort::GetFirstName()
{
	PIMAGE_EXPORT_DIRECTORY pExportDesc = GetExportTab();
	if (pExportDesc)
	{
		return pExportDesc->AddressOfNames;
	}
	return 0;
}

DWORD APExplort::GetName(int nIndex)
{
	if (nIndex<GetNameCout())
	{
		DWORD dwFirstName =GetFirstName();
		return dwFirstName+nIndex*4;
	}
	return 0;
}

char* APExplort::GetNameValue(int nIndex)
{
	DWORD dwNameRva=GetName(nIndex);
	if (dwNameRva)
	{
		DWORD dwNameValueRva=*(DWORD*)RvaToPtr(dwNameRva);
		return (char*)RvaToPtr(dwNameValueRva);
	}
	return NULL;
}

DWORD APExplort::GetFirstNameOrd()
{
	PIMAGE_EXPORT_DIRECTORY pExportDesc=GetExportTab();
	if (pExportDesc)
	{
		return pExportDesc->AddressOfNameOrdinals;
	}
	return 0;
}

DWORD APExplort::GetNameOrd(int nIndex)
{
	if (nIndex<GetNameCout())
	{
		DWORD dwFirstNameOrd =GetFirstNameOrd();
		return dwFirstNameOrd+nIndex*2;
	}
	return 0;
}

WORD APExplort::GetNameOrdValue(int nIndex)
{
	DWORD dwNameOrdRva =GetNameOrd(nIndex);
	if (dwNameOrdRva)
	{
		return *(WORD*)RvaToPtr(dwNameOrdRva);
	}
}