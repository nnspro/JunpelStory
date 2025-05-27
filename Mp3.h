#pragma once

class CMySound
{ 
    public:
	    UINT GetPlayTime();

		//출력용 생성자
     	CMySound       (LPSTR lpszFileName);
		//녹음용 생성자
     	CMySound       (LPSTR lpszFileName,DWORD Sec);
		//소멸자
     	~CMySound      ();
		//재생
        BOOL Play();
		//멈춤
		void Pause();
		//처음으로
		BOOL Home();
		//마지막으로
		BOOL End();

		BOOL PlayTime();
		BOOL PlayEndTime();
		//데이타의 총 시간
        UINT m_nTime;
	protected:	      
		MCI_OPEN_PARMS mciOpenParms;
		MCI_PLAY_PARMS mciPlayParms;
		MCI_RECORD_PARMS mciRecordParms;
		MCI_SAVE_PARMS mciSaveParms;
		DWORD wDeviceID;
};
