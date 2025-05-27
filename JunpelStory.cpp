#include <windows.h> 
#include <stdio.h>
#include <commctrl.h>

#include "GameManager.h"

#include "JunpelStory.h"  

#pragma comment		(lib, "winmm.lib")
#pragma comment		(lib, "dxguid.lib")
#pragma comment		(lib, "d3dx9.lib")
#pragma comment		(lib, "d3d9.lib")
#pragma comment		(lib, "dsound.lib")

LPCTSTR		lpszAppName		= "ADDY™";// 이 플램에 이름이자 클래스 이름이자 아이콘 이름
LPCTSTR		lpszTitle		= "ADDY™";	// Title 메시지..

HWND            hh_wnd;									

CGameManager    *m_pGameManager = NULL;

BOOL InitWindow( HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;   // 윈도우 클래스
	// 클래스 등록.
   	wc.style         = 	0; 						// 스타일
	wc.lpfnWndProc   = (WNDPROC)WndProc;					// 윈도우 프로시져 지정..
	wc.cbClsExtra    = 0;									// 여분의 클래스와..      
	wc.cbWndExtra    = 0;									// 여분의 윈도우..
	wc.hInstance     = hInstance;							// 인스턴스 핸들..
	wc.hIcon         = LoadIcon( hInstance, lpszAppName );  // 아이콘..
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // Cursor.. 
	wc.hbrBackground = (HBRUSH)/*(COLOR_BTNFACE+1);*/GetStockObject(BLACK_BRUSH);// 클라이언트 영역의 바탕색깔..
	wc.lpszMenuName  = lpszAppName;                         // Menu 이름.. 리소스에서..
	wc.lpszClassName = lpszAppName;                         // 클래스 이름..
	wc.cbSize        = sizeof( WNDCLASSEX );                //
	wc.hIconSm       = (HICON)LoadImage( hInstance, lpszAppName,   // 아이콘..
								IMAGE_ICON, 16, 16,
								LR_DEFAULTCOLOR );

	if ( !RegisterClassEx( &wc ) )	return( FALSE );        // 윈도우 등록..

   // 윈도우 생성.
   	HWND HWnd = CreateWindowEx(0,//WS_EX_TOPMOST,
						lpszAppName,
                        lpszTitle,
                        WS_CAPTION,
						0,0,
						800,  // 화면 최대 크기..
						600,
                        NULL,
                        NULL,
                        hInstance,
                        NULL
                      );
 
	if ( !HWnd ) return( FALSE );

	hh_wnd = HWnd;

	ShowWindow( HWnd,nCmdShow);
	UpdateWindow( HWnd );

	return TRUE;
}

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine, int nCmdShow)
{
MSG msg;  // 메시지..
static BOOL UpGrade=TRUE;

	/*if(atoi(lpCmdLine) != -543454)
	{
		MessageBox(hh_wnd,"패치 프로그램으로 실행해 주세요.","아디",MB_OK);
		exit(0);
	} */

	if (InitWindow(hInstance,nCmdShow) == FALSE) return FALSE;
	
	m_pGameManager = new CGameManager();
	if (m_pGameManager->Init_JunePle_Game(hInstance,hh_wnd) == FALSE)return FALSE;

	while(1)   
	{

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_pGameManager->Render();
	}
	
	SAFE_DELETE_(m_pGameManager);
	return 0; 
}
LRESULT CALLBACK
WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		/*case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					//cKill(pMAIN);
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
			} 
			break;
		*/
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default :
			return( DefWindowProc(hWnd,Msg, wParam, lParam ) );
	}
	return( 0L );
}
