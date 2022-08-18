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
	BOOL load(TCHAR* bgmFilePath);			//����bgm
	BOOL replay(TCHAR* bgmFilePath = NULL);	//��ͷ��ʼѭ�����ţ������ηǿ�ʱ���ȼ���
	BOOL stop();	//ֹͣ�������ر��豸
private:
	MCIDEVICEID m_deviceID;
	MCIERROR m_err;							//������
	TCHAR m_errInfo[_ERR_LENGTH_];			//�����ַ���
};

