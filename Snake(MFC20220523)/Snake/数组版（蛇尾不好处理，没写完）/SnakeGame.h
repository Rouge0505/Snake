#pragma once
#include <list>
using namespace std;

enum MapInfo				//��ͼÿ���������ִ���ʲô����
{
	MAP_FREE_SPACE = 0,		//���ɿռ�
	MAP_TARGET,				//Ŀ��
	MAP_SNAKE_BODY,			//����
	MAP_SNAKE_HEAD_UP,		//��ͷ��������
	MAP_SNAKE_HEAD_DOWN,	//��ͷ��������
	MAP_SNAKE_HEAD_LEFT,	//��ͷ��������
	MAP_SNAKE_HEAD_RIGHT,	//��ͷ��������
};

enum GameStatus				//��Ϸ״̬
{
	STATUS_RUNNING = 0,		//��������
	STATUS_PAUSE,			//��ͣ
	STATUS_COLLISION,		//ײǽ
	STATUS_BITE_ONESELF,	//ҧ���Լ�
	STATUS_FORCED_STOP		//ǿ��ֹͣ
};

enum
{
	DIRECTION_UP = 0,		//����-��
	DIRECTION_DOWN,			//����-��
	DIRECTION_LEFT,			//����-��
	DIRECTION_RIGHT			//����-��
};

struct CCoordinates	//��ά���꣬�����Ͻ�Ϊ(0, 0)ת����һά����Ϊ0
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
	BYTE m_status;		//��Ϸ״̬
	BYTE m_nRows;		//����
	BYTE m_nColumns;	//����
	BYTE m_direction;	//ǰ������

	clock_t m_currentMoment;	//��ǰ֡ʱ�̣���λ����
	clock_t m_lastMoment;		//��һ֡ʱ�̣���λ����
	DWORD m_runTimeSaufPause;	//��ȥͣ��ʱ�䣬������ʱ�䣬��λ����
	DWORD m_lastRunMoment;		//�ϴ�����ʱ�̣���λ����

	
	DWORD m_runInterval;			//���м������λ����
	CCoordinates m_headPos;		//��ͷλ��
	CCoordinates m_targetPos;	//Ŀ��λ��

	BYTE*  m_lineMap;			//����ά��������һά����
	BYTE** m_map;				//��ͼ��Ϣ

	BYTE m_length;				//�߳�
	BYTE m_mapSize;				//��ͼ�ж��ٸ�����

private:
	BOOL init();	//��ʼ��
	void run();		//����
	DWORD planeCoordinates2Line(CCoordinates& coord);	//��ά����תһά
	void lineCoordinates2Plane(DWORD lineCoord, CCoordinates& coord);	//һά����ת��ά

	void updateTargetPosition();	//�����ɿռ��������ȡĿ��λ��
	void updateRunInterval();	//�����ߵĳ��Ȼ�ȡ���м��

public:
	static UINT snakeGameProc(LPVOID lPvoid);
	BOOL hasTerminated();	//�Ѿ�ֹͣ���˳��߳�
	void forcedTerminate();	//ǿ��ֹͣ
	BOOL setDirection(DWORD direction);	//���÷����������÷���TRUE�����򷵻�FALSE
	void setPause();	//������ͣ
	const list<CCoordinates>* getSnakePosition();
	const CCoordinates* getTargetPosition();
};

