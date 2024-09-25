// Week5_HW.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Week5_HW.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WEEK5HW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEEK5HW));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEEK5HW));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WEEK5HW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

HPEN nPen, oPen;

WCHAR text[30];
WCHAR record[10];

RECT playArea;

RECT player = { 100, 100, 150, 150 };

RECT com = { 700, 500, 750, 550 };

RECT itemBox = { 200, 200, 235, 235 };

RECT clientArea;

RECT intersec;

int comTime = 500;

MINMAXINFO* minMaxInf;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static double liveTime = 0.00;
	static int score = 0;

	switch (message)
	{
	case WM_CREATE:

		srand(time(NULL));

		SetTimer(hWnd, CLOCK, 10, NULL);
		SetTimer(hWnd, COM_MOVE, comTime, NULL);
		SetTimer(hWnd, COM_FAST, 5000, NULL);

		GetClientRect(hWnd, &clientArea);

		playArea.left = clientArea.left + GROUND_MARGIN;
		playArea.right = clientArea.right - GROUND_MARGIN;
		playArea.top = clientArea.top + GROUND_TOP;
		playArea.bottom = clientArea.bottom - GROUND_MARGIN;
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case CLOCK:
			liveTime += 0.01;
			InvalidateRect(hWnd, NULL, TRUE);


			break;

		case ITEM_SPAWN:
			do
			{
				itemBox.left = (rand() % playArea.right - 40) + 30;
				itemBox.top = (rand() % playArea.bottom - 40) + 30;
				itemBox.right = itemBox.left + 35;
				itemBox.bottom = itemBox.top + 35;
			} while (itemBox.left < playArea.left || itemBox.right > playArea.right || itemBox.top < playArea.top || itemBox.bottom > playArea.bottom);

			KillTimer(hWnd, ITEM_SPAWN);
			break;

		case COM_FAST:

			if (comTime <= 50)
			{
				KillTimer(hWnd, COM_FAST);
				SetTimer(hWnd, PLAY_ZONE, 10000, NULL);
					break;
			}
			comTime -= 50;
			KillTimer(hWnd, COM_MOVE);
			SetTimer(hWnd, COM_MOVE, comTime, NULL);
			break;

		case COM_MOVE:
			if (player.left < com.left)
			{
				com.left -= COM_SPEED;
				com.right -= COM_SPEED;
			}
			else {
				com.left += COM_SPEED;
				com.right += COM_SPEED;
			}

			if (player.top < com.top)
			{
				com.top -= COM_SPEED;
				com.bottom -= COM_SPEED;
			}
			else {
				com.top += COM_SPEED;
				com.bottom += COM_SPEED;
			}

			if (IntersectRect(&intersec, &com, &player))
			{
				KillTimer(hWnd, CLOCK);
				KillTimer(hWnd, ITEM_SPAWN);
				KillTimer(hWnd, COM_FAST);
				KillTimer(hWnd, COM_MOVE);
				KillTimer(hWnd, PLAY_ZONE);
				_stprintf_s(text, L"기록 : %.2f 초\n먹은 구체 : %d 개", liveTime, score);
				MessageBox(hWnd, text, L"게임 오버", MB_OK);

				/// 프로그램 종료
				PostQuitMessage(0);
			}
			break;
		case PLAY_ZONE:

			if (playArea.right < 300 || playArea.bottom < 200)
			{
				KillTimer(hWnd, PLAY_ZONE);
				break;
			}

			itemBox.left = 1500;
			itemBox.top = 1500;
			itemBox.right = itemBox.left + 35;
			itemBox.bottom = itemBox.top + 35;

			KillTimer(hWnd, ITEM_SPAWN);
			playArea.right = (int)(playArea.right * 0.8);
			playArea.bottom = (int)(playArea.bottom * 0.8);
			SetTimer(hWnd, ITEM_SPAWN, 3000, NULL);
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)     /// 키보드 부가 정보는 WPARAM
		{
		case VK_LEFT:
			player.left -= PLAYER_SPEED;
			player.right -= PLAYER_SPEED;
			break;

		case VK_RIGHT:
			player.left += PLAYER_SPEED;
			player.right += PLAYER_SPEED;
			break;

		case VK_UP:
			player.top -= PLAYER_SPEED;
			player.bottom -= PLAYER_SPEED;
			break;

		case VK_DOWN:
			player.top += PLAYER_SPEED;
			player.bottom += PLAYER_SPEED;
			break;
		}

		/// 플레이 영역 판단
		if (player.left <= playArea.left)
		{
			player.left = playArea.left;
			player.right = playArea.left + 50;
		}
		if (player.right >= playArea.right)
		{
			player.right = playArea.right;
			player.left = player.right - 50;
		}
		if (player.top <= playArea.top)
		{
			player.top = playArea.top;
			player.bottom = player.top + 50;
		}
		if (player.bottom >= playArea.bottom)
		{
			player.bottom = playArea.bottom;
			player.top = player.bottom - 50;
		}

		/// 아이템 충돌 판정
		if (IntersectRect(&intersec, &itemBox, &player))
		{
			score++;

			/// 아이템 충돌 판정시 안보이는 곳으로 이동

			itemBox.left = 1500;
			itemBox.top = 1500;
			itemBox.right = itemBox.left + 35;
			itemBox.bottom = itemBox.top + 35;
			/// 3초 후 랜덤한 곳으로 스폰
			SetTimer(hWnd, ITEM_SPAWN, 3000, NULL);

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		/// 화면 초기화
		InvalidateRect(hWnd, &playArea, TRUE);
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
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
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		SIZE textSize;


		Rectangle(hdc, playArea.left, playArea.top, playArea.right, playArea.bottom);
		Rectangle(hdc, player.left, player.top, player.right, player.bottom);
		Rectangle(hdc, com.left, com.top, com.right, com.bottom);

		nPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		oPen = (HPEN)SelectObject(hdc, nPen);

		Ellipse(hdc, itemBox.left, itemBox.top, itemBox.right, itemBox.bottom);

		SelectObject(hdc, oPen);
		DeleteObject(nPen);

		wsprintf(text, L"202307034 김형균");
		GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);
		TextOut(hdc, clientArea.right - (textSize.cx + 15), 15, text, lstrlen(text));

		wsprintf(text, L"5주차 과제");

		GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);
		TextOut(hdc, clientArea.right - (textSize.cx + 15), textSize.cy + 30, text, lstrlen(text));

		_stprintf_s(text, L"현재 기록 : %.2f 초", liveTime);
		TextOut(hdc, 15, 15, text, lstrlen(text));

		wsprintf(text, L"먹은 구체 : %d 개", score);
		TextOut(hdc, 15, 40, text, lstrlen(text));

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

// 정보 대화 상자의 메시지 처리기입니다.
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