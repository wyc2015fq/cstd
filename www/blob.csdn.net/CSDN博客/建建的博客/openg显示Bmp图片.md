# openg显示Bmp图片 - 建建的博客 - CSDN博客
2017年08月16日 11:25:01[纪建](https://me.csdn.net/u013898698)阅读数：149
#include <stdio.h>
#include <stdlib.h>
#include<glut.h>
int width;
int height;
static GLint     PixelLength;
const GLint new_width = 380;
const GLint new_height = 380;
//设置图像指针
GLubyte* pixels;
GLuint load_texture()
{
    //OpenGL纹理用整型数表示
    GLuint texture_ID;
FILE* pFile = fopen("Bliss.bmp", "rb");
if( pFile == 0 );
//exit(0);
// 读取图象的大小信息
fseek(pFile, 0x0012, SEEK_SET);
fread(&width, sizeof(width), 1, pFile);
fread(&height, sizeof(height), 1, pFile);
// 计算像素数据长度
PixelLength = width * 3;
while( PixelLength % 4 != 0 )
++PixelLength;
PixelLength *= width;
    pixels = new GLubyte[PixelLength];
if( pixels == 0 );
fseek(pFile, 54, SEEK_SET);
fread(pixels, PixelLength, 1, pFile);
// 关闭文件
fclose(pFile);
GLint new_line_bytes = new_width * 3;
while( new_line_bytes % 4 != 0 )
++new_line_bytes;
GLintnew_total_bytes = new_line_bytes * new_height;
// 分配内存
GLubyte* new_pixels = (GLubyte*)malloc(new_total_bytes);
gluScaleImage(GL_RGB,
width, height, GL_UNSIGNED_BYTE, pixels,
new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
free(pixels);
pixels=new_pixels;
width=new_height;
height=new_width;
    //将texture_ID设置为2D纹理信息
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //纹理放大缩小使用线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //将图像内存用作纹理信息
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
free(new_pixels);
    return texture_ID;
}
void display()
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //获取纹理对象
    GLuint image = load_texture();
    //重新设置OpenGL窗口：原点位置为左上角，x轴从左到右，y轴从上到下，坐标值与像素坐标值相同
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);//左下角x坐标, 左下角y坐标, wigth, height
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);//左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标
    //显示纹理
    glEnable(GL_TEXTURE_2D);              //允许使用纹理
    glBindTexture(GL_TEXTURE_2D, image);                   //选择纹理对象
                                                           //原始完全填充四边形
    glBegin(GL_POLYGON);  //设置为多边形纹理贴图方式并开始贴图
    glTexCoord2f(0.0f, 0.0f); glVertex2f(width,height);    //纹理左上角对应窗口左上角
    glTexCoord2f(0.0f, 1.0f); glVertex2f(width, 0);        //纹理左下角对应窗口左下角
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0, 0);            //纹理右下角对应窗口右下角
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0, height);       //纹理右上角对应窗口右上角
    glEnd();    //结束贴图*/
    glDisable(GL_TEXTURE_2D);    //禁止使用纹理
                                //双缓存交换缓存以显示图像
    glutSwapBuffers();
}
void main(int argc, char** argv)
{
    //初始化GL
    glutInit(&argc, argv);
    //设置显示参数(双缓存，RGB格式)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //设置窗口尺寸：width*height
    glutInitWindowSize(1280, 720);
    //设置窗口位置：在屏幕左上角像素值(100,100)处
    glutInitWindowPosition(100, 100);
    //设置窗口名称
    glutCreateWindow("OpenGL");
    //显示函数，display事件需要自行编写
    glutDisplayFunc(display);
    //重复循环GLUT事件
    glutMainLoop();
}
