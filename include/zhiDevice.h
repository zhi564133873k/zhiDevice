#pragma once
#include<vector>
#include<utility>
#include<map>
#include"zhi_matrix.h"
#include"figure.h"


class camera {
public:
	matrix_c tranMatrix;

	camera(int, int);
	void update();
	void lookat(const vector_c&, const vector_c&, const vector_c&);
	void lookat(float, float, float, float, float, float, float, float, float);
	void setWidth(int);
	void setHeight(int);
	void setAspect(int, int);
	void setWorld(matrix_c);

private:
	matrix_c world;
	matrix_c view;
	matrix_c projection;
	int width, height;

	void set_perspective(float, float, float, float);
	void set_perspective();
};

class zhiDevice {
public:
	zhiDevice(unsigned int **framebuffer, int width, int height) :framebuffer(framebuffer), width(width), height(height), camera(width,height){
		zbuf = (char*)malloc(sizeof(float*) * height + width * sizeof(float) * height+64);
		zbuffer = (float**)zbuf;
		for (int i = 0; i < height; ++i) {
			zbuffer[i] = (float*)(zbuf+sizeof(float*) * height + width * sizeof(float)* i);
		}
	};
	zhiDevice(unsigned int **framebuffer) :zhiDevice(framebuffer, 800, 600) {};
	zhiDevice() :zhiDevice(nullptr, 800, 600) {};

	~zhiDevice() {
		free(zbuf);
	}

	void drawFrames() {
		if (framebuffer == nullptr) {
			return;
		}
		drawBackGround();
		for (auto obj : objects) {
			for (auto ite : obj.second) {
				drawplane(projection(ite.p1), projection(ite.p2), projection(ite.p3));
				//drawWire(projection(ite.p1.point), projection(ite.p2.point), projection(ite.p3.point));
			}
		}
	}

