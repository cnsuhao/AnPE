#include "APEBasic.h"

//�̳���APEBasic�Դ��������ܽ�����չ
class APExplort : public APEFile
{
public:
	WORD	GetNameOrdValue(int nIndex);
	DWORD	GetNameOrd(int nIndex);
	DWORD	GetFirstNameOrd();
	char*	GetNameValue(int nIndex);
	DWORD	GetName(int nIndex);
	DWORD	GetFirstName();
	DWORD	GetFuncValue(int nIndex);
	DWORD	GetFunc(int nIndex);
	DWORD	GetFirstFunc();
	int		GetNameCout();
	int		GetFuncCout();
	PIMAGE_EXPORT_DIRECTORY	GetExportTab();
	APExplort();
	virtual ~APExplort();
	void	operator =(APEFile& lPeFile);


};