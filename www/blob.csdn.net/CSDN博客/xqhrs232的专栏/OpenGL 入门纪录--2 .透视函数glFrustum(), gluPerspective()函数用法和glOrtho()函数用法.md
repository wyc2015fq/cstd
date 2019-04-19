# OpenGL 入门纪录--2 .透视函数glFrustum(), gluPerspective()函数用法和glOrtho()函数用法 - xqhrs232的专栏 - CSDN博客
2010年12月05日 14:01:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1849
原文地址::[http://hi.baidu.com/zhujianzhai/blog/item/24e9c11ec9a91c64f724e43c.html](http://hi.baidu.com/zhujianzhai/blog/item/24e9c11ec9a91c64f724e43c.html)
在OpenGL中，如果想对模型进行操作，就要对这个模型的状态(当前的矩阵)乘上这个操作对应的一个矩阵.
如果乘以变换矩阵(平移, 缩放, 旋转), 那相乘之后, 模型的位置被变换;
如果乘以投影矩阵(将3D物体投影到2D平面), 相乘后, 模型的投影方式被设置;
如果乘以纹理矩阵(), 模型的纹理方式被设置.
而用来指定乘以什么类型的矩阵, 就是glMatriMode(GLenum mode);
glMatrixMode有3种模式: GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理.
所以，在操作投影矩阵以前，需要调用函数：
glMatrixMode(GL_PROJECTION); //将当前矩阵指定为投影矩阵
然后把矩阵设为单位矩阵：
glLoadIdentity();
然后调用glFrustum()或gluPerspective(),它们生成的矩阵会与当前的矩阵相乘,生成透视的效果；
1.glFrustum()
![](http://hiphotos.baidu.com/xun1573/pic/item/136bd9a11820a797471064de.jpg)
这个函数原型为：
　　void glFrustum(GLdouble left, GLdouble Right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
创建一个透视型的视景体。其操作是创建一个透视投影的矩阵，并且用这个矩阵乘以当前矩阵。这个函数的参数只定义近裁剪平面的左下角点和右上角点的三维空间坐标，即（left，bottom，-near）和（right，top，-near）;最后一个参数far是远裁剪平面的离视点的距离值，其左下角点和右上角点空间坐标由函数根据透视投影原理自动生成。near和far表示离视点的远近，它们总为正值(near/far 必须>0)。
2.gluPerspective()
![](http://hiphotos.baidu.com/xun1573/pic/item/0f7a118b90256c0ec8fc7ada.jpg)
这个函数原型为：
void gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFar);
　　创建一个对称的透视型视景体，但它的参数定义于前面的不同，如图。其操作是创建一个对称的透视投影矩阵，并且用这个矩阵乘以当前矩阵。参数fovy定义视野在Y-Z平面的角度，范围是[0.0, 180.0]；参数aspect是投影平面宽度与高度的比率；参数Near和Far分别是近远裁剪面到视点(沿Z负轴)的距离，它们总为正值。
　　以上两个函数缺省时，视点都在原点，视线沿Z轴指向负方向。
void mydisplay (void)
{
     ......
    glMatrixMode (GL_PROJECTION);
    LoadIdentity ();
    Frustum (left, right, bottom, top, near, far);
    ......
}
3.glOrtho()
![](http://hiphotos.baidu.com/zhujianzhai/pic/item/a42d8a00276db632728da5bb.jpg)
这个函数的原型为:
glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
六个参数, 前两个是x轴最小坐标和最大坐标,中间两个是y轴,最后两个是z轴值
它创建一个平行视景体（就是一个长方体空间区域）。
实际上这个函数的操作是创建一个正射投影矩阵，并且用这个矩阵乘以当前矩阵。
其中近裁剪平面是一个矩形，矩形左下角点三维空间坐标是（left，bottom，-near），
右上角点是（right，top，-near）；远裁剪平面也是一个矩形，左下角点空间坐标是（left，bottom，-far），右上角点是（right，top，-far）。
注意，所有的near和far值同时为正或同时为负, 值不能相同。如果没有其他变换，正射投影的方向平行于Z轴，且视点朝向Z负轴。这意味着物体在视点前面时far和near都为负值，物体在视点后面时far和near都为正值。
只有在视景体里的物体才能显示出来。
如果最后两个值是(0，0)，也就是near和far值相同了，视景体深度没有了，整个视景体都被压成个平面了，就会显示不正确。
//============================================================================
//特别注意的地方----2010--12--08
1。glFrustum函数中near和far表示离视点的远近，它们总为正值。
2。glFrustum函数中近平面的坐标确定----（left，bottom，-near）和（right，top，-near）特别注意这个near值进行了负运算；
远平面的确定也要进行负运算------所以这个远近平面只是用的是正数但实际要表达的坐标是负值。
3。glTranslate（X，Y，Z）函数，视点通常在坐标原点处，Z值为负表示把物替沿Z轴负向平移，远离视点，Z值为正同理。
4。glFrustum与gluPerspective函数的作用与原理都差不多，都只是为了定义一个棱台形状的视景体（在其坐标内部可以看得见，在其坐标外就看不见-----不能正常在屏幕坐标中显示），一个通过坐标来定位可视棱台大小，另外一个是通过角度来定位可视棱台大小。
5。glViewport函数定义的是一个视口，为了在屏幕坐标全屏显示，其高跟宽的定义通常跟LCD屏幕大小等同。

