#pragma once


#include "APEBasic.h"


//�̳���APEFile�Դ�������ܽ�����չ

class APEImport : public APEFile
{
public:
	BOOL ReLocalImport(DWORD dwRVA);
	DWORD GetImportThunk(PIMAGE_IMPORT_DESCRIPTOR);
	void operator=(APEFile& lPeFile);
	PIMAGE_IMPORT_BY_NAME GetImportFucByName(DWORD RavThunk);	//�õ�ĳ��API����������
	PIMAGE_IMPORT_DESCRIPTOR GetImportDesc(int index);			//�õ�ĳ��dll�����
	int	GetImportDesCount();									//�õ����ж��ٸ�dll
	PIMAGE_IMPORT_DESCRIPTOR GetFirstImportDesc();				//�õ��׸������
	APEImport();
	virtual ~APEImport();

};


