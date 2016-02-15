#pragma once
#include<vector>
#include"zhi_matrix.h"

typedef struct { float u, v; } texcoord;//贴图坐标

typedef struct { float r, g, b; } color;

class vertex {
friend std::vector<trapezoid> getTrap(vertex&, vertex&, vertex&);
public:	
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
	std::vector<trapezoid> trap_vect;
	if (p1.point.y < p2.point.y) { std::swap(p1, p2); }
	if (p1.point.y < p3.point.y) { std::swap(p1, p3); }
	if (p2.point.y < p3.point.y) { std::swap(p2, p3); }
	if ((p1.point.y != p2.point.y || p1.point.y != p3.point.y) && (p1.point.x != p2.point.x || p1.point.x != p3.point.x)) {
		if (p2.point.y == p3.point.y) {//平底		
			if (p3.point.x > p2.point.x)
				std::swap(p3, p2);
			trapezoid trap(p3.point.y, p1.point.y, p3, p1, p2, p3);
			if (trap.top < trap.bottom)
				trap_vect.push_back(trap);
		} else if (p2.point.y == p1.point.y) {//平顶
			if (p2.point.x > p1.point.x)
				std::swap(p1, p2);
			trapezoid trap(p3.point.y, p1.point.y, p3, p2, p3, p1);
			if (trap.top < trap.bottom)
				trap_vect.push_back(trap);
		} else {
			/*
			*trap[0].top = p1->pos.y;
			*trap[0].bottom = p2->pos.y;
			*trap[1].top = p2->pos.y;
			*trap[1].bottom = p3->pos.y;
			*/
			float xl = (p2.point.y - p1.point.y) * (p3.point.x - p1.point.x) / (p3.point.y - p1.point.y) + p1.point.x;
			if (xl <= p1.point.x) {		// triangle left
				trapezoid trap1(p3.point.y, p2.point.y, p3, p2, p3, p1);
				trapezoid trap2(p2.point.y, p1.point.y, p2, p1, p3, p1);
			} else {					// triangle right
				trapezoid trap1(p3.point.y, p2.point.y, p3, p1, p3, p2);
				trapezoid trap2(p2.point.y, p1.point.y, p3, p1, p2, p1);
			}
		}
	}
	return trap_vect;
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
	trapezoid(float top, float bottom, vertex& lv1, vertex& lv2, vertex& rv1, vertex& rv2):top(top),bottom(bottom) {};
	float top, bottom;
	edge left, right;
};

typedef struct { vertex v, step; int x, y, w; } scanline;//扫描线 MARK要写成class