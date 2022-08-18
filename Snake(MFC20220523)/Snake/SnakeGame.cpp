#include "stdafx.h"
#include "SnakeGame.h"


CSnakeGame::CSnakeGame(DWORD nRows, DWORD nColumns)
{
	memset(this, 0, sizeof(*this));
	m_nRows = nRows;
	m_nColumns = nColumns;
	m_pSnakePosition = new list<CCoordinates>;
	m_pFreeSpace = new list<DWORD>;
	m_runInterval = MAX_RUN_INTERVAL;
	m_lastMoment = clock();
	m_direction = DIRECTION_RIGHT;
}


CSnakeGame::~CSnakeGame()
{
	if (m_pSnakePosition)
		delete m_pSnakePosition;
	if (m_pFreeSpace)
		delete m_pFreeSpace;
}

BOOL CSnakeGame::init()
{
	if (!m_nRows || !m_nRows)
		return FALSE;
	CCoordinates initPos(m_nColumns / 2, m_nRows / 2);
	m_pSnakePosition->push_back(initPos);	//��ʼ�߳���Ϊ1��λ������

	DWORD lineCoord = planeCoordinates2Line(initPos);	//�߳�ʼλ��һά����

	for (int i = 0; i < m_nRows * m_nColumns; i++)	//���߳�ʼλ��֮��Ϊ���ɿռ�
		if (i != lineCoord)
			m_pFreeSpace->push_back(i);

	srand(time(NULL));	//��ʼ�����������
	updateTargetPosition();	//�����ɿռ��������ȡĿ��λ��
	return TRUE;
}

void CSnakeGame::run()
{
	m_currentMoment = clock();
	if (m_status == STATUS_RUNNING)	//û����ͣ�Ÿ���ʱ��
		m_runTimeSaufPause += (m_currentMoment - m_lastMoment);
	m_lastMoment = m_currentMoment;
	if (m_runTimeSaufPause - m_lastRunMoment <= m_runInterval)	//û�����м��������
		return;

	m_lastRunMoment = m_runTimeSaufPause;

	list<CCoordinates>::iterator iter = m_pSnakePosition->begin();	//��ͷ
	CCoordinates head = *iter;
	switch (m_direction)
	{
	case DIRECTION_UP:
		head.y--;
		break;
	case DIRECTION_DOWN:
		head.y++;
		break;
	case DIRECTION_LEFT:
		head.x--;
		break;
	case DIRECTION_RIGHT:
		head.x++;
		break;
	default:
		break;
	}

	
	if (head == m_targetPos)	//�Ե�ʳ���ͷ���ӣ�Ŀ����ѡ�����ɿռ䲻��
	{
		m_pSnakePosition->push_front(m_targetPos);

		updateTargetPosition();					//����Ŀ��λ��
		updateRunInterval();					//�������м��
	}
	else if (head.x >= m_nColumns || head.y >= m_nRows)	//ײǽ��xy�޷��ţ������-1�����255��Ҳ�ᳬ�����ֵ
		m_status = STATUS_COLLISION;
	else  //û�Ե�ʳ��Ҳûײǽ����ͷ������β���٣�Ҫ�ж��Ƿ�ҧ���Լ������ɿռ����߷������
	{
		CCoordinates tail = m_pSnakePosition->back();	//�ó���β
		m_pSnakePosition->pop_back();		//��βɾ��
		list<CCoordinates>::iterator iter = find(m_pSnakePosition->begin(), m_pSnakePosition->end(), head);
		if (iter != m_pSnakePosition->end())	//�µ�ͷ������ĳһ���غϣ���ҧ���Լ�
		{
			m_status = STATUS_BITE_ONESELF;
			return;
		}
		m_pSnakePosition->push_front(head);	//��ͷ����
		DWORD headLineCoord = planeCoordinates2Line(head);	//ͷһά����
		DWORD tailLineCoord = planeCoordinates2Line(tail);	//βһά����
		m_pFreeSpace->remove(headLineCoord);		//���ɿռ�ɾ���µ�ͷ�����Ӿɵ�β
		m_pFreeSpace->push_back(tailLineCoord);

	}
	m_changeDirectionForbidden = FALSE;	//һ��������º������ٴθ�������

	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_DRAW_SNAKE, 0, 0);
}

DWORD CSnakeGame::planeCoordinates2Line(CCoordinates & coord)
{
	return coord.y * m_nColumns + coord.x;	//��(2, 0)ת����һά����Ӧ����2��(3, 1)����1 * m_nColumns + 3
}

