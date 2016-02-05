#include<vector>
#include<utility>
class zhiDevice {
public:
	zhiDevice(unsigned int **framebuffer, int width, int height) :framebuffer(framebuffer), width(width), height(height) {};
	zhiDevice(unsigned int **framebuffer) :zhiDevice(framebuffer, 800, 600) {};
	zhiDevice() :zhiDevice(nullptr, 800, 600) {};

	void drawFrames() {
		if (framebuffer==nullptr) {
			return;
		}
		drawBackGround();

		drawWire();
	}

	void ReSizeScreen(unsigned int **framebuffer, int width, int height) {
		zhiDevice::width = width;
		zhiDevice::height = height;
		zhiDevice::framebuffer = framebuffer;
	}

	void insertPoint(int x, int y) {
		point.emplace_back(x, y);
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

	void setBackgroundColor(unsigned int color) {
		backgroundColor = color;
	}

private:
	int width, height;
	unsigned int **framebuffer = nullptr;

	unsigned int backgroundColor = 0x000000;
	std::vector<std::pair<int, int>> point;

	void drawBackGround() {
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				drawPixel(i, j, backgroundColor);
			}
		}
	}


	void drawWire() {
		drawLine(100, 100, 700, 500);
		drawLine(100, 500, 700, 100);
		drawLine(100, 100, 500, 700);
		drawLine(500, 100, 100, 700);
	}


	void drawLine(int x1, int y1, int x2, int y2) {
		drawLine(x1, y1, x2, y2, 0x000000);
	}

	void drawLine(int x1,int y1,int x2,int y2,unsigned int color) {
		double k = double(y2 - y1) / (x2 - x1), d;
		if (!CohenSutherland(x1, y1, x2, y2,k)) {
			return;
		}	
		int start, end, x, y;
		bool xSym=false,xySym=false;
		if (k<0) {
			xSym = true;
		}
		if (k > 1 || k < -1) {
			xySym = true;
		}
		if (xSym) {
			x1 = -x1;
			x2 = -x2;
		}
		if (xySym) {
			std::swap(x1, y1);
			std::swap(x2, y2);
		}
		k = double(y2 - y1) / (x2 - x1);
		d = k - 0.5;
		if (x1>x2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		} 
		start = x1;
		y = y1;
		end = x2;
		for (x = start; x != end; ++x) {
			if (d > 0) {
				d += (k - 1);
				y += 1;
			} else {
				d += k;
			}
			if (xSym) {
				if (xySym) {
					drawPixel(-y, x, color);
				} else {
					drawPixel(-x, y, color);
				}
			} else {
				if (xySym) {
					drawPixel(y, x, color);
				} else {
					drawPixel(x, y, color);
				}
			}
		}
	}

	bool CohenSutherland(int& x1, int& y1, int& x2, int& y2,double k) {
		int p1code = getConhenCode(x1, y1);
		int p2code = getConhenCode(x2, y2);
		if (p1code | p2code == 0) {
			return true;
		} else if (p1code&p2code != 0) {
			return false;
		}
		changeCohenXY(x1, y1, p1code, k);
		changeCohenXY(x2, y2, p2code, k);
		return true;
	}

	void changeCohenXY(int& x, int& y,int code, double k) {
		int xTemp = x, yTemp = y ,height=(zhiDevice::height-1),width=(zhiDevice::width-1);
		switch (code) {
		case 8: // N  
			x = xTemp + (height - yTemp) / k + 0.5;
			y = height;
			break;
		case 4: // S  
			x = xTemp + (-yTemp) / k + 0.5;
			y = 0;
			break;
		case 1: // W  
			y = yTemp + (-xTemp)*k + 0.5;
			x = 0;
			break;
		case 2: // E  
			y = yTemp + (width - xTemp) * k + 0.5;
			x = width;
			break;
		case 9: // NW  			
			xTemp = xTemp + (height - yTemp) / k + 0.5;
			yTemp = height;
			if (xTemp < 0 || xTemp > width) {
				x = 0;
				y = yTemp + (-xTemp)*k + 0.5;
			} else {
				x = xTemp;
				y = yTemp;
			}
			break;
		case 10: // NE  		
			xTemp = xTemp + (height - yTemp) / k + 0.5;
			yTemp = height;
			if (xTemp < 0 || xTemp > width) {
				x = width;
				y = yTemp + (width - xTemp)*k + 0.5;
			} else {
				x = xTemp;
				y = yTemp;
			}
			break;
		case 6: // SE 			
			xTemp = xTemp + (-yTemp) / k + 0.5;
			yTemp = 0;
			if (xTemp < 0 || xTemp > width) {
				x = width;
				y = yTemp + (width - xTemp) * k + 0.5;
			} else {
				x = xTemp;
				y = yTemp;
			}
			break;
		case 5: // SW  			
			xTemp = xTemp + (0 - yTemp) / k + 0.5;
			yTemp = 0;
			if (xTemp < 0 || xTemp > width) {
				x = 0;
				y = yTemp + (-xTemp)*k + 0.5;
			} else {
				x = xTemp;
				y = yTemp;
			}
			break;
		default:
			break;
		}
	}

	int getConhenCode(int x, int y) {
		int code=0;
		if (y>height) {
			code |= 8;
		} else if (y<0) {
			code |= 4;
		}
		if (x>width) {
			code |= 2;
		} else if (x<0) {
			code |= 1;
		}
		return code;
	}


	void drawPixel(int x,int y,unsigned int color) {
			framebuffer[(height-y-1)][x] = color;
	}
};