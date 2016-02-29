#pragma once
#include<vector>
#include"zhi_matrix.h"

typedef struct { float u, v; } texcoord;//贴图坐标

int CMID(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }

class color {
public:
	float r = 0;
	float g = 0; 
	float b = 0;

	color() {};
	color(unsigned int color) {
		r = color >> 16;
		g = (color >> 8) - ((int)(r) << 8);
		b = color - ((int)(r) << 16) - ((int)(g) << 8);
	};
	color(float r, float g, float b) :r(r), g(g), b(b) {};
private:
	
};

class trapezoid;

class vertex {
	friend std::vector<trapezoid> getTrap(vertex, vertex, vertex);
public:	
	vector_c mutable point;
	color mutable color;
	texcoord mutable tc;	
	float mutable rhw;

	vertex(float x, float y, float z) :point(x, y, z) {};
	vertex(float x, float y, float z, unsigned int color) :point(x, y, z),color(color) {};
	vertex(float x, float y, float z, float r, float g, float b) :point(x, y, z), color(r, g, b) {};
	vertex(float x, float y, float z, float u, float v) :point(x, y, z), tc({ u,v }) {};
	vertex(float x, float y, float z, float u, float v, float r, float g, float b) :point(x, y, z), tc({ u,v }), color(r, g, b) {};
	vertex(vector_c v):point(v) {};
	vertex() {};
	//vertex(const vertex& v):point(v.point),tc(v.tc),color(v.color),rhw(rhw) {};
	void operator += (const vertex&) const;


	void rhw_init() const{
		float rhw = 1.0f / point.w;
		vertex::rhw = rhw;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	};	

	void division(const vertex& x1, const vertex& x2, float width) {
		float inv = 1.0f / width;
		 point.x = (x2.point.x - x1.point.x) * inv;
		 point.y = (x2.point.y - x1.point.y) * inv;
		 point.z = (x2.point.z - x1.point.z) * inv;
		 point.w = (x2.point.w - x1.point.w) * inv;
		 tc.u = (x2.tc.u - x1.tc.u) * inv;
		 tc.v = (x2.tc.v - x1.tc.v) * inv;
		 color.r = (x2.color.r - x1.color.r) * inv;
		 color.g = (x2.color.g - x1.color.g) * inv;
		 color.b = (x2.color.b - x1.color.b) * inv;
		 rhw = (x2.rhw - x1.rhw) * inv;
	}

};

class Triangle {
public:
	vertex p1;
	vertex p2;
	vertex p3;
	unsigned int texture;
	Triangle(float x1, float y1, float z1,unsigned int color1, float x2, float y2, float z2, unsigned int color2, float x3, float y3, float z3, unsigned int color3) :p1(vertex(x1, y1, z1)), p2(vertex(x2, y2, z2)), p3(vertex(x3, y3, z3)) {};
	Triangle(vertex p1, vertex p2, vertex p3) :p1(p1), p2(p2), p3(p3) {};
	Triangle(vertex p1, vertex p2, vertex p3, unsigned int texture) :p1(p1), p2(p2), p3(p3), texture(texture) {};
	Triangle() {};
};

class edge { 
public:
	vertex v, v1, v2; 
	edge(vertex v1, vertex v2):v1(v1),v2(v2) {};
	void edge_interp(float y) {
		float t = (y - v1.point.y) / (v2.point.y - v1.point.y);
		v.point.vector_interp(v1.point, v2.point, t);
		v.tc.u = interp(v1.tc.u, v2.tc.u, t);
		v.tc.v = interp(v1.tc.v, v2.tc.v, t);
		v.color.r = interp(v1.color.r, v2.color.r, t);
		v.color.g = interp(v1.color.g, v2.color.g, t);
		v.color.b = interp(v1.color.b, v2.color.b, t);
		v.rhw = interp(v1.rhw, v2.rhw, t);
	}
};//梯形的边 v1,v2为起始和终点

class trapezoid {
public:
	trapezoid(float top, float bottom,const vertex& lv1, const vertex& lv2, const vertex& rv1, const vertex& rv2):top(top),bottom(bottom),left(lv1,lv2),right(rv1,rv2) {};
	float top, bottom;
	edge left, right;
	void trapezoid_edge_interp(float y) {
		left.edge_interp(y);
		right.edge_interp(y);
	};
};

class scanline_c{
public:
	vertex v, step; 
	int x, y, w; 
	scanline_c(const trapezoid& trap, int y):y(y) {
		x = (int)(trap.left.v.point.x + 0.5f);
		w = (int)(trap.right.v.point.x + 0.5f) - x;
		v = trap.left.v;
		if (trap.left.v.point.x >= trap.right.v.point.x) 
			w = 0;
		step.division(trap.left.v, trap.right.v, (trap.right.v.point.x - trap.left.v.point.x));
	};
};//扫描线

void vertex::operator+=(const vertex & v) const {
	point.x += v.point.x;
	point.y += v.point.y;
	point.z += v.point.z;
	point.w += v.point.w;
	rhw += v.rhw;
	tc.u += v.tc.u;
	tc.v += v.tc.v;
	color.r += v.color.r;
	color.g += v.color.g;
	color.b += v.color.b;
}

std::vector<trapezoid> getTrap(vertex p1, vertex p2, vertex p3) {
	std::vector<trapezoid> trap_vect;
	if (p1.point.y < p2.point.y) { std::swap(p1, p2); }
	if (p1.point.y < p3.point.y) { std::swap(p1, p3); }
	if (p2.point.y < p3.point.y) { std::swap(p2, p3); }
	if ((p1.point.y != p2.point.y || p1.point.y != p3.point.y) && (p1.point.x != p2.point.x || p1.point.x != p3.point.x)) {
		if (p2.point.y == p3.point.y) {//平顶		
			if (p3.point.x > p2.point.x)
				std::swap(p3, p2);
			trapezoid trap(p3.point.y, p1.point.y, p3, p1, p2, p1);
			if (trap.top < trap.bottom)
				trap_vect.push_back(trap);
		} else if (p2.point.y == p1.point.y) {//平底
			if (p2.point.x > p1.point.x)
				std::swap(p1, p2);
			trapezoid trap(p3.point.y, p1.point.y, p3, p2, p3, p1);
			if (trap.top < trap.bottom)
				trap_vect.push_back(trap);
		} else {
			float xl = p3.point.x + (p2.point.x - p3.point.x) * (p1.point.y - p3.point.y) / (p2.point.y - p3.point.y);
			if (xl <= p1.point.x) {		// triangle left
				trapezoid trap1(p3.point.y, p2.point.y, p3, p2, p3, p1);
				trapezoid trap2(p2.point.y, p1.point.y, p2, p1, p3, p1);	
				trap_vect.push_back(trap1);
				trap_vect.push_back(trap2);
			} else {					// triangle right
				trapezoid trap1(p3.point.y, p2.point.y, p3, p1, p3, p2);
				trapezoid trap2(p2.point.y, p1.point.y, p3, p1, p2, p1);
				trap_vect.push_back(trap1);
				trap_vect.push_back(trap2);
			}			
		}
	}
	return trap_vect;
}