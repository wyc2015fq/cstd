# opengl绘制Bezier Curve - youfangyuan - CSDN博客
2013年08月15日 22:15:54[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1375标签：[opengl](https://so.csdn.net/so/search/s.do?q=opengl&t=blog)
个人分类：[图形图像](https://blog.csdn.net/youfangyuan/article/category/1566503)
[http://read.pudn.com/downloads16/sourcecode/windows/opengl/60869/OpenGL%20-%20Bezier%20Curve-1/Bezier%20Curve.c__.htm](http://read.pudn.com/downloads16/sourcecode/windows/opengl/60869/OpenGL%20-%20Bezier%20Curve-1/Bezier%20Curve.c__.htm)
```cpp
#include "glut.h"  
  
int Line_Count = 80;   
// 线条数越大 曲面越平滑   
// 线条数越小 曲面越粗糙   
// 可以从 左下 右下 两条曲线得到对比  
  
// 左上曲线控制点  
GLfloat ControlPoints0[4][3] =   
{  
    { -4.0 ,  1.0 , 0.0 } , // 控制点在数组中的顺序决定曲线样式  
    { -1.0 ,  1.0 , 0.0 } ,  
    { -1.0 ,  4.0 , 0.0 } ,   
    { -4.0 ,  4.0 , 0.0 }     
};  
  
// 右上曲线控制点  
GLfloat ControlPoints1[4][3] =   
{  
    {  4.0 ,  1.0 , 0.0 } ,   
    {  1.0 ,  4.0 , 0.0 } ,   
    {  1.0 ,  1.0 , 0.0 } ,   
    {  4.0 ,  4.0 , 0.0 }     
};  
  
// 右下曲线控制点  
GLfloat ControlPoints2[4][3] =   
{  
    {  1.0 , -4.0 , 0.0 } ,  
    {  4.0 , -4.0 , 0.0 } ,   
    {  1.0 , -1.0 , 0.0 } ,  
    {  4.0 , -1.0 , 0.0 }  
};  
  
// 左下曲线控制点  
GLfloat ControlPoints3[4][3] =   
{  
    { -4.0 , -4.0 , 0.0 } ,  
    { -1.0 , -4.0 , 0.0 } ,   
    { -4.0 , -1.0 , 0.0 } ,  
    { -1.0 , -1.0 , 0.0 }  
};  
  
void Display( void )  
{  
    int i;  
  
    glClear( GL_COLOR_BUFFER_BIT );  
    glLoadIdentity();  
  
    // 先绘制控制点 再绘制曲线  
    // 左上曲线  
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ControlPoints0[0][0]);   
    glColor3f( 0 , 0 , 1.0 );  
    glBegin( GL_POINTS );  
        for( i = 0 ; i < 4 ; i++ )   
        {  
            glVertex3fv( ControlPoints0[i] );  
        }  
    glEnd();  
  
    glColor3f(0.0, 1.0, 0.0);  
    glBegin( GL_LINE_STRIP );  
        for( i = 0 ; i <= Line_Count ; i++ )   
        {  
            glEvalCoord1f( (GLfloat)i/Line_Count );  
        }    
    glEnd();  
  
    // 右上曲线  
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ControlPoints1[0][0]);  
    glColor3f( 1.0 , 0.0 , 0.0 );  
    glBegin( GL_POINTS );  
        for( i = 0 ; i < 4 ; i++ )   
        {  
            glVertex3fv( ControlPoints1[i] );  
        }  
    glEnd();  
  
    glColor3f(0.0, 1.0, 0.0);  
    glBegin( GL_LINE_STRIP );  
        for( i = 0 ; i <= Line_Count ; i++ )   
        {  
            glEvalCoord1f( (GLfloat)i/Line_Count );  
        }    
    glEnd();  
  
    // 右下曲线  
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ControlPoints2[0][0]);  
    glColor3f( 0.0 , 0.0 , 1.0 );  
    glBegin( GL_POINTS );  
        for( i = 0 ; i < 4 ; i++ )   
        {  
            glVertex3fv( ControlPoints2[i] );  
        }  
    glEnd();  
  
    glColor3f(0.0, 1.0, 0.0);  
    glBegin( GL_LINE_STRIP );  
        for( i = 0 ; i <= Line_Count ; i++ )   
        {  
            glEvalCoord1f( (GLfloat)i/Line_Count );  
        }    
    glEnd();  
  
    // 左下曲线  
    Line_Count = 8; // 降低线条数 曲线变粗糙  
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ControlPoints3[0][0]);  
    glColor3f( 1.0 , 1.0 , 0.0 );  
    glBegin( GL_POINTS );  
        for( i = 0 ; i < 4 ; i++ )   
        {  
            glVertex3fv( ControlPoints3[i] );  
        }  
    glEnd();  
  
    glColor3f(0.0, 1.0, 0.0);  
    glBegin( GL_LINE_STRIP );  
        for( i = 0 ; i <= Line_Count ; i++ )   
        {  
            glEvalCoord1f( (GLfloat)i/Line_Count );  
        }    
    glEnd();  
  
    glFlush();  
}  
  
void Initialize()  
{  
    glClearColor( 0.0 , 0.0 , 0.0 , 0.0 );  
    glShadeModel( GL_SMOOTH );  
    glEnable(GL_MAP1_VERTEX_3);  
    glEnable( GL_LINE_SMOOTH ); // 平滑线条  
    glPointSize( 6.0 ); // 把点变大一点 看的清楚  
}  
  
void Reshape( int Width , int Height )  
{  
    glViewport( 0 , 0 , (GLsizei)Width , (GLsizei)Height );  
  
    glMatrixMode( GL_PROJECTION );  
    glLoadIdentity();  
  
    if( Width <= Height )  
    {  
        glOrtho( -5.0 , 5.0 , -5.0 * (GLfloat)Height / (GLfloat)Width , 5.0 * (GLfloat)Height / (GLfloat)Width , -5.0 , 5.0 );  
    }  
  
    else  
    {  
        glOrtho( -5.0 * (GLfloat)Width / (GLfloat)Height , 5.0 * (GLfloat)Width / (GLfloat)Height , -5.0 , 5.0 , -5.0 , 5.0 );  
    }  
  
    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity();  
}  
  
void main()  
{  
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );  
    glutInitWindowSize( 700 , 700 );  
    glutInitWindowPosition( 162 , 34 );  
    glutCreateWindow( "Bezier Curve" );  
    Initialize();  
    glutDisplayFunc( Display );  
    glutReshapeFunc( Reshape );  
    glutMainLoop();  
}
```
