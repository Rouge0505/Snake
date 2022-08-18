
// SnakeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Snake.h"
#include "SnakeDlg.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSnakeDlg 对话框



CSnakeDlg::CSnakeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SNAKE_DIALOG, pParent)
	, m_pSnakeGame(NULL)
	, m_testX(N_COLUMNS - 1)
	, m_testY(N_ROWS - 1)
	, m_bKeyDown(FALSE)
	//, m_bHasOnceStarted(FALSE)
	, m_nColumns(_MAX_NUM_COLUMNS_)
	, m_nRows(_MAX_NUM_ROWS_)
	, m_xDiffWndClient(0)
	, m_yDiffWndClient(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), _T("Snake.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

void CSnakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DRAW_SNAKE, handleDrawGame)
	ON_MESSAGE(WM_GAME_OVER, handleGameOver)
	ON_MESSAGE(WM_FULL_SPEED, handleFullSpeed)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSnakeDlg 消息处理程序

BOOL CSnakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//int a = sizeof(CCoordinates);
	//a = sizeof(CSnakeGame);
	//CLOCKS_PER_SEC;
	//CRect rect(0, 0, N_COLUMNS * BLOCK_WIDTH, N_ROWS * BLOCK_WIDTH);	//l t r b
	//CRect rect(0, 0, N_COLUMNS * BLOCK_WIDTH + 100, N_ROWS * BLOCK_WIDTH + 100);	//l t r b

	//CRect rect(0, 0, N_COLUMNS * BLOCK_WIDTH + 18, N_ROWS * BLOCK_WIDTH + 40);	//l t r b
	//
	//GetWindowRect(&rect);

	//CRect rect1;
	//GetClientRect(&rect1);
	//m_xDiffWndClient = rect.right - rect1.right;
	//m_yDiffWndClient = rect.bottom - rect1.bottom;
	//rect.SetRect(0, 0, N_COLUMNS * BLOCK_WIDTH + m_xDiffWndClient, N_ROWS * BLOCK_WIDTH + m_yDiffWndClient);
	//MoveWindow(&rect);

	updateSize();
	m_bgm.replay(_T("lazy.mp3"));
	//m_fastBgm.load(_T("百石元 - 猪突猛進 (勇往直前).mp3"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnakeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//if (!m_pSnakeGame)
		//	return;
		
		
		//drawRange(&dc);
		//drawBlock(&dc, m_testX, m_testY);
		//drawBlock(&dc, 8, 12);
		
		CPaintDC dc(this); // 用于绘制的设备上下文
		//CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		//CBrush* pOldBrush = dc.SelectObject(pBrush);

		//static int test = 0;
		//test++;
		//if (m_pSnakeGame != NULL)
		//{
		//	
		//	drawBlock(&dc, 12, 7);
		//}
		
		if (m_pSnakeGame)
		{
			
			//m_pSnakeGame->lock();
			const list<CCoordinates>* pSnake = m_pSnakeGame->getSnakePosition();
			for (list<CCoordinates>::const_iterator iter = pSnake->begin(); iter != pSnake->end(); iter++)
			{
				const CCoordinates& coord = *iter;
				drawBlock(&dc, coord.x, coord.y);
			}
			//m_pSnakeGame->unlock();

			const CCoordinates* pTarget = m_pSnakeGame->getTargetPosition();
			drawCircle(&dc, pTarget->x, pTarget->y);
		}

		//dc.SelectObject(pOldBrush);
		
		//CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSnakeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSnakeDlg::drawBlock(CPaintDC* pDc, DWORD x, DWORD y)
{
	CRect rect(x * BLOCK_WIDTH, y * BLOCK_WIDTH, (x + 1) * BLOCK_WIDTH, (y + 1) * BLOCK_WIDTH);	//l t r b
	pDc->Rectangle(&rect);
	//pDc->FillRect(&rect, NULL);
}

void CSnakeDlg::drawCircle(CPaintDC * pDc, DWORD x, DWORD y)
{
	CRect rect(x * BLOCK_WIDTH, y * BLOCK_WIDTH, (x + 1) * BLOCK_WIDTH, (y + 1) * BLOCK_WIDTH);	//l t r b
	pDc->Ellipse(&rect);
}

void CSnakeDlg::drawRange(CPaintDC * pDc)
{
	CRect rect(0, 0, N_COLUMNS * BLOCK_WIDTH, N_ROWS * BLOCK_WIDTH);
	//pDc->FillRect(&rect, NULL);
	pDc->Rectangle(&rect);
}



