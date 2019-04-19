# Nehe的OpenGL框架(MFC版) - xqhrs232的专栏 - CSDN博客
2009年11月20日 15:29:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：889标签：[mfc																[框架																[null																[buffer](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
Nehe的几个OpenGL框架都是Win32
 sdk版本的，我现在需要在MFC下学习OpenGL,今天看了他写的第一个OpenGL框架，就在MFC中实现了下。为了简单起见，把全屏那部分就抛弃掉了，毕竟重点不在这上面，而且MFC要实现这个全屏的功能也不像sdk那么容易。。。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->// openGLDemoView.h : interface of the COpenGLDemoView class
//
/////////////////////////////////////////////////////////////////////////////
protected:
    BOOL SetWindowPixelFormat(HDC hDC);//设置像素格式
    BOOL CreateViewGLContext(HDC hDC);//创建绘制环境(RC)并使之成为当前绘制环境
    BOOL InitGL(GLvoid);//初始化openGL
int DrawGLScene(GLvoid);//绘图代码区
int m_GLPixelIndex;
    HGLRC m_hGLContext;//绘制环境
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->// openGLDemoView.cpp : implementation of the COpenGLDemoView class
//
COpenGLDemoView::COpenGLDemoView()
{
// TODO: add construction code here
this->m_GLPixelIndex =0;
this->m_hGLContext = NULL;
}
BOOL COpenGLDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
// TODO: Modify the Window class or styles here by modifying
//  the CREATESTRUCT cs
    cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);//openGL必需的
return CView::PreCreateWindow(cs);
}
BOOL COpenGLDemoView::SetWindowPixelFormat(HDC hDC)
{//定义窗口的像素格式
    PIXELFORMATDESCRIPTOR pixelDesc=
    {
sizeof(PIXELFORMATDESCRIPTOR),
1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
        PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,
        PFD_TYPE_RGBA,
24,
0,0,0,0,0,0,
0,
0,
0,
0,0,0,0,
32,
0,
0,
        PFD_MAIN_PLANE,
0,
0,0,0
    };
this->m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);//选择最相近的像素格式
if(this->m_GLPixelIndex==0)
    {//选择失败
this->m_GLPixelIndex =1;//默认的像素格式
if(DescribePixelFormat(hDC,this->m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
        {//用默认的像素格式进行设置
return FALSE;
        }
    }
if(SetPixelFormat(hDC,this->m_GLPixelIndex,&pixelDesc)==FALSE)
    {
return FALSE;
    }
return TRUE;
}
BOOL COpenGLDemoView::InitGL(GLvoid)                                        // All Setup For OpenGL Goes Here
{
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
       glClearColor(0.0,0.0,0.0,0.0);// Black Background
    glClearDepth(1.0f);                                    // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations
return TRUE;                                        // Initialization Went OK
}
BOOL COpenGLDemoView::CreateViewGLContext(HDC hDC)
{
this->m_hGLContext = wglCreateContext(hDC);//创建RC
if(this->m_hGLContext==NULL)
    {//创建失败
return FALSE;
    }
if(wglMakeCurrent(hDC,this->m_hGLContext)==FALSE)
    {//选为当前RC失败
return FALSE;
    }
return TRUE;
}
int COpenGLDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
if (CView::OnCreate(lpCreateStruct) ==-1)
return-1;
// TODO: Add your specialized creation code here
    HWND hWnd =this->GetSafeHwnd();    
    HDC hDC = ::GetDC(hWnd);
if(this->SetWindowPixelFormat(hDC)==FALSE)
    {//设置像素格式
return0;
    }
if(this->CreateViewGLContext(hDC)==FALSE)
    {//创建RC并选为所用
return0;
    }
if(!this->InitGL())
    {//初始化openGL
return0;
    }
return0;
}
void COpenGLDemoView::OnDestroy() 
{
    CView::OnDestroy();
// TODO: Add your message handler code here
if(wglGetCurrentContext()!=NULL)
    {
        wglMakeCurrent(NULL,NULL);
    }
if(this->m_hGLContext!=NULL)
    {
        wglDeleteContext(this->m_hGLContext);
this->m_hGLContext = NULL;
    }
}
void COpenGLDemoView::OnSize(UINT nType, int cx, int cy) 
{
    CView::OnSize(nType, cx, cy);
// TODO: Add your message handler code here
    GLsizei width,height;
    width = cx;
    height = cy;
if (height==0)                                        // Prevent A Divide By Zero By
    {
        height=1;                                        // Making Height Equal One
    }
    glViewport(0,0,width,height);                        // Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix
// Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);//透视投影
    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}
void COpenGLDemoView::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
this->DrawGLScene();
}
int COpenGLDemoView::DrawGLScene(GLvoid)                                   
{// Here's Where We Do All The Drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear Screen And Depth Buffer
    glLoadIdentity();                                    // Reset The Current Modelview Matrix
    glTranslatef(-1.5f,0.0f,-6.0f);//物体左移1.5,向内移6,相当于移动镜头一样，让物体进入镜头中
    glBegin(GL_TRIANGLES);                            // 绘制三角形
        glColor3f(255.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);                    // 上顶点
        glColor3f(0.0f,255.0f,0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);                    // 左下
        glColor3f(0.0f,0.0f,255.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);                    // 右下
    glEnd();                                // 三角形绘制结束
glTranslatef(3.0f,0.0f,0.0f);
glBegin(GL_QUADS);                            //  绘制正方形
        glColor3f(255.0f,0.0f,0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);                    // 左上
        glColor3f(0.0f,255.0f,0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);                    // 右上
        glColor3f(0.0f,0.0f,255.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);                    // 左下
        glColor3f(255.255f,255.0f,255.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);                    // 右下
    glEnd();                                // 正方形绘制结束
    glFlush();
return TRUE;                                        // Everything Went OK
}
![200772901..jpg](http://images.cnblogs.com/cnblogs_com/phinecos/070614/200772901..jpg)
