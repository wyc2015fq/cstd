# OpenGL超级宝典笔记——纹理映射 - L_Andy的专栏 - CSDN博客

2016年11月30日 11:18:19[卡哥](https://me.csdn.net/L_Andy)阅读数：609



#### 纹理环境

OpenGL是如何把纹理元素的颜色和几何图元的颜色结合起来的？是通过纹理环境的模式来控制。设置纹理环境模式的函数如下：

void glTexEnvi(GLenum target, GLenum pname, GLint param);

void glTexEnvf(GLenum target, GLenum pname, GLfloat param);

void glTexEnviv(GLenum target, GLenum pname, GLint *param);

void glTexEnvfv(GLenum target, GLenum pname, GLfloat *param);

这个函数有许多的选项，一些高级的选项会在下一节进行介绍。在前面的金字塔例子中，我们在把纹理映射到几何图元之前，设置环境模式为GL_MODULATE。

GL_MODULATE模式是把纹理元素的颜色乘以几何图元（进行光照计算之后）的颜色。通过这种模式，我们可以用彩色的几何图形与纹理结合来产生不同的效果。

![](https://img-blog.csdn.net/20161130112258545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们还可以用GL_REPLACE模式简单地覆盖掉纹理下面的结合图形的颜色。这样片段的颜色值将直接采用纹理的颜色。这样就消除了几何图形对纹理的影响。如果纹理有alpha通道，我们还可以开启混合效果，来创建透明的纹理。

![](https://img-blog.csdn.net/20161130112309460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

还有一种贴纸模式参数为GL_DECAL。当纹理没有alpah通道的时候，其效果和GL_REPLACE是一样的。如果有alpah通道，那么将会把纹理元素的颜色和纹理下面片段的颜色进行alpha混合。

纹理还可以使用GL_BLEND环境模式，来与一个常量值进行混合。在设置这个模式的时候，必须设置纹理环境的颜色。

GLfloat fColor[4] = {1.0f, 0.0f, 0.0f, 0.0f};

glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, fColor);

还有一种GL_ADD模式，简单地把纹理元素的颜色与下面的片段颜色进行相加，超过1.0的调整为1.0. 这样会使得很多地方看起来是白色的或接近白色。

![](https://img-blog.csdn.net/20161130112316560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 纹理参数

在纹理映射的过程中，有许多参数会影响渲染的方式和效果。可以通过下面的四个函数来设置纹理参数：

void glTexParameterf(GLenum target, GLenum pname, GLfloat param);

void glTexParameteri(GLenum target, GLenum pname, GLint param);

void glTexParameterfv(GLenum target, GLenum pname, GLfloat *param);

void glTexParameteriv(GLenum target, GLenum pname, GLint *param);

target参数可以是GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D.第二个参数pname是告诉OpenGL哪个参数被设置。最后一个参数是纹理的参数值。

#### 基础过滤

不像像素图渲染到颜色缓冲区一样，在纹理映射过程中，纹理元素与屏幕上的像素几乎不是一一对应的。一般情况下，在把纹理映射到几何图形的表面上时，我们需要对其进行拉伸或收缩。

根据一个纹理贴图的拉伸或收缩来计算颜色片段的过程称为纹理过滤。OpenGL中有放大过滤器和缩小过滤器。这两种过滤器的pname参数分别为GL_TEXTURE_MAG_FILTER，GL_TEXTURE_MIN_FILTER。目前为止，我们使用GL_NEAREST（最邻近） 和 GL_LINEAR（线性）两种过滤方式。注意：你需要为GL_TEXTURE_MIN_FILTER选择其中一种，因为在默认的过滤器不能用于Mipmap。

最邻近过滤方法是一种最简单、最快速的过滤方式。不管纹理坐标落入哪个纹理单元，这个纹理单元的颜色就作为这个片段的纹理颜色。最邻近过滤的方式虽然快，但会走样特别是在纹理被拉伸到特别大的时候有大片的斑驳状像素。设置最邻近的方法：

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

![](https://img-blog.csdn.net/20161130112320576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

线性过滤比最邻近过滤做更多的工作，但它的效果更好。而且在现代的硬件下，这种开销可以忽略不计。线性过滤会取周围的纹理单元进行加权平均得到当前纹理单元的颜色。线性过滤的特征是当纹理被放大时，会产生“模糊”的效果。设置线性过滤：

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

![](https://img-blog.csdn.net/20161130112325467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 纹理环绕

在纹理映射中，纹理坐标的范围为[0.0,1.0]。当纹理坐标超出这个值时，OpenGL会根据你设置的环绕模式来处理这种情况。我们可以分别为每个纹理坐标轴(s,t,r)设置一个环绕模式，通过glTexParameteri 第二个参数为GL_TEXTURE_WRAP_S，GL_TEXTURE_WRAP_T或者GL_TEXTURE_WRAP_R.环绕的模式有GL_REPEAT,GL_CLAMP,GL_CLAMP_TO_EDGE, GL_WRAP_TO_BORDER.

GL_REPEAT环绕模式只是简单的重复纹理。例如：1.1出的纹理单元与0.1处的纹理单元是相同的。在平铺式的纹理应用到大型几何图形的时候，非常有用。一个设计良好的无缝小型纹理紧挨着平铺到大型几何图形上看起来像是无缝的大型纹理。

![](https://img-blog.csdn.net/20161130112329961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

GL_CLAMP环绕模式是超过1.0的截取为1.0。

如果环绕纹理的意义是是否对纹理进行重复那么使用GL_REPEAT和GL_CLAMP两种模式就足够了。然而纹理的环绕模式，会影响到如何在纹理贴图边缘进行纹理过滤。对于GL_NEAREST过滤方式来讲，纹理的环绕模式对它并没有什么影响，因为纹理坐标总是对应到纹理贴图上的某一个单元。但对GL_LINEAR模式来说，纹理的环绕模式会影响它对纹理边缘的处理效果。因为它是对周围的纹理元素作一个平均值。

对于GL_CLAMP截取型的环绕模式，还提供了一些选项来处理纹理边缘。GL_CLAMP_TO_EDGE环绕模式简单地忽略边缘的纹理采样，不把它们包括在平均值中。GL_CLAMP_TO_BORDER环绕模式在纹理坐标在0.0到1.0范围之外时只使用边界纹理单元。边界纹理单元是作为围绕基本图像的额外的行和列，与基本纹理图像一起加载的。

下图显示不同参数的效果：

将1张2*2的纹理（分别为红，绿，蓝，黑），贴到一个方块左下角。

方块四个点的纹理坐标依次指定为(0,0), (2,0), (2,2), (0,2)。

根据上述的规则，坐标将进行截取，截取到相应的范围。

我们绘制暗白色线将纹理坐标为1的地方在图中标注出来：

注：border颜色默认为黑色

![](https://img-blog.csdn.net/20161130112334170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 滤镜：GL_NEAREST 缠绕模式：GL_CLAMP 或 GL_CLAMP_TO_EDGE

![](https://img-blog.csdn.net/20161130112338070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

滤镜：GL_NEAREST  缠绕模式：GL_CLAMP_TO_BORDER。

![](https://img-blog.csdn.net/20161130112342561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

滤镜：GL_LINEAR  缠绕模式：GL_CLAMP。

![](https://img-blog.csdn.net/20161130112346680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

滤镜：GL_NEAREST  缠绕模式：GL_CLAMP_TO_EDGE。

![](https://img-blog.csdn.net/20161130112439196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

滤镜：GL_NEAREST  缠绕模式：GL_CLAMP_TO_BORDER。

#### 带纹理的卡通

Toon-shading（也叫单元格着色）使用一维贴图作为一个查找颜色的查找表格去找到其中一个固定的颜色（使用GL_NEAREST)来填充几何图形。

通过几何图形的表面法线与指向光源的向量来计算光源照射到模型表面的强度。表面法线与指向光源向量的点击取值范围为0.0到1.0。以这个值作为顶点纹理坐标，给这个顶点找到对应的纹理元素。下面是一个使用一维纹理来制造卡通效果的例子：

#include "gltools.h" 

#include "math3d.h" #include <math.h> //指向光线的向量 M3DVector3f vLightDir = {-1.0f, 1.0f, 1.0f}; void SetupRC()

{ //一维纹理,逐渐加深的红色 GLbyte toonMap[4][3] = {{32, 0, 0},

  {64, 0, 0}, {128, 0, 0}, {192, 0, 0}};

  glClearColor(0.0f, 0.0f, 0.5f, 1.0f); //开启隐藏面裁剪和深度测试 glCullFace(GL_BACK);

  glFrontFace(GL_CCW);

  glEnable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST); //设置纹理参数和纹理环境 glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);

  glTexEnvf(GL_TEXTURE_1D, GL_TEXTURE_ENV_MODE, GL_DECAL); //加载纹理 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, toonMap);

  glEnable(GL_TEXTURE_1D);

} //画甜甜圈 void DrawDoughnut(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor, M3DVector3f vLightDir)

{

  M3DMatrix44f mInvertLight;

  M3DMatrix44f mModelView;

  M3DVector3f vNewLight;

  M3DVector3f vNormal;

  glGetFloatv(GL_MODELVIEW_MATRIX, mModelView); double majorStep = 2.0 * M3D_PI / numMajor; double minorStep = 2.0 * M3D_PI / numMinor; //把光变换到物体坐标空间中,即乘以反转的模型视图矩阵 m3dInvertMatrix44(mInvertLight, mModelView);

  m3dTransformVector3(vNewLight, vLightDir, mInvertLight);

  vNewLight[0] -= mInvertLight[12];

  vNewLight[1] -= mInvertLight[13];

  vNewLight[2] -= mInvertLight[14];

  m3dNormalizeVector(vNewLight); for (int i = 0; i < numMajor; ++i)

  { double a0 = i * majorStep; double a1 = a0 + majorStep;

    GLfloat x0 = (GLfloat)cos(a0);

    GLfloat y0 = (GLfloat)sin(a0);

    GLfloat x1 = (GLfloat)cos(a1);

    GLfloat y1 = (GLfloat)sin(a1);

  glBegin(GL_TRIANGLE_STRIP); for (int j = 0; j <= numMinor; ++j)

    { double s = j * minorStep;

      GLfloat c = (GLfloat)cos(s);

      GLfloat r = minorRadius * c + majorRadius;

      GLfloat z = minorRadius * (GLfloat)sin(s); //第一个点 vNormal[0] = x0*c;

      vNormal[1] = y0*c;

      vNormal[2] = z/minorRadius;

      m3dNormalizeVector(vNormal); //设置纹理坐标为光的强度 //数学中向量点积就是来判断两个向量的夹角的大小 glTexCoord1f(m3dDotProduct(vNewLight, vNormal));

      glVertex3f(x0*r, y0*r, z); //第二个点 vNormal[0] = x1*c;

      vNormal[1] = y1*c;

      vNormal[2] = z/minorRadius;

      m3dNormalizeVector(vNormal); //设置纹理坐标为光的强度 glTexCoord1f(m3dDotProduct(vNewLight, vNormal));

      glVertex3f(x1*r, y1*r, z);

    }

  glEnd();

  }

} void RenderScene()

{ static GLfloat yRot = 0.0f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

    glTranslatef(0.0f, 0.0f, -2.5f);

    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    DrawDoughnut(0.35f, 0.15f, 50, 25, vLightDir);

  glPopMatrix();

  glutSwapBuffers();

  yRot += 0.5f;

} void TimerFunc(int value)

{

  glutPostRedisplay();

  glutTimerFunc(50, TimerFunc, 1);

} void ChangeSize(GLsizei w, GLsizei h)

{ if (h == 0)

    h = 1;

  GLfloat fAspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  gluPerspective(35.5, fAspect, 1.0, 50.0);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();

  glutPostRedisplay();

} int main(int args, char *argv[])

{

  glutInit(&args, argv);

  glutInitWindowSize(800, 600);

  glutInitDisplayMode(GL_DOUBLE | GL_RGB | GL_DEPTH);

  glutCreateWindow("CARTOON");

  SetupRC();

  glutDisplayFunc(RenderScene);

  glutReshapeFunc(ChangeSize);

  glutTimerFunc(50, TimerFunc, 1);

  glutMainLoop(); return 0;

}

效果图：

![](https://img-blog.csdn.net/20161130112947734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


