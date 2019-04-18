# QT显示框架嵌入Vs控制台工程 - wishchinYang的专栏 - CSDN博客
2015年12月03日 16:46:21[wishchin](https://me.csdn.net/wishchin)阅读数：1635
**  一、一些准备工作：**
1.安装Qt for VS 的插件；
        安装Qt for VS 的插件
        下载地址：[http://download.qt.io/official_releases/vsaddin/](http://download.qt.io/official_releases/vsaddin/)
        安装Qt 4.8.6
        下载地址：[http://download.qt.io/official_releases/qt/4.8/4.8.6/qt-opensource-windows-x86-vs2010-4.8.6.exe.mirrorlist](http://download.qt.io/official_releases/qt/4.8/4.8.6/qt-opensource-windows-x86-vs2010-4.8.6.exe.mirrorlist)
 2.进行一些设置：
        找到菜单项：
        Qt——> Qt Option——> 选取版本   
![](https://img-blog.csdn.net/20151203164235258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
——>  ——>
在工程选项中添加必须的包含文件和lib文件
然后可以使用Qt
**二、工程和代码：**
Vs控制台工程可以直接使用Qt的显示框架，使用类似于Qt-IDE的主函数代码：
```cpp
int _tmain(int argc, char* argv[])
{
    QApplication a(argc, argv);
    CPlot *objViewer = new CPlot();
    objViewer->show();
    
    return a.exec();
}
```
头文件代码：
```cpp
#pragma once
/*
*/
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QGLWidget>
#include <gl/glut.h>
#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include "OpenGLViewer.h"
class QApplication;
class QMainWindow;
class QWidget;
class QAction;
class QMenu;
class QToolBar;
class CPlot: public QMainWindow{
	Q_OBJECT
public:
	CPlot(QWidget *parent = 0);
	//CPlot();
	~CPlot(void);
	//测试OpenGL画图
public:
	static void RenderScene();
	void SetupRC();
	static void ChangeSize( GLsizei w, GLsizei h );
	void drawCircle(int argc, char *argv[]);
	GLfloat boundingRadius;
	GLfloat LightDistanceRatio;
	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	GLfloat xscale;
	GLfloat yscale;
	GLfloat zscale;
	GLfloat transX;
	GLfloat transY;
	GLfloat transZ;
	void draw3dAxis();
	void draw3dAxis(int argc, char *argv[]);
	void updatePos(const Mat  &rMat);
private:
	CEkfSlam m_Slamer;
	private slots:
		void openFile();
		//void closeFile();
		void segmentObj();
		void capture();
private:
	OpenGLViewer *openglViewer;
private:
	void initializeGL();
	void setMaterial();
	void GLMaterial(const OpenGL::Material& material);
	void setLight();
	void setAntiAliasing();
	float getBoundingRadius();
	void setTexture(IplImage* img);
	void loadTexture();
	void resetGLLightPosition();
	void loadMeshFile(char* filename);
	void createActions();
	void createMenus();
	void createToolBars();
private:
	QAction *loadFileAction;
	QAction *closeFileAction;
	QAction *segmentObjAction;
	QAction *captureAction;
	QMenu *fileMenu;
	QMenu *toolMenu;
	QToolBar *fileToolBar;
	QToolBar *toolsBar;
	vector<int> faceColors;
	vector<QColor> FaceColorList;
	vector<double>ssdf;
	QPoint lastPos;
	Core::Geometry::MyMesh *mesh;
	int key_type;
	GLuint texName;
	Mat textImage;
public:
	//1.使用 OpenCV Mat画图！用于显示图像和特征点匹配！
	cv::Mat  m_Canvas;
	cv::Mat  m_CanvasSrc;
private:
	//2.使用 VTK画出点云！用于显示地图和方位演化！
	Eigen::MatrixXf  m_FeatureMap;
	//画出十字光标
	int cvDrawCrossCursor(
		cv::Mat &Canvas,cv::Point &PointS,cv::Point &PointE,cv::Scalar &Color,int Width,int CV_A_Type,int Mark);
	//画十字光标，中心点、线长度、色彩、线宽
	int cvDrawCrossCursor(
		cv::Mat &Canvas,cv::Point &Center,int Length,cv::Scalar &Color,int Width,int CV_A_Type,int Mark);
};
```
源码文件代码：
```cpp
#include "StdAfx.h"
#include "Plot.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QMenuBar>
#include <QDesktopWidget>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
class QFileDialog;
class QString;
class QMenuBar;
class QDesktopWidget;
using namespace Qt;
using namespace OpenGL;
////此种用法是错误的，使用mainwindow之前必须构建一个Application！
CPlot::CPlot(QWidget *parent) : QMainWindow(parent){
	openglViewer = new OpenGLViewer();
	this->setCentralWidget(openglViewer);
	this->setWindowTitle("Wishchin's PCL Window");
	this->setGeometry((QApplication::desktop()->width()-1.5 *QApplication::desktop()->height())/2,20,640,480);
	this->createActions();
	this->createMenus();
	this->createToolBars();
	//初始化画布
	this->m_Canvas.create(640,480,CV_8UC3);
	this->m_CanvasSrc.create(640,480,CV_8UC3);
	//this->m_FeatureMap.resize(0);
}
//CPlot::CPlot(){
//	//初始化画布
//	this->m_Canvas.create(640,480,CV_8UC3);
//
//	//this->m_FeatureMap.resize(0);
//}
CPlot::~CPlot(void)
{
}
void CPlotMark0022(){}
//画出十字光标
int CPlot::cvDrawCrossCursor(
	cv::Mat &Canvas,cv::Point &PointS,cv::Point &PointE,cv::Scalar &Color,int Width,int CV_A_Type,int Mark)
{
	return 1;
}
//画十字光标，中心点、线长度、色彩、线宽
int CPlot::cvDrawCrossCursor(
	cv::Mat &Canvas,cv::Point &Center,int Length,cv::Scalar &Color,int Width,int CV_A_Type,int Mark)
{
	int H = Length/2;
	cv::Point PointS;cv::Point PointE;
	PointS.x =Center.x ;
	PointS.y =Center.y -H;
	PointE.x =Center.x ;
	PointE.y =Center.y +H;
	cv::line(this->m_Canvas,PointS,PointE,Color,Width,CV_A_Type,Mark);
	PointS.x =Center.x -H;
	PointS.y =Center.y;
	PointE.x =Center.x +H;
	PointE.y =Center.y;
	cv::line(this->m_Canvas,PointS,PointE,Color,Width,CV_A_Type,Mark);
	return 1;
}
void CPlotMark003(){}
void CPlot::draw3dAxis(int argc, char *argv[])
{
	Mat rMat(1,3,CV_32F);
	rMat.at<float>(0,0) = 1;
	if (1<0)
		rMat.at<float>(0,1) = 1;
	else
		rMat.at<float>(0,1) = 1;
	if(1<0)
		rMat.at<float>(0,2) = 1;
	else
		rMat.at<float>(0,2) = 1;
	this->updatePos(rMat);
	openglViewer->updateGL();
	return;
}
void CPlot::updatePos(const Mat& rMat)
{
	Mat rrMat;
	rMat.convertTo(rrMat , CV_32F);
	//rMat.convertTo(rMat , CV_32F);
	rotationX = rrMat.at<float>(0,0); 
	rotationY = rrMat.at<float>(0,1);
	rotationZ = rrMat.at<float>(0,2);
}
void CPlot::draw3dAxis()
{
	GLfloat x = GLfloat(640) / 480;
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(-2.0 , 6.0, -4.0, 0.0 , 0.0 , 0.0 , 0.0, 1.0 , 0.0);
	float len = 0.2;
	//The world axis /coordinate system！
	//1. Line！
	glColor3f(0.0f,0.0f,1.0f);  
	glBegin(GL_LINES);  
	glVertex3f(-2.0f,00.0f,0.0f);//X line！
	glVertex3f(2.0f,0.0f,0.0f);  
	glVertex3f(0.0f,-2.0f,0.0f); //Y line！ 
	glVertex3f(0.0f,2.0f,0.0f);  
	glVertex3f(0.0f,0.0f,-2.0f); //Z line！ 
	glVertex3f(0.0f,0.0f,2.0f);  
	glEnd();  
	//The world axis /coordinate system！
	//2. arrows!
	glColor3f(1.0f,0.0f,0.0f); // x arrows 
	glPushMatrix();  
	glTranslatef(2.0f,0.0f,0.0f);  
	glRotatef(90.0f,0.0f,1.0f,0.0f);  
	glutSolidCone(0.1,0.3,10,10);  
	glTranslatef(0.0f,0.0f,0.4f); 
	glBegin(GL_LINES);  
	glVertex3f(-len,len,0.0f);  
	glVertex3f(len,-len,0.0f);   
	glEnd(); 
	glBegin(GL_LINES);  
	glVertex3f(len,len,0.0f);  
	glVertex3f(-len,-len,0.0f);   
	glEnd(); 
	glPopMatrix();  
	glColor3f(0.0f,1.0f,0.0f); // y arrows 
	glPushMatrix();  
	glTranslatef(0.0f,2.0f,0.0f);  
	glRotatef(-90.0f,1.0f,0.0f,0.0f);  
	glutSolidCone(0.1,0.3,10,10);  
	glTranslatef(0.0f,0.0f,0.4f); 
	glBegin(GL_LINES);  
	glVertex3f(-len,len,0.0f);  
	glVertex3f(0,0,0.0f);   
	glEnd(); 
	glBegin(GL_LINES);  
	glVertex3f(len,len,0.0f);  
	glVertex3f(0,0,0.0f);   
	glEnd(); 
	glBegin(GL_LINES);  
	glVertex3f(0,-len,0.0f);  
	glVertex3f(0,0,0.0f);   
	glEnd(); 
	glPopMatrix();  
	glColor3f(0.0f,0.0f,1.0f); // z  arrows
	glPushMatrix();  
	glTranslatef(0.0f,0.0f,2.0f);  
	glRotatef(90.0f,0.0f,0.0f,1.0f);  
	glutSolidCone(0.1,0.3,10,10);  
	glTranslatef(0.0f,0.0f,0.4); 
	glTranslatef(0.0f,0.0f,0.4f); 
	glBegin(GL_LINES);  
	glVertex3f(-len,len,0.0f);  
	glVertex3f(len,len,0.0f);   
	glEnd(); 
	glBegin(GL_LINES);  
	glVertex3f(len,len,0.0f);  
	glVertex3f(-len,-len,0.0f);   
	glEnd(); 
	glBegin(GL_LINES);  
	glVertex3f(-len,-len,0.0f);  
	glVertex3f(len,-len,0.0f);   
	glEnd(); 
	glPopMatrix(); 
	glTranslatef(transX,transY,-transZ);
	glRotatef(rotationX , 1.0,0.0,0.0);
	glRotatef(rotationY , 0.0,1.0,0.0);
	glRotatef(rotationZ , 0.0,0.0,1.0);
	glScalef(xscale, yscale, zscale);
	//The Cube aixs / coordinate system！
	//1. The axis line! 
	glColor3f(1.0f,1.0f,1.0f);  
	glBegin(GL_LINES);  
	glVertex3f(-1.2f,00.0f,0.0f);  
	glVertex3f(1.2f,0.0f,0.0f);  
	glVertex3f(0.0f,-1.2f,0.0f);  
	glVertex3f(0.0f,1.2f,0.0f);  
	glVertex3f(0.0f,0.0f,-1.2f);  
	glVertex3f(0.0f,0.0f,1.2f);  
	glEnd();  
	//The Cube aixs / coordinate system！
	//2. The axis arrow!
	glColor3f(1.0f,0.0f,0.0f); //x arrow
	glPushMatrix();  
	glTranslatef(1.2f,0.0f,0.0f);  
	glRotatef(90.0f,0.0f,1.0f,0.0f);  
	glutSolidCone(0.05,0.15,10,10);  
	glPopMatrix();  
	glColor3f(0.0f,1.0f,0.0f); // y  
	glPushMatrix();  
	glTranslatef(0.0f,1.2f,0.0f);  
	glRotatef(-90.0f,1.0f,0.0f,0.0f);  
	glutSolidCone(0.05,0.15,10,10);  
	glPopMatrix();  
	glColor3f(0.0f,0.0f,1.0f); // z  
	glPushMatrix();  
	glTranslatef(0.0f,0.0f,1.2f);  
	glRotatef(90.0f,0.0f,0.0f,1.0f);  
	glutSolidCone(0.05,0.15,10,10);  
	glPopMatrix(); 
	////The Cube Model
	//for(int i=0;i<mesh->getFCount();i++){
	//	glLoadName(i);
	//	glBegin(GL_TRIANGLES);
	//	double r,g,b;
	//	FaceColorList[1].getRgbF(&r,&g,&b);
	//	glColor3d(r,g,b);
	//	for(int j=0;j<3;j++){
	//		MyPoint_ p = mesh->getPoint(mesh->getFace(i).getRef(j));
	//		glNormal3d(p.GetNormal()[0],p.GetNormal()[1],p.GetNormal()[2]);
	//		glVertex3f(p.GetPoint()[0], p.GetPoint()[1], p.GetPoint()[2]);
	//	}
	//	glEnd();
	//}
	//glFlush();
}
//测试使用OpenGL画圆！
void CPlot::drawCircle(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (400, 300);
	glutInitWindowPosition (100, 100);
	glutCreateWindow( "Point examples" );
	glutDisplayFunc( this->RenderScene );
	glutReshapeFunc( this->ChangeSize );
	SetupRC();
	glutMainLoop();
	return ;
}
void CPlot::RenderScene()
{
	//清空颜色缓冲区，填充的颜色由 glClearColor( 0, 0.0, 0.0, 1 ); 指定为黑色
	glClear( GL_COLOR_BUFFER_BIT );
	//绘制一个点
	{
		glColor3f( 1.0f, 0.0f, 0.0f );//指定点的颜色,红色
		glPointSize( 9 );//指定点的大小，9个像素单位
		glBegin( GL_POINTS );//开始画点
		{
			glVertex3f(0.0f, 0.0f, 0.0f); // 在坐标为(0,0,0)的地方绘制了一个点
		}
		glEnd();//结束画点
	}
	//绘制一个点圆
	{
		glColor3f( 0.0f, 1.0f, 0.0f );//指定点的颜色,绿色
		glPointSize( 3 );//指定点的大小，3个像素单位
		glBegin( GL_POINTS );
		{
#define PI 3.14159f
#define RADIUS 50.f
			GLfloat x = 0, y = 0, angle = 0.0;
			for ( angle = 0; angle <= 2.0f * PI; angle += 0.1f )
			{
				x = RADIUS * sin( angle );
				y = RADIUS * cos( angle );
				glVertex3f( x, y, 0 );
			}
		}
		glEnd();
	}
	//绘制x、y坐标轴
	{
		glColor3f( 0.0f, 0.0f, 1.0f );//指定线的颜色,蓝色
		glBegin( GL_LINES );
		{
			// x-axis
			glVertex3f( -100.0f, 0.0f, 0.0f);
			glVertex3f( 100.0f, 0.0f, 0.0f);
			// x-axis arrow
			glVertex3f( 100.0f, 0.0f, 0.0f);
			glVertex3f( 93.0f, 3.0f, 0.0f);
			glVertex3f( 100.0f, 0.0f, 0.0f);
			glVertex3f( 93.0f,-3.0f, 0.0f);
			// y-axis
			glVertex3f( 0.0f, -100.0f, 0.0f);
			glVertex3f( 0.0f, 100.0f, 0.0f);
			glVertex3f( 0.0f, 100.0f, 0.0f);
			glVertex3f( 3.0f, 93.0f, 0.0f);
			glVertex3f( 0.0f, 100.0f, 0.0f);
			glVertex3f( -3.0f, 93.0f, 0.0f);
		}
		glEnd();
	}
	glutSwapBuffers();
}
void CPlot::SetupRC()
{
	glClearColor( 0, 0.0, 0.0, 1 );
	glColor3f( 1.0f, 0.0f, 0.0f );
}
void CPlot::ChangeSize( GLsizei w, GLsizei h )
{
	GLfloat nRange = 100.0f;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	else
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void CPlotMark004(){}
void CPlot::initializeGL()
{
	//loadTexture();
	loadMeshFile("cub.off");//loadMeshFile("sword.off");//
	//qglClearColor(QColor(204,204,204)/*Qt::white*/);//暂时注销，wishchin！！！
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLineWidth(1.5);
	//glColor3d(100,100,100);
	setMaterial();
	setLight();
}
void CPlot::setLight() {
	GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_specular[]= {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL  , GL_RGB);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL , GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	resetGLLightPosition();
}
void CPlot::setMaterial() {
	glEnable(GL_COLOR_MATERIAL);
	GLMaterial(
		OpenGL::Material::GetMaterial(OpenGL::Material::Default));
}
void CPlot::GLMaterial(const OpenGL::Material& material)
{	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
	glMaterialfv(GL_BACK, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_BACK, GL_SPECULAR, material.specular);
	glMaterialfv(GL_BACK, GL_AMBIENT, material.ambient);
	glMaterialf(GL_BACK, GL_SHININESS, material.shininess);
}
void CPlot::setAntiAliasing() {
	glEnable ( GL_POLYGON_SMOOTH );
	glEnable( GL_LINE_SMOOTH );
	glEnable ( GL_POINT_SMOOTH );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_COLOR_MATERIAL);
}
void CPlot::setTexture(IplImage* img){
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
	glGenTextures(1, &texName);  
	glBindTexture(GL_TEXTURE_2D, texName);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height,   
		0, GL_RGB, GL_UNSIGNED_BYTE, img->imageData);
	glEnable(GL_TEXTURE_2D);  
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);  
	glBindTexture(GL_TEXTURE_2D, texName);  
}
void CPlot::resetGLLightPosition() {
	boundingRadius = getBoundingRadius();
	GLfloat light_position[] = {0.0f, 0.0, 
		(float)(LightDistanceRatio*boundingRadius) , 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
float CPlot::getBoundingRadius() {
	//if (meshes.size() == 0) return 0;
	Core::Geometry::Vector3D pmin, pmax;
	//auto it = meshes.begin();
	//do {
	//	MyMesh* last = *it;
	BoundingBox box = mesh->calBoundBox();
	// 		if (it == meshes.begin()) {
	// 			pmin = box.min;
	// 			pmax = box.max;
	// 		} else {
	// 			pmin = min(pmin, box.min);
	// 			pmax = max(pmax, box.max);
	// 		}
	// 	} while ((++it) != meshes.end());
	pmax = box.max ; pmin = box.min;
	return sqrt(
		pow(pmax[0] - pmin[0],2)+
		pow(pmax[1] - pmin[1],2)+
		pow(pmax[2] - pmin[2],2)
		);
}
void CPlot::loadMeshFile(char* filename)
{
	xscale = 1.0; yscale = 1.0; zscale = 1.0;
	transX = 0.0; transY = 0.0;
	faceColors.clear();
	mesh->LoadFromFile(filename);
	mesh->calFaceNormal();
	mesh->calVertexNormal();
	int fCount = mesh->getFCount();
	char buf[255];
	sprintf(buf, "%s", filename);
	int len = strlen(buf);
	buf[len-1] = 'f'; buf[len-2] = 'd'; buf[len-3] = 's';
	FILE *fp = NULL;
}
void CPlotMark005(){}
//初始化坐标系，画出世界坐标系、标志BOX、Box坐标系！
void CPlot::createActions(){
	loadFileAction    = new QAction(tr("&Load"), this);
	closeFileAction   = new QAction(tr("&Close"), this);
	segmentObjAction  = new QAction(tr("&Segment"), this);
	captureAction     = new QAction(tr("Capture") , this);
	connect(loadFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(closeFileAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(segmentObjAction, SIGNAL(triggered()), this, SLOT(segmentObj()));
	connect(captureAction, SIGNAL(triggered()), this, SLOT(capture()));
}
void CPlot::createMenus(){
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(loadFileAction);
	fileMenu->addAction(closeFileAction);
	fileMenu->addAction(captureAction);
	toolMenu = menuBar()->addMenu(tr("&Tool"));
	toolMenu->addAction(segmentObjAction);
	toolMenu->addAction(captureAction);
}
void CPlot::createToolBars(){
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addAction(loadFileAction);
	fileToolBar->addAction(closeFileAction);
	toolsBar = addToolBar(tr("Tool"));
	toolsBar->addAction(segmentObjAction);
}
void CPlot::openFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load a Shape"), ".", tr("Object Model (*.obj *.off)"));
	this->openglViewer->loadMeshFile(filename.toLatin1().data());
	this->openglViewer->updateGL();
}
void CPlot::segmentObj(){
}
//进行数据读入
void CPlot::capture(){
	//////////////////////////////////////////////////////////////////////////
	// Do what you want
	// example
	// you can call openglViewer->updatePos(const Mat& rMat) function to show your Sensorfunsion
	// result, note that once you call this function, you can just call openglViewer->updateGL()
	// the re-paint the GL
	// like:
	char buffer[255];
	//ifstream pfile("/home/hll260/aiglass/proj/firefly-3288/1.txt");
	fstream pfile;
	pfile.open("D://SensorFusionVector//1030.txt");
	fstream outfile,outfile2;
	outfile.open("D://SensorFusionVector//2.txt");
	outfile2.open("D://SensorFusionVector//1103.txt");
	if(!pfile)
	{  
		printf("Can not open file!!"); 
		_exit(1);
	}
	long long sp = pfile.tellg();
	while(sp <= 207)
	{
		pfile.seekg(0,ios::end);
		sp = pfile.tellg();
		cout << sp << endl;
	}
	float ax,ay,az,gx,gy,gz;
	char testgy[10];
	char *gyy="Gyro";
	char testac[10];
	char *acc="Acce";
	SensorFusion sf;
	int bbbb=1;
	vector <float> gyro(3,0);
	vector <float> accel(3,0);
	long long lastsp=0;
	pfile.seekg(lastsp);
	for(int i=1;i<=10;i++)
	{
		pfile.getline(buffer,100);
	}
	lastsp = pfile.tellg();
	char buffer1[50];
	char buffer2[50];
	long linep=0;
	while (1)//!pfile.eof()
	{
		pfile.seekg(0,ios::end);
		pfile.clear();
		sp = pfile.tellg();
		if (linep==0)
			pfile.seekg(lastsp);
		else
			pfile.seekg(linep);
		//cout <<sp<<"  "<<lastsp<<endl;
		if (sp>lastsp)
		{
			lastsp=sp;
			while(linep+144<sp)
			{
				pfile.getline(buffer1,40);
				sscanf (buffer1,"%4s,%f,%f,%f",testac,&ax,&ay,&az);
				pfile.getline(buffer2,40);
				sscanf (buffer2,"%4s,%f,%f,%f",testgy,&gx,&gy,&gz);
				linep = pfile.tellg();
				if(strcmp(testac, acc) == 0&&strcmp(testac, testgy) == 0)
				{
					pfile.getline(buffer2,40);
					sscanf (buffer2,"%4s,%f,%f,%f",testgy,&gx,&gy,&gz);
					linep = pfile.tellg();
					if(strcmp(testac, acc) == 0)
					{
						accel.push_back(ax);
						accel.push_back(ay);
						accel.push_back(az);
						for (int i=0;i<3;i++)
						{
							accel.erase(accel.begin());
						}
					}
					if (strcmp(testgy, gyy) == 0)
					{  
						gyro.push_back(gx);
						gyro.push_back(gy);
						gyro.push_back(gz);
						for (int i=0;i<3;i++)
						{
							gyro.erase(gyro.begin());
						}
					}
				}
				else
				{
					if(strcmp(testac, acc) == 0)
					{
						accel.push_back(ax);
						accel.push_back(ay);
						accel.push_back(az);
						for (int i=0;i<3;i++)
						{
							accel.erase(accel.begin());
						}
					}
					if (strcmp(testgy, gyy) == 0)
					{  
						gyro.push_back(gx);
						gyro.push_back(gy);
						gyro.push_back(gz);
						for (int i=0;i<3;i++)
						{
							gyro.erase(gyro.begin());
						}
					}
				}
				sf.SensorPretreatment(gyro);
				sf.handlemessage(accel,gyro,0.001);
				Mat rMat(1,3,CV_32F);
				//弧度 转角度
				rMat.at<float>(0,0) = -sf.jiaodu[1]*57.3;
				if (sf.jiaodu[0]<0)
					rMat.at<float>(0,1) = 360-sf.jiaodu[0]*57.3;
				else
					rMat.at<float>(0,1) = sf.jiaodu[0]*57.3;
				if(sf.jiaodu[2]<0)
					rMat.at<float>(0,2) = sf.jiaodu[2]*57.3;
				else
					rMat.at<float>(0,2) = sf.jiaodu[2]*57.3;
				//传入三维坐标参数为绝对位置（相对于原点）
				rMat.at<float>(0,3) = sf.LocX;
				rMat.at<float>(0,4) = sf.LocY;
				rMat.at<float>(0,5) = sf.LocZ;
				openglViewer->updatePos(rMat);
				openglViewer->updateGL();
				outfile<<sf.jiaodu[0]*57.3<<" "<<sf.jiaodu[1]*57.3<<" "<<sf.jiaodu[2]*57.3<<endl;
				cout<<sf.jiaodu[0]*57.3<<" "<<sf.jiaodu[1]*57.3<<" "<<sf.jiaodu[2]*57.3<<" "<<gyro.size()<<endl;
				outfile2<<double(accel[0])<<" "<<double(accel[1])<<" "<<double(accel[2])<<endl;
				outfile2<<double(gyro[0])<<" "<<double(gyro[1])<<" "<<double(gyro[2])<<endl;
				outfile2<<"sp:"<<sp<<"    linep:"<<linep<<endl;
			}
		}
		else
		{   
			continue;
		}
	}
	outfile.close();
	outfile2.close();
	pfile.close();
}
```
还有一些其他的程序段，等整理好之后再进行上传....................

