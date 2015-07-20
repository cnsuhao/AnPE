#pragma once


#include "APEBasic.h"


//继承自APEFile对处理导入表功能进行扩展

class APEImport : public APEFile
{
public:
	BOOL ReLocalImport(DWORD dwRVA);
	DWORD GetImportThunk(PIMAGE_IMPORT_DESCRIPTOR);
	void operator=(APEFile& lPeFile);
	PIMAGE_IMPORT_BY_NAME GetImportFucByName(DWORD RavThunk);	//得到某个API函数的名字
	PIMAGE_IMPORT_DESCRIPTOR GetImportDesc(int index);			//得到某个dll导入表
	int	GetImportDesCount();									//得到共有多少个dll
	PIMAGE_IMPORT_DESCRIPTOR GetFirstImportDesc();				//得到首个导入表
	APEImport();
	virtual ~APEImport();

};


