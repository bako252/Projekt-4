#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <math.h>
#include <gdiplus.h>
#include <Windows.h>
#include <atlstr.h>
#include <windowsx.h>
#include <string>
#include <CommCtrl.h>
#include <fstream>

using namespace Gdiplus;

#define MAX_LOADSTRING 100
#define TMR_1 1

HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			

INT value;


HWND hwndButton;
HWND slider;

double const PI = 3.14159265358979323846;


int x = 0, y = -200, r = 250, dy = 200, dx = 0, b, next, grab = 0, height = 0, szpudlo = 50, grabpudlo, zmianax, zmianay,zapis=0,nagranie=0, przesunieciey=700 , przesunieciex = 500;
int boxa = 50, boxb = 100, boxc = 150, boxd=200, czas=4;
double arc = -90 , arc2 = 90  , arc3;
float R, arcR;
int slidervalue = 1;
float var = 0;
float distance = 0;
std::vector<Point> data;
RECT drawArea1 = { 0, 0, 1000, 1000 };


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);



struct box {
	unsigned int x;
	unsigned int y;
	unsigned int h;
};


box pudlo[4] = { 400,przesunieciey-boxa,boxa,300,przesunieciey-boxb,boxb,200,przesunieciey-boxc,boxc,100,przesunieciey-boxd,boxd};

void MyOnPaint(HDC hdc, std::fstream& hPlik)
{
	Graphics graphics(hdc);
	
	Pen pen(Color(255, 0, 150, 255), 10);
	Pen pen1(Color(255, 0, 255, 0), 2);
	Pen pen2(Color(255, 0, 0, 0), 5);
	Pen pen3(Color(255, 255, 0, 255), 5);
	
	if (nagranie == 0) {
		if (grab == 1) {
			pudlo[grabpudlo].x = przesunieciex + x + dx - zmianax;
			pudlo[grabpudlo].y = przesunieciey + y + dy - zmianay;
		}
		
	}
	if (nagranie == 1) {
		zapis = 0;
			hPlik >> x >> y >> dx >> dy >> pudlo[0].x >> pudlo[0].y >>
				pudlo[1].x >> pudlo[1].y >>
				pudlo[2].x >> pudlo[2].y >>
				pudlo[3].x >> pudlo[3].y;
	}

	for (int i = 0; i < 4; i++) {
		graphics.DrawRectangle(&pen3, pudlo[i].x, pudlo[i].y, szpudlo, pudlo[i].h);
	}

	graphics.DrawLine(&pen2, 50, przesunieciey, 1000, przesunieciey);
	graphics.DrawLine(&pen, przesunieciex, przesunieciey, x+przesunieciex, y + przesunieciey);
	graphics.DrawLine(&pen, x+przesunieciex, y + przesunieciey, dx+x+przesunieciex,dy+y+przesunieciey);

	TCHAR buffer_one[300];
	TCHAR buffer_two[300];
	TCHAR buffer_three[300];

	_stprintf( buffer_one, TEXT("arc = %f arc2 = %f"), arc*-1.0,arc2) ;
	_stprintf(buffer_two, TEXT("Grabbed box height = %d  "), height);
	_stprintf(buffer_three, TEXT("Arm tip height = %f  arcR = %f"), (dy*(-1.0)+y*(-1.0)), arcR);

	TextOut(hdc, 50, 50, buffer_one, 16+sizeof(arc)+sizeof(arc2));
	TextOut(hdc, 50, 100, buffer_two, 21+sizeof(height));
	TextOut(hdc, 50, 150, buffer_three, 39 + sizeof(dy));

}

void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea,std::fstream& hPlik)
{
	
		InvalidateRect(hWnd, drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc, hPlik);
	if (zapis == 1)
		hPlik << x << " " << y << " " << dx << " " << dy << " " << pudlo[0].x << " " << pudlo[0].y << " "
		<< pudlo[1].x << " " << pudlo[1].y << " "
		<< pudlo[2].x << " " << pudlo[2].y << " "
		<< pudlo[3].x << " " << pudlo[3].y << "\n";
	EndPaint(hWnd, &ps);
}

void sprawdz() {
	if (grab == 0) {
		for (int i = 0; i < 4; i++) {
			if (-pudlo[i].x + (x+dx+przesunieciex) < szpudlo && -pudlo[i].x + (x + dx + przesunieciex) > 0){
				if (-pudlo[i].y + (y + dy + przesunieciey) < 10 || -pudlo[i].y + (y + dy + przesunieciey) > -10) {
					grab = 1;
					grabpudlo = i;
					height = pudlo[i].h;
					zmianax = -pudlo[i].x + (x + dx + przesunieciex);
					zmianay = -pudlo[i].y + (y + dy + przesunieciey);
				}
			}
		}
	}
	else grab = 0;
}

