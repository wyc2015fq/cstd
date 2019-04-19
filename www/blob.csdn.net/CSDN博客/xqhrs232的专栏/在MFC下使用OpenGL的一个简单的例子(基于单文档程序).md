# 在MFC下使用OpenGL的一个简单的例子(基于单文档程序) - xqhrs232的专栏 - CSDN博客
2009年11月05日 14:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1106
 最近开始准备做OpenGL方面的东西，万事开头难，真是体会颇深，简直就是寸步难行。有时候发各帖子到网上，不停地刷新，希望能有高手指点，可是，每次收获并不是太大。也许，每个程序员都有一个痛苦的过程，我当然不能例外了。不过，痛苦之后有收获才是最大的快乐。
       正因为这个原因，当我度过了自己的第一难关时，心里爽死了；不过，想到自己的痛苦经历，真是往事不堪回首；想必还会有人不断地开始学习OpenGL吧，希望我写出来的东西，能减轻他们的痛苦经历。
      在MFC下编OpenGL的程序，首先要设置环境：
      首先在XXView.h下，加入头文件
        #include"gl/gl.h"
        #include"gl/glu.h"
        #include"gl/glaux.h"
     然后再把下面的文件加入到自己的工程中。
          VC所在的盘:/Program Files/Microsoft Visual Studio/VC98/Lib/OPENGL32.LIB
           VC所在的盘:/Program Files/Microsoft Visual Studio/VC98/Lib/GLU32.LIB.LIB
           VC所在的盘:/Program Files/Microsoft Visual Studio/VC98/Lib/GLAUX.LIB.LIB
     可能有些你目前用不到，不过没有关系，以后再用就不用改了。
     下面是主要的四部分：
         4－1：
    void CMySunView::OnInitialUpdate() 
{
 CView::OnInitialUpdate();
 //
 m_pDC = new CClientDC(this);
 m_hDC=m_pDC->GetSafeHdc();
 static PIXELFORMATDESCRIPTOR pfd = 
 {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW   |          // support window
          PFD_SUPPORT_OPENGL |          // support OpenGL
          PFD_DOUBLEBUFFER,             // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        16,                             // 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int   m_PixelFormat;
    m_PixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
    ::SetPixelFormat(m_hDC, m_PixelFormat, &pfd); 
 //
 m_hRC = ::wglCreateContext( m_hDC ); 
 wglMakeCurrent( m_hDC, m_hRC ); 
}
4－2：
void CMySunView::OnDraw(CDC* pDC)
{
 CMySunDoc* pDoc = GetDocument();
 ASSERT_VALID(pDoc);
 // 
 ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 float size=100.0f;
 glLineWidth(3.0f);  // 
 // axis -- x
 glColor3f(1,0,0);
 glBegin(GL_LINES); 
 glVertex3f(-size,0,0);
 glVertex3f(size,0,0);
 glEnd();
 // axis -- y
 glColor3f(0,1,0);
 glBegin(GL_LINES); 
glVertex3f(0,-size,0);
 glVertex3f(0,size,0);
 glEnd();
 // axis -- z
 glColor3f(0,0,1);
 glBegin(GL_LINES); 
 glVertex3f(0,0,-size);
 glVertex3f(0,0,size);
 glEnd();
 //
 glLineWidth(1.0f); // ssj 
 ::glFinish(); 
 SwapBuffers( m_hDC ); 
}
4－3
void CMySunView::OnSize(UINT nType, int cx, int cy) 
{
 CView::OnSize(nType, cx, cy);
 //
 if ( 0 >= cx || 0 >= cy )
  return;
 ::glClearColor(0.0f, 0.0f, 0.0f, 255.0f);
 // specify the back of the buffer as clear depth
    ::glClearDepth(1.0f);
 // enable depth testing
    ::glEnable(GL_DEPTH_TEST);
 ::glViewport(0, 0, cx, cy); 
 ::glMatrixMode( GL_PROJECTION );
 ::glLoadIdentity();
 ::glOrtho( xOrg - xMax, xOrg + xMax, yOrg - yMax, yOrg + yMax,
  zOrg-zMax, zOrg + zMax);
 ::glRotatef( xrotate, 1.0f, 0.0f, 0.0f );
 ::glRotatef( yrotate, 0.0f, 1.0f, 0.0f );
 ::glRotatef( zrotate, 0.0f, 0.0f, 1.0f );
 ::glMatrixMode( GL_MODELVIEW); 
 ::glLoadIdentity();
 Invalidate();
}
4－4
BOOL CMySunView::OnEraseBkgnd(CDC* pDC) 
{
      return true;
}
void CMySunView::OnDestroy() 
{
 CView::OnDestroy();
 wglDeleteContext(m_hRC); 
}
  CMySunView::CMySunView()
{
  xrotate=45.0f;
  yrotate=0.0f;
  zrotate=-15.0f; 
  xOrg=0, xMax=42;
  yOrg=0, yMax=70;
  zOrg=0, zMax=40;
}
class CMySunView : public CView
{
..........
public:
 HGLRC m_hRC;
 HDC   m_hDC;
 CDC* m_pDC;
 int   width,height;
 float xrotate;
 float yrotate;
 float zrotate; 
 float xOrg, xMax;
 float yOrg, yMax;
 float zOrg, zMax;
.........
}
        上面是主要的步骤，当然，其他个性化的东西都可以添加上去，这是最小化的最基本的东西，可能也会有不妥地地方，不过已经能看到东西了。
        我想程序这么简单就不要做太多的解释了，有兴趣的话，继续探讨。
