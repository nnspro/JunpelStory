#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <mmsystem.h>

#include "Direct3D.h"
#include "TextOutput.h"

#define ID_EDIT		101101 // 에디트 박스 아이디 

class CChatManager
{
private:
	int					m_CurPosX;			// Cursor 위치
	char*				m_ChatBuf;			// 어플리케이션과 공유
	char				m_Name[15];			// 이름 
	int					m_NameLen;			// 이름의 길이

	CD3dxFont			*m_pD3dxfont;
	CDirect3D			*m_pDirect3d;

	ID3DXFont*			m_pFontCom;			// 폰트 콤 
	HWND				m_hEdit;			// 에디트 박스 핸들
	HWND				m_hWnd;				// 윈도우 핸들

	BOOL				m_bInput;			// 입력 모드인가?
	BOOL				m_bCursor;			// Cursor가 켜있는지?

	RECT				m_RectOut;		// 출력할 위치
	RECT				m_RectIn;			// 입력 받을 위치

	D3DCOLOR			m_Color;			// 폰트 색상
	int					m_OutCount;		// 문자열 출력 남은 초

	BOOL Init();							// 초기화
	BOOL CleanUp();							// 종료 준비
	BOOL SetStrIn();						// 에디트 박스의 내용을 문자열 변수에 저장

public:
//	CChatManager() ;//{ Init(CDirect3D *pDirect3d); }				// 생성자
	~CChatManager() { CleanUp(); }			// 소멸자
	
	BOOL Init(CDirect3D *pDirect3d,int fontsize);
	BOOL LinkDevice(HWND hWnd, HINSTANCE hInst,int line);	// 장치 연결
	BOOL RestoreDevice(ID3DXFont* pFont);	// 폰트 재 연결
	
	BOOL AddOutStr(char* Str);				// 출력 문자열 추가
	BOOL InputStart();						// 입력 모드 시작 
	BOOL InputCancel();						// 입력 모드 취소
	void InputEnd();						// 입력을 끝내고 출력 문자열에 추가

	BOOL SetColor(D3DCOLOR Color);			// 출력 색상 설정
	BOOL SetOutPos(int x, int y);	// 출력 위치 설정
	BOOL SetInPos(int x, int y);			// 입력 위치 설정

	BOOL SetMyName(char* Name);				// 이름 설정
	BOOL SetChatBuf(char* Buf);				// 공유 버퍼 설정

	BOOL Render(BOOL Pass_yn);              // 렌더링
	
	char				m_StrOut[256];  	// 출력할 문자열
	
}; // CChatManager
static WNDPROC OldEditProc = NULL;	// 원본 에디트 박스의 Proc
static CChatManager* g_pAppChat = NULL; // 이 클래스의 주소를 저장
// 서브클래싱 Proc
static LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

