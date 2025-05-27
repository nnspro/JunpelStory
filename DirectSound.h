#pragma once

#include <windows.h>
#include <dsound.h>

class YK_JinDsound {
	public:
		//변수들.
		LPDIRECTSOUND lpds;
		BYTE *lpSound;

		//함수들.
		YK_JinDsound();
		~YK_JinDsound();

		bool Destroy();

		bool Init(HWND hwnd);
		
		bool LoadFile(char *szFileName, LPDIRECTSOUNDBUFFER *lpds_buffer);
		bool Playing(LPDIRECTSOUNDBUFFER lpds_buffer, bool looping);
		bool Playing2(LPDIRECTSOUNDBUFFER lpds_buffer, bool looping);
		bool Stop(LPDIRECTSOUNDBUFFER lpds_buffer);
		bool IsPlaying(LPDIRECTSOUNDBUFFER lpds_buffer);
		bool SetCurrentPosition(LPDIRECTSOUNDBUFFER lpds_buffer);
};

struct snd {
	LPDIRECTSOUNDBUFFER bff;
};

struct sound {
	snd jump;
	snd holkick;
	snd sliding;
	snd mongun;
	snd mugi;
	snd hitmon;
	snd hituser;
	snd mondie;
	snd userdie;
	snd eat;
	snd eat_e;
	snd lvup;
	snd out;
	snd no;
	snd open;
	snd gay;
	snd btn;
	snd btn0;
	snd btn1;
	snd btn2;
	snd pet;
	//snd wait;
};
extern YK_JinDsound m_pDSound;
extern sound        m_psnd;

