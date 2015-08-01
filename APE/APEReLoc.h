#pragma  once



#include "APEBasic.h"

//继承自APEFile 对处理重定位功能进行扩展
class APEReLoc : public APEFile
{
	BOOL RelocVA(DWORD dwNewAddress, DWORD dwNewImageBase, DWORD &dwOldVA);
	BOOL RelocMySelf(DWORD dwNewSecAddress,DWORD dwImageBase);
	DWORD GetReLocRVA(int nRelocIndex,int nTypeOffsetIndex);
	WORD  GetTypeOffset(int nRelocIndex,int nOffsetIndex);
	WORD  GetReLocType(int nRelocIndex,int nOffsetIndex);
	int   GetTypeOffsetCount(int nIndex);
	PIMAGE_BASE_RELOCATION GetReLoc(int nCount,DWORD *rdwOffset);
	int GetReLocCount();
	PIMAGE_BASE_RELOCATION GetFirstReloc();
	APEReLoc();
	virtual ~APEReLoc();
	void operator = (APEFile& lPeFile);

};