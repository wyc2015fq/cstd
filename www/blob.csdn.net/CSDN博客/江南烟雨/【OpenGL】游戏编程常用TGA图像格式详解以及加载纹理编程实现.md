# 【OpenGL】游戏编程常用TGA图像格式详解以及加载纹理编程实现 - 江南烟雨 - CSDN博客
2012年06月08日 20:23:19[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：9857
TGA格式图像是游戏中十分常见的一种图像格式，所以有必要了解其内部格式以及编程实现。
TGA图像一般有非压缩和压缩两种格式，下面分别进行介绍。
**一、非压缩TGA图像**
注：前面的标记绿色的部分（共12字节）表示对于所有的非压缩TGA格式图像值都是相同的！所以通常用来在读取数据时鉴别是否为TGA图像。
|名称|偏移|长度|说明| | |
|----|----|----|----|----|----|
|图像信息字段长度|0|1|本字段是 1 字节无符号整型，指出图像信息字段（见本子表的后面）长度，其取值范围是 0 到 255 ，当它**为**** 0 ****时表示没有图像的信息字段**。| | |
|颜色表类型|1|1|0 表示没有颜色表，1 表示颜色表存在。由于本格式是无颜色表的，因此**此项通常被忽略。**| | |
|图像类型码|2|1|该字段**总为**** 2**，这也是此类型为格式 2 的原因。| | |
|颜色表规格字段|颜色表首址|3|2|颜色表首的入口索引，整型（低位-高位）|**如果颜色表字段为0，则忽略该字段**|
|颜色表的长度|5|2|颜色表的表项总数，整型（低位-高位）| | |
|颜色表项位数|7|1|位数（bit），16 代表 16 位 TGA ，24 代表 24 位 TGA ，32 代表 32 位 TGA| | |
|图像规格字段|图像X 坐标起始位置|8|2|图像左下角X坐标的整型（低位-高位）值| |
|图像Y 坐标起始位置|10|2|图像左下角 Y坐标的整型（低位-高位）值| | |
|图像宽度|12|2|以像素为单位，图像宽度的整型（低位-高位）| | |
|图像高度|14|2|以像素为单位，图像宽度的整型（低位-高位）| | |
|图像每像素存储占用位数|16|**1**|它的值为**16，24 或 32 **等等。决定了该图像是 TGA 16，TGA24,TGA 32 等等。| | |
|图像描述符字节|17|1|bits 3-0 - 每像素对应的属性位的位数；对于TGA 16，该值为 0 或 1，对于 TGA                     24，该值为 0，对于 TGA 32，该值为 8。bit 4    - 保留，必须为 0bit 5    - 屏幕起始位置标志0 = 原点在左下角1 = 原点在左上角对于 truevision 图像必须为 0bits 7-6 - 交叉数据存储标志00 = 无交叉01 = 两路奇/偶交叉10 = 四路交叉11 = 保留**（一般这个字节设为0x00即可）**| | |
|图像信息字段|18|可变|包含一个自由格式的，长度是图像由“图像信息字段”指定。它常常被忽略（即偏移 0 处值为 0 ），注意其最大可以含有 255 个字符。如果需要存储更多信息，可以放在图像数据之后。| | |
|颜色表数据|可变|可变|**如果颜色表类型为 0****，则该域不存在**，否则越过该域直接读取图像颜色表规格中描述了每项的字节数，为 2，3，4 之一。| | |
|图像数据|可变|可变|RGB颜色数据，存放顺序为：BGR(A)| | |
下面的程序实现了绘制一个立方体，并进行纹理贴图。
**需要注意的是：TGA图像中数据存放的顺序是BGR(A)，而在OpenGL中顺序是RGB(A)，所以在进行纹理生成的时候必须先进行格式的转化。**
在OpenGL中只能加载24位或者32位的TGA图像生成纹理。
TGATexture.h定义了一些结构体以及函数声明：
```cpp
#ifndef TGATEXTURE_H
#define TGATEXTURE_H
#include <GL/glut.h>
#include <iostream>
using namespace std;
//纹理结构体定义
typedef struct
{
	GLubyte *imageData;//图像数据
	GLuint bpp;//像素深度
	GLuint width;//图像宽度
	GLuint height;//图像高度
	GLuint texID;//对应的纹理ID
}TextureImage;
//加载TGA图像，生成纹理
bool LoadTGA(TextureImage *texture,char *fileName);
#endif
```
TGATexture.cpp则包含加载TGA图像生成纹理的函数具体实现：
```cpp
#include "TGATexture.h"
//加载TGA图像（无压缩格式），生成纹理
bool LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)
	FILE *file = fopen(filename, "rb");						// Open The TGA File
	if(	file==NULL ||										// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}
	texture->width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
	//OpenGL中纹理只能使用24位或者32位的TGA图像
	if(	texture->width	<=0	||								// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}
	texture->bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	texture->imageData=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data
	if(	texture->imageData==NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data
		fclose(file);										// Close The File
		return false;										// Return False
	}
	//RGB数据格式转换，便于在OpenGL中使用
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	fclose (file);											// Close The File
	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	if (texture[0].bpp==24)									// Was The TGA 24 Bits
	{
		type=GL_RGB;										// If So Set The 'type' To GL_RGB
	}
	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);
	return true;											// Texture Building Went Ok, Return True
}
```
main.cpp主程序：
```cpp
#include "TGATexture.h"
TextureImage texture[1];
GLfloat xRot,yRot,zRot;//control cube's rotation
int init()
{
	if(!LoadTGA(&texture[0],"GSK1.tga"))
		return GL_FALSE;
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	return GL_TRUE;
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-5.0f);
	glRotatef(xRot,1.0f,0.0f,0.0f);
	glRotatef(yRot,0.0f,1.0f,0.0f);
	glRotatef(zRot,0.0f,0.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,texture[0].texID);
	glBegin(GL_QUADS); 
	// Front Face 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
	glEnd(); 
	glBindTexture(GL_TEXTURE_2D,texture[0].texID);
	glBegin(GL_QUADS); 
	// Back Face 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 
	glEnd(); 
	glBindTexture(GL_TEXTURE_2D,texture[0].texID);
	glBegin(GL_QUADS); 
	// Top Face 
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
	glEnd(); 
	glBindTexture(GL_TEXTURE_2D,texture[0].texID);
	glBegin(GL_QUADS); 
	// Bottom Face 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 
	glEnd(); 
	glBindTexture(GL_TEXTURE_2D,texture[0].texID);
	glBegin(GL_QUADS); 
	// Right face 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); 
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); 
	glEnd(); 
	glBindTexture(GL_TEXTURE_2D,texture[0].texID);
	glBegin(GL_QUADS); 
	// Left Face 
	// Bottom Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); 
	// Bottom Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); 
	// Top Right Of The Texture and Quad 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); 
	// Top Left Of The Texture and Quad 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); 
	glEnd(); 
	glutSwapBuffers();
}
void reshape(int w,int h)
{
	if (0 == h)
		h = 1;
	
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)w / (GLfloat)h,1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void keyboard(unsigned char key,int x,int y)
{
	switch(key){
		case 'x':
			xRot += 1.0f;
			glutPostRedisplay();
			break;
		case 'y':
			yRot += 1.0f;
			glutPostRedisplay();
			break;
		case 'z':
			zRot += 1.0f;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Texture Map");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201206/08/1339146578_7539.jpg)
**二、压缩TGA图像**
其格式如下表：
|名称|偏移|长度|说明| | |
|----|----|----|----|----|----|
|图像信息字段长度|0|1|本字段是 1 字节无符号整型，指出图像信息字段（见本子表的后面）长度，其取值范围是 0 到 255 ，当它为 0 时表示没有图像的信息字段。| | |
|颜色表类型|1|1|0 表示没有颜色表，1 表示颜色表存在。由于本格式是无颜色表的，因此此项通常被忽略。| | |
|图像类型码|2|1|该字段总为 10 ，这也是此类型为格式10 的原因。| | |
|颜色表规格字段|颜色表首址|3|2|颜色表首的入口索引，整型（低位-高位）|如果颜色表字段为0，则忽略该字段|
|颜色表的长度|5|2|颜色表的表项总数，整型（低位-高位）| | |
|颜色表项位数|7|1|位数（bit），16 代表 16 位 TGA ，24 代表 24 位 TGA ，32 代表 32 位 TGA| | |
|图像规格字段|图像 X 坐标起始位置|8|2|图像左下角 X坐标的整型（低位-高位）值| |
|图像 Y 坐标起始位置|10|2|图像左下角 Y坐标的整型（低位-高位）值| | |
|图像宽度|12|2|以像素为单位，图像宽度的整型（低位-高位）| | |
|图像高度|14|2|以像素为单位，图像宽度的整型（低位-高位）| | |
|图像每像素存储占用位数|16|1|它的值为16，24 或 32 等等。决定了该图像是 TGA 16，TGA24,TGA 32 等等。| | |
|图像描述符字节|17|1|bits 3-0 - 每像素对应的属性位的位数；对于TGA 16，该值为 0 或 1，对于 TGA                     24，该值为 0，对于 TGA 32，该值为 8。bit 4    - 保留，必须为 0bit 5    - 屏幕起始位置标志0 = 原点在左下角1 = 原点在左上角对于 truevision 图像必须为 0bits 7-6 - 交叉数据存储标志00 = 无交叉01 = 两路奇/偶交叉10 = 四路交叉11 = 保留| | |
|图像信息字段|18|可变|包含一个自由格式的，长度是图像由“图像信息字段”指定。它常常被忽略（即偏移 0 处值为 0 ），注意其最大可以含有 255 个字符。如果需要存储更多信息，可以放在图像数据之后。| | |
|颜色表数据|可变|可变|如果颜色表类型为 0，则该域不存在，否则越过该域直接读取图像颜色表规格中描述了每项的字节数，为 2，3，4 之一。| | |
|图像数据|可变|可变|**采用RLE压缩后的RGB颜色数据。**| | |
Tga的压缩算法采用了RLE算法，RLE算法的基本思想是将数据分为两大类：
A：连续的不重复字节
B：连续的重复字节
RLE算法应用于RGB格式的图片压缩中，则把数据分为：
A：连续的不重复像素颜色值
B：连续的重复像素颜色值
然后将数据按这两类数据分成若干长度不相等数据块，**每个数据块的开始都是一个****1个字节长度的header**（RLE在纯数据压缩中header位2个字节16位），后面紧跟着data数据块，如下。
|Header（1个字节）|Data（变长）|
每个header的第一位作为标记：**0表示A类颜色数据，1表示B****类颜色数据**。剩下的7位意义如下：
对于A类数据：表示data有多少个像素的RGB颜色值。取值0-127，0表示1个像素，所以最多为128个像素，data块则为这些不重复的像素RGB颜色值。
对于B类数据：表示有多少个像素具有相同的RGB颜色值。取值0-127，0表示1个像素，所以最多为128个像素，data仅包含一个像素的颜色值，即为重复的那个颜色值。
读取其像素数据部分需要注意一下，需要分情况讨论看是A类数据还是B类数据：
```cpp
if(m_pImageData) delete []m_pImageData;
m_pImageData = new unsigned char[m_iImageDataSize];
 
int iBytePerPixel = m_iBitsPerPixel/8;
int iPixelCnt = m_iImageWidth * m_iImageHeight;
int iCurPixel = 0;
unsigned char ucColorBuffer[4] = {'/0'};
 
do{
            BYTE chunkheader = 0;
            is.read((char*)&chunkheader, sizeof(BYTE));
 
            //run length data
            if(chunkheader & 0x80)//B类数据
            {
                        int iCnt = (chunkheader & 0x7f) + 1;//计算连续的颜色值重复的像素数。注意：最高位的1是标记位，所以要特殊处理
                        is.read((char*)ucColorBuffer, iBytePerPixel);
                       
                        for(int i = 0; i < iCnt; i++)
                        {
                                    m_pImageData[iCurPixel*iBytePerPixel + 0] = ucColorBuffer[0];
                                    m_pImageData[iCurPixel*iBytePerPixel + 1] = ucColorBuffer[1];
                                    m_pImageData[iCurPixel*iBytePerPixel + 2] = ucColorBuffer[2];
                                    if(m_iBitsPerPixel == 32)
                                    {
                                                m_pImageData[iCurPixel*iBytePerPixel + 3] = ucColorBuffer[3];
                                    }
 
                                    iCurPixel++;
 
                                    if(iCurPixel > iPixelCnt)
                                                return false;
                        }
            }
            //no processed data
            else//A类数据
            {
                        int iCnt = chunkheader + 1;//颜色值不重复的像素数量，注意要加一
                        for(int i = 0; i < iCnt; i++)
                        {                                             
                                    is.read((char*)ucColorBuffer, iBytePerPixel);
 
                                    m_pImageData[iCurPixel*iBytePerPixel + 0] = ucColorBuffer[0];
                                    m_pImageData[iCurPixel*iBytePerPixel + 1] = ucColorBuffer[1];
                                    m_pImageData[iCurPixel*iBytePerPixel + 2] = ucColorBuffer[2];
                                    if(m_iBitsPerPixel == 32)
                                    {
                                                m_pImageData[iCurPixel*iBytePerPixel + 3] = ucColorBuffer[3];
                                    }
 
                                    iCurPixel++;
                                               
                                    if(iCurPixel > iPixelCnt)
                                                return false;
                        }
            }
 
}while(iCurPixel < iPixelCnt);
```
