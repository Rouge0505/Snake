#pragma once


// CSettingsDlg 对话框

#define _MAX_NUM_ROWS_			16	//最大行数
#define _MAX_NUM_COLUMNS_		24	//最大列数

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	DWORD m_nColumns;	//游戏列数
	DWORD m_nRows;		//游戏行数
	virtual void OnOK();

public:
	DWORD getColumnNum();
	DWORD getRowNum();
	void setColumnNum(DWORD columnNum);
	void setRowNum(DWORD rowNum);
	virtual BOOL OnInitDialog();
};
