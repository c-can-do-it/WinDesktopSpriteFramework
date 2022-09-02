// WinSprite.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SpriteRenderWindow.h"

using namespace SpriteFrameWork;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
 
	SpriteRenderWindow renderWindow(hInstance);

	renderWindow.Loop();
	

    return 0;
}


