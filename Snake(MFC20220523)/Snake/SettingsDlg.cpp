// Settings.cpp : 实现文件
//

#include "stdafx.h"
#include "Snake.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"


// CSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTINGS, pParent)
	, m_nColumns(0)
	, m_nRows(0)
{
	
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_N_COLUMNS, m_nColumns);
	DDX_Text(pDX, IDC_N_ROWS, m_nRows);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序


void CSettingsDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	if (m_nColumns > _MAX_NUM_COLUMNS_)
	{
		CString strError;
		strError.Format(_T("超过最大列数%d"), _MAX_NUM_COLUMNS_);
		MessageBox(strError);
		return;
	}

	if (m_nRows > _MAX_NUM_ROWS_)
	{
		CString strError;
		strError.Format(_T("超过最大行数%d"), _MAX_NUM_ROWS_);
		MessageBox(strError);
		return;
	}
	
	CDialogEx::OnOK();
}

DWORD CSettingsDlg::getColumnNum()
{
	return m_nColumns;
}

DWORD CSettingsDlg::getRowNum()
{
	return m_nRows;
}

void CSettingsDlg::setColumnNum(DWORD columnNum)
{
	m_nColumns = columnNum;
}

void CSettingsDlg::setRowNum(DWORD rowNum)
{
	m_nRows = rowNum;
}


BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