void CSnakeGame::lineCoordinates2Plane(DWORD lineCoord, CCoordinates & coord)
{
	coord.y = lineCoord / m_nColumns;
	coord.x = lineCoord % m_nColumns;
}

void CSnakeGame::updateTargetPosition()
{
	DWORD freeSize = m_pFreeSpace->size();	//���ɿռ������
	if (!freeSize)
	{
		m_status = STATUS_WIN;	//���ɸ���û�ˣ�������ƽ�棬ʤ��
		return;
	}
	
	DWORD lineCoord = rand() % freeSize;	//����0��freeSize - 1���������������Ϊ���ɿռ�Ŀ������

	list<DWORD>::iterator iter = m_pFreeSpace->begin();
	advance(iter, lineCoord);
	lineCoord = *iter;	//Ŀ��һά����
	lineCoordinates2Plane(lineCoord, m_targetPos);	//Ŀ���ά����
	m_pFreeSpace->remove(lineCoord);	//���ɿռ�ȥ��Ŀ��λ��
}

void CSnakeGame::updateRunInterval()
{
	m_runInterval -= RUN_INTERVAL_DESCEND;
	if (m_runInterval < MIN_RUN_INTERVAL)
		m_runInterval = MIN_RUN_INTERVAL;
	if (!m_bFullspeed)
	{
		if (m_runInterval == MIN_RUN_INTERVAL)
		{
			m_bFullspeed = TRUE;
			SendMessage(AfxGetMainWnd()->m_hWnd, WM_FULL_SPEED, 0, 0);
		}
	}
	//DWORD length = m_pSnakePosition->size();
	//if (length > 10)
	//	m_runInterval = 300;
	//else
	//	m_runInterval = 1000 - 70 * length;

}

void CSnakeGame::showResult()
{
	TCHAR msg[64] = { 0 };
	TCHAR restartMsg[32] = _T("��S�����¿�ʼ��Ϸ��");
	switch (m_status)
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
	MessageBox(NULL, msg, _T("��ʾ"), NULL);
}

UINT CSnakeGame::snakeGameProc(LPVOID lPvoid)
{
	CSnakeGame* pGame = (CSnakeGame*)lPvoid;

	if (!pGame->init())
	{
		delete pGame;	//��ʼ��ʧ��
		return 1;
	}

	while (!pGame->hasTerminated())	//!3���ȼ���->2���ȼ�
	{
		pGame->run();
	}

	SendMessage(AfxGetMainWnd()->m_hWnd, WM_GAME_OVER, pGame->m_status, pGame->m_bFullspeed);
	//pGame->showResult();

	delete pGame;

	return 0;
}

BOOL CSnakeGame::hasTerminated()
{
	return m_status >= STATUS_COLLISION;
}

void CSnakeGame::forcedTerminate(BOOL bCloseDlg)
{
	if (bCloseDlg)
		m_status = STATUS_CLOSE_DLG;
	else
		m_status = STATUS_FORCED_STOP;
}

BOOL CSnakeGame::setDirection(DWORD direction)
{
	if (direction > DIRECTION_RIGHT)	//�Ƿ�����
		return FALSE;
	if (m_changeDirectionForbidden)		//��ֹ�任����
		return FALSE;
	m_changeDirectionForbidden = TRUE;
	switch (m_direction)
	{
	case DIRECTION_UP:
		if (direction == DIRECTION_DOWN)
			return FALSE;
		break;
	case DIRECTION_DOWN:
		if (direction == DIRECTION_UP)
			return FALSE;
		break;
	case DIRECTION_LEFT:
		if (direction == DIRECTION_RIGHT)
			return FALSE;
		break;
	case DIRECTION_RIGHT:
		if (direction == DIRECTION_LEFT)
			return FALSE;
		break;
	default:
		return FALSE;
	}
	m_direction = direction;
	return 0;
}

void CSnakeGame::setPause()
{
	if (m_status == STATUS_PAUSE)
		m_status = STATUS_RUNNING;
	else if (m_status == STATUS_RUNNING)
		m_status = STATUS_PAUSE;
}

const list<CCoordinates>* CSnakeGame::getSnakePosition()
{
	return m_pSnakePosition;
}

const CCoordinates * CSnakeGame::getTargetPosition()
{
	return &m_targetPos;
}
