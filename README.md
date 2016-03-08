# zhiDevice
c++实现3D软件渲染，运用基本图形学知识实现软件渲染器，虽然没做深入的优化，但是有完整的功能。
#实现功能
1、三维坐标到屏幕二维坐标的转换
2、线框渲染，纯色渲染，贴图渲染
3、深度缓存
4、背面消隐
5、简单的光照系统
#使用算法
1、Bresenham线段光栅化算法
2、Cohen-Sutherland算法
3、扫描线填充法
4、构建支持欧拉和UVN的相机系统
===
#演示
绘制线框(WIREFRAME)

![image](https://github.com/zhi564133873k/zhiDevice/blob/master/demo/wire.jpg)

颜色渲染(COLOR)

![image](https://github.com/zhi564133873k/zhiDevice/blob/master/demo/color.jpg)

纹理渲染(MAPPING)

![image](https://github.com/zhi564133873k/zhiDevice/blob/master/demo/mapping.jpg）
