#pragma once


// CSettingsDlg �Ի���

#define _MAX_NUM_ROWS_			16	//�������
#define _MAX_NUM_COLUMNS_		24	//�������

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	DWORD m_nColumns;	//��Ϸ����
	DWORD m_nRows;		//��Ϸ����
	virtual void OnOK();

public:
	DWORD getColumnNum();
	DWORD getRowNum();
	void setColumnNum(DWORD columnNum);
	void setRowNum(DWORD rowNum);
	virtual BOOL OnInitDialog();
};