	void clear() {
		objects.clear();
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				zbuffer[i][j] = -1.0;
			}
		}
	}

	void setLookAt(const vector_c & eye, const vector_c & at, const vector_c & up) {
		camera.lookat(eye, at, up);
	}

	void setLookAt(float eyex, float eyey, float eyez, float atx, float aty, float atz, float upx, float upy, float upz) {
		camera.lookat(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz);
	}

	void setWorld(matrix_c world) {
		camera.setWorld(world);
	}

	void ReSizeScreen(unsigned int **framebuffer, int width, int height) {
		zhiDevice::width = width;
		zhiDevice::height = height;
		zhiDevice::framebuffer = framebuffer;
		camera.setAspect(width,height);
		free(zbuf);
		zbuf = (char*)malloc(sizeof(float*) * height + width * sizeof(float) * height + 64);
		zbuffer = (float**)zbuf;
		for (int i = 0; i < height; ++i) {
			zbuffer[i] = (float*)(zbuf + sizeof(float*) * height + width * sizeof(float)* i);
		}
	}

	int newObject() {
		int No = MaxObjectNo;
		if (ifObjectExist(No)) {
			No = -1;		
		} else {
			objectNo = No;
			++MaxObjectNo;
		}
		return No;
	}

	bool deleteObject(unsigned int objNo) {
		return objects.erase(objectNo);
	}

	bool ifObjectExist(unsigned int objNo) {
		return objects.count(objNo);
	}

	bool activeObject(unsigned int objNo) {
		if (ifObjectExist(objNo)) {
			objectNo = objNo;
		} else {
			return false;
		}
	}

	int insertSquare(vector_c& p1, vector_c& p2, vector_c& p3, vector_c& p4) {
		return insertSquare(p1, p2, p3, p4, 0x000000);
	}

	int insertSquare(vector_c& p1, vector_c& p2, vector_c& p3, vector_c& p4, unsigned int color) {
		insertTriangle(p1, p2, p3, color);
		insertTriangle(p3, p4, p1, color);
		return objectNo;
	}

	int insertSquare(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4) {
		return insertSquare(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, 0x000000);
	}

	int insertSquare(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, unsigned int color) {
		insertTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, color);
		insertTriangle(x3, y3, z3, x4, y4, z4, x1, y1, z1, color);
		return objectNo;
	}

	int insertTriangle(vector_c& p1, vector_c& p2, vector_c& p3) {
		return insertTriangle(p1, p2, p3, 0x000000);;
	}

	int insertTriangle(vector_c p1, vector_c p2, vector_c p3, unsigned int color) {
		if (ifObjectExist(objectNo)) {
			objects[objectNo].emplace_back(p1, p2, p3);
		} else {
			newObject();
			objects[objectNo].emplace_back(p1, p2, p3);
		}
		return objectNo;
	}

	int insertTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
		return insertTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, 0x000000);
	}

	int insertTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, unsigned int color) {
		if (ifObjectExist(objectNo)) {
			objects[objectNo].emplace_back(x1, y1, z1, x2, y2, z2, x3, y3, z3);
		} else {
			newObject();
			objects[objectNo].emplace_back(x1, y1, z1, x2, y2, z2, x3, y3, z3);
		}
		return objectNo;
	}

	void setWidth(int width) {
		zhiDevice::width = width;
		camera.setWidth(width);
	}

	void setHeight(int height) {
		zhiDevice::height = height;
		camera.setHeight(height);
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
	float **zbuffer = nullptr;
	char* zbuf = nullptr;//整个深度缓存区 方便一次释放内存

	camera camera;
	unsigned int objectNo=0;
	unsigned int MaxObjectNo = 0;
	unsigned int backgroundColor = 0x000000;
	std::map<int, std::vector<Triangle>> objects;

	void drawBackGround() {
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				drawPixel(i, j, backgroundColor);
				zbuffer[height-j-1][i] = -1;
			}
		}
	}

	void drawplane(vertex v1, vertex v2, vertex v3) {
		//drawTriangle(v1, v2, v3, 0x000000);
	}

	void drawWire(vector_c v1, vector_c v2, vector_c v3) {
			drawLine(v1.x, v1.y, v2.x, v2.y, 0x000000);
			drawLine(v2.x, v2.y, v3.x, v3.y, 0x000000);
			drawLine(v3.x, v3.y, v1.x, v1.y, 0x000000);
	}

	vertex projection(vertex p) {
		p.point = p.point*camera.tranMatrix;
		float w = p.point.w;
		homogenize(p.point);
		p.point.w = w;
		p.rhw_init();
		return p;
	}

	vector_c projection(vector_c p) {
		return homogenize(p*camera.tranMatrix);
	}

	void drawTriangle(vector_c p1, vector_c p2, vector_c p3, unsigned int color) {
		if (p1.y < p2.y) {
			std::swap(p1, p2);
		}
		if (p1.y < p3.y) {
			std::swap(p1, p3);
		}
		if (p2.y < p3.y) {
			std::swap(p2, p3);
		}
		if (p2.y==p3.y) {//平底
			drawFlatBotTri(p2, p3, p1, color);
		} else if (p2.y == p1.y) {//平顶
			drawFlatTopTri(p1, p2, p3, color);
		} else {
			float xl = (p2.y - p1.y) * (p3.x - p1.x) / (p3.y - p1.y) + p1.x;
			drawFlatBotTri(p2, p3, p1, color);
			drawFlatTopTri(p1, p2, p3, color);
		}

		//if (p1.y == p2.y) {
		//	if (p3.y < p1.y) {
		//		drawFlatTopTri(p1.x, p2.x, p2.y, p3.x, p3.y, p1.w, p2.w, p3.w, color);
		//	} else {
		//		drawFlatBotTri(p1.x, p2.x, p2.y, p3.x, p3.y, p1.w, p2.w, p3.w, color);
		//	}
		//} else if (p2.y == p3.y) {
		//	if (p1.y < p2.y) {
		//		drawFlatTopTri(p2.x, p3.x, p3.y, p1.x, p1.y, p2.w, p3.w, p1.w, color);
		//	} else {
		//		drawFlatBotTri(p2.x, p3.x, p3.y, p1.x, p1.y, p2.w, p3.w, p1.w, color);
		//	}
		//} else if (p1.y == p3.y) {
		//	if (p2.y < p1.y) {
		//		drawFlatTopTri(p1.x, p3.x, p3.y, p2.x, p2.y, p1.w, p3.w, p2.w, color);
		//	} else {
		//		drawFlatBotTri(p1.x, p3.x, p3.y, p2.x, p2.y, p1.w, p3.w, p2.w, color);
		//	}
		//} else {
		//	if (p1.y < p2.y) {
		//		std::swap(p1, p2);
		//	}
		//	if (p1.y < p3.y) {
		//		std::swap(p1, p3);
		//	}
		//	if (p2.y < p3.y) {
		//		std::swap(p2, p3);
		//	}
		//	float xl = (p2.y - p1.y) * (p3.x - p1.x) / (p3.y - p1.y) + p1.x;
		//	drawFlatTopTri(p2.x, xl, p2.y, p3.x, p3.y, p2.w, p1.w, p3.w, color);
		//	drawFlatBotTri(xl, p2.x, p2.y, p1.x, p1.y, p3.w, p2.w, p1.w, color);
		//}
	}

	//平底三角形 p1,p2为y相同的点
	void drawFlatBotTri(vector_c& p1, vector_c& p2, vector_c& p3, unsigned int color) {

		//z1 = 1.0f / z1;
		//z2 = 1.0f / z2;
		//z3 = 1.0f / z3;
		//if (x1 > x2) {
		//	std::swap(x1, x2);
		//	std::swap(z1, z2);
		//}

		//double k1 = (x3 - x1) / (y3 - y), k2 = (x3 - x2) / (y3 - y), zp1 = (z3 - z1) / (y3 - y), zp2 = (z3 - z2) / (y3 - y);
		//float xs = x1, xe = x2, zs = z1, ze = z2;
		//for (int i = y; i < y3; ++i, xs += k1, xe += k2, zs += zp1, ze += zp2) {
		//	if (xs > xe) {
		//		break;
		//	}
		//	drawHorizLine(xs - 1, xe, i, zs, ze, color);
		//}
	}

	//平顶三角形 p1,p2为y相同的点
	void drawFlatTopTri(vector_c& p1, vector_c& p2, vector_c& p3, unsigned int color) {
		//z1 = 1.0f / z1;
		//z2 = 1.0f / z2;
		//z3 = 1.0f / z3;
		//if (x1 > x2) {
		//	std::swap(x1, x2);
		//	std::swap(z1, z2);
		//}
		//double k1 = (x3 - x1) / (y3 - y), k2 = (x3 - x2) / (y3 - y), zp1 = (z3 - z1) / (y3 - y), zp2 = (z3 - z2) / (y3 - y);
		//float xs = x1, xe = x2, zs = z1, ze = z2;
		//for (int i = y; i >= y3; --i, xs -= k1, xe -= k2, zs -= zp1, ze -= zp2) {
		//	if (xs > xe) {
		//		break;
		//	}
		//	drawHorizLine(xs - 1, xe, i, zs, ze, color);
		//}
	}

	void drawHorizLine(int x1, int x2, int y, float zs, float ze, unsigned int color) {
		float z = zs, zk = (ze - zs) / (x2 - x1);
		for (int i = x1; i < x2; ++i, z += zk) {
			if (z > zbuffer[height - y-1][i]) {
				drawPixel(i, y, color);
				zbuffer[height - y-1][i] = z;
			}
		}
	}

	void drawLine(int x1, int y1, int x2, int y2) {
		drawLine(x1, y1, x2, y2, 0x000000);
	}

	void drawLine(int x1,int y1,int x2,int y2,unsigned int color) {
		int ox1 = x1, ox2 = x2, oy1 = y1, oy2 = y2;
		double k = double(y2 - y1) / (x2 - x1), d;
		if (!CohenSutherland(x1, y1, x2, y2,k)) {
			return;
		}	
		int cx1 = x1, cx2 = x2, cy1 = y1, cy2 = y2;
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

	vector_c homogenize(const vector_c& p){
		vector_c vector;
		float rhw = 1.0f / p.w;
		vector.x = (p.x * rhw + 1.0f) * width * 0.5f;
		vector.y = (1.0f - p.y * rhw) * height * 0.5f;
		vector.z = p.z * rhw;
		vector.w = 1.0f;
		//vector.w = p.w;
		return vector;
	}

	bool CohenSutherland(int& x1, int& y1, int& x2, int& y2,double k) {
		int p1code = getConhenCode(x1, y1);
		int p2code = getConhenCode(x2, y2);
		if ((p1code | p2code) == 0) {
			return true;
		} else if ((p1code&p2code) != 0) {
			return false;
		}
		changeCohenXY(x1, y1, p1code, k);
		changeCohenXY(x2, y2, p2code, k);
		if (p1code | p2code == 15) {
			if (x1<0||x2<0||y1<0||y2<0) {
				return false;
			}
		}
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
		if (y>=height) {
			code |= 8;
		} else if (y<0) {
			code |= 4;
		}
		if (x>=width) {
			code |= 2;
		} else if (x<0) {
			code |= 1;
		}
		return code;
	}

	void drawPixel(int x,int y,unsigned int color) {
		//if (!(x>=width||x<0||y>=height||y<0)) {
			framebuffer[(height - y - 1)][x] = color;
		//}			
	}
};

