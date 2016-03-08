#include"zhiDevice.h"
#include"mWindow.h"
#include"LoadTexture.h"
zhiDevice *device=nullptr;
mWindow & mwindow = mWindow::Instance();
unsigned int textureNo=0;
void DrawFunc();
void reSizeScreen();
void forward();
void backward();
float pos = 2;
float alpha = 0;
int cubeNo;
vertex cube[8] = {
	vertex(1 ,-1, 1, 1.0, 0.2, 0.2),
	vertex(-1,-1, 1, 0.2, 1.0, 0.2),
	vertex(-1, 1, 1, 0.2, 0.2, 1.0),
	vertex(1 , 1, 1, 1.0, 0.2, 1.0),
	vertex(1 ,-1,-1, 1.0, 1.0, 0.2),
	vertex(-1,-1,-1, 0.2, 1.0, 1.0),
	vertex(-1, 1,-1, 1.0, 0.3, 0.3),
	vertex(1 , 1,-1, 0.2, 1.0, 0.3)
};

void init_texture() {
	static unsigned int texture[256][256];
	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}
	textureNo=device->createMap(texture,sizeof(unsigned int), 256, 256);
};

void insertModel(vertex& p1, vertex& p2, vertex& p3, vertex& p4, unsigned int textureNo) {
	vertex t1 = p1, t2 = p2, t3 = p3, t4 = p4;
	t1.tc.u = 0, t1.tc.v = 0, t2.tc.u = 0, t2.tc.v = 1;
	t3.tc.u = 1, t3.tc.v = 1, t4.tc.u = 1, t4.tc.v = 0;
	device->insertSquare(t1, t2, t3, t4, textureNo);
}

void insertModel(vertex& p1, vertex& p2, vertex& p3, vertex& p4, unsigned int textureNo, unsigned int color) {
	vertex t1 = p1, t2 = p2, t3 = p3, t4 = p4;
	t1.tc.u = 0, t1.tc.v = 0, t2.tc.u = 0, t2.tc.v = 1;
	t3.tc.u = 1, t3.tc.v = 1, t4.tc.u = 1, t4.tc.v = 0;
	t1.color = Color(color);
	t2.color = Color(color);
	t3.color = Color(color);
	t4.color = Color(color);
	device->insertSquare(t1, t2, t3, t4, textureNo);
}

void initObject() {
	cubeNo = device->newObject();
	insertModel(cube[0], cube[3], cube[2], cube[1], textureNo);
	insertModel(cube[4], cube[5], cube[6], cube[7], textureNo);
	insertModel(cube[0], cube[1], cube[5], cube[4], textureNo);
	insertModel(cube[1], cube[2], cube[6], cube[5], textureNo);
	insertModel(cube[2], cube[3], cube[7], cube[6], textureNo);
	insertModel(cube[3], cube[0], cube[4], cube[7], textureNo);
}

void init() {
	mwindow.setFPS(30);
	textureNo = LoadTexture(L"1.BMP", *device);
	if (textureNo == -1) {
		init_texture();
	}
	initObject();
	device->setBGColor(0xFFFFFF);
	device->setRenderState(cubeNo, MAPPING);
	device->setCullBack(true);
	//device->setCVVCheck(true);
	device->setLightPosition(Light0, vector_c(4, 7, 4));
	//device->setLight(Light0, AMBIENT, Color(0xFF0000));
	device->setLight(Light0, DIFFUSE, Color(0x00FF00));
	//device->activeLight(Light0);
	mwindow.SetDisplayFunc(DrawFunc);
	mwindow.SetReSize(reSizeScreen);
	mwindow.SetKeyDownEvent(VK_UP, forward);
	mwindow.SetKeyDownEvent(VK_DOWN, backward);
}

int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int		nCmdShow) {

	mwindow.Create3DWindow();
	
	device=new zhiDevice(mwindow.framebuffer,mwindow.GetWidth(),mwindow.GetHeight());
	
	init();

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
		static render_state state = MAPPING;
		if (state == COLOR) {
			state = MAPPING;
		} else if (state == MAPPING) {
			state = WIREFRAME;
		} else if (state == WIREFRAME) {
			state = COLOR;
		}
		device->setRenderState(cubeNo, state);
	}
	device->setLookAt(pos, pos, pos, 0, 0, 0, 0, 1, 0);
	matrix_c world,temp;
	world.set_rotate(0, 1, 0, alpha);
	//temp.set_rotate();
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