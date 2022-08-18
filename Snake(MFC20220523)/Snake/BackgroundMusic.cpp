#include "stdafx.h"
#include "BackgroundMusic.h"


CBackgroundMusic::CBackgroundMusic()
{
	memset(this, 0, sizeof(*this));
}


CBackgroundMusic::~CBackgroundMusic()
{
	if (m_deviceID)
		mciSendCommand(m_deviceID, MCI_CLOSE, MCI_WAIT, 0);
}

BOOL CBackgroundMusic::load(TCHAR * bgmFilePath)
{
	if (m_deviceID)	//如果已有打开设备，先关闭
		mciSendCommand(m_deviceID, MCI_CLOSE, 0, 0);

	MCI_OPEN_PARMS openParams = { 0 };
	openParams.lpstrElementName = bgmFilePath;
	m_err = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)&openParams);
	if (m_err)	//成功返回0
	{
		mciGetErrorString(m_err, m_errInfo, _ERR_LENGTH_);
		return FALSE;
	}
		
	m_deviceID = openParams.wDeviceID;

	return TRUE;
}

BOOL CBackgroundMusic::replay(TCHAR* bgmFilePath)
{
	if (bgmFilePath)
		load(bgmFilePath);
	if (!m_deviceID)
		return FALSE;
	MCI_PLAY_PARMS playParams = { 0 };
	m_err = mciSendCommand(m_deviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT | MCI_FROM, (DWORD_PTR)&playParams);	//成功返回0
	if (m_err)	//成功返回0
	{
		mciGetErrorString(m_err, m_errInfo, _ERR_LENGTH_);
		return FALSE;
	}
	return !m_err;
}

BOOL CBackgroundMusic::stop()
{
	m_err = mciSendCommand(m_deviceID, MCI_STOP, 0, 0);	//成功返回0
	if (m_err)	//成功返回0
	{
		mciGetErrorString(m_err, m_errInfo, _ERR_LENGTH_);
		return FALSE;
	}
	return !m_err;
}

