# M8 AlphaBlt使用方法 - xqhrs232的专栏 - CSDN博客
2012年10月29日 22:56:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：762
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/loseleo/article/details/4873649](http://blog.csdn.net/loseleo/article/details/4873649)
相关网帖
1.100分求一个IDirectDrawSurface::AlphaBlt的例子----[http://topic.csdn.net/u/20100528/13/6e389217-0fb4-4625-b3a6-900bda75057c.html?seed=374487230&r=80027050#r_80027050](http://topic.csdn.net/u/20100528/13/6e389217-0fb4-4625-b3a6-900bda75057c.html?seed=374487230&r=80027050#r_80027050)子
# AlphaBlend到底能不能进行逐像素的Alpha混合
2.AlphaBlend到底能不能进行像素级的Alpha混合----[http://topic.csdn.net/u/20091021/13/88832508-e01a-405f-acff-91f965439da2.html?seed=1962093460&r=80027131#r_80027131](http://topic.csdn.net/u/20091021/13/88832508-e01a-405f-acff-91f965439da2.html?seed=1962093460&r=80027131#r_80027131)
3.有哪位用过DirectDraw的AlphaBlt----[http://bbs.meizu.com/viewthread.php?tid=922873](http://bbs.meizu.com/viewthread.php?tid=922873)
啥也不说，先上代码
static int nAlpha = 0;
  int step = 10;//这个是用来设置步长的，也就是每次衰减或者增加多少
  DDALPHABLTFX alphatx;
  alphatx.dwSize = sizeof(alphatx);
  alphatx.ddargbScaleFactors.alpha = nAlpha;
  pDDSurface->AlphaBlt(NULL, m_pDDSBackground, NULL, DDBLT_WAITNOTBUSY, &alphatx);
  nAlpha = nAlpha + step;// set the alpha value
  if(nAlpha > UCHAR_MAX)
  {
   nAlpha = UCHAR_MAX;
   m_bStartButtonDown =false;//reset the value about transmate
   nAlpha = 0;
   theAPP.m_MainWnd.SwitchScene(1);//Change to scene 1
  }
应该很明显了，就不讲解了。另外提供一个清屏代码
{
 DDBLTFX ddbltfx;
 memset(&ddbltfx, 0, sizeof(ddbltfx));
 ddbltfx.dwSize = sizeof(ddbltfx);
 ddbltfx.dwFillColor = 0;
 pSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAITNOTBUSY, &ddbltfx);
}

//====================================================
备注::
1>不知道DirectDraw里面的Alphablt跟GDI的AlphaBlend函数比较起来，效率怎么样，那个更快，那个更慢！
2>不知道DirectDraw里面的Alphablt能不能调到系统的硬件加速功能
3>在一些地方看到如果不支持DirectDraw的硬件加速就会调Hardware Emulation Layer，其实也就是GDI接口
文章地址::[http://www.cnblogs.com/wangkewei/archive/2009/12/24/1631667.html](http://www.cnblogs.com/wangkewei/archive/2009/12/24/1631667.html)
下图是DirectDraw的架构，从中我们看到DirectDraw走最右边的一条线时即使用了硬件加速，硬件不支持需要用Software Emulation(软件模拟实现)时会走Hardware Emulation Layer这条线，其实调用的是GDI的API(因为需要CPU去处理，所以叫做软件模拟实现)。
![clip_image002[7]](http://images.cnblogs.com/cnblogs_com/wangkewei/WindowsLiveWriter/Windows2D2_DBBC/clip_image002%5B7%5D_thumb.jpg)
4》DirectDraw中的AlphaBlt只能指定一个常量的ALPHA值进行混合？不能像AlphaBlend那样做到像素级的混合？
5》Alpha混合很多时候要进行混合的乘法运算，特别是像素级别的混合做的乘法运算就更多了，乘法运算一般很难通过GPU去完成而不需要CPU的参与，从这个层面
是不是可以说Alpha混合很难去用GPU来实现硬件的加速处理，不知道这样理解对不对？！

