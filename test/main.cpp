#include"zhiDevice.h"
#include"zhiWindow.h"
#include"LoadTexture.h"

class Main :winApp {
public:
	void initialization() {
		window=(zhiWindow *)WindowManager::pointer();	
		device = new zhiDevice(window->getFrameBuffer(), window->GetWidth(), window->GetHeight());
	}

	void initData() {
	
		WindowManager::Instance().setFPS(30);
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
		device->activeLight(Light0);
		SetKeyFunction(VK_UP, Main::forward);
		SetKeyFunction(VK_DOWN, Main::backward);
		SetKeyFunction(VK_LEFT, Main::left);
		SetKeyFunction(VK_RIGHT, Main::right);
		SetKeyFunction(VK_SPACE, Main::space);
	}

	void ReSizeScene() {
		device->ReSizeScreen(window->getFrameBuffer(), window->GetWidth(), window->GetHeight());
	}

	static void forward() {
		pos -= 0.05f;
	}
	static void backward() {
		pos += 0.05f;
	}
	static void left() {
		alpha += 0.1f;
	}
	static void right() {
		alpha -= 0.1f;
	}
	static void space() {
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

	void DrawScene() {
		device->setLookAt(pos, pos, pos, 0, 0, 0, 0, 1, 0);
		matrix_c world, temp;
		world.set_rotate(0, 1, 0, alpha);
		//temp.set_rotate();
		device->setWorld(world);
		device->drawFrames();
	}

private:
	zhiWindow * window;
	static zhiDevice * device;
	unsigned int textureNo = 0;
	static float pos;
	static float alpha;
	static int cubeNo;

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
		textureNo = device->createMap(texture, sizeof(unsigned int), 256, 256);
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
		insertModel(cube[0], cube[3], cube[2], cube[1], textureNo, 0x010101);
		insertModel(cube[4], cube[5], cube[6], cube[7], textureNo, 0x010101);
		insertModel(cube[0], cube[1], cube[5], cube[4], textureNo, 0x010101);
		insertModel(cube[1], cube[2], cube[6], cube[5], textureNo, 0x010101);
		insertModel(cube[2], cube[3], cube[7], cube[6], textureNo, 0x010101);
		insertModel(cube[3], cube[0], cube[4], cube[7], textureNo, 0x010101);
	}
};

zhiDevice * Main::device = nullptr;
float Main::pos = 2;
float Main::alpha = 0;
int Main::cubeNo = 0;


Main main;