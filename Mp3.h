#pragma once

class CMySound
{ 
    public:
	    UINT GetPlayTime();

		//��¿� ������
     	CMySound       (LPSTR lpszFileName);
		//������ ������
     	CMySound       (LPSTR lpszFileName,DWORD Sec);
		//�Ҹ���
     	~CMySound      ();
		//���
        BOOL Play();
		//����
		void Pause();
		//ó������
		BOOL Home();
		//����������
		BOOL End();

		BOOL PlayTime();
		BOOL PlayEndTime();
		//����Ÿ�� �� �ð�
        UINT m_nTime;
	protected:	      
		MCI_OPEN_PARMS mciOpenParms;
		MCI_PLAY_PARMS mciPlayParms;
		MCI_RECORD_PARMS mciRecordParms;
		MCI_SAVE_PARMS mciSaveParms;
		DWORD wDeviceID;
};
