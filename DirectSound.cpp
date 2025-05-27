#include "DirectSound.h"

YK_JinDsound::YK_JinDsound()
{
	lpSound = NULL;
	lpds = NULL;
}

YK_JinDsound::~YK_JinDsound()
{
	Destroy();
}

bool YK_JinDsound::Destroy()
{
	if(lpds != NULL) {
		lpds->Release();
		lpds = NULL;
	}
	if(lpSound != NULL) {
		delete [] lpSound;
		lpSound = NULL;
	}
	return true;
}

bool YK_JinDsound::Init(HWND hwnd)
{
	if(DirectSoundCreate(NULL, &lpds, NULL) != DS_OK) {
		return false;
	}

	if(lpds->SetCooperativeLevel(hwnd, DSSCL_NORMAL) != DS_OK) {
		return false;
	}

	return true;
}

bool YK_JinDsound::LoadFile(char *szFileName, LPDIRECTSOUNDBUFFER *lpds_buffer)
{
	HMMIO hmfr;
	PCMWAVEFORMAT pcmwf;
	DSBUFFERDESC dsbd;
	LPVOID write1, write2;
	DWORD length1, length2;
	MMCKINFO parent, child;
	WAVEFORMATEX wfmtx;
	
	parent.ckid=(FOURCC)0;
	parent.cksize=0;
	parent.fccType=(FOURCC)0;
	parent.dwDataOffset=0;
	parent.dwFlags=0;

	child=parent;

	if((hmfr=mmioOpen(szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF)) == NULL) {
		return FALSE;
	}

	parent.fccType=mmioFOURCC('W', 'A', 'V', 'E');

	if(mmioDescend(hmfr, &parent, NULL, MMIO_FINDRIFF)) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	child.ckid=mmioFOURCC('f', 'm', 't', ' ');

	if(mmioDescend(hmfr, &child, &parent, 0)) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	if(mmioRead(hmfr, (char*)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	if(wfmtx.wFormatTag != WAVE_FORMAT_PCM) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	if(mmioAscend(hmfr, &child, 0)) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	child.ckid=mmioFOURCC('d', 'a', 't', 'a');

	if(mmioDescend(hmfr, &child, &parent, MMIO_FINDCHUNK)) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	if(lpSound != NULL) {
		delete [] lpSound;
		lpSound = NULL;
	}

	lpSound = new BYTE[child.cksize];

	if(lpSound == NULL) {
		return FALSE;
	}

	if((DWORD)mmioRead(hmfr, (char *)lpSound, child.cksize) != child.cksize) {
		mmioClose(hmfr, 0);
		return FALSE;
	}

	mmioClose(hmfr, 0);

	memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
	memset(&dsbd, 0, sizeof(DSBUFFERDESC));

	pcmwf.wf.wFormatTag=WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels=wfmtx.nChannels > 1 ? 2 : 1;
	pcmwf.wf.nSamplesPerSec=wfmtx.nSamplesPerSec;
	pcmwf.wf.nBlockAlign=wfmtx.nBlockAlign;
	pcmwf.wf.nAvgBytesPerSec=pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample=wfmtx.wBitsPerSample;

	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = child.cksize;
	dsbd.lpwfxFormat = (LPWAVEFORMATEX) &pcmwf;

	if(lpds->CreateSoundBuffer(&dsbd, lpds_buffer, NULL) != DS_OK) {
		return FALSE;
	}

	if((*lpds_buffer)->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0) == DSERR_BUFFERLOST) {
		(*lpds_buffer)->Restore();
		if((*lpds_buffer)->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0) != DS_OK) {
			return FALSE;
		}
	}

	memcpy(write1, lpSound, length1);

	if(write2 != NULL) {
		memcpy(write2, lpSound+length1, length2);
	}

	if((*lpds_buffer)->Unlock(write1, length1, write2, length2) != DS_OK) {
		return FALSE;
	}

	if(lpSound != NULL) {
		delete [] lpSound;
		lpSound = NULL;
	}

	return TRUE;
}

bool YK_JinDsound::Playing(LPDIRECTSOUNDBUFFER lpds_buffer, bool looping)
{
	if(IsPlaying(lpds_buffer)) {
		return true;
	}
	//Stop(lpds_buffer);
	//SetCurrentPosition(lpds_buffer);
	
	if(lpds_buffer->Play(0, 0, looping ? DSBPLAY_LOOPING : 0) != DS_OK) {
		return false;
	}

	return true;
}

bool YK_JinDsound::Playing2(LPDIRECTSOUNDBUFFER lpds_buffer, bool looping)
{
	if(lpds_buffer->Play(0, 0, looping ? DSBPLAY_LOOPING : 0) != DS_OK) {
		return false;
	}

	return true;
}

bool YK_JinDsound::Stop(LPDIRECTSOUNDBUFFER lpds_buffer)
{
	if(lpds_buffer->Stop() != DS_OK) return false;

	return true;
}

bool YK_JinDsound::IsPlaying(LPDIRECTSOUNDBUFFER lpds_buffer)
{
	DWORD Status;

	lpds_buffer->GetStatus(&Status);

	if((Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING) return true;

	return false;
}

bool YK_JinDsound::SetCurrentPosition(LPDIRECTSOUNDBUFFER lpds_buffer)
{
	if(lpds_buffer->SetCurrentPosition(0) != DS_OK) return false;

	return true;
}