#pragma once

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