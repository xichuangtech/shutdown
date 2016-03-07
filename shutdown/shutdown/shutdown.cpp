// systray.cpp : Defines the exported functions for the DLL application.
//

// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "shutdown.h"

// Message posted into message loop when Notification Icon is clicked
#define WM_SYSTRAY_MESSAGE (WM_USER + 1)

static HWND hWnd;

BOOL(*_shutdown_query_endsession)();
BOOL(*_shutdown_endsession)();


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		ShutdownBlockReasonCreate(hWnd, L"正在保存上传信息...");
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_QUERYENDSESSION:
		return _shutdown_query_endsession();
	case WM_ENDSESSION:
	{
		BOOL  ret = _shutdown_endsession();
		ShutdownBlockReasonDestroy(hWnd);
		return ret;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MyRegisterClass(HINSTANCE hInstance, TCHAR* szWindowClass) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow, TCHAR* szWindowClass) {
	HWND hWnd = CreateWindow(szWindowClass, TEXT(""), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}


int nativeLoop(void(*shutdown_ready)(int ignored), BOOL(*shutdown_query_endsession)(), BOOL(*shutdown_endsession)()) {
	_shutdown_query_endsession = shutdown_query_endsession;
	_shutdown_endsession = shutdown_endsession;

	HINSTANCE hInstance = GetModuleHandle(NULL);
	TCHAR* szWindowClass = TEXT("ShutdownClass");
	MyRegisterClass(hInstance, szWindowClass);
	hWnd = InitInstance(hInstance, FALSE, szWindowClass); // Don't show window
	if (!hWnd) {
		return EXIT_FAILURE;
	}
	shutdown_ready(0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return EXIT_SUCCESS;
}

void  quit()
{
	DestroyWindow(hWnd);
}