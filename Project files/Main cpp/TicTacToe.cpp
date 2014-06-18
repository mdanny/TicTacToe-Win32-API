// TicTacToe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TicTacToe.h"
#include <string>
#include <atlstr.h>
enum EMarker {blank, signX, signO};

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;					// current instance
HINSTANCE g_hInst;					// another instance
HINSTANCE hInstance;
TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
EMarker playerTurn = signX;
EMarker board[3][3] =
	{{blank, blank, blank},
	{blank, blank, blank},
	{blank, blank, blank}};
const int sqrSize = 200; 				// size of our cell
int counterX=0;
int counterO=0; 					//count won games
CString myString;
HWND label1, label2;					//handler for the labels

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
	wcex.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//  FUNCTION: DrawBoard

void DrawBoard(HDC hdc)
{
	HPEN hPenOld;
	// Draw the board lines
	HPEN hLinePen;
	COLORREF lineColor;
	lineColor = RGB(0, 0, 0);
	hLinePen = CreatePen(PS_SOLID, 7, lineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Draw two vertical lines
	for (int iX = sqrSize; iX < 3*sqrSize; iX += sqrSize) {
		MoveToEx(hdc, iX, 0, NULL);
		LineTo(hdc, iX, 3*sqrSize);
	}
	// Draw two horizontal lines
	for (int iY = sqrSize; iY < 3*sqrSize; iY += sqrSize) {
		MoveToEx(hdc, 0, iY, NULL);
		LineTo(hdc, 3*sqrSize, iY);
	}

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}
//functions to draw X and O
void DrawX(HDC hdc, int iX, int iY)
{
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF lineColor;
	lineColor = RGB(71, 60, 139);//color of X
	const int penWidth = 10;//Width of X lines
	hLinePen = CreatePen(PS_SOLID, penWidth, lineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Get bounds
	const int bottomX	= iX*sqrSize + 2*penWidth;
	const int topX	= (iX + 1)*sqrSize - 2*penWidth;
	const int bottomY	= iY*sqrSize + 2*penWidth;
	const int topY	= (iY + 1)*sqrSize - 2*penWidth;

	MoveToEx(hdc, bottomX, bottomY, NULL);
	LineTo(hdc, topX, topY);
	MoveToEx(hdc, bottomX, topY, NULL); 
	LineTo(hdc, topX, bottomY);

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

//functions to draw O
void DrawO(HDC hdc, int iX, int iY) {
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF lineColor;
	lineColor = RGB(255,165,79); //color of O
	const int penWidth = 10;// width of O
	hLinePen = CreatePen(PS_SOLID, penWidth, lineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Get bounds
	const int bottomX	= iX*sqrSize + 2*penWidth;
	const int topX	= (iX + 1)*sqrSize - 2*penWidth;
	const int bottomY	= iY*sqrSize + 2*penWidth;
	const int topY	= (iY + 1)*sqrSize - 2*penWidth;

	Arc(hdc, bottomX, bottomY, topX, topY, 0, 0, 0, 0);

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

bool PlaceX(LPARAM lParam)
{
	int iPosX = LOWORD(lParam);
	int iPosY = HIWORD(lParam);
	int sqNrX = (iPosX/sqrSize);
	int sqNrY = (iPosY/sqrSize);

	if ((playerTurn == signX) &&
		(sqNrX <= 3) && (sqNrY < 3) &&
		(board[sqNrY][sqNrX]) == blank)
	{
		board[sqNrY][sqNrX] = signX;
		return true;
	}
	return false;
}

bool HasWon() {
	// Check for win through upper left
	if (board[0][0] != blank) {
		if (board[0][1] == board[0][0] &&
			board[0][2] == board[0][0])
		{
			return true;
		}
		if (board[1][0] == board[0][0] &&
			board[2][0] == board[0][0])
		{
			return true;
		}
	}
	// Check for win through center
	if (board[1][1] != blank) {
		if (board[0][0] == board[1][1] &&
			board[2][2] == board[1][1])
		{
			return true;
		}
		if (board[0][2] == board[1][1] &&
			board[2][0] == board[1][1])
		{
			return true;
		}
		if (board[1][0] == board[1][1] &&
			board[1][2] == board[1][1])
		{
			return true;
		}
		if (board[0][1] == board[1][1] &&
			board[2][1] == board[1][1])
		{
			return true;
		}
	}
	// Check for win through lower right
	if (board[2][2] != blank) {
		if (board[2][0] == board[2][2] &&
			board[2][1] == board[2][2])
		{
			return true;
		}
		if (board[0][2] == board[2][2] &&
			board[1][2] == board[2][2])
		{
			return true;
		}
	}
	return false;
}

bool IsBoardFull()
{
	for (int iX = 0; iX < 3; ++iX) {
		for (int iY = 0; iY < 3; ++iY) {
			if (board[iY][iX] == blank) {
				return false;
			}
		}
	}
	return true;
}

void ResetGame()
{
	for (int iX = 0; iX < 3; ++iX) {
		for (int iY = 0; iY < 3; ++iY) {
			board[iY][iX] = blank;
		}
	}
	playerTurn = signX;
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
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
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
