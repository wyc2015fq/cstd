# OpenGL坐标体系理解之一(转载) - xqhrs232的专栏 - CSDN博客
2010年01月29日 14:55:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1239标签：[float																[2010																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
原文地址：[http://hi.baidu.com/xoxoxo/blog/item/ffb9ed116db105cfa7ef3fa8.html](http://hi.baidu.com/xoxoxo/blog/item/ffb9ed116db105cfa7ef3fa8.html)
我CSDN上相关的贴子:http://hi.csdn.net/link.php?url=http://topic.csdn.net%2Fu%2F20100129%2F13%2Fdfec2c96-3b6a-4f0e-af1a-790b1bf90f70.html
//####################################################################################################
//举例论证
float mp[16];
float mt[16];
float mvp[16];
 enum {GLH_WIDTH = 800,GLH_HEIGHT=480};
 float aspect =GLH_WIDTH/GLH_HEIGHT;
 static float rot = 0;
         rot += 5;
 matIdentity(mp);
 matIdentity(mt);
 matIdentity(mvp);
 matPerspective(mp, 90,  aspect, 1, 1001);
        matTranslate(mt, 0, 0, -400.0f);
        matRotate(mt, rot, 0, 1.0f, 0.0f);
 matMult((float*)mvp, (float*)mp, (float*)mt);
//计算出来的mvp的矩阵最后用于对定义的顶点进行变换。
//再看顶点的坐标定义值
static float vw = 400; 
static float vh = 400;
 static GLfloat vVerticesRect1[] = {
  -vw,  vh, 0,
  -vw, -vh, 0,
  vw, -vh, 0,
  vw, vh, 0,
 };
 static GLfloat vVerticesRect2[] = {
                vw,  vh, 0,
  vw, -vh, 0,
  3*vw, -vh, 0,
  3*vw, vh, 0,
 };
       static const GLfloat cube_texcoord[] =
 {
  0.0, 1.0,
  0.0, 0.0, 
  1.0, 0.0,
  1.0, 1.0,
 };
//###########################################################################3
//基本的分析
1。为什么定义的顶点最大值值为+400，最小值为-400？------------因为matTranslate(mt, 0, 0, -400.0f);在Z轴方向移动了400个单位
2。matPerspective(mp, 90,  aspect, 1, 1001);表示视角为90度，因为建立的是对称的透视投影矩阵，所以其实在半边是45度的视角，这样就建立了一个等腰三角形的视角。
    可视的近远平面距离为(1，1001)，所以近平面的大小为2*2的正方形界面，matTranslate(mt, 0, 0, -400.0f)让物体离相机400个单位，视角为45度，所以在物体平面上的大小为
    800*800的正方形界面，这样物体平面的坐标范围为(-400，-400)~~(400，400)，视点原点在中间(0，0)处
3。纹理的坐标范围一直在(0，1)之间
4。物体平面一般界于近平面和远平面之间，如果物体平面定义的物体顶点正好占住了整个物体平面，那么远平面上的东西就被物体平面挡住了从而看不到了。
5。永远要记得透视投影的相似三角形边的对应比例关系，有了这个比例关系，知道近平面的坐标范围就可以推算物体平面允许的坐标范围值。这样就可以定义物体平面上顶点的值了。
6。透视投影的两个相关函数
A.glFrustum----通过数值,其操作是创建一个透视投影矩阵
B.gluPerspective-----通过角度,其操作是创建一个对称的透视投影矩阵 
上面的两个函数的意义其实是一样的,都是建立一种三角形边的比例关系,不过glFrustum通过的数据长度来限制,gluPerspective通过的是一个角度(实际的角度为其值的一半,因为是对称的)
7。glFrustum+gluPerspective中的zNear,zFar建立了透视投影三角形边的比例关系。像上面的例子中的比例关系就是1/1001。再有了视角值，近平面/物体平面/远平面允许的坐标范围都可以计算出来了。
8。物体平面定义的顶点的Y轴跟纹理的Y轴的正方向都是向下的，X轴正轴都向右，但顶点坐标的原点在屏幕中心处，纹理坐标的原点在屏幕左上角(跟GDI的原点一致)。
 9。世界坐标/本地坐标/屏幕坐标最开始是一致的，原点(0,0)都在屏幕的中央
10。屏幕坐标的范围在没有改变的时候是(-1,-1)~~(1,1)-----相机位置改变+视角改变了这个屏幕允许(可看的到)的坐标范围就会对应变化。
11。透视投影是建立一个透视视景体----也是一个棱台
