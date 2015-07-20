#include "APEBasic.h"

#include "APEBasic.h"
//继承APEBasic中基础处理功能并对节处理进行扩展.
class APESection : public APEFile
{
public:
	BOOL WriterSectionData(int nIndex,DWORD dwOffset,BYTE* lpBuffer,DWORD *dwSize);//写入节数据
	BYTE* GetSectionData(int nIndex,DWORD* rdwSize);							   //得到节数据
	BOOL AddSection(char* pSectinName,DWORD &dwSectionSize,DWORD dwSectionStact);  //添加一个节
	DWORD OffsetToRva(DWORD dwOffset);											
	DWORD RvaToOffset(DWORD dwRva);
	DWORD GetVRK(int nSectionIndex);
	int OffsetToSectionIndex(DWORD dwOffset);
	int RvaToSectionIndex(DWORD dwRva);
	APESection();
	virtual ~APESection();
	void operator =(APEFile& lPeFile);

};