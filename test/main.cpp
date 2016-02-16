#include"zhiDevice.h"
#include"mWindow.h"

zhiDevice *device=nullptr;
mWindow & mwindow = mWindow::Instance();
void DrawFunc();
void reSizeScreen();
void forward();
void backward();
float pos = 3.5;
float alpha = 1;
int cubeNo;
vertex cube[8] = {
	vertex(1,-1,1, 1.0, 0.2, 0.2),
	vertex(-1,-1,1,0.2, 1.0, 0.2),
	vertex(-1,1,1, 0.2, 0.2, 1.0),
	vertex(1,1,1,1.0, 0.2, 1.0),
	vertex(1,-1,-1,1.0, 1.0, 0.2),
	vertex(-1,-1,-1,0.2, 1.0, 1.0),
	vertex(-1,1,-1, 1.0, 0.3, 0.3),
	vertex(1,1,-1,0.2, 1.0, 0.3)
};

int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int		nCmdShow) {

	mwindow.Create3DWindow();

	device=new zhiDevice(mwindow.framebuffer,mwindow.GetWidth(),mwindow.GetHeight());
	cubeNo = device->newObject();
	device->insertSquare(cube[0], cube[1], cube[2], cube[3]);
	device->insertSquare(cube[4], cube[5], cube[6], cube[7]);
	device->insertSquare(cube[0], cube[4], cube[5], cube[1]);
	device->insertSquare(cube[1], cube[5], cube[6], cube[2]);
	device->insertSquare(cube[2], cube[6], cube[7], cube[3]);
	device->insertSquare(cube[3], cube[7], cube[4], cube[0]);
	device->setBackgroundColor(0xFFFFFF);
	mwindow.SetDisplayFunc(DrawFunc);
	mwindow.SetReSize(reSizeScreen);
	mwindow.SetKeyFunction(VK_UP, forward);
	mwindow.SetKeyFunction(VK_DOWN, backward);
	WindowsLoop();

	return 0;							// ÍË³ö³ÌÐò
}

void DrawFunc() {
	if (mwindow.GetKey(VK_LEFT)) {
		alpha += 0.1f;
	} else {
		if (mwindow.GetKey(VK_RIGHT)) {
			alpha -= 0.1f;
		}
	}
	if (mwindow.GetKey(VK_SPACE)) {
		//device->deleteObject(cubeNo);
	}
	device->setLookAt(pos, 0, 0, 0, 0, 0, 0, 0,1);
	matrix_c world;
	world.set_rotate(-1, -0.5, 1, alpha);
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