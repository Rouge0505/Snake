#pragma once
#include <mmsystem.h>
#include <digitalv.h>
#pragma comment(lib, "winmm.lib")
#define _ERR_LENGTH_	31
class CBackgroundMusic
{
public:
	CBackgroundMusic();
	~CBackgroundMusic();
	BOOL load(TCHAR* bgmFilePath);			//加载bgm
	BOOL replay(TCHAR* bgmFilePath = NULL);	//从头开始循环播放，当传参非空时，先加载
	BOOL stop();	//停止，但不关闭设备
private:
	MCIDEVICEID m_deviceID;
	MCIERROR m_err;							//错误码
	TCHAR m_errInfo[_ERR_LENGTH_];			//错误字符串
};

