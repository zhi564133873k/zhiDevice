#include"zhiDevice.h"
#include"mWindow.h"

zhiDevice *device=nullptr;
mWindow & mwindow = mWindow::Instance();
void DrawFunc();
void reSizeScreen();
void forward();
void backward();
float pos = 3;
float alpha = 0;
vector_c cube[8] = {
	vector_c(1,1,1),
	vector_c(-1,1,1),
	vector_c(-1,-1,1),
	vector_c(1,-1,1),
	vector_c(1,1,-1),
	vector_c(-1,1,-1),
	vector_c(-1,-1,-1),
	vector_c(1,-1,-1)
};

int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int		nCmdShow) {

	mwindow.Create3DWindow();

	device=new zhiDevice(mwindow.framebuffer,mwindow.GetWidth(),mwindow.GetHeight());
	device->insertSquare(cube[0], cube[1], cube[2], cube[3]);
	device->insertSquare(cube[7], cube[4], cube[0], cube[3]);
	device->insertSquare(cube[4], cube[5], cube[1], cube[0]);
	device->insertSquare(cube[1], cube[5], cube[6], cube[2]);
	device->insertSquare(cube[5], cube[4], cube[7], cube[6]);
	device->insertSquare(cube[3], cube[2], cube[6], cube[7]);
	device->setBackgroundColor(0xFFFFFF);
	mwindow.SetDisplayFunc(DrawFunc);
	mwindow.SetReSize(reSizeScreen);
	mwindow.SetKeyFunction(VK_UP, forward);
	mwindow.SetKeyFunction(VK_DOWN, backward);
	WindowsLoop();

	return 0;							// �˳�����
}

void DrawFunc() {
	if (mwindow.GetKey(VK_LEFT)) {
		alpha += 0.05f;
	} else {
		if (mwindow.GetKey(VK_RIGHT)) {
			alpha -= 0.05f;
		}
	}
	device->setLookAt(pos, pos, pos, 0, 0, 0, 0, 0,-1);
	matrix_c world;
	world.set_rotate(0, 0, 1, alpha);
	device->setWorld(world);
	device->drawFrames();
}

void reSizeScreen() {
	device->ReSizeScreen(mwindow.framebuffer,mwindow.GetWidth(),mwindow.GetHeight());
}

void forward() {
	pos -= 0.05f;
}
void backward() {
	pos += 0.05f;
}