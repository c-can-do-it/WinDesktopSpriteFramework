#pragma once
#include <string>
#include <Windows.h>




namespace SpriteFrameWork
{


class SpriteRenderWindow
{
private:
	HINSTANCE m_hIns;
	unsigned int m_width, m_height;
	COLORREF m_TransparantKeyColor;
	HWND m_hWnd;
	bool m_leftpress = false;

	int m_mouseX;
	int m_mouseY;
public:
	SpriteRenderWindow(HINSTANCE hInstance = NULL,unsigned int width=300,unsigned int height=300, COLORREF keycolor=RGB(0,0,0));
	~SpriteRenderWindow();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int Loop();

private:
	ATOM MyRegisterClass( std::wstring classname);
	BOOL InitInstance(std::wstring classname,std::wstring title);
protected:
	void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
	BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	//void OnRButtonUp(HWND hWnd, int x, int y, UINT flags);
	//void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void OnPaint(HWND hWnd);
	void UpdateFrame(HDC hdc);
	

};


}
