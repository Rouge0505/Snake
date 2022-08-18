
// SnakeDlg.h : ͷ�ļ�
//

#pragma once
#include "SnakeGame.h"
#include "BackgroundMusic.h"

#define	BLOCK_WIDTH		40	//��һ�����ӵĿ��
#define N_ROWS			16	//����
#define N_COLUMNS		24	//����

// CSnakeDlg �Ի���
class CSnakeDlg : public CDialogEx
{
// ����
public:
	CSnakeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CSnakeGame* m_pSnakeGame;

	DWORD m_testX;	//�����ã�����X����
	DWORD m_testY;	//�����ã�����Y����
	BOOL m_bKeyDown;	//��ʼΪFALSE�����°���ʱ��TRUE������ʱ��FALSE

	void drawBlock(CPaintDC* pDc, DWORD x, DWORD y);	//��������Ϊ(x,y)��һ������
	void drawCircle(CPaintDC* pDc, DWORD x, DWORD y);	//��������Ϊ(x,y)��һ��Բ
	void drawRange(CPaintDC* pDc);						//�����ߵ����з�Χ
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT handleDrawGame(WPARAM wParam, LPARAM lParam);
	LRESULT handleGameOver(WPARAM wParam, LPARAM lParam);
	LRESULT handleFullSpeed(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	//BOOL m_bHasOnceStarted;	//�ѿ�ʼ��
	DWORD m_nColumns;	//��Ϸ����
	DWORD m_nRows;		//��Ϸ����
	DWORD m_xDiffWndClient;	//���ڴ�С�Ϳͻ�����С֮��Ĳ�x
	DWORD m_yDiffWndClient;	//���ڴ�С�Ϳͻ�����С֮��Ĳ�y

	void updateSize();		//���´��ڴ�С
public:
	afx_msg void OnClose();
	static UINT closeProc(LPVOID lPvoid);	//�رնԻ��򣬵ȴ����ڴ��ͷ�
private:
	/*CBackgroundMusic m_slowBgm;
	CBackgroundMusic m_fastBgm;*/
	CBackgroundMusic m_bgm;
	static UINT fullSpeedMusicProc(LPVOID lPvoid);
	static UINT gameOverMusicProc(LPVOID lPvoid);
};
