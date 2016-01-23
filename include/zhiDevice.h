class zhiDevice {
public:
	zhiDevice(unsigned int **framebuffer, int width, int height) :framebuffer(framebuffer), width(width), height(height) {};
	zhiDevice(unsigned int **framebuffer) :zhiDevice(framebuffer, 800, 600) {};
	zhiDevice() :zhiDevice(nullptr, 800, 600) {};

	void drawFrames() {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				framebuffer[i][j] = 0x787878;
			}
		}
	}

	void ReSizeScreen(unsigned int **framebuffer, int width, int height) {
		zhiDevice::width = width;
		zhiDevice::height = height;
		zhiDevice::framebuffer = framebuffer;
	}

	void setWidth(int width) {
		zhiDevice::width = width;
	}

	void setHeight(int height) {
		zhiDevice::height = height;
	}

	void setFrameBuffer(unsigned int **framebuffer) {
		zhiDevice::framebuffer = framebuffer;
	}
private:
	int width, height;
	unsigned int **framebuffer = nullptr;

	//unsigned int 
	
};