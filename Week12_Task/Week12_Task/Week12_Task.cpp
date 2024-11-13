// Week12_Task.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Week12_Task.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

bool leftBtn = false;                               //마우스 왼쪽 버튼이 눌렸는지
int g_x, g_y;                                           //마우스의 이전 좌표를 저장하는 변수
int g_drawType = MODE_STRAIGHT;                              //그리기 모드를 설정하는 변수 (기본 : 직선모드)

HDC hdc;                                                //Device Context를 핸들링하는 변수

HPEN myPen, osPen;                              //이전, 현재 선색을 저장하는 변수
HBRUSH myBrush, osBrush;                    //이전, 현재 면색을 저장하는 변수
COLORREF faceColor = WHITE, lineColor = BLACK;             //설정한 면색과 선색을 저장하는 변수

int lineWidth = 5;                                     //선 굵기 변수


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
	LoadStringW(hInstance, IDC_WEEK12TASK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEEK12TASK));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEEK12TASK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WEEK12TASK);
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	switch (message)
	{
	case WM_USER + 1:
		g_drawType = lParam;
		break;
	case WM_USER + 2:
		switch (lParam)
		{
		case FACE_WHITE:
			faceColor = WHITE;
			break;
		case FACE_BLACK:
			faceColor = BLACK;
			break;
		case FACE_RED:
			faceColor = RED;
			break;
		case FACE_BLUE:
			faceColor = BLUE;
			break;
		case FACE_GREEN:
			faceColor = GREEN;
			break;
		}
		break;
	case WM_USER + 3:
		switch (lParam)
		{
		case LINE_SKYBLUE:
			lineColor = SKYBLUE;
			break;
		case LINE_BLACK:
			lineColor = BLACK;
			break;
		case LINE_RED:
			lineColor = RED;
			break;
		case LINE_BLUE:
			lineColor = BLUE;
			break;
		case LINE_GREEN:
			lineColor = GREEN;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		leftBtn = true;
		hdc = GetDC(hWnd);

		myPen = CreatePen(PS_SOLID, lineWidth, lineColor);
		osPen = (HPEN)SelectObject(hdc, myPen);

		myBrush = CreateSolidBrush(faceColor);
		osBrush = (HBRUSH)SelectObject(hdc, myBrush);

		g_x = x;
		g_y = y;

		if (g_drawType == MODE_STRAIGHT) {
			MoveToEx(hdc, g_x, g_y, NULL);
		}

		break;

	case WM_MOUSEMOVE:

		if (leftBtn) {

			if (g_drawType == MODE_FREE) {
				MoveToEx(hdc, g_x, g_y, NULL);
				LineTo(hdc, x, y);
				g_x = x;
				g_y = y;
			}

		}
		break;

	case WM_LBUTTONUP:
		leftBtn = false;

		if (g_drawType == MODE_STRAIGHT) {
			LineTo(hdc, x, y);
		}
		else if (g_drawType == MODE_RECT) {
			Rectangle(hdc, g_x, g_y, x, y);
		}
		else if (g_drawType == MODE_ELLIPSE) {
			Ellipse(hdc, g_x, g_y, x, y);
		}


		DeleteObject(myPen);
		DeleteObject(myBrush);

		ReleaseDC(hWnd, hdc);

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
