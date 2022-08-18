
// SnakeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Snake.h"
#include "SnakeDlg.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSnakeDlg �Ի���



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


// CSnakeDlg ��Ϣ�������

BOOL CSnakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	//m_fastBgm.load(_T("��ʯԪ - ��ͻ���M (����ֱǰ).mp3"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//if (!m_pSnakeGame)
		//	return;
		
		
		//drawRange(&dc);
		//drawBlock(&dc, m_testX, m_testY);
		//drawBlock(&dc, 8, 12);
		
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ����ר�ô����/
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)
		return TRUE;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (m_bKeyDown)	//��ֻ֤��Ӧһ��KEYDOWN
			return TRUE;
		m_bKeyDown = TRUE;

		switch (pMsg->wParam)
		{
		case 'S':			//��ʼ
			//if (GetKeyState(VK_CONTROL) & 0x8000)	//������Ӧ��ϼ�����Ϊ��m_bKeyDown��ʶ
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
		case 'P':			//��ͣ
			if (m_pSnakeGame)
			{
				m_pSnakeGame->setPause();
				CString captain;
				GetWindowText(captain);
				if (captain == _T("Snake"))
					SetWindowText(_T("Snake(��ͣ��)"));
				else
					SetWindowText(_T("Snake"));
			}
				
			break;
		case 'E':			//ֹͣ
			if (m_pSnakeGame)
			{
				m_pSnakeGame->forcedTerminate();
				m_pSnakeGame = NULL;
			}
			break;
		case 'R':			//����
		{
			if (!m_pSnakeGame)	//û����Ϸ�в���������
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
	TCHAR restartMsg[32] = _T("��S�����¿�ʼ��Ϸ��");
	switch (wParam)
	{
	case STATUS_COLLISION:
		_stprintf(msg, _T("ײǽ������%s"), restartMsg);
		break;
	case STATUS_BITE_ONESELF:
		_stprintf(msg, _T("ҧ���Լ���%s"), restartMsg);
		break;
	case STATUS_FORCED_STOP:
		_stprintf(msg, _T("ǿ��ֹͣ��%s"), restartMsg);
		break;
	case STATUS_WIN:
		_stprintf(msg, _T("����ʤ�ˣ�%s"), restartMsg);
		break;
	default:
		break;
	}
	if (wParam != STATUS_CLOSE_DLG)	//ֱ�ӹرնԻ���Ͳ�������
	{
		
		if (lParam)		//ȫ����
		{
			//m_bgm.stop();
			//AfxBeginThread(gameOverMusicProc, this);
			//m_bgm.replay(_T("����ֱ�� - Okamochi & Jersey.mp3"));
			m_bgm.replay(_T("lazy.mp3"));
			//m_fastBgm.stop();
			//m_slowBgm.replay();
		}
			
		MessageBox(msg, _T("��ʾ"), NULL);	//������ǣ�MessageBox�����ڼ䣬��Ȼ���Խ�OnPaint����
	}
		
	//MessageBox(_T("��Ϸ������"));
	m_pSnakeGame = NULL;
	return LRESULT();
}

LRESULT CSnakeDlg::handleFullSpeed(WPARAM wParam, LPARAM lParam)
{
	/*m_slowBgm.stop();
	m_fastBgm.replay();*/
	//m_bgm.replay(_T("��ʯԪ - ��ͻ���M (����ֱǰ).mp3"));
	m_bgm.replay(_T("excited.mp3"));
	/*m_bgm.stop();
	AfxBeginThread(fullSpeedMusicProc, this);*/
	return LRESULT();
}


BOOL CSnakeDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/
	if (m_pSnakeGame)	//ֻ����Ϸ�����У���Ҫ�����ػ�
		return CDialogEx::OnEraseBkgnd(pDC);
	//if (m_bHasOnceStarted)	//ֻ����Ϸû��ʼ������ӭ������Ҫ������ͼ	//ע�͵����д���������󽫶Ի����ϳ���Ļ���ϻ������ػ滶ӭ����233 20220819
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
	font.CreatePointFont(200, _T("�����п�"), NULL);
	CFont* pOldFont = pDC->SelectObject(&font);
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.08, CString(_T("S������ʼ��Ϸ")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.16, CString(_T("P������ͣ��Ϸ")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.24, CString(_T("E����ֹͣ��Ϸ")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.32, CString(_T("R�������óߴ�")));
	pDC->TextOut(rect.Width() * 0.55, rect.Height() * 0.4, CString(_T("�������ҿ��Ʒ���")));
	pDC->SelectObject(pOldFont);

	return TRUE;
}

void CSnakeDlg::updateSize()
{
	CRect wndRect;	//l t r b
	GetWindowRect(&wndRect);	//��ʼ����֮ǰ�������Ͻ�Ϊԭ�㣬֮����Ļ���Ͻ�Ϊԭ��
	if (!m_xDiffWndClient || !m_yDiffWndClient)	//��һ�ν��������Ȼ�ȡ���ڴ�С�Ϳͻ�����С֮��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//������Ӧ�ü��жϣ���ֹ�ظ������̣߳���ʵ���ϵ���رյ�˲��Ի���͹��ˣ����Բ�������ж���
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
	//volatile void* ptr = pDlg->m_pSnakeGame;	//volatileĪ��ʧЧ���ɴ�ֱ��д���
	//while (ptr = pDlg->m_pSnakeGame);
	void* p = &pDlg->m_pSnakeGame;
	_asm	//�ȴ�m_pSnakeGame���㣬��ʾ���ڴ��ͷ�����
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
