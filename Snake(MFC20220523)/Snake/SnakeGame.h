#pragma once
#include <list>
using namespace std;

#define MAX_RUN_INTERVAL		500		//最大运行间隔
#define MIN_RUN_INTERVAL		100		//最小运行间隔
#define RUN_INTERVAL_DESCEND	40		//每吃一个目标减小间隔量

enum
{
	STATUS_RUNNING = 0,		//正在运行
	STATUS_PAUSE,			//暂停
	STATUS_COLLISION,		//撞墙
	STATUS_BITE_ONESELF,	//咬到自己
	STATUS_FORCED_STOP,		//强制停止
	STATUS_CLOSE_DLG,		//关闭对话框
	STATUS_WIN,				//蛇铺满屏幕，胜利！
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

	
	WORD m_runInterval;			//运行间隔，单位毫秒
	CCoordinates m_targetPos;	//目标位置

	list<CCoordinates>* m_pSnakePosition;	//蛇每一节的位置存成链表，坐标为二维坐标，从(0,0)开始，最大(m_nRows - 1, m_nColumns - 1)
	list<DWORD>* m_pFreeSpace;				//蛇和目标之外的自由空间，坐标为一维坐标，从0开始，最大m_nRows * m_nColumns - 1
	//CCriticalSection* m_pLock;				//线程锁，运行间隔足够长，不必加锁了

	BOOL m_changeDirectionForbidden;		//变换方向后，下次更新坐标前，禁止再次变换方向，以防咬到自己
	BOOL m_bFullspeed;						//是否已达到最高速度，用于bgm切换

private:
	BOOL init();	//初始化
	void run();		//运行
	DWORD planeCoordinates2Line(CCoordinates& coord);	//二维坐标转一维
	void lineCoordinates2Plane(DWORD lineCoord, CCoordinates& coord);	//一维坐标转二维

	void updateTargetPosition();	//在自由空间内随机获取目标位置
	void updateRunInterval();	//根据蛇的长度获取运行间隔
	void showResult();			//游戏结束，显示结果

public:
	static UINT snakeGameProc(LPVOID lPvoid);
	BOOL hasTerminated();	//已经停止，退出线程
	void forcedTerminate(BOOL bCloseDlg = FALSE);	//强制停止，传参为TRUE表示关闭对话框
	BOOL setDirection(DWORD direction);	//设置方向，允许设置返回TRUE，否则返回FALSE
	void setPause();	//设置暂停
	const list<CCoordinates>* getSnakePosition();
	const CCoordinates* getTargetPosition();
};

