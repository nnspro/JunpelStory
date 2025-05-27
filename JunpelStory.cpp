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

LPCTSTR		lpszAppName		= "ADDY��";// �� �÷��� �̸����� Ŭ���� �̸����� ������ �̸�
LPCTSTR		lpszTitle		= "ADDY��";	// Title �޽���..

HWND            hh_wnd;									

CGameManager    *m_pGameManager = NULL;

BOOL InitWindow( HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;   // ������ Ŭ����
	// Ŭ���� ���.
   	wc.style         = 	0; 						// ��Ÿ��
	wc.lpfnWndProc   = (WNDPROC)WndProc;					// ������ ���ν��� ����..
	wc.cbClsExtra    = 0;									// ������ Ŭ������..      
	wc.cbWndExtra    = 0;									// ������ ������..
	wc.hInstance     = hInstance;							// �ν��Ͻ� �ڵ�..
	wc.hIcon         = LoadIcon( hInstance, lpszAppName );  // ������..
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // Cursor.. 
	wc.hbrBackground = (HBRUSH)/*(COLOR_BTNFACE+1);*/GetStockObject(BLACK_BRUSH);// Ŭ���̾�Ʈ ������ ��������..
	wc.lpszMenuName  = lpszAppName;                         // Menu �̸�.. ���ҽ�����..
	wc.lpszClassName = lpszAppName;                         // Ŭ���� �̸�..
	wc.cbSize        = sizeof( WNDCLASSEX );                //
	wc.hIconSm       = (HICON)LoadImage( hInstance, lpszAppName,   // ������..
								IMAGE_ICON, 16, 16,
								LR_DEFAULTCOLOR );

	if ( !RegisterClassEx( &wc ) )	return( FALSE );        // ������ ���..

   // ������ ����.
   	HWND HWnd = CreateWindowEx(0,//WS_EX_TOPMOST,
						lpszAppName,
                        lpszTitle,
                        WS_CAPTION,
						0,0,
						800,  // ȭ�� �ִ� ũ��..
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
MSG msg;  // �޽���..
static BOOL UpGrade=TRUE;

	/*if(atoi(lpCmdLine) != -543454)
	{
		MessageBox(hh_wnd,"��ġ ���α׷����� ������ �ּ���.","�Ƶ�",MB_OK);
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
