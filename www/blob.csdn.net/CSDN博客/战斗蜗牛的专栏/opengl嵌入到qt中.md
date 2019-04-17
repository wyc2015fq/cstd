# opengl嵌入到qt中 - 战斗蜗牛的专栏 - CSDN博客





2012年06月11日 11:44:58[vbskj](https://me.csdn.net/vbskj)阅读数：1452









### [OpenGL嵌入到Qt中](http://blog.csdn.net/tulun/article/details/5581132)


分类： [Qt](http://blog.csdn.net/tulun/article/category/681553)2010-05-12
 09:16211人阅读[评论](http://blog.csdn.net/tulun/article/details/5581132#comments)(0)[收藏](http://blog.csdn.net/tulun/article/details/5581132)[举报](http://blog.csdn.net/tulun/article/details/5581132#report)



将OpenGL嵌入到Qt中的流程如下：




nehewidget.h







**[c-sharp]**[view
 plain](http://blog.csdn.net/tulun/article/details/5581132#)[copy](http://blog.csdn.net/tulun/article/details/5581132#)[print](http://blog.csdn.net/tulun/article/details/5581132#)[?](http://blog.csdn.net/tulun/article/details/5581132#)



- #ifndef NEHEWIDGET_H
- #define NEHEWIDGET_H
- #include <Qt/qgl.h>
- 
- class NeHeWidget : public QGLWidget  
- {  
-     Q_OBJECT  
- 
- public:  
- 
-   NeHeWidget( QWidget* parent = 0, constchar* name = 0, bool fs = false );  
-   ~NeHeWidget();  
- 
- protected:  
- 
- void initializeGL();  
- void paintGL();  
- void resizeGL( int width, int height );  
- 
- void keyPressEvent( QKeyEvent *e );  
- void loadGLTextures();  
- 
- protected:  
- 
- bool fullscreen;  
-   GLfloat xRot, yRot, zRot;  
-   GLfloat zoom;  
-   GLfloat xSpeed, ySpeed;  
-   GLuint texture[3];  
-   GLuint filter;  
- 
- bool light;  
- };  
- 
- #endif // NEHEWIDGET_H







nehewidget.cpp







**[c-sharp]**[view
 plain](http://blog.csdn.net/tulun/article/details/5581132#)[copy](http://blog.csdn.net/tulun/article/details/5581132#)[print](http://blog.csdn.net/tulun/article/details/5581132#)[?](http://blog.csdn.net/tulun/article/details/5581132#)



- #include <QtGui/QApplication>
- #include <QtGui/QMainWindow>
- #include <QtGui/QWidget>
- #include <QtGui/QImage.h>
- #include <QtGui/qevent.h>
- 
- #include "nehewidget.h"
- 
- GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };  
- GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };  
- GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };  
- 
- NeHeWidget::NeHeWidget( QWidget* parent, constchar* name, bool fs )  
-     : QGLWidget( parent/*, name */)  
- {  
-   xRot = yRot = zRot = 0.0;  
-   zoom = -5.0;  
-   xSpeed = ySpeed = 0.0;  
- 
-   filter = 0;  
- 
-   light = false;  
- 
-   fullscreen = fs;  
-   setGeometry( 100, 100, 640, 480 );  
- //setCaption("NeHe's Texture, Lighting & Keyboard Tutorial");
- 
- if ( fullscreen )  
-     showFullScreen();  
- }  
- 
- NeHeWidget::~NeHeWidget()  
- {  
- }  
- 
- void NeHeWidget::paintGL()  
- {  
-   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
-   glLoadIdentity();  
-   glTranslatef(  0.0,  0.0, zoom );  
- 
-   glRotatef( xRot,  1.0,  0.0,  0.0 );  
-   glRotatef( yRot,  0.0,  1.0,  0.0 );  
- 
-   glBindTexture( GL_TEXTURE_2D, texture[filter] );  
- 
-   glBegin( GL_QUADS );  
-     glNormal3f( 0.0, 0.0, 1.0 );  
-     glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );  
-     glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );  
-     glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );  
-     glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );  
- 
-     glNormal3f( 0.0, 0.0, -1.0 );  
-     glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );  
-     glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );  
-     glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );  
-     glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );  
- 
-     glNormal3f( 0.0, 1.0, 0.0 );  
-     glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );  
-     glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );  
-     glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );  
-     glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );  
- 
-     glNormal3f( 0.0, -1.0, 0.0 );  
-     glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );  
-     glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );  
-     glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );  
-     glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );  
- 
-     glNormal3f( 1.0, 0.0, 0.0 );  
-     glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );  
-     glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );  
-     glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );  
-     glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );  
- 
-     glNormal3f( -1.0, 0.0, 0.0 );  
-     glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );  
-     glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );  
-     glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );  
-     glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );  
-   glEnd();  
- 
-   xRot += xSpeed;  
-   yRot += ySpeed;  
- }  
- 
- void NeHeWidget::initializeGL()  
- {  
-   loadGLTextures();  
- 
-   glEnable( GL_TEXTURE_2D );  
-   glShadeModel( GL_SMOOTH );  
-   glClearColor( 0.0, 0.0, 0.0, 0.5 );  
-   glClearDepth( 1.0 );  
-   glEnable( GL_DEPTH_TEST );  
-   glDepthFunc( GL_LEQUAL );  
-   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );  
- 
-   glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );  
-   glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );  
-   glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );  
- 
-   glEnable( GL_LIGHT1 );  
- }  
- 
- void NeHeWidget::resizeGL( int width, int height )  
- {  
- if ( height == 0 )  
-   {  
-     height = 1;  
-   }  
-   glViewport( 0, 0, (GLint)width, (GLint)height );  
-   glMatrixMode( GL_PROJECTION );  
-   glLoadIdentity();  
-   gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );  
-   glMatrixMode( GL_MODELVIEW );  
-   glLoadIdentity();  
- }  
- 
- void NeHeWidget::keyPressEvent( QKeyEvent *e )  
- {  
- switch ( e->key() )  
-   {  
- case Qt::Key_L:  
-     light = !light;  
- if ( !light )  
-     {  
-       glDisable( GL_LIGHTING );  
-     }  
- else
-     {  
-       glEnable( GL_LIGHTING );  
-     }  
-     updateGL();  
- break;  
- case Qt::Key_F:  
-     filter += 1;;  
- if ( filter > 2 )  
-     {  
-       filter = 0;  
-     }  
-     updateGL();  
- break;  
- case Qt::Key_PageUp:  
-     zoom -= 0.2;  
-     updateGL();  
- break;  
- case Qt::Key_PageDown:  
-     zoom += 0.2;  
-     updateGL();  
- break;  
- case Qt::Key_Up:  
-     xSpeed -= 0.01;  
-     updateGL();  
- break;  
- case Qt::Key_Down:  
-     xSpeed += 0.01;  
-     updateGL();  
- break;  
- case Qt::Key_Right:  
-     ySpeed += 0.01;  
-     updateGL();  
- break;  
- case Qt::Key_Left:  
-     ySpeed -= 0.01;  
-     updateGL();  
- break;  
- case Qt::Key_F2:  
-     fullscreen = !fullscreen;  
- if ( fullscreen )  
-     {  
-       showFullScreen();  
-     }  
- else
-     {  
-       showNormal();  
-       setGeometry( 100, 100, 640, 480 );  
-     }  
-     update();  
- break;  
- case Qt::Key_Escape:  
-     close();  
-   }  
- }  
- 
- void NeHeWidget::loadGLTextures()  
- {  
-   QImage tex, buf;  
- if ( !buf.load( ":/MyPicture/Crate.bmp" ) )  
-   {  
-     qWarning( "Could not read image file, using single-color instead." );  
-  QImage dummy( 128, 128, /*32*/QImage::Format_Indexed8);  
-  dummy.fill( Qt::green );  
-     buf = dummy;  
-   }  
-   tex = QGLWidget::convertToGLFormat( buf );  
- 
-   glGenTextures( 3, &texture[0] );  
- 
-   glBindTexture( GL_TEXTURE_2D, texture[0] );  
-   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );  
-   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );  
-   glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,  
-       GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );  
- 
-   glBindTexture( GL_TEXTURE_2D, texture[1] );  
-   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );  
-   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
-   glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,  
-       GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );  
- 
-   glBindTexture( GL_TEXTURE_2D, texture[2] );  
-   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );  
-   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );  
-   gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, tex.width(), tex.height(),  
-       GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );  
- }  