void pozycja(int next)
{	
	arc3 = arc2 - arc;
	y = r * sin(arc * PI / 180);
	x = r * cos(arc * PI / 180);
	dy = r * cos(arc3 * PI / 180);
	dx = r * sin(arc3 * PI / 180);
	int wys;
	if (grab == 1) wys = pudlo[grabpudlo].h;
	else wys = 0;
	R = sqrt(pow(dx + x, 2) + pow(dy + y+ wys, 2));
	arcR = acos((dx+x) / R);
	if(dy+y > 0) arcR = (-1.0)*(arcR * 180) / PI;
	if(dy+y <= 0) arcR = (arcR * 180) / PI;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	

	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	HFONT myfont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	HFONT myfont1 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

	hInst = hInstance;


	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1500, 1000, NULL, NULL, hInstance, NULL);

	slider = CreateWindowEx(0, TRACKBAR_CLASS, TEXT("Slider"),
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TOOLTIPS | TBS_VERT,
		870, 110, 50, 200, hWnd, (HMENU)ID_SLIDER, NULL, NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Rec"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		920, 110, 80, 50, hWnd, (HMENU)ID_recBUTTON, GetModuleHandle(NULL), NULL);
	SendMessage(hwndButton, WM_SETFONT, WPARAM(myfont), TRUE);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Stop"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		920, 160, 80, 50, hWnd, (HMENU)ID_stopBUTTON, GetModuleHandle(NULL), NULL);
	SendMessage(hwndButton, WM_SETFONT, WPARAM(myfont), TRUE);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Delete"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		920, 210, 80, 50, hWnd, (HMENU)ID_delBUTTON, GetModuleHandle(NULL), NULL);
	SendMessage(hwndButton, WM_SETFONT, WPARAM(myfont), TRUE);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Start"),
		WS_CHILD | WS_VISIBLE,
		920, 260, 80, 50, hWnd, (HMENU)ID_startBUTTON, GetModuleHandle(NULL), NULL);
	SendMessage(hwndButton, WM_SETFONT, WPARAM(myfont), TRUE);

	SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG(10, 50));
	


	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	std::fstream hPlik;
	hPlik.open("C:\\Users\\oem\\Desktop\\dane.TXT", std::ios::in | std::ios::app);
	if (hPlik.good() == false)
	{
		MessageBox(NULL, TEXT("Nie mozna otworzyc pliku."), TEXT("Blad"), MB_ICONEXCLAMATION);
		PostQuitMessage(0);
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_recBUTTON:
			zapis = 1;
			repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			break;
		case ID_stopBUTTON:
			hPlik.seekg(0);
			zapis = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			break;
		case ID_startBUTTON:
			nagranie = 1;
			hPlik.seekg(0);
			while (hPlik) {
				repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
				Sleep(czas);
			}
			nagranie = 0;
			break;
		case ID_delBUTTON:
			hPlik.close();
			hPlik.open("C:\\Users\\oem\\Desktop\\dane.TXT", std::ios::out | std::ios::trunc);
			hPlik.close();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			for (int i = 0; i < 1+slidervalue; i++) {
				if (arcR < 179 && arc * (-1.0) < 180) {
					arc -= 1;
					pozycja(next);
				}
				repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			}
			break;
		case VK_RIGHT:
			for (int i = 0; i < 1+slidervalue; i++) {
				if (arcR > 1 && arc * (-1.0) > 0) {
					arc += 1;
					pozycja(next);
				}
				repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			}
			break;
		case VK_UP:
			for (int i = 0; i < 1 + slidervalue; i++) {
				if (arcR > 1 && arc2 > -85) {
				arc2 -= 1;
				pozycja(next);
				}
				repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			}
			break;
		case VK_DOWN:
			for (int i = 0; i < 1 + slidervalue; i++) {
				if (arcR < 179 && arc2 < 265) {
					arc2 += 1;
					pozycja(next);
				}
				repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			}
			break;
		case VK_SPACE:
			sprawdz();
			repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			if (grab == 0) {
				int p = 0;
				while (pudlo[grabpudlo].y < przesunieciey - pudlo[grabpudlo].h) {
					pudlo[grabpudlo].y++;
					p++;
					if (p % 4 == 0)
						repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
				}
				height = 0;
				repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
			}
			break;
		default:
			break;
	}
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_VSCROLL:
		slidervalue = SendMessage(slider, TBM_GETPOS, 0, 0);
		slidervalue = slidervalue / 10.0;
		czas = 20.0/slidervalue;
		break;
	case WM_LBUTTONDOWN:
		repaintWindow(hWnd, hdc, ps, &drawArea1, hPlik);
		SetFocus(hWnd);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

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
