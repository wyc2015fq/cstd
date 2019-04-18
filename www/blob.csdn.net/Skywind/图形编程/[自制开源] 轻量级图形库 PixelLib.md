# Skywind Inside » [自制开源] 轻量级图形库 PixelLib
## [自制开源] 轻量级图形库 PixelLib
January 10th, 2010[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
图像：64种不同的像素格式，色彩空间变换，多种图形图像变换。
质量：支持3种级别抗锯齿效果，高质量几何图形绘制。
实现：轻量级纯软件实现，100% C代码（仅700KB代码）。
优化：SSE2/MMX优化
地址：[https://github.com/skywind3000/pixellib](https://github.com/skywind3000/pixellib)
**1. 图像变换：**
![](http://www.skywind.me/mw/images/8/8e/Pixellib_image_transform.png)
支持仿射变换和透视变换，提供大量图像变换操作接口。
**2. 抗锯齿：**
![](http://www.skywind.me/mw/images/9/9c/Pixellib_anti_aliasing.png)
所有图形绘制支持3级不同程度的抗锯齿效果。
**3. 图像绘制：**
![](http://www.skywind.me/mw/images/4/41/Pixellib_image_drawing.png)
图像任意拉伸，旋转，3D旋转，并且同时进行色彩空间变换。全部采用浮点数坐标，图像移动更为平滑。
**4. 几何作图：**
![](http://www.skywind.me/mw/images/1/1e/Pixellib_geometry.png)
全面的抗锯齿几何作图效果。
**5. 图像扭曲：**
![](http://www.skywind.me/mw/images/c/c9/Pixellib_image_warp.png)
在源图像上布置若干关键点，然后改变这些关键点在屏幕上的对于位置即可实现图像扭曲。
**使用 Pixellib 来渲染 iOS 风格的图标 **
![](http://www.skywind.me/mw/images/e/e7/Pixellib_quality.png)
