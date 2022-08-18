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
	for (int i = 0; i < m_mapSize; i++)		//将地图都填充成自由空间
		m_lineMap[i] = MAP_FREE_SPACE;
	m_map = (BYTE**)m_lineMap;				//同样的内存空间，分别通过一维数组和二维数组的方式去索引

	m_length = 1;												//初始蛇长度为1
	m_headPos.setCoordinates(m_nColumns / 2, m_nRows / 2);		//位于中心
	m_map[m_headPos.x][m_headPos.y] = MAP_SNAKE_HEAD_RIGHT;		//方向向右

	srand(time(NULL));	//初始化随机数种子
	updateTargetPosition();	//在自由空间内随机获取目标位置
	return TRUE;
}

void CSnakeGame::run()
{
	m_currentMoment = clock();
	m_runTimeSaufPause += (m_currentMoment - m_lastMoment);
	m_lastMoment = m_currentMoment;
	if (m_runTimeSaufPause - m_lastRunMoment <= m_runInterval)	//没到运行间隔，返回
		return;

	m_lastRunMoment = m_runTimeSaufPause;

	list<CCoordinates>::iterator iter = m_pSnakePosition->begin();	//蛇头
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

	if (head == m_targetPos)	//吃到食物，蛇头增加，目标重选，自由空间不变
	{
		m_pSnakePosition->push_front(m_targetPos);

		updateTargetPosition();
	}
	else if (head.x >= m_nRows || head.y >= m_nColumns)	//撞墙，xy无符号，如果是-1则会是255，也会超过最大值
		m_status = STATUS_COLLISION;
	else  //没吃到食物也没撞墙，蛇头增加蛇尾减少，要判断是否咬到自己；自由空间与蛇反向更新
	{
		CCoordinates tail = m_pSnakePosition->back();	//拿出蛇尾
		m_pSnakePosition->pop_back();		//蛇尾删除
		list<CCoordinates>::iterator iter = find(m_pSnakePosition->begin(), m_pSnakePosition->end(), head);
		if (iter != m_pSnakePosition->end())	//新的头和身体某一节重合，即咬到自己
		{
			m_status = STATUS_BITE_ONESELF;
			return;
		}
		m_pSnakePosition->push_front(head);	//蛇头增加
		DWORD headLineCoord = planeCoordinates2Line(head);	//头一维坐标
		DWORD tailLineCoord = planeCoordinates2Line(tail);	//尾一维坐标
		m_pFreeSpace->remove(headLineCoord);		//自由空间删除新的头，增加旧的尾
		m_pFreeSpace->push_back(tailLineCoord);
	}
}

DWORD CSnakeGame::planeCoordinates2Line(CCoordinates & coord)
{
	return coord.y * m_nColumns + coord.x;	//如(2, 0)转换成一维坐标应该是2，(3, 1)则是1 * m_nColumns + 3
}

void CSnakeGame::lineCoordinates2Plane(DWORD lineCoord, CCoordinates & coord)
{
	coord.x = lineCoord / m_nColumns;
	coord.y = lineCoord % m_nColumns;
}

void CSnakeGame::updateTargetPosition()
{
	DWORD freeSize = m_mapSize - m_length;	//自由空间格子数
	DWORD targetIndex = rand() % freeSize;	//生成[0, freeSize - 1]闭区间随机整数，为自由空间目标索引
	int count = -1;
	DWORD index = 0;
	while (count < targetIndex)
	{
		if (m_lineMap[index++] == MAP_FREE_SPACE)
			count++;
	}


	if (!freeSize)
		return;
	DWORD lineCoord = rand() % freeSize;	//生成0到freeSize - 1闭区间随机整数，为自由空间目标索引

	list<DWORD>::iterator iter = m_pFreeSpace->begin();
	advance(iter, lineCoord);
	lineCoord = *iter;	//目标一维坐标
	lineCoordinates2Plane(lineCoord, m_targetPos);	//目标二维坐标
	m_pFreeSpace->remove(lineCoord);	//自由空间去掉目标位置
	updateRunInterval();					//更新运行间隔
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
		delete pGame;	//初始化失败
		return 1;
	}

	while (!pGame->hasTerminated())	//!3优先级，->2优先级
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
	if (direction > DIRECTION_RIGHT)	//非法输入
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
