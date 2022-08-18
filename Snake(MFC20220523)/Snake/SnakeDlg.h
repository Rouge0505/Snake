
// SnakeDlg.h : 头文件
//

#pragma once
#include "SnakeGame.h"
#include "BackgroundMusic.h"

#define	BLOCK_WIDTH		40	//蛇一个格子的宽度
#define N_ROWS			16	//行数
#define N_COLUMNS		24	//列数

// CSnakeDlg 对话框
class CSnakeDlg : public CDialogEx
{
// 构造
public:
	CSnakeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CSnakeGame* m_pSnakeGame;

	DWORD m_testX;	//测试用，方块X坐标
	DWORD m_testY;	//测试用，方块Y坐标
	BOOL m_bKeyDown;	//初始为FALSE，按下按键时置TRUE，弹起时置FALSE

	void drawBlock(CPaintDC* pDc, DWORD x, DWORD y);	//画蛇坐标为(x,y)的一个格子
	void drawCircle(CPaintDC* pDc, DWORD x, DWORD y);	//画蛇坐标为(x,y)的一个圆
	void drawRange(CPaintDC* pDc);						//画出蛇的运行范围
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT handleDrawGame(WPARAM wParam, LPARAM lParam);
	LRESULT handleGameOver(WPARAM wParam, LPARAM lParam);
	LRESULT handleFullSpeed(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	//BOOL m_bHasOnceStarted;	//已开始过
	DWORD m_nColumns;	//游戏列数
	DWORD m_nRows;		//游戏行数
	DWORD m_xDiffWndClient;	//窗口大小和客户区大小之间的差x
	DWORD m_yDiffWndClient;	//窗口大小和客户区大小之间的差y

	void updateSize();		//更新窗口大小
public:
	afx_msg void OnClose();
	static UINT closeProc(LPVOID lPvoid);	//关闭对话框，等待堆内存释放
private:
	/*CBackgroundMusic m_slowBgm;
	CBackgroundMusic m_fastBgm;*/
	CBackgroundMusic m_bgm;
	static UINT fullSpeedMusicProc(LPVOID lPvoid);
	static UINT gameOverMusicProc(LPVOID lPvoid);
};