BOOL CSnakeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)
		return TRUE;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (m_bKeyDown)	//保证只响应一次KEYDOWN
			return TRUE;
		m_bKeyDown = TRUE;

		switch (pMsg->wParam)
		{
		case 'S':			//开始
			//if (GetKeyState(VK_CONTROL) & 0x8000)	//不会响应组合键，因为有m_bKeyDown标识
			//{
			//	int a = 0;
			//}
			if (!m_pSnakeGame)
			{
				m_pSnakeGame = new CSnakeGame(m_nRows, m_nColumns);
				AfxBeginThread(CSnakeGame::snakeGameProc, m_pSnakeGame);
				//m_bHasOnceStarted = TRUE;
			}
			break;
		case 'P':			//暂停
			if (m_pSnakeGame)
			{
				m_pSnakeGame->setPause();
				CString captain;
				GetWindowText(captain);
				if (captain == _T("Snake"))
					SetWindowText(_T("Snake(暂停中)"));
				else
					SetWindowText(_T("Snake"));
			}
				
			break;
		case 'E':			//停止
			if (m_pSnakeGame)
			{
				m_pSnakeGame->forcedTerminate();
				m_pSnakeGame = NULL;
			}
			break;
		case 'R':			//设置
		{
			if (!m_pSnakeGame)	//没在游戏中才允许设置
			{
				CSettingsDlg dlg;
				dlg.setColumnNum(m_nColumns);
				dlg.setRowNum(m_nRows);
				if (IDOK == dlg.DoModal())
				{
					m_nColumns = dlg.getColumnNum();
					m_nRows = dlg.getRowNum();
					updateSize();
					Invalidate();
					//PostMessage(WM_ERASEBKGND,)
				}
				m_bKeyDown = FALSE;
				return TRUE;
			}
			
		}
			break;
		case VK_UP:
			if (m_pSnakeGame)
				m_pSnakeGame->setDirection(DIRECTION_UP);
			break;
		case VK_DOWN:
			if (m_pSnakeGame)
				m_pSnakeGame->setDirection(DIRECTION_DOWN);
			break;
		case VK_LEFT:
			if (m_pSnakeGame)
				m_pSnakeGame->setDirection(DIRECTION_LEFT);
			break;
		case VK_RIGHT:
			if (m_pSnakeGame)
				m_pSnakeGame->setDirection(DIRECTION_RIGHT);
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		m_bKeyDown = FALSE;
		break;
	default:
		break;
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CSnakeDlg::handleDrawGame(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return LRESULT();
}

LRESULT CSnakeDlg::handleGameOver(WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = { 0 };
	TCHAR restartMsg[32] = _T("按S键重新开始游戏！");
	switch (wParam)
	{
	case STATUS_COLLISION:
		_stprintf(msg, _T("撞墙死亡！%s"), restartMsg);
		break;
	case STATUS_BITE_ONESELF:
		_stprintf(msg, _T("咬到自己！%s"), restartMsg);
		break;
	case STATUS_FORCED_STOP:
		_stprintf(msg, _T("强制停止！%s"), restartMsg);
		break;
	case STATUS_WIN:
		_stprintf(msg, _T("您获胜了！%s"), restartMsg);
		break;
	default:
		break;
	}
	if (wParam != STATUS_CLOSE_DLG)	//直接关闭对话框就不弹框了
	{
		
		if (lParam)		//全速了
		{
			//m_bgm.stop();
			//AfxBeginThread(gameOverMusicProc, this);
			//m_bgm.replay(_T("佐藤直纪 - Okamochi & Jersey.mp3"));
			m_bgm.replay(_T("lazy.mp3"));
			//m_fastBgm.stop();
			//m_slowBgm.replay();
		}
			
		MessageBox(msg, _T("提示"), NULL);	//神奇的是，MessageBox阻塞期间，居然可以进OnPaint函数
	}
		
	//MessageBox(_T("游戏结束！"));
	m_pSnakeGame = NULL;
	return LRESULT();
}

LRESULT CSnakeDlg::handleFullSpeed(WPARAM wParam, LPARAM lParam)
{
	/*m_slowBgm.stop();
	m_fastBgm.replay();*/
	//m_bgm.replay(_T("百石元 - 猪突猛進 (勇往直前).mp3"));
	m_bgm.replay(_T("excited.mp3"));
	/*m_bgm.stop();
	AfxBeginThread(fullSpeedMusicProc, this);*/
	return LRESULT();
}


BOOL CSnakeDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/
	if (m_pSnakeGame)	//只有游戏进行中，需要擦除重绘
		return CDialogEx::OnEraseBkgnd(pDC);
	//if (m_bHasOnceStarted)	//只有游戏没开始过，欢迎界面需要画背景图	//注释掉这行代码后，死亡后将对话框拖出屏幕再拖回来会重绘欢迎界面233 20220819
	//	return TRUE;
		//return CDialogEx::OnEraseBkgnd(pDC);


	/*static int count = 0;
	if (count)
		return TRUE;
	count++;*/


	CBitmap bitmap;
	bitmap.DeleteObject();
	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), _T("SnakeWelcome.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BOOL bSuccess = bitmap.Attach(hBitmap);
	//bitmap.LoadBitmap(IDB_SNAKE_WELCOME);
	//bitmap.LoadBitmap(_T("E:/VS2015 Projects/Snake(MFC)/Snake/res/SnakeWelcome.bmp"));

	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	//CPaintDC dc(this);
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(&bitmap);

	CRect rect;
	GetClientRect(&rect);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	CFont font;
	font.CreatePointFont(200, _T("华文行楷"), NULL);
	CFont* pOldFont = pDC->SelectObject(&font);
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.08, CString(_T("S键：开始游戏")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.16, CString(_T("P键：暂停游戏")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.24, CString(_T("E键：停止游戏")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.32, CString(_T("R键：设置尺寸")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.4, CString(_T("上下左右控制方向")));
	pDC->SelectObject(pOldFont);

	return TRUE;
}

void CSnakeDlg::updateSize()
{
	CRect wndRect;	//l t r b
	GetWindowRect(&wndRect);	//初始化完之前窗口左上角为原点，之后屏幕左上角为原点
	if (!m_xDiffWndClient || !m_yDiffWndClient)	//第一次进函数，先获取窗口大小和客户区大小之差
	{
		CRect clntRect;
		GetClientRect(&clntRect);
		m_xDiffWndClient = wndRect.Width() - clntRect.Width();
		m_yDiffWndClient = wndRect.Height() - clntRect.Height();
	}
	wndRect.right = wndRect.left + m_nColumns * BLOCK_WIDTH + m_xDiffWndClient;
	wndRect.bottom = wndRect.top + m_nRows * BLOCK_WIDTH + m_yDiffWndClient;
	//wndRect.SetRect(0, 0, N_COLUMNS * BLOCK_WIDTH + m_xDiffWndClient, N_ROWS * BLOCK_WIDTH + m_yDiffWndClient);
	MoveWindow(&wndRect);

}


void CSnakeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//理论上应该加判断，禁止重复开启线程，但实际上点击关闭的瞬间对话框就关了，所以不加这个判断了
	if (m_pSnakeGame)
	{
		m_pSnakeGame->forcedTerminate(TRUE);
		AfxBeginThread(closeProc, this);
		return;
		//while (m_pSnakeGame);
	}

	CDialogEx::OnClose();
}

UINT CSnakeDlg::closeProc(LPVOID lPvoid)
{
	CSnakeDlg* pDlg = (CSnakeDlg*)lPvoid;
	//volatile void* ptr = pDlg->m_pSnakeGame;	//volatile莫名失效，干脆直接写汇编
	//while (ptr = pDlg->m_pSnakeGame);
	void* p = &pDlg->m_pSnakeGame;
	_asm	//等待m_pSnakeGame清零，表示堆内存释放完啦
	{
	label_cmp:
		mov eax, dword ptr ds : [p];
		mov eax, dword ptr ds : [eax];
		test eax, eax;
		jne label_cmp;
	}
	
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}

UINT CSnakeDlg::fullSpeedMusicProc(LPVOID lPvoid)
{
	CSnakeDlg* pDlg = (CSnakeDlg*)lPvoid;
	pDlg->m_bgm.replay(_T("excited.mp3"));
	return 0;
}

UINT CSnakeDlg::gameOverMusicProc(LPVOID lPvoid)
{
	CSnakeDlg* pDlg = (CSnakeDlg*)lPvoid;
	pDlg->m_bgm.replay(_T("lazy.mp3"));
	return 0;
}
