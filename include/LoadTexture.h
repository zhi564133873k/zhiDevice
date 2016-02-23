#pragma once
#include <windows.h>
#include"zhiDevice.h"
int LoadTexture(LPTSTR szFileName ,zhiDevice& device){
	HBITMAP hBMP;         
	BITMAP BMP;
	unsigned int textureNo=0;
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!hBMP)
		return -1; 
	GetObject(hBMP, sizeof(BMP), &BMP);
	textureNo = device.createTexture(BMP.bmBits, (long)(BMP.bmBitsPixel / 8), BMP.bmWidth, BMP.bmHeight);
	DeleteObject(hBMP); 
	return textureNo;
}