camera::camera(int width, int height) {
	camera::width = width;
	camera::height = height;
	world.set_identity();
	view.set_identity();
	set_perspective();
	update();
}

void camera::update() {
	tranMatrix = (world*view)*projection;
}

void camera::set_perspective(float fovy, float aspect, float zn, float zf) {//π/2 长宽比 近裁剪面 远裁剪面
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	projection.set_zero();
	projection[0][0] = (float)(fax / aspect);
	projection[1][1] = (float)(fax);
	projection[2][2] = zf / (zf - zn);
	projection[3][2] = -zn * zf / (zf - zn);
	projection[2][3] = 1;
	update();
}

void camera::set_perspective() {
	set_perspective(3.1415926f * 0.5f, (float)width / ((float)height), 1.0f, 500.0f);
}

void camera::lookat(const vector_c & eye, const vector_c & at, const vector_c & up) {
	vector_c xaxis, yaxis, zaxis;

	zaxis = at - eye;
	zaxis.normalize();
	xaxis = up*zaxis;
	xaxis.normalize();
	yaxis = zaxis*xaxis;

	view[0][0] = xaxis.x;
	view[1][0] = xaxis.y;
	view[2][0] = xaxis.z;
	view[3][0] = -xaxis.dotproduct(eye);

	view[0][1] = yaxis.x;
	view[1][1] = yaxis.y;
	view[2][1] = yaxis.z;
	view[3][1] = -yaxis.dotproduct(eye);

	view[0][2] = zaxis.x;
	view[1][2] = zaxis.y;
	view[2][2] = zaxis.z;
	view[3][2] = -zaxis.dotproduct(eye);

	view[0][3] = view[1][3] = view[2][3] = 0.0f;
	view[3][3] = 1.0f;
	update();
}

void camera::lookat(float eyex, float eyey, float eyez, float atx, float aty, float atz, float upx, float upy, float upz) {
	lookat(vector_c(eyex, eyey, eyez, 1.0f), vector_c(atx, aty, atz, 1.0f), vector_c(upx, upy, upz, 1.0f));
}

void camera::setWidth(int width) {
	camera::width = width;
	set_perspective();
}

void camera::setHeight(int height) {
	camera::height = height;
	set_perspective();
}

void camera::setAspect(int width, int height) {
	camera::width = width;
	camera::height = height;
	set_perspective();
}

void camera::setWorld(matrix_c world) {
	camera::world = world;
	update();
}