# 【OpenGL】顶点变换常用函数总结 - 江南烟雨 - CSDN博客
2012年03月07日 21:28:30[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3325
所属专栏：[](https://blog.csdn.net/column/details/.html)
OpenGL中顶点变换的步骤可以用下图来描述：
![](http://hi.csdn.net/attachment/201203/7/0_133112725415XF.gif)
其顺序依次为（和照相机比喻）：
1、把照相机固定在三脚架上，并对准场景（视图变换）。
2、对场景镜像安排，使各个物体在照片中得位置是我们所希望的（模型变换）。
3、选择相机镜头，并调整放大倍数（投影变换）。
4、确定最终照片的大小（视口变换）。
![](http://hi.csdn.net/attachment/201203/7/0_1331127385G16P.gif)
一、视图变换：
相当于把照相机固定在三脚架上并使它对准场景。
void gluLookAt(GLdouble eyeX , GLdouble eyeY , GLdouble eyeZ , GLdouble 
centerX , GLdouble centerY , GLdouble centerZ , GLdouble upX , GLdouble upY , GLdouble upZ );
其参数分别表示眼睛位置、瞄向、以及哪个方向是朝上的。
二、模型变换：
其目的是设置模型的位置和方向。
void glScalef(GLfloat  x, GLfloat  y, GLfloat  z);
进行缩放。参数指定了在上那个方向上的缩放情况。
void glTranslatef(GLfloat  x, GLfloat  y, GLfloat  z);
移动模型。
void glRotatef(GLfloat  angle, GLfloat  x,GLfloat  y,GLfloat  z)
指定按照从原点到点（x,y,z）的直线进行旋转的角度。
三、投影变换：
目的是确定视野，确定哪些物体应该位于视野之内以及他们被看到的程度。
分为透视投影和正交投影。
（1）透视投影：
void glFrustum(GLdouble  left, 
    GLdouble  right, 
    GLdouble  bottom, 
    GLdouble  top, 
    GLdouble  nearVal, 
    GLdouble  farVal);
参数分别指定了垂直裁剪平面的左右位置、水平裁剪平面的上下位置以及相机离深度裁剪平面的近、远距离。
glFrustrum定义了一个平截头体，它计算一个用于实现透视投影的矩阵，并把它与当前的投影矩阵（一般为单位矩阵）相乘。
注意：平截头体并不要求一定是对称的，它的轴也不需要与Z轴对齐。
glFrustrum使用起来不是很只管，我们可以用OpenGL工具函数库的gluPerspective函数。
void gluPerspective(GLdouble  fovy, GLdouble  aspect, GLdouble  zNear, GLdouble zFar);
参数fovy表示眼睛张开的角度。aspect表示纵横比x/y。zNear和zFar分别表示观察者离近远裁剪平面的距离。
注意：gluPerspective仅限于创建沿视线方向同时对称与x轴和y轴的平截头体。但是我们通常所需要的也就是这种平截头体。
（2）正投影
void glOrtho(     GLdouble       left,
     GLdouble       right,
     GLdouble       bottom,
     GLdouble       top,
     GLdouble       nearVal,
     GLdouble       farVal);
创建一个正交平行的视景体。参数和glFrustrum一样。
如果是把二维图像投影到二维屏幕这种情况，可以使用OpenGL工具函数库中的gluOrtho2D函数。它与它的三维版本glOrtho基本相同。
四、视口变换：
决定了场景所映射的有效屏幕区域的形状。
void glViewport(GLint  x, GLint  y, GLsizei  width, GLsizei  height);
设置视口及像素将要显示的区域。参数分别指定窗口内部有效屏幕空间的原点(注意(0,0)表示窗口的左下角)以及有效屏幕区域的宽度和高度。
