#pragma once
#include"figure.h"


/*
*�������
*
*/
class light {
public:
	vector_c position;//��Դλ��
	vector_c dir;//���շ���
	Color ambient;//����������ɫ
	Color diffuse;//���������ɫ
	Color specular;//���淴�����ɫ
	float kc, kl, kq;//˥������
	float spot_inner;//�۹����׶��
	float spot_outer;//�۹����׶��
	float pf;//�۹��ָ������
};

/*
*����
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
	float ka, kd, ks, power;//����ϵ��
	Color ra, rd, rs;//������ϵ������ɫ�Ļ�
};