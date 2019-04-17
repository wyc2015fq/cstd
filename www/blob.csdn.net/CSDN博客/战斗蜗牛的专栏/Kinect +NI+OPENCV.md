# Kinect +NI+OPENCV - 战斗蜗牛的专栏 - CSDN博客





2012年08月07日 13:06:26[vbskj](https://me.csdn.net/vbskj)阅读数：4136








# 基于Kinect-OpenNI-OpenCV-OpenGL的环境三维重构


### Wikipedia，自由的百科全书



项目源码详见：[http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=13042](http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=13042)


![Image:mykinect.jpg](http://www.opencv.org.cn/images/3/39/Mykinect.jpg)


前几天刚入手了期待已久的 Kinect ，用于实验室机器人项目的视觉导航与环境理解。 

首先要做的是破解-->连接PC-->获取深度数据和图像数据-->三维点云显示 这么几项基本工作。 

开始仿照的是 饮水思源[[1]](http://www.cnblogs.com/yangyangcv/) 博客的方法（使用VS2008在windows平台上试用Kinect[[2]](http://www.cnblogs.com/yangyangcv/archive/2010/11/27/1889867.html)
 ），利用 CL-NUI-Platform 来破解，它的最新版是1.0.0.1210，但我在XP上用会当机，后来换 1.0.0.1121 版的就可以用了。CL NUI 提供了十分简便易用的接口，在OpenCV 上调用很简单，另外它还提供了 Kinect 底座马达的控制接口和 LED 灯颜色的选择接口，其例程中可以操控 Kinect 上下摆动。如果只需要获取深度数据和图像数据，CL NUI 就够用了。不过要做深入的应用，比如人体姿态识别、骨架提取、深度数据与图像数据的合并等等，就该用到 OpenNI 了。


国内的 CNKINECT[[3]](http://www.cnkinect.com/) 是个不错的 Kinect 开发论坛，版块丰富，有很多资料可供借鉴。我通过论坛介绍的 方法[[4]](http://www.cnkinect.com/forum-viewthread-tid-21-extra-%26page%3D1-page-1.html)
 成功配置了 OpenNI + Kinect，先是用最新版的 OpenNI+SensorKinect+NITE ，但在 XP 下不能正常运行，可能跟 .net 平台有关，老实按上面论坛的方法装就成功了。另外用 CMake + VS2008 装了最新的 OpenCV_SVN，开始试过在 CMake 里选择 With TBB，但诡异的是 TBB 似乎只适用于VS2005，在 VS2008 编译后试用里面的 samples 老是提示报错找不到msvcp80.dll，重新用 CMake 配置取消了 With TBB，就一切正常了。





[[编辑](http://www.opencv.org.cn/index.php?title=%E5%9F%BA%E4%BA%8EKinect-OpenNI-OpenCV-OpenGL%E7%9A%84%E7%8E%AF%E5%A2%83%E4%B8%89%E7%BB%B4%E9%87%8D%E6%9E%84&action=edit&section=1)]

## 一、深度摄像机的视角调整 与 深度/彩色图像的合并

通过研究 OpenCV_SVN 与 OpenNI 相关的代码（cap_openni.cpp）发现，OpenCV 目前只支持对Kinect的深度图、视差图和彩色/灰度图及相关属性的读取，更进一步的设置还没有实现。参考台湾 Heresy’space[[5]](http://kheresy.wordpress.com/) 的博客文章《透过
 OpneNI 合并 Kinect 深度以及彩色影像资料》[[6]](http://kheresy.wordpress.com/2011/01/21/combine_depth_and_image_from_kinect/)，想把深度图和彩色图合并显示，但是由于
 Kinect 的深度摄像机和彩色摄像机是在不同的位置，而且镜头本身的参数也不完全相同，所以两个摄像机所取得的画面会有些微的差异（如图1 左下角子图OpenGL三维点云显示窗口所示，天花板的两个日光灯对应深度图和彩色图的区域并没有重合，而是错位了）。


![Image:kinect_opencv_04.jpg](http://www.opencv.org.cn/images/7/7e/Kinect_opencv_04.jpg)

图1 

根据 Heresy 的分析，需要对深度摄像机的视角进行修正，在 OpenNI 下只需要一行代码就可以实现： 
// 6. correct view port 
mDepthGenerator.GetAlternativeViewPointCap().SetViewPoint( mImageGenerator );


不过在 OpenCV 中并没有提供这一项设置，其源代码 modules\highgui\src\cap_openni.cpp 中 setDepthGeneratorProperty 并没有任何实质的属性设置。为此，**需要改写该函数，并且要在相应的头文件 modules\highgui\include\opencv2\highgui\highgui_c.h 中添加新的参数 id**，具体如下：

- 将 cap_openni.cpp 第 344 行的 setDepthGeneratorProperty 改写如下： 
bool CvCapture_OpenNI::setDepthGeneratorProperty(int propIdx,double propValue ){ 
    bool res =false; 
    CV_Assert( depthGenerator.IsValid());switch( propIdx ){case CV_CAP_PROP_OPENNI_VIEW_POINT : 
        depthGenerator.GetAlternativeViewPointCap().SetViewPoint( imageGenerator ); 
        res =true;break;default: 
        CV_Error( CV_StsBadArg,"Depth generator does not support such parameter for setting.\n"); 
        res =false;}return res;}


- 在 highgui_c.h 的第 348 行下添加改变视角的参数 ID 号： 
CV_CAP_PROP_OPENNI_VIEW_POINT        =24,
然后在第 352 行下添加： 
CV_CAP_OPENNI_DEPTH_GENERATOR_VIEW_POINT = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_VIEW_POINT
从而使得 OpenCV 的 VideoCapture 属性关于 OpenNI 的如下所示： 
// OpenNI map generators 
    CV_CAP_OPENNI_DEPTH_GENERATOR =0, 
    CV_CAP_OPENNI_IMAGE_GENERATOR =1<<31, 
    CV_CAP_OPENNI_GENERATORS_MASK =1<<31,// Properties of cameras avalible through OpenNI interfaces 
    CV_CAP_PROP_OPENNI_OUTPUT_MODE      =20, 
    CV_CAP_PROP_OPENNI_FRAME_MAX_DEPTH  =21,// in mm 
    CV_CAP_PROP_OPENNI_BASELINE         =22,// in mm 
    CV_CAP_PROP_OPENNI_FOCAL_LENGTH     =23,// in pixels '''CV_CAP_PROP_OPENNI_VIEW_POINT        = 24,''' 
    CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE = CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_OPENNI_OUTPUT_MODE, 
    CV_CAP_OPENNI_DEPTH_GENERATOR_BASELINE = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_BASELINE, 
    CV_CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_FOCAL_LENGTH,'''CV_CAP_OPENNI_DEPTH_GENERATOR_VIEW_POINT = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_VIEW_POINT'''
改写完上述源代码后保存，并且**用 CMake 和 VS2008 重新编译一次 OpenCV**，我们就可以用 OpenCV 的接口来控制 Kinect 深度摄像头的视角，使其深度数据和图像数据能够很好的重合起来，如图2所示，注意与图1相比，**右上角的伪彩色视差图四周出现了黑边**，这就是视角调整后的效果：


![Image:kinect_opencv_08.jpg](http://www.opencv.org.cn/images/2/2e/Kinect_opencv_08.jpg)

图2 




[[编辑](http://www.opencv.org.cn/index.php?title=%E5%9F%BA%E4%BA%8EKinect-OpenNI-OpenCV-OpenGL%E7%9A%84%E7%8E%AF%E5%A2%83%E4%B8%89%E7%BB%B4%E9%87%8D%E6%9E%84&action=edit&section=2)]

## 二、三维点云坐标的计算

在 OpenCV 中实际上已经提供了三维点云坐标计算的接口，通过 Kinect 的深度数据可以很快换算出三维点云坐标，cap_openni.cpp 中这部分代码具体如下：

IplImage* CvCapture_OpenNI::retrievePointCloudMap(){int cols = depthMetaData.XRes(), rows = depthMetaData.YRes();if( cols <=0|| rows <=0)return0; 
 
    cv::Mat depth; 
    getDepthMapFromMetaData( depthMetaData, depth, noSampleValue, shadowValue );constfloat badPoint =0; 
    cv::Mat pointCloud_XYZ( rows, cols, CV_32FC3, cv::Scalar::all(badPoint));for(int y =0; y < rows; y++){for(int x =0; x < cols; x++){unsignedshort d = depth.at(y, x);// Check for invalid measurements if( d == CvCapture_OpenNI::INVALID_PIXEL_VAL)// not valid continue; 
 
            XnPoint3D proj, real; 
            proj.X= x; 
            proj.Y= y; 
            proj.Z= d; 
            depthGenerator.ConvertProjectiveToRealWorld(1,&proj,&real); 
            pointCloud_XYZ.at(y,x)= cv::Point3f( real.X*0.001f, real.Y*0.001f, real.Z*0.001f);// from mm to meters }} 
 
    outputMaps[CV_CAP_OPENNI_POINT_CLOUD_MAP].mat= pointCloud_XYZ;return outputMaps[CV_CAP_OPENNI_POINT_CLOUD_MAP].getIplImagePtr();}
不过可以看到上面核心的代码就一行： 
depthGenerator.ConvertProjectiveToRealWorld(1,&proj,&real);
具体是怎样实现的呢？我们可以进一步找 OpenNI 的源代码来分析。不过这里我是从原来双目视觉的经验上自己编写代码来实现三维点云坐标的计算。实际上 Kinect 的深度摄像头成像也**类似于普通的双目立体视觉，只要获取了两个摄像头之间的基线（baseline）和焦距（focal length）、以及视差数据，通过构造矩阵 Q，利用 OpenCV 的 reprojectimageTo3D 函数，也可以计算出三维坐标**。


下面我们通过以下代码看看矩阵 Q 的构造过程： 
capture.set(CV_CAP_OPENNI_DEPTH_GENERATOR_VIEW_POINT,1.0);// 调整深度摄像头视角 double b = capture.get( CV_CAP_OPENNI_DEPTH_GENERATOR_BASELINE );// mm double F = capture.get( CV_CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH );// pixels double q[]={1,0,0,-320.0,0,1,0,-240.0,0,0,0, F,0,0,1./b,0}; 
Mat matQ(4,4, CV_64F, q);



而三维点云坐标的计算，以及深度、视差、彩色图像的读取，则如下所示： 
// 抓取数据 if(!capture.grab()){[cout](http://www.opengroup.org/onlinepubs/009695399/functions/cout.html)<<"Can not grab images."<< endl;return-1;}// 读取深度数据 
        capture.retrieve( depthMap, CV_CAP_OPENNI_DEPTH_MAP ); 
        minMaxLoc( depthMap,&mindepth,&maxdepth );// 读取视差数据 
        capture.retrieve( disparityMap, CV_CAP_OPENNI_DISPARITY_MAP_32F );        
        colorizeDisparity( disparityMap, colorDisparityMap, maxDisparity ); 
        colorDisparityMap.copyTo( validColorDisparityMap, disparityMap !=0);        
        imshow("colorized disparity map", validColorDisparityMap );// 读取彩色图像数据 
        capture.retrieve( bgrImage, CV_CAP_OPENNI_BGR_IMAGE ); 
        imshow("Color image", bgrImage );// 利用视差数据计算三维点云坐标 
        cv::reprojectImageTo3D(disparityMap, xyzMap, matQ,true);
这里值得注意的是，在计算视差时，如果视差图是采用默认的 CV_8UC1 格式（参数ID是**CV_CAP_OPENNI_DISPARITY_MAP**），由于视差被限制在[0-255]整数范围内，造成一定的误差，所得到的三维坐标数据会出现分层，如图3和图4所示：


![Image:kinect_opencv_02.jpg](http://www.opencv.org.cn/images/c/c1/Kinect_opencv_02.jpg)

图3 Matlab绘制的三维点云，不同深度上有明显分层 

![Image:kinect_opencv_03.jpg](http://www.opencv.org.cn/images/c/cf/Kinect_opencv_03.jpg)

图4 左为直接得到的深度数据Mesh图，中为由8位视差数据计算得到的深度数据，右为对应的视差数据 

而采用 32 位浮点格式来获取视差数据（参数ID：**CV_CAP_OPENNI_DISPARITY_MAP_32F**），则消除了这种因视差误差造成深度数值分层的现象，如图5、6所示：


![Image:kinect_opencv_05.jpg](http://www.opencv.org.cn/images/8/80/Kinect_opencv_05.jpg)

图5 深度数值的分层现象基本消除 

![Image:kinect_opencv_06.jpg](http://www.opencv.org.cn/images/4/4a/Kinect_opencv_06.jpg)

图6 两种方式得到的深度数据一致 




[[编辑](http://www.opencv.org.cn/index.php?title=%E5%9F%BA%E4%BA%8EKinect-OpenNI-OpenCV-OpenGL%E7%9A%84%E7%8E%AF%E5%A2%83%E4%B8%89%E7%BB%B4%E9%87%8D%E6%9E%84&action=edit&section=3)]

## 三、利用OpenGL显示三维点云数据

这方面主要是基于 学习笔记（15） [[7]](http://blog.csdn.net/chenyusiyuan/archive/2010/06/24/5691481.aspx)的内容，不过当时是通过另设 OpenCV 窗口设置 Trackbar 来调整 OpenGL 的视点位置和摄像机位置。在这里则主要参考了 OpenCV 论坛的帖子《[HQ] OpenCV和OpenGL编程：关于显示点云数据-Stereo Vision源码分享》[[8]](http://www.opencv.org.cn/viewtopic.php?f=1&t=8722&start=0&sid=122b9ac635ac12d74e14a0f0943fe77c)中
 villager5 提供的方法来调整摄像机位置，做了一定的修改，使得鼠标左键拖曳能够实现对上下左右的视角变换，鼠标右键拖曳能够实现视距远近的变换，不再需要用 Trackbar 来调整。下面是相关的代码：




#define SIGN(x) ( (x)<0 ? -1:((x)>0?1:0 ) )//////////////////////////////////////////////////////////////////////////// //---OpenGL 全局变量float xyzdata[480][640][3];float texture[480][640][3];int glWinWidth =640, glWinHeight =480;int width=640, height=480;double eyex, eyey, eyez, atx, aty, atz;// eye* - 摄像机位置，at* - 注视点位置
 
bool leftClickHold =false, rightClickHold =false;int mx,my;// 鼠标按键时在 OpenGL 窗口的坐标int ry =90, rx =90;// 摄像机相对注视点的观察角度double mindepth, maxdepth;// 深度数据的极值 double radius =6000.0;// 摄像机与注视点的距离/************************************************************************//*                                           OpenGL响应函数                                                 *//************************************************************************///////////////////////////////////////////////////////////////////////////// 鼠标按键响应函数void mouse(int button,int state,int x,int y){if(button == GLUT_LEFT_BUTTON){if(state == GLUT_DOWN){
			leftClickHold=true;}else{
			leftClickHold=false;}}if(button== GLUT_RIGHT_BUTTON){if(state == GLUT_DOWN){
			rightClickHold=true;}else{
			rightClickHold=false;}}}//////////////////////////////////////////////////////////////////////////// 鼠标运动响应函数void motion(int x,int y){int rstep =5;if(leftClickHold==true){if( abs(x-mx)> abs(y-my)){
			rx += SIGN(x-mx)*rstep;}else{
			ry -= SIGN(y-my)*rstep;}
 
		mx=x;
		my=y;
		glutPostRedisplay();}if(rightClickHold==true){
		radius += SIGN(y-my)*100.0;
		radius = std::max( radius,100.0);
		mx=x;
		my=y;
		glutPostRedisplay();}}//////////////////////////////////////////////////////////////////////////// 三维图像显示响应函数void renderScene(void){// clear screen and depth buffer
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// Reset the coordinate system before modifying 
	glLoadIdentity();// set the camera position
	atx =0.0f;
	aty =0.0f;
	atz =( mindepth - maxdepth )/2.0f;
	eyex = atx + radius * sin( CV_PI * ry /180.0f)* cos( CV_PI * rx/180.0f); 
	eyey = aty + radius * cos( CV_PI * ry/180.0f); 
	eyez = atz + radius * sin( CV_PI * ry /180.0f)* sin( CV_PI * rx/180.0f);
	gluLookAt (eyex, eyey, eyez, atx, aty, atz,0.0,1.0,0.0);
	glRotatef(0,0,1,0);
	glRotatef(-180,1,0,0);// 对点云数据进行三角化// 参考自：http://www.codeproject.com/KB/openGL/OPENGLTG.aspx// we are going to loop through all of our terrain's data points,// but we only want to draw one triangle strip for each set along the x-axis.for(int i =0; i < height; i++){
		glBegin(GL_TRIANGLE_STRIP);for(int j =0; j < width; j++){// for each vertex, we calculate the vertex color, // we set the texture coordinate, and we draw the vertex./*
			   the vertexes are drawn in this order:
 
			   0  ---> 1
			          /
				    /
			      /
			   2  ---> 3
			*/// draw vertex 0
			glTexCoord2f(0.0f,0.0f);
			glColor3f(texture[i][j][0]/255.0f, texture[i][j][1]/255.0f, texture[i][j][2]/255.0f);
			glVertex3f(xyzdata[i][j][0], xyzdata[i][j][1], xyzdata[i][j][2]);// draw vertex 1
			glTexCoord2f(1.0f,0.0f);
			glColor3f(texture[i+1][j][0]/255.0f, texture[i+1][j][1]/255.0f, texture[i+1][j][2]/255.0f);
			glVertex3f(xyzdata[i+1][j][0], xyzdata[i+1][j][1], xyzdata[i+1][j][2]);// draw vertex 2
			glTexCoord2f(0.0f,1.0f);
			glColor3f(texture[i][j+1][0]/255.0f, texture[i][j+1][1]/255.0f, texture[i][j+1][2]/255.0f);
			glVertex3f(xyzdata[i][j+1][0], xyzdata[i][j+1][1], xyzdata[i][j+1][2]);// draw vertex 3
			glTexCoord2f(1.0f,1.0f);
			glColor3f(texture[i+1][j+1][0]/255.0f, texture[i+1][j+1][1]/255.0f, texture[i+1][j+1][2]/255.0f);
			glVertex3f(xyzdata[i+1][j+1][0], xyzdata[i+1][j+1][1], xyzdata[i+1][j+1][2]);}
		glEnd();}// enable blending
	glEnable(GL_BLEND);// enable read-only depth buffer
	glDepthMask(GL_FALSE);// set the blend function to what we use for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);// set back to normal depth buffer mode (writable)
	glDepthMask(GL_TRUE);// disable blending
	glDisable(GL_BLEND);/* 	float x,y,z;
	// 绘制图像点云
	glPointSize(1.0); 
	glBegin(GL_POINTS);
	for (int i=0;i<height;i++){ 
		for (int j=0;j<width;j++){
			// color interpolation 
			glColor3f(texture[i][j][0]/255, texture[i][j][1]/255, texture[i][j][2]/255);
			x= xyzdata[i][j][0];
			y= xyzdata[i][j][1]; 
			z= xyzdata[i][j][2]; 
			glVertex3f(x,y,z); 
		}
	}
	glEnd(); */
 
	glFlush();
	glutSwapBuffers();}//////////////////////////////////////////////////////////////////////////// 窗口变化图像重构响应函数void reshape (int w,int h){
	glWinWidth = w;
	glWinHeight = h;
	glViewport (0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45,(GLfloat)w /(GLfloat)h,1.0,15000.0);	
	glMatrixMode (GL_MODELVIEW);}//////////////////////////////////////////////////////////////////////////// 载入三维坐标数据void load3dDataToGL(IplImage* xyz){
	CvScalar s;//accessing the image pixelsfor(int i=0;i<height;i++){for(int j=0;j<width;j++){
			s=cvGet2D(xyz,i,j);// s.val[0] = x, s.val[1] = y, s.val[2] = z
			xyzdata[i][j][0]= s.val[0];
			xyzdata[i][j][1]= s.val[1];
			xyzdata[i][j][2]= s.val[2];}}}//////////////////////////////////////////////////////////////////////////// 载入图像纹理数据void loadTextureToGL(IplImage* img){	
	CvScalar ss;//accessing the image pixelsfor(int i=0;i<height;i++){for(int j=0;j<width;j++){
			ss=cvGet2D(img,i,j);// ss.val[0] = red, ss.val[1] = green, ss.val[2] = blue
			texture[i][j][2]= ss.val[0];
			texture[i][j][1]= ss.val[1];
			texture[i][j][0]= ss.val[2];}}}



