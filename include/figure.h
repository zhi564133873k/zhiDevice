#pragma once
#include<vector>
#include"zhi_matrix.h"

typedef struct { float u, v; } texcoord;//贴图坐标

typedef struct { float r, g, b; } color;

class vertex {
public:
	friend std::vector<trapezoid> getTrap(vertex&, vertex&, vertex&);
	vector_c point; 
	texcoord tc; 
	color color;
	float rhw; 

	vertex(float x, float y, float z) :point(x, y, z) {};
	vertex(vector_c v):point(v) {};
	vertex() {};
	//vertex(const vertex& v):point(v.point),tc(v.tc),color(v.color),rhw(rhw) {};

	void rhw_init() {
		float rhw = 1.0f / point.w;
		vertex::rhw = rhw;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	};	
};

std::vector<trapezoid> getTrap(vertex& p1, vertex& p2, vertex& p3) {
	std::vector<trapezoid> trap;
	if (p1.point.y < p2.point.y) { std::swap(p1, p2); }
	if (p1.point.y < p3.point.y) { std::swap(p1, p3); }
	if (p2.point.y < p3.point.y) { std::swap(p2, p3); }
	if (p1.point.y == p2.point.y && p1.point.y == p3.point.y) return trap;
	if (p1.point.x == p2.point.x && p1.point.x == p3.point.x) return trap;
	/*
	*MARK
	*/
}

class Triangle {
public:
	vertex p1;
	vertex p2;
	vertex p3;
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) :p1(vertex(x1, y1, z1)), p2(vertex(x2, y2, z2)), p3(vertex(x3, y3, z3)) {};
	Triangle(vertex p1, vertex p2, vertex p3) :p1(p1), p2(p2), p3(p3) {};
	Triangle() {};
};

typedef struct { vertex v, v1, v2; } edge;//梯形的边 v1,v2为起始和终点

class trapezoid {
public:
	trapezoid(float top, float bottom, vertex lv1, vertex lv2, vertex rv1, vertex rv2):top(top),bottom(bottom) {};
private:
	float top, bottom;
	edge left, right;
};

typedef struct { vertex v, step; int x, y, w; } scanline;//扫描线 MARK要写成class