main.cpp







**[c-sharp]**[view
 plain](http://blog.csdn.net/tulun/article/details/5581132#)[copy](http://blog.csdn.net/tulun/article/details/5581132#)[print](http://blog.csdn.net/tulun/article/details/5581132#)[?](http://blog.csdn.net/tulun/article/details/5581132#)



- #include <QtGui/QApplication>
- #include <QtGui/QMainWindow>
- #include <QtGui/QWidget>
- #include <QtGui/QMessageBox>
- #include <Qt/qgl.h>
- 
- #include "nehewidget.h"
- 
- int main( int argc, char **argv )  
- {  
- bool fs = false;  
- 
-   QApplication a(argc,argv);  
- 
- switch( QMessageBox::information( 0,  
- "Start FullScreen?",  
- "Would You Like To Run In Fullscreen Mode?",  
-       QMessageBox::Yes,  
-       QMessageBox::No | QMessageBox::Default ) )  
-   {  
- case QMessageBox::Yes:  
-     fs = true;  
- break;  
- case QMessageBox::No:  
-     fs = false;  
- break;  
-   }  
- 
-   NeHeWidget w( 0, 0, fs );  
- //a.setMainWidget( &w );//QT4.X以上版本不需要了
- //a.setActiveWindow( &w );
-   w.show();  
- 
- return a.exec();  
- }  







 Qt_2.pro  这是配置文件







**[c-sharp]**[view
 plain](http://blog.csdn.net/tulun/article/details/5581132#)[copy](http://blog.csdn.net/tulun/article/details/5581132#)[print](http://blog.csdn.net/tulun/article/details/5581132#)[?](http://blog.csdn.net/tulun/article/details/5581132#)



- INCLUDEPLAT += E:/Qt/ogl/gl  
- LIBS += -L/E:/Qt/ogl /  
-     -lglu32  
- LIBS += -L/E:/Qt/ogl /  
-     -lopengl32  
- QT += opengl  
- HEADERS += nehewidget.h  
- SOURCES += nehewidget.cpp /  
-     main.cpp  
- RESOURCES += nehe.qrc  









nehe.qrc   这是资源文件







**[c-sharp]**[view
 plain](http://blog.csdn.net/tulun/article/details/5581132#)[copy](http://blog.csdn.net/tulun/article/details/5581132#)[print](http://blog.csdn.net/tulun/article/details/5581132#)[?](http://blog.csdn.net/tulun/article/details/5581132#)



- <RCC>  
-   <qresource prefix="MyPicture" >  
-      <file>Crate.bmp</file>  
-   </qresource>  
- </RCC>  





Crate.bmp是个图片




在QtCreator中创建一个Qt空工程，


将以上几个文件加入到工程中。


就可以编译运行了。




环境框架：



![](http://hi.csdn.net/attachment/201005/12/0_1273627672X3ad.gif)








我的实现结果：




![](http://hi.csdn.net/attachment/201005/12/0_1273627626n8Nj.gif)






