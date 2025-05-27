#include <windows.h>
#include <MMSystem.h>
//#include "mmsystem.h"
#include "Mp3.h"

//������ ���̺�ȭ���� ��½�Ű�� ������
CMySound::CMySound(LPSTR filename)
{
	//ȭ�� ����
	mciOpenParms.lpstrElementName = filename;
	//����̽� �� waveaudio��
	mciOpenParms.lpstrDeviceType = "MPEGVideo";
	//MCI_OPEN ����� �ش�.
	mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,
					(DWORD)(LPVOID)&mciOpenParms);
	//���� ����̽� ���̵� �޴���.
	wDeviceID = mciOpenParms.wDeviceID;		
	//Callback �Լ� ����
	mciPlayParms.dwCallback = NULL;
	// ��ü ��½ð��� �˱� ���� m_nTime�� �����Ѵ�.
	MCI_STATUS_PARMS mciStatusParms;			
	mciStatusParms.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);
	m_nTime=(LONG)mciStatusParms.dwReturn;

}
//������ ���� ������
CMySound::CMySound(LPSTR filename,DWORD Sec)
{
	BOOL erroCode;
	//ȭ�� �̸� ���� ����̽��� ����
	mciOpenParms.lpstrElementName = "";
	mciOpenParms.lpstrDeviceType = "MPEGVideo";

	if(erroCode=mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,
								(DWORD)(LPVOID)&mciOpenParms))
	{
			return;
	}
   
	wDeviceID = mciOpenParms.wDeviceID;		
	//���� �ð� ����
	mciRecordParms.dwTo=Sec*1000;
	//���� ���� MCI_WAIT �̱� ������ ���� �߿���
	//�ƹ� �ϵ� ���� �ʴ´�.
	if(erroCode=mciSendCommand(wDeviceID,MCI_RECORD,MCI_TO | MCI_WAIT,
									(DWORD)(LPVOID)&mciRecordParms))
	{
			mciSendCommand(wDeviceID,MCI_CLOSE,0,NULL);
			return;
	}
	//������ ������ ȭ�Ͽ� �����Ѵ�.
	mciSaveParms.lpfilename=filename;
	if(erroCode=mciSendCommand(wDeviceID,MCI_SAVE,MCI_SAVE_FILE | MCI_WAIT,
										(DWORD)(LPVOID)&mciSaveParms))
	{
			mciSendCommand(wDeviceID,MCI_CLOSE,0,NULL);
			return;
	}
		

}
//�Ҹ���
CMySound::~CMySound()
{
	mciSendCommand(wDeviceID,MCI_CLOSE,0,NULL);
}
//���
BOOL CMySound::Play()
{
		mciSendCommand(wDeviceID,MCI_PLAY,MCI_NOTIFY,
					(DWORD)(LPVOID) &mciPlayParms);
	return TRUE;						
}
//����
void CMySound::Pause()
{
	mciSendCommand(wDeviceID,MCI_PAUSE,MCI_NOTIFY,
					(DWORD)(LPVOID) &mciPlayParms);
					
}
//ó������
BOOL CMySound::Home (void)
{
	mciSendCommand (wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD) (LPVOID) NULL);
	return TRUE;
}
//����������
BOOL CMySound::End (void)
{
	mciSendCommand (wDeviceID, MCI_SEEK, MCI_SEEK_TO_END, (DWORD) (LPVOID) NULL);
	return TRUE;
}


UINT CMySound::GetPlayTime()
{
	return m_nTime;
}

BOOL CMySound::PlayTime()
{
MCI_STATUS_PARMS  mciStatusParms;
//MCI_SEEK_PARMS     mciSeekParms;
DWORD len, pos;


   // ��ü ������ ���̸� �˾� ���ϴ�.
   mciStatusParms.dwItem = MCI_STATUS_LENGTH;

   mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM,
	   (DWORD)(LPVOID)&mciStatusParms);

   len = mciStatusParms.dwReturn;


   // ������ ��ġ�� �˾Ƴ��ϴ�.

   mciStatusParms.dwItem = MCI_STATUS_POSITION;

   mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, 
	   (DWORD)(LPVOID)&mciStatusParms);

   pos = mciStatusParms.dwReturn;

   if(pos < 0 || pos >= len)return FALSE;

   return TRUE;
}
BOOL CMySound::PlayEndTime()
{
MCI_STATUS_PARMS  mciStatusParms;
//MCI_SEEK_PARMS     mciSeekParms;
DWORD /*len,*/ pos;


  /* // ��ü ������ ���̸� �˾� ���ϴ�.
   mciStatusParms.dwItem = MCI_STATUS_LENGTH;

   mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM,
	   (DWORD)(LPVOID)&mciStatusParms);

   len = mciStatusParms.dwReturn;

*/
   // ������ ��ġ�� �˾Ƴ��ϴ�.

   mciStatusParms.dwItem = MCI_STATUS_POSITION;

   mciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, 
	   (DWORD)(LPVOID)&mciStatusParms);

   pos = mciStatusParms.dwReturn;

   if(pos >= m_nTime)return FALSE;

   return TRUE;
}