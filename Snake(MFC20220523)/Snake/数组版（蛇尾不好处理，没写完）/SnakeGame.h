#pragma once
#include <list>
using namespace std;

enum MapInfo				//地图每个格子数字代表什么含义
{
	MAP_FREE_SPACE = 0,		//自由空间
	MAP_TARGET,				//目标
	MAP_SNAKE_BODY,			//蛇身
	MAP_SNAKE_HEAD_UP,		//蛇头——向上
	MAP_SNAKE_HEAD_DOWN,	//蛇头——向下
	MAP_SNAKE_HEAD_LEFT,	//蛇头——向左
	MAP_SNAKE_HEAD_RIGHT,	//蛇头——向右
};

enum GameStatus				//游戏状态
{
	STATUS_RUNNING = 0,		//正在运行
	STATUS_PAUSE,			//暂停
	STATUS_COLLISION,		//撞墙
	STATUS_BITE_ONESELF,	//咬到自己
	STATUS_FORCED_STOP		//强制停止
};

enum
{
	DIRECTION_UP = 0,		//方向-上
	DIRECTION_DOWN,			//方向-下
	DIRECTION_LEFT,			//方向-左
	DIRECTION_RIGHT			//方向-右
};

struct CCoordinates	//二维坐标，如左上角为(0, 0)转换成一维坐标为0
{
	BYTE x;
	BYTE y;
	CCoordinates()
	{
		memset(this, 0, sizeof(*this));
	}

	CCoordinates(DWORD x, DWORD y)
	{
		this->x = x;
		this->y = y;
	}

	void setCoordinates(DWORD x, DWORD y)
	{
		this->x = x;
		this->y = y;
	}

	bool operator== (const CCoordinates& another)
	{
		return (memcmp(this, &another, sizeof(*this)) == 0);
	}
};

class CSnakeGame
{
public:
	CSnakeGame(DWORD nRows, DWORD nColumns);
	~CSnakeGame();

private:
	BYTE m_status;		//游戏状态
	BYTE m_nRows;		//行数
	BYTE m_nColumns;	//列数
	BYTE m_direction;	//前进方向

	clock_t m_currentMoment;	//当前帧时刻，单位毫秒
	clock_t m_lastMoment;		//上一帧时刻，单位毫秒
	DWORD m_runTimeSaufPause;	//除去停顿时间，总运行时间，单位毫秒
	DWORD m_lastRunMoment;		//上次运行时刻，单位毫秒

	
	DWORD m_runInterval;			//运行间隔，单位毫秒
	CCoordinates m_headPos;		//蛇头位置
	CCoordinates m_targetPos;	//目标位置

	BYTE*  m_lineMap;			//将二维数组拉成一维数组
	BYTE** m_map;				//地图信息

	BYTE m_length;				//蛇长
	BYTE m_mapSize;				//地图有多少个格子

private:
	BOOL init();	//初始化
	void run();		//运行
	DWORD planeCoordinates2Line(CCoordinates& coord);	//二维坐标转一维
	void lineCoordinates2Plane(DWORD lineCoord, CCoordinates& coord);	//一维坐标转二维

	void updateTargetPosition();	//在自由空间内随机获取目标位置
	void updateRunInterval();	//根据蛇的长度获取运行间隔

public:
	static UINT snakeGameProc(LPVOID lPvoid);
	BOOL hasTerminated();	//已经停止，退出线程
	void forcedTerminate();	//强制停止
	BOOL setDirection(DWORD direction);	//设置方向，允许设置返回TRUE，否则返回FALSE
	void setPause();	//设置暂停
	const list<CCoordinates>* getSnakePosition();
	const CCoordinates* getTargetPosition();
};

