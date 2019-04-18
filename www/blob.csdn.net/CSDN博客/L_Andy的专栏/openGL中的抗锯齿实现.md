# openGL中的抗锯齿实现 - L_Andy的专栏 - CSDN博客

2016年06月28日 09:39:04[卡哥](https://me.csdn.net/L_Andy)阅读数：3144


图形绘制到屏幕时，光栅化图像通过离散的像素点来表示，因此所绘制的图元（比如直线）会产生锯齿，这种锯齿也称作走样。消除锯齿的技术称为反走样，也叫做抗锯齿，通过这种处理可在一定程度上消除锯齿的影响，增强显示效果。在OpenGL中，可通过函数glHint（）来对图像质量和绘制速度之间的权衡作一些控制，其函数形式为：

void glHint(GLenum target,GLenum hint);

　　参数target说明控制什么行为：GL_POINT_SMOOTH_HINT、GL_LINE_SMOOTH_HINT和GL_POLYGON_SMOOTH_HINT分别指定点、线和多边形的采样质量；GL_FOG_HINT指出雾是按像素进行（GL_NICEST）还是按顶点进行（GL_FASTEST）；GL_PERSPECTIVE_CORRECTION_HINT指定了颜色纹理插值的质量并可纠正由单纯线性插值所带来的一些视觉错误。参数hint可以是：GL_FASTEST（给出最有效的选择）、GL_NICEST（给出最高质量的选择）、GL_DONT_CARE（没有选择）。

　　虽然在OpenGL的颜色索引模式下也可以实现反走样，但仍建议在RGBA模式下进行。对图元进行反走样时也要先调用glEnable（）函数启动反走样（其参数为GL_POINT、GL_LINE_SMOOTH或GL_POLYGON_SMOOTH）。如果是在RGBA模式下进行反走样，还必须与融合配合使用，通常使用GL_SRC_ALPHA和GL_ONE_MINUS_SRC_ALPHA分别作为源和目的因子。

    openGL中使用反走样，需要在设置函数中进行设置，下面设置反走样的代码：

// Initialize OpenGL's rendering modes  

void initRendering()  

{  
glEnable ( GL_DEPTH_TEST );  

 // Uncomment out the first block of code below, and then the second block,  

 //  to see how they affect line and point drawing.  

 // The following commands should cause points and line to be drawn larger  

 // than a single pixel width.  

    glPointSize(8);  
glLineWidth(5);  

 // The following commands should induce OpenGL to create round points and   

 // antialias points and lines.  (This is implementation dependent unfortunately).  

 // RGBA mode antialias need cooperate with blend function.  
glEnable(GL_POINT_SMOOTH);  
glEnable(GL_LINE_SMOOTH);  
glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines  
glEnable(GL_BLEND);  
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

}  

写一个openGL程序画一个点，如果没有抗锯齿，则为方形的。如果我们启动抗锯齿设置，则点是一个圆点。

未启用

![](https://img-blog.csdn.net/20160628093556977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

启用

![](https://img-blog.csdn.net/20160628093601468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

写一个opengl 画一个线，很明显，启用抗锯齿后，粗线变成了矩形，而未启动则是一个平行四边形

未启用

![](https://img-blog.csdn.net/20160628093606024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

启用

![](https://img-blog.csdn.net/20160628093609931?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

