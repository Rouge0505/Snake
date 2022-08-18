#include "stdafx.h"
#include "SnakeGame.h"


CSnakeGame::CSnakeGame(DWORD nRows, DWORD nColumns)
{
	memset(this, 0, sizeof(*this));
	m_nRows = nRows;
	m_nColumns = nColumns;
	m_runInterval = 1000;
	m_lastMoment = clock();
}


CSnakeGame::~CSnakeGame()
{
	if (m_lineMap)
		delete m_lineMap;
}

BOOL CSnakeGame::init()
{
	m_mapSize = m_nRows * m_nColumns;
	if (!m_mapSize)
		return FALSE;

	m_lineMap = new BYTE[m_mapSize];
	for (int i = 0; i < m_mapSize; i++)		//����ͼ���������ɿռ�
		m_lineMap[i] = MAP_FREE_SPACE;
	m_map = (BYTE**)m_lineMap;				//ͬ�����ڴ�ռ䣬�ֱ�ͨ��һά����Ͷ�ά����ķ�ʽȥ����

	m_length = 1;												//��ʼ�߳���Ϊ1
	m_headPos.setCoordinates(m_nColumns / 2, m_nRows / 2);		//λ������
	m_map[m_headPos.x][m_headPos.y] = MAP_SNAKE_HEAD_RIGHT;		//��������

	srand(time(NULL));	//��ʼ�����������
	updateTargetPosition();	//�����ɿռ��������ȡĿ��λ��
	return TRUE;
}

void CSnakeGame::run()
{
	m_currentMoment = clock();
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

		updateTargetPosition();
	}
	else if (head.x >= m_nRows || head.y >= m_nColumns)	//ײǽ��xy�޷��ţ������-1�����255��Ҳ�ᳬ�����ֵ
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
}

DWORD CSnakeGame::planeCoordinates2Line(CCoordinates & coord)
{
	return coord.y * m_nColumns + coord.x;	//��(2, 0)ת����һά����Ӧ����2��(3, 1)����1 * m_nColumns + 3
}

void CSnakeGame::lineCoordinates2Plane(DWORD lineCoord, CCoordinates & coord)
{
	coord.x = lineCoord / m_nColumns;
	coord.y = lineCoord % m_nColumns;
}

void CSnakeGame::updateTargetPosition()
{
	DWORD freeSize = m_mapSize - m_length;	//���ɿռ������
	DWORD targetIndex = rand() % freeSize;	//����[0, freeSize - 1]���������������Ϊ���ɿռ�Ŀ������
	int count = -1;
	DWORD index = 0;
	while (count < targetIndex)
	{
		if (m_lineMap[index++] == MAP_FREE_SPACE)
			count++;
	}


	if (!freeSize)
		return;
	DWORD lineCoord = rand() % freeSize;	//����0��freeSize - 1���������������Ϊ���ɿռ�Ŀ������

	list<DWORD>::iterator iter = m_pFreeSpace->begin();
	advance(iter, lineCoord);
	lineCoord = *iter;	//Ŀ��һά����
	lineCoordinates2Plane(lineCoord, m_targetPos);	//Ŀ���ά����
	m_pFreeSpace->remove(lineCoord);	//���ɿռ�ȥ��Ŀ��λ��
	updateRunInterval();					//�������м��
}

void CSnakeGame::updateRunInterval()
{
	DWORD length = m_pSnakePosition->size();
	if (length > 10)
		m_runInterval = 300;
	else
		m_runInterval = 1000 - 70 * length;

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

	delete pGame;

	return 0;
}

BOOL CSnakeGame::hasTerminated()
{
	return m_status >= STATUS_COLLISION;
}

void CSnakeGame::forcedTerminate()
{
	m_status = STATUS_FORCED_STOP;
}

BOOL CSnakeGame::setDirection(DWORD direction)
{
	if (direction > DIRECTION_RIGHT)	//�Ƿ�����
		return FALSE;
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
