#include"zhiDevice.h"
#include"mWindow.h"

zhiDevice *device=nullptr;
mWindow & mwindow = mWindow::Instance();
void DrawFunc();
void reSizeScreen();

int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int		nCmdShow) {

	mwindow.Create3DWindow();

	device=new zhiDevice(mwindow.framebuffer,mwindow.GetWidth(),mwindow.GetHeight());
	device->insertPoint(1, 1);
	device->insertPoint(1, -1);
	device->insertPoint(-1, -1);
	device->insertPoint(-1, 1);
	device->setBackgroundColor(0xFFFFFF);
	mwindow.SetDisplayFunc(DrawFunc);
	mwindow.SetReSize(reSizeScreen);
	WindowsLoop();

	return 0;							// ÍË³ö³ÌÐò
}

void DrawFunc() {
	device->drawFrames();
}

void reSizeScreen() {
	device->ReSizeScreen(mwindow.framebuffer,mwindow.GetWidth(),mwindow.GetHeight());
}