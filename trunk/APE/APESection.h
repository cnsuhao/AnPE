#include "APEBasic.h"

#include "APEBasic.h"
//�̳�APEBasic�л��������ܲ��Խڴ��������չ.
class APESection : public APEFile
{
public:
	BOOL WriterSectionData(int nIndex,DWORD dwOffset,BYTE* lpBuffer,DWORD *dwSize);//д�������
	BYTE* GetSectionData(int nIndex,DWORD* rdwSize);							   //�õ�������
	BOOL AddSection(char* pSectinName,DWORD &dwSectionSize,DWORD dwSectionStact);  //���һ����
	DWORD OffsetToRva(DWORD dwOffset);											
	DWORD RvaToOffset(DWORD dwRva);
	DWORD GetVRK(int nSectionIndex);
	int OffsetToSectionIndex(DWORD dwOffset);
	int RvaToSectionIndex(DWORD dwRva);
	APESection();
	virtual ~APESection();
	void operator =(APEFile& lPeFile);

};