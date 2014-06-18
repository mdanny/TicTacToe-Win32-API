#include "stdafx.h"
#include "TicTacToe.h"
#include <string>
#include <atlstr.h>
enum EMarker {blank, signX, signO};

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
HINSTANCE g_hInst;					// current instance
HINSTANCE hInstance;
TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
EMarker playerTurn = signX;
EMarker board[3][3] =
{{blank, blank, blank},
	{blank, blank, blank},
	{blank, blank, blank}};
const int sqrSize = 200; // size of our cell
int counterX=0;
int counterO=0; //count won games
CString myString;
HWND label1, label2;

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    
    hInst = hInstance; // Store instance handle in our global variable
    
    hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                        CW_USEDEFAULT, 0, 620, 700, NULL, NULL, hInstance, NULL);
    
    if (!hWnd)
    {
        return FALSE;
    }
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    return TRUE;
}

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

void DrawX(HDC hdc, int iX, int iY)
{
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF lineColor;
	lineColor = RGB(71, 60, 139);
	const int penWidth = 10;
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

void DrawO(HDC hdc, int iX, int iY) {
	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF lineColor;
	lineColor = RGB(255,165,79);
	const int penWidth = 10;
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
		(sqNrX < 3) && (sqNrY < 3) &&
		(board[sqNrY][sqNrX]) == blank)
	{
		board[sqNrY][sqNrX] = signX;
		return true;
	}
	return false;
}

bool PlaceO(LPARAM lParam)
{
	int iPosX = LOWORD(lParam);
	int iPosY = HIWORD(lParam);
	int sqNrX = (iPosX/sqrSize);
	int sqNrY = (iPosY/sqrSize);
    
	if ((playerTurn == signO) &&
		(sqNrX < 3) && (sqNrY < 3) &&
		(board[sqNrY][sqNrX]) == blank)
	{
		board[sqNrY][sqNrX] = signO;
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
