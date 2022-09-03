#include "SpriteRenderWindow.h"
#include <windowsx.h>
#include "SpriteUtils.h"
#include <thread>



namespace SpriteFrameWork
{
	// 全局变量:
	HINSTANCE g_hInst = NULL;

	SpriteRenderWindow::SpriteRenderWindow(HINSTANCE hInstance,unsigned int width,unsigned int height, COLORREF keycolor)
	{
		if (hInstance)
			m_hIns = hInstance;
		else
			m_hIns = GetModuleHandle(NULL);

		m_width = width;
		m_height = height;
		m_TransparantKeyColor = keycolor;

		std::wstring classname = L"SpriteRenderWindow";
		std::wstring title = L"Sprite1.0";


		MyRegisterClass(classname);

		InitInstance(classname, title);


	}

	SpriteRenderWindow::~SpriteRenderWindow()
	{

	}

	LRESULT CALLBACK SpriteRenderWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		SpriteRenderWindow* pSpriteWindow = nullptr;

		if (message == WM_CREATE) {
			pSpriteWindow = (SpriteRenderWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			pSpriteWindow->m_hWnd = hWnd;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pSpriteWindow);
		}
		else {
			pSpriteWindow = (SpriteRenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}

		if (!pSpriteWindow) return DefWindowProc(hWnd, message, wParam, lParam);

		switch (message)
		{


			HANDLE_MSG(hWnd, WM_CREATE, pSpriteWindow->OnCreate);

			HANDLE_MSG(hWnd, WM_LBUTTONDOWN, pSpriteWindow->OnLButtonDown);
			HANDLE_MSG(hWnd, WM_LBUTTONUP, pSpriteWindow->OnLButtonUp);
			HANDLE_MSG(hWnd, WM_MOUSEMOVE, pSpriteWindow->OnMouseMove);
			//HANDLE_MSG(hWnd, WM_RBUTTONUP, pSpriteWindow->OnRButtonUp);
			HANDLE_MSG(hWnd, WM_PAINT, pSpriteWindow->OnPaint);

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	HDC hdc = BeginPaint(hWnd, &ps);
		//	// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//	EndPaint(hWnd, &ps);
		//}
	/*	break;*/
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}


	ATOM SpriteRenderWindow::MyRegisterClass(std::wstring classname)
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hIns;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = classname.c_str();
		wcex.hIconSm = NULL;

		return RegisterClassExW(&wcex);
	}

	BOOL SpriteRenderWindow::InitInstance(std::wstring classname, std::wstring title)
	{
		extern HINSTANCE g_hInst;
		g_hInst = m_hIns; // 将实例句柄存储在全局变量中

		HWND hWnd = CreateWindowW(classname.c_str(), title.c_str(),WS_POPUP| WS_CLIPCHILDREN| WS_CLIPSIBLINGS|WS_CAPTION,
			0, 0, m_width, m_height, nullptr, nullptr, m_hIns, this);

		if (!hWnd)
		{
			m_hWnd = NULL;
			return FALSE;
		}

		m_hWnd = hWnd;

		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		return TRUE;
	}

	int SpriteRenderWindow::Loop()
	{
		MSG msg;

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		return 0;
	}


	void SpriteRenderWindow::OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
	{
		if (!fDoubleClick) {
			SetCapture(hwnd);
			m_leftpress = TRUE;
			m_mouseX = x;
			m_mouseY = y;

		}


	}
	void SpriteRenderWindow::OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
	{
		if (m_leftpress) {
			m_leftpress = FALSE;
			m_mouseX = 0;
			m_mouseY = 0;
			ReleaseCapture();  
		}

	}
	void SpriteRenderWindow::OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
	{
		if (m_leftpress) {
			RECT winRect = { 0 };
			GetWindowRect(hwnd, &winRect);
			int cx = winRect.left + (x - m_mouseX);
			int cy = winRect.top + (y - m_mouseY);
			MoveWindow(hwnd, cx, cy, winRect.right - winRect.left, winRect.bottom - winRect.top, TRUE);	
		}
	}

	HBITMAP gCurrentBitMap = NULL;
	std::thread* t1 = NULL;
	bool bQuit = false;

	BOOL SpriteRenderWindow::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED /*| WS_EX_TOOLWINDOW*/ | WS_EX_TOPMOST);
		SetLayeredWindowAttributes(hWnd, m_TransparantKeyColor, 255, LWA_COLORKEY);   // LWA_ALPHA | LWA_COLORKEY
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	

		std::thread* t1 = new std::thread([&]() {
		
		
	   const char* files[] = {"25.bmp","26.bmp","27.bmp","28.bmp"};

        int i = 0;

			while (!bQuit)
			{

				std::this_thread::sleep_for(std::chrono::seconds(1));

				int index = i % 4;

				std::string testfile = SpriteFrameWork::Utils::GetRootDir() + "\\actors\\son\\act1\\" + files[index];

				gCurrentBitMap = (HBITMAP)LoadImageA(g_hInst, testfile.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);


				::InvalidateRect(m_hWnd, NULL, TRUE);
				//::InvalidateWindow(hwnd, NULL, TRUE);
				UpdateWindow(m_hWnd);




				i++;
		    
			
			}
		
		
		
		});





		return TRUE;
	}
	
	void SpriteRenderWindow::OnPaint(HWND hWnd)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rc;

		GetClientRect(hWnd, &rc);
		HBRUSH brush = CreateSolidBrush(m_TransparantKeyColor);
		FillRect(hdc, &rc, brush);

		DeleteObject(brush);


		UpdateFrame(hdc);


		



		EndPaint(hWnd, &ps);


	}



	void SpriteRenderWindow::UpdateFrame(HDC hdc)
	{

		//std::string testfile =SpriteFrameWork::Utils::GetRootDir() + "\\actors\\son\\act1\\25.bmp";

		//gCurrentBitMap = (HBITMAP)LoadImageA(g_hInst, testfile.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (gCurrentBitMap == NULL) return;

		BITMAP bm = { 0 };
		GetObject(gCurrentBitMap, sizeof(bm), &bm);

		HDC hMemdc = CreateCompatibleDC(hdc);
		//SelectBitmap(hMemdc, hBitmap);

		HBITMAP hBitmapOld = (HBITMAP)SelectObject(hMemdc, gCurrentBitMap);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hMemdc, 0, 0, SRCCOPY);
		SelectObject(hMemdc, hBitmapOld);

		DeleteObject(gCurrentBitMap);

		gCurrentBitMap = NULL;

		DeleteDC(hMemdc);

	}




}
