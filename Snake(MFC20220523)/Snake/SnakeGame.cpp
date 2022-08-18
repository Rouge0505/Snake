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
	m_pSnakePosition->push_back(initPos);	//初始蛇长度为1，位于中心

	DWORD lineCoord = planeCoordinates2Line(initPos);	//蛇初始位置一维坐标

	for (int i = 0; i < m_nRows * m_nColumns; i++)	//除蛇初始位置之外为自由空间
		if (i != lineCoord)
			m_pFreeSpace->push_back(i);

	srand(time(NULL));	//初始化随机数种子
	updateTargetPosition();	//在自由空间内随机获取目标位置
	return TRUE;
}

void CSnakeGame::run()
{
	m_currentMoment = clock();
	if (m_status == STATUS_RUNNING)	//没有暂停才更新时间
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

		updateTargetPosition();					//更新目标位置
		updateRunInterval();					//更新运行间隔
	}
	else if (head.x >= m_nColumns || head.y >= m_nRows)	//撞墙，xy无符号，如果是-1则会是255，也会超过最大值
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
	m_changeDirectionForbidden = FALSE;	//一次坐标更新后，允许再次更换方向

	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_DRAW_SNAKE, 0, 0);
}

DWORD CSnakeGame::planeCoordinates2Line(CCoordinates & coord)
{
	return coord.y * m_nColumns + coord.x;	//如(2, 0)转换成一维坐标应该是2，(3, 1)则是1 * m_nColumns + 3
}

void CSnakeGame::lineCoordinates2Plane(DWORD lineCoord, CCoordinates & coord)
{
	coord.y = lineCoord / m_nColumns;
	coord.x = lineCoord % m_nColumns;
}

void CSnakeGame::updateTargetPosition()
{
	DWORD freeSize = m_pFreeSpace->size();	//自由空间格子数
	if (!freeSize)
	{
		m_status = STATUS_WIN;	//自由格子没了，蛇铺满平面，胜利
		return;
	}
	
	DWORD lineCoord = rand() % freeSize;	//生成0到freeSize - 1闭区间随机整数，为自由空间目标索引

	list<DWORD>::iterator iter = m_pFreeSpace->begin();
	advance(iter, lineCoord);
	lineCoord = *iter;	//目标一维坐标
	lineCoordinates2Plane(lineCoord, m_targetPos);	//目标二维坐标
	m_pFreeSpace->remove(lineCoord);	//自由空间去掉目标位置
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
	TCHAR restartMsg[32] = _T("按S键重新开始游戏！");
	switch (m_status)
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
	MessageBox(NULL, msg, _T("提示"), NULL);
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
	if (direction > DIRECTION_RIGHT)	//非法输入
		return FALSE;
	if (m_changeDirectionForbidden)		//禁止变换方向
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
