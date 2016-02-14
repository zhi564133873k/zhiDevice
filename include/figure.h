#pragma once
#include"zhi_matrix.h"
class Triangle {
public:
	vector_c p1;
	vector_c p2;
	vector_c p3;
	unsigned int color = 0x000000;
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, unsigned int color) :p1(vector_c(x1, y1, z1)), p2(vector_c(x2, y2, z2)), p3(vector_c(x3, y3, z3)), color(color) {};
	Triangle(vector_c p1, vector_c p2, vector_c p3, unsigned int color) :p1(p1), p2(p2), p3(p3), color(color) {};
	Triangle() {};
};

typedef struct { float u, v; } texcoord;//贴图坐标

typedef struct { vector_c point; texcoord tc; unsigned int color; float rhw; } vertex;//MARK要写成class

typedef struct { vertex v, v1, v2; } edge;//梯形的边 v1,v2为起始和终点

class trapezoid {
public:
	trapezoid(float top, float bottom, vertex lv1, vertex lv2, vertex rv1, vertex rv2):top(top),bottom(bottom) {};
private:
	float top, bottom;
	edge left, right;
};

typedef struct { vertex v, step; int x, y, w; } scanline;//扫描线 MARK要写成class