// TicTacToe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TicTacToe.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TICTACTOE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int menuId, menuEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			bool bValidMove = PlaceX(lParam);
			if (bValidMove) {
				playerTurn = signO;
			} else {
				::MessageBox(hWnd, _T("It's O Turn!"), _T("Warning"), MB_OK);
			}
			// Repaint the window after the update
			InvalidateRect(hWnd, 0, TRUE);
		    break;
		}

	case WM_RBUTTONDOWN:
		{
			bool bValidMove = PlaceO(lParam);
			if (bValidMove) {
				playerTurn = signX;
			} else {
				::MessageBox(hWnd, _T("It's X Turn!"), _T("Warning"), MB_OK);
			}
			// Repaint the window after the update
			InvalidateRect(hWnd, 0, TRUE);
			break;
		}

	case WM_COMMAND:
		menuId    = LOWORD(wParam);
		menuEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (menuId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_NEW_GAME:
			ResetGame();
			// Repaint the window after the update
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CREATE:
		label1 = CreateWindow(L"Static", L"X", WS_CHILD | WS_VISIBLE, 0, 620, 20 ,25,hWnd,0, g_hInst, 0);
		label2 = CreateWindow(L"Static", L"O", WS_CHILD | WS_VISIBLE, 40, 620, 20 ,25,hWnd,0, g_hInst, 0);
	
	case WM_PAINT:
		{
		hdc = BeginPaint(hWnd, &ps);
		// Draw the board lines
		DrawBoard(hdc);

		// Draw Xs and Os
		for (int iX = 0; iX < 3; ++iX) {
			for (int iY = 0; iY < 3; ++iY) {
				if (board[iY][iX] == signX) {
					DrawX(hdc, iX, iY);
				} else if (board[iY][iX] == signO) {
					DrawO(hdc, iX, iY);
				}
			}
		}

		// Check for end of game conditions
		if (HasWon()) {
			

			if (playerTurn == signO) {
				::MessageBox(hWnd, _T("X Won!"), _T("Another Round?"), MB_OK);
				counterX+= 1;
				myString.Format(_T("%d"), counterX);
				SetWindowText(label1, myString);
				
			} else {
				::MessageBox(hWnd, _T("O Won!"), _T("Another Round?"), MB_OK);
				counterO+= 1;
				myString.Format(_T("%d"), counterO);
				SetWindowText(label2, myString);
			}
			ResetGame();
			InvalidateRect(hWnd, 0, TRUE);
		} else {
			// If there is no win, check for a draw
			if (IsBoardFull()) {
				::MessageBox(hWnd, _T("It's A Draw!"), _T("Another Round?"), MB_OK);
				ResetGame();
				// Repaint the window after the update
				InvalidateRect(hWnd, 0, TRUE);
			}
		}
		EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}