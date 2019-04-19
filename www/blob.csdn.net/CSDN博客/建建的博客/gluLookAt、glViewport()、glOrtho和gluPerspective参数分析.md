# gluLookAt、glViewport()、glOrtho和gluPerspective参数分析 - 建建的博客 - CSDN博客
2017年08月16日 11:21:38[纪建](https://me.csdn.net/u013898698)阅读数：226
## 一、 gluLookAt和投影
void gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble centerx,GLdouble centery,GLdouble centerz,GLdouble upx,GLdouble upy,GLdouble upz);
　　gluLookAt的前三个参数定义了观察点的位置，但同时也定了投影函数如正交投影函数各个参数的原点！
正交投影是指物体和照相机之间的距离并不影响他看上去的大小。
void glOrtho(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble near,GLdouble far)各参数意义如图所示，用来标记正交空间长方体。
![](http://pic002.cnblogs.com/images/2012/375602/2012111422003684.jpg)
　　　　若在程序中有glOrtho(-2.0,2.0,-2.0,2.0,5.0,100.0);和gluLookAt(1.0,0.0,0.0,1.0,0.0,-4.0,0.0,1.0,0.0);而物体坐标为(0,0,-10)则表示以（1,0,0）点为原点定义了一个宽为4高为4长为95的立方体，其左下角绝对坐标为（1-2,0-2,0-5）=（-1,-2,-5）,物体显示时偏左。
![](http://pic002.cnblogs.com/images/2012/375602/2012111501405879.jpg)
　　　　若物体坐标为(0,0,-4) 此时会显示黑屏。因为物体在正交立方体外。
　　　　对于透视投影函数void gluPerspective(GLdouble fovy, //角度
　　　　　　　　　　　　　　　　　　　　　　GLdouble aspect,//视景体的宽高比
　　　　　　　　　　　　　　　　　　　　　　GLdouble zNear,//沿z轴方向的两裁面之间的距离的近处
　　　　　　　　　　　　　　　　　　　　　　GLdouble zFar //沿z轴方向的两裁面之间的距离的远处)
　　　其中，zNear和zFar两个参数也是以观察点为原点的相对坐标。
## 二、投影变换到视口变换
　　视口是一个矩形的窗口区域，三维图像通过映射生成二维平面的像素后，就是在这片区域绘制的。默认情况下，视口被设置为打开窗口的整个像素矩形。
glViewport(GLint x,GLint y,GLsizei width,GLsizei height)定义了视口在窗口中的区域同时也规定了二维的像素平面到视口区域的映射关系。
![](http://pic002.cnblogs.com/images/2012/375602/2012111500555622.jpg)
    若有glViewport(0,0,w/2,h),即有：
![](http://pic002.cnblogs.com/images/2012/375602/2012111501135974.jpg)
也可以利用glViewPort()生成多窗口模式：
```
glViewport(0.0,0.0,afxw/2,afxh/2);
    glCallList(box);
     glViewport(afxw/2,0.0,afxw/2,afxh/2);     
    glCallList(box);
    glViewport(0.0,afxh/2,afxw/2,afxh/2);     
    glCallList(box);
    glViewport(afxw/2,afxh/2,afxw/2,afxh/2);     
    glCallList(box);
```
                  生成效果如图所示：                          ![](http://pic002.cnblogs.com/images/2012/375602/2012111501195026.jpg)
