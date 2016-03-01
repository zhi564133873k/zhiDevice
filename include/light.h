#pragma once
#include"figure.h"


/*
*管理光照
*
*/
class light {
public:
	vector_c position;//光源位置
	vector_c dir;//光照方向
	Color ambient;//环境光照颜色
	Color diffuse;//漫反射光颜色
	Color specular;//镜面反射光颜色
	float kc, kl, kq;//衰减因子
	float spot_inner;//聚光灯内锥角
	float spot_outer;//聚光灯外锥角
	float pf;//聚光灯指数因子
};

/*
*材质
*
*/
class texture {
public:
	texture(Color color, float ka, float kd, float ks) :color(color), ka(ka), kd(kd), ks(ks) {
		ra = color*ka;
		rd = color*kd;
		rs = color*ks;
	};

private:
	Color color;
	float ka, kd, ks, power;//反射系数
	Color ra, rd, rs;//各反射系数与颜色的积
};