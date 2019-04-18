# Qt：&OpenCV—Q图像处理基本操作（Code） - wishchinYang的专栏 - CSDN博客
2013年10月20日 19:30:32[wishchin](https://me.csdn.net/wishchin)阅读数：1118
原文链接：[http://www.cnblogs.com/emouse/archive/2013/03/31/2991333.html](http://www.cnblogs.com/emouse/archive/2013/03/31/2991333.html)
作者写作一系列：[http://www.cnblogs.com/emouse/category/449213.html](http://www.cnblogs.com/emouse/category/449213.html)
## Qt图像的缩放显示 
实现图像缩放的方法很多，在 [OpenCV&Qt学习之一——打开图片文件并显示](http://www.cnblogs.com/emouse/archive/2013/03/29/2988717.html) 的例程中，label控件是通过
```cpp
ui->imagelabel->resize(ui->imagelabel->pixmap()->size());
```
来实现适应图像显示的，但是由于窗口固定，在打开的图像小于控件大小时就会缩在左上角显示，在打开图像过大时则显示不全。因此这个例程中首先实现图像适合窗口的缩放显示。
由于是基于OpenCV和Qt的图像处理，因此图像的缩放处理在OpenCV和Qt都可以完成，我这里就把OpenCV用作图像的原始处理，Qt用作显示处理，因此缩放显示由Qt完成。
Qt中QImage提供了用于缩放的基本函数，而且功能非常强大，使用Qt自带的帮助可以检索到相关信息。
函数原型：
```cpp
QImage QImage::scaled ( const QSize & size, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation ) const
```
这是直接获取大小，还有另一种形式：
QImage QImage::scaled ( int width, int height, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation ) const
函数说明以及参数在文档中已经说的非常清楚了，文档摘录如下：
> 
Returns a copy of the image scaled to a rectangle defined by the given*size* according to the given*aspectRatioMode* and*transformMode*.
![image](http://images.cnitblog.com/blog/337520/201303/31102332-e2a57dbc52f54aeca4f0876986731d19.png)
- If *aspectRatioMode* is [Qt::IgnoreAspectRatio](), the image is scaled to*size*.
- If *aspectRatioMode* is [Qt::KeepAspectRatio](), the image is scaled to a rectangle as large as possible
 inside*size*, preserving the aspect ratio. 
- If *aspectRatioMode* is [Qt::KeepAspectRatioByExpanding](), the image is scaled to a rectangle as small as
 possible outside*size*, preserving the aspect ratio.
官方文档中已经说的比较清楚了，代码实现也比较简单，代码如下：
```cpp
{
    QImage imgScaled ;
    imgScaled = img.scaled(ui->imagelabel->size(),Qt::KeepAspectRatio);
//  imgScaled = img.QImage::scaled(ui->imagelabel->width(),ui->imagelabel->height(),Qt::KeepAspectRatio);
    ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled));
}
```
显示效果如下：
![image](http://images.cnitblog.com/blog/337520/201303/31135807-934fe7d6ed2741399b9f213963bb5d7a.png)
## QImage的一点疑问与理解 
在查找资料时参考了这篇  [Qt中图像的显示与基本操作](http://www.cnblogs.com/Romi/archive/2012/03/14/2396533.html) 博客，但是存在一些疑点，博客中相关代码如下：
```
QImage* imgScaled = new QImage；
*imgScaled=img->scaled(width,height,Qt::KeepAspectRatio);
ui->label->setPixmap(QPixmap::fromImage(*imgScaled));
```
对于以上代码通过和我之前的代码做简单对比，发现有几点不一样的地方：
- 图像的定义方式，这里的定义方式为QImage* imgScale = new QImage
- scaled函数的调用方式，一个是imgScaled = img.scaled后者为*imgScaled=img->scaled，我最开始也是将.写为->一直没找出错误，提示base operand of '->' has non-pointer type 'QImage'
继续查找Qt的帮助手册，发现QImage的构造函数还真是多：
> 
#### Public Functions
**[QImage]()** ()
**[QImage]()** ( const QSize &*size*, Format*format* )
**[QImage]()** ( int *width*, int*height*, Format *format* )
**[QImage]()** ( uchar **data*, int*width*, int*height*, Format*format* )
**[QImage]()** ( const uchar **data*, int*width*, int*height*, Format*format* )
**[QImage]()** ( uchar **data*, int*width*, int*height*, int*bytesPerLine*, Format*format* )
**[QImage]()** ( const uchar **data*, int*width*, int*height*, int*bytesPerLine*, Format*format* )
**[QImage]()** ( const char * const[]*xpm* )
**[QImage]()** ( const QString &*fileName*, const char **format* = 0 )
**[QImage]()** ( const char **fileName*, const char **format* = 0 )
**[QImage]()** ( const QImage &*image* )
**[~QImage]()** ()
QImage提供了适用于不同场合的构造方式，在手册中对他们也有具体的应用，但是我仍然没找到QImage image;和QImage* image = new QImage这两种究竟对应的是哪两种，有什么区别和不同。 在上一篇博文 [OpenCV&Qt学习之二——QImage的进一步认识](http://www.cnblogs.com/emouse/archive/2013/03/29/2989592.html)  中提到了对于图像数据的一点认识，其中提到QImage是对现有数据的一种重新整合，是一种格式，但是数据还是指向原来的。从这里来看还需要根据构造方式具体区别，并不完全正确。
凌乱查了查资料，网上的资料就那几个，互相转来转去的，而且多数比较老，仍然没有帮助我想通关于这里面数据结构的一些疑问，Qt 和 OpenCV对C和指针的要求还是比较高的，长时间从单片机类的程序过来那点功底还真不够，具体的C应用都忘光了。这个问题只能暂时搁置，在后面的学习中慢慢理解。
## 基于OpenCV的图像初步处理
### 以下两个例程根据书籍 OpenCV 2 Computer Vision Application Programming Cookbook中的相关例程整理，这是一本比较新也比较基础的入门书籍。
### salt-and-pepper noise
关于图像数据的基础知识参见这段介绍：
> 
Fundamentally, an image is a matrix of numerical values. This is why OpenCV 2 manipulates them using the cv::Mat data structure. Each element of the matrix represents one pixel. For a gray-level image (a "black-and-white" image), pixels are unsigned 8-bit
 values where 0 corresponds to black and corresponds 255 to white. For a color image, three such values per pixel are required to represent the usual three primary color channels {Red, Green, Blue}. A matrix element is therefore made, in this case, of a triplet
 of values.
这儿以想图像中添加saltand-pepper noise为例，来说明如何访问图像矩阵中的独立元素。saltand-pepper noise就是图片中一些像素点，随机的被黑色或者白色的像素点所替代，因此添加saltand-pepper noise也比较简单，只需要随机的产生行和列，将这些行列值对应的像素值更改即可，当然通过上面的介绍，需要更改RGB3个通道。程序如下：
```cpp
void Widget::salt(cv::Mat &image,int n)
{
    int i,j;
    for (int k=0; k<n; k++)
    {
        i= qrand()%image.cols;
        j= qrand()%image.rows;
        if (image.channels() == 1) { // gray-level image
            image.at<uchar>(j,i)= 255;
        } else if (image.channels() == 3) { // color image
            image.at<cv::Vec3b>(j,i)[0]= 255;
            image.at<cv::Vec3b>(j,i)[1]= 255;
            image.at<cv::Vec3b>(j,i)[2]= 255;
        }
    }
}
```
```
对Win 7系统中的自带图像考拉进行处理后的效果如下图所示（程序是Ubuntu 12.04下的）：
```
![image](http://images.cnitblog.com/blog/337520/201303/31150014-7211fd8adf404aec9383b4dc2d76f271.png)
### 减少色彩位数
在很多处理中需要对图片中的所有像素进行遍历操作，采用什么方式进行这个操作是需要思考的问题，关于这个问题的论述可以参考下面一段简介：
> 
Color images are composed of 3-channel pixels. Each of these channels corresponds to the intensity value of one of the three primary colors (red, green, blue). Since each of these values is an 8-bit unsigned char, the total number of colors
 is 256x256x256, which is more than 16 million colors. Consequently, to reduce the complexity of an analysis, it is sometimes useful to reduce the number of colors in an image. One simple way to achieve this goal is to simply subdivide the RGB space into cubes
 of equal sizes. For example, if you reduce the number of colors in each dimension by 8, then you would obtain a total of 32x32x32 colors. Each color in the original image is then assigned a new color value in the color-reduced image that corresponds to the
 value in the center of the cube to which it belongs.
这个例子就是通过操作每一个像素点来减少色彩的位数，基本内容在以上的英文引文中已经有了介绍，代码的实现也比较直接。在彩色图像中，3个通道的数据是依次排列的，每一行的像素三个通道的值依次排列，cv::Mat中的通道排列顺序为BGR，那么一个图像需要的地址块空间为uchar 宽×高×3.但是需要注意的是，有些处理器针对行数为4或8的图像处理更有效率，因此为了提高效率就会填充一些额外的像素，这些额外的像素不被显示和保存，值是忽略的。
实现这个功能的代码如下：
```cpp
// using .ptr and []
void Widget::colorReduce0(cv::Mat &image, int div)
{
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      for (int j=0; j<nl; j++)
      {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++)
          {
            // process each pixel ---------------------
                data[i]= data[i]/div*div+div/2;
            // end of pixel processing ----------------
          } // end of line
      }
}
```
data[i]= data[i]/div*div+div/2; 通过整除的方式，就像素位数进行减少，这里没明白的是为啥后面还要加上div/2。
效果如下：
![image](http://images.cnitblog.com/blog/337520/201304/01110106-93baf049edab4a0f8a3cff17a36538df.png)
程序源代码：
```cpp
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}
Widget::~Widget()
{
    delete ui;
}
void Widget::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                ".",tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug()<<"filenames:"<<fileName;
    image = cv::imread(fileName.toAscii().data());
    ui->imgfilelabel->setText(fileName);
    //here use 2 ways to make a copy
//    image.copyTo(originalimg);          //make a copy
    originalimg = image.clone();        //clone the img
    qimg = Widget::Mat2QImage(image);
    display(qimg);                      //display by the label
    if(image.data)
    {
        ui->saltButton->setEnabled(true);
        ui->originalButton->setEnabled(true);
        ui->reduceButton->setEnabled(true);
    }
}
QImage Widget::Mat2QImage(const cv::Mat &mat)
{
    QImage img;
    if(mat.channels()==3)
    {
        //cvt Mat BGR 2 QImage RGB
        cvtColor(mat,rgb,CV_BGR2RGB);
        img =QImage((const unsigned char*)(rgb.data),
                    rgb.cols,rgb.rows,
                    rgb.cols*rgb.channels(),
                    QImage::Format_RGB888);
    }
    else
    {
        img =QImage((const unsigned char*)(mat.data),
                    mat.cols,mat.rows,
                    mat.cols*mat.channels(),
                    QImage::Format_RGB888);
    }
    return img;
}
void Widget::display(QImage img)
{
    QImage imgScaled;
    imgScaled = img.scaled(ui->imagelabel->size(),Qt::KeepAspectRatio);
//  imgScaled = img.QImage::scaled(ui->imagelabel->width(),ui->imagelabel->height(),Qt::KeepAspectRatio);
    ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled));
}
void Widget::on_originalButton_clicked()
{
    qimg = Widget::Mat2QImage(originalimg);
    display(qimg);
}
void Widget::on_saltButton_clicked()
{
    salt(image,3000);
    qimg = Widget::Mat2QImage(image);
    display(qimg);
}
void Widget::on_reduceButton_clicked()
{
    colorReduce0(image,64);
    qimg = Widget::Mat2QImage(image);
    display(qimg);
}
void Widget::salt(cv::Mat &image, int n)
{
    int i,j;
    for (int k=0; k<n; k++)
    {
        i= qrand()%image.cols;
        j= qrand()%image.rows;
        if (image.channels() == 1)
        { // gray-level image
            image.at<uchar>(j,i)= 255;
        }
        else if (image.channels() == 3)
        { // color image
            image.at<cv::Vec3b>(j,i)[0]= 255;
            image.at<cv::Vec3b>(j,i)[1]= 255;
            image.at<cv::Vec3b>(j,i)[2]= 255;
        }
    }
}
// using .ptr and []
void Widget::colorReduce0(cv::Mat &image, int div)
{
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      for (int j=0; j<nl; j++)
      {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++)
          {
            // process each pixel ---------------------
                data[i]= data[i]/div*div+div/2;
            // end of pixel processing ----------------
          } // end of line
      }
}
复制代码
 
复制代码
#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
namespace Ui {
class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void on_openButton_clicked();
    QImage Mat2QImage(const cv::Mat &mat);
    void display(QImage image);
    void salt(cv::Mat &image, int n);
    void on_saltButton_clicked();
    void on_reduceButton_clicked();
    void colorReduce0(cv::Mat &image, int div);
    void on_originalButton_clicked();
private:
    Ui::Widget *ui;
    cv::Mat image;
    cv::Mat originalimg; //store the original img
    QImage qimg;
    QImage imgScaled;
    cv::Mat rgb;
};
#endif // WIDGET_H
复制代码
书中还给了其他十余种操作的方法：
复制代码
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// using .ptr and []
void colorReduce0(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
                  data[i]= data[i]/div*div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// using .ptr and * ++ 
void colorReduce1(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
                 *data++= *data/div*div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// using .ptr and * ++ and modulo
void colorReduce2(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
       
                  int v= *data;
                  *data++= v - v%div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// using .ptr and * ++ and bitwise
void colorReduce3(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// direct pointer arithmetic
void colorReduce4(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      int step= image.step; // effective width
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      // get the pointer to the image buffer
      uchar *data= image.data;
      for (int j=0; j<nl; j++) {
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *(data+i)= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
            data+= step;  // next line
      }
}
// using .ptr and * ++ and bitwise with image.cols * image.channels()
void colorReduce5(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<image.cols * image.channels(); i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// using .ptr and * ++ and bitwise (continuous)
void colorReduce6(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      if (image.isContinuous())  {
          // then no padded pixels
          nc= nc*nl; 
          nl= 1;  // it is now a 1D array
       }
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// using .ptr and * ++ and bitwise (continuous+channels)
void colorReduce7(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols ; // number of columns
      if (image.isContinuous())  {
          // then no padded pixels
          nc= nc*nl; 
          nl= 1;  // it is now a 1D array
       }
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {
          uchar* data= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= *data&mask + div/2;
            *data++= *data&mask + div/2;
            *data++= *data&mask + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// using Mat_ iterator 
void colorReduce8(cv::Mat &image, int div=64) {
      // get iterators
      cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
      cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();
      for ( ; it!= itend; ++it) {
        
        // process each pixel ---------------------
        (*it)[0]= (*it)[0]/div*div + div/2;
        (*it)[1]= (*it)[1]/div*div + div/2;
        (*it)[2]= (*it)[2]/div*div + div/2;
        // end of pixel processing ----------------
      }
}
// using Mat_ iterator and bitwise
void colorReduce9(cv::Mat &image, int div=64) {
      // div must be a power of 2
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
      // get iterators
      cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
      cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();
      // scan all pixels
      for ( ; it!= itend; ++it) {
        
        // process each pixel ---------------------
        (*it)[0]= (*it)[0]&mask + div/2;
        (*it)[1]= (*it)[1]&mask + div/2;
        (*it)[2]= (*it)[2]&mask + div/2;
        // end of pixel processing ----------------
      }
}
// using MatIterator_ 
void colorReduce10(cv::Mat &image, int div=64) {
      // get iterators
      cv::Mat_<cv::Vec3b> cimage= image;
      cv::Mat_<cv::Vec3b>::iterator it=cimage.begin();
      cv::Mat_<cv::Vec3b>::iterator itend=cimage.end();
      for ( ; it!= itend; it++) { 
        
        // process each pixel ---------------------
        (*it)[0]= (*it)[0]/div*div + div/2;
        (*it)[1]= (*it)[1]/div*div + div/2;
        (*it)[2]= (*it)[2]/div*div + div/2;
        // end of pixel processing ----------------
      }
}
void colorReduce11(cv::Mat &image, int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols; // number of columns
              
      for (int j=0; j<nl; j++) {
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
                  image.at<cv::Vec3b>(j,i)[0]=     image.at<cv::Vec3b>(j,i)[0]/div*div + div/2;
                  image.at<cv::Vec3b>(j,i)[1]=     image.at<cv::Vec3b>(j,i)[1]/div*div + div/2;
                  image.at<cv::Vec3b>(j,i)[2]=     image.at<cv::Vec3b>(j,i)[2]/div*div + div/2;
 
            // end of pixel processing ----------------
 
            } // end of line                   
      }
}
// with input/ouput images
void colorReduce12(const cv::Mat &image, // input image 
                 cv::Mat &result,      // output image
                 int div=64) {
      int nl= image.rows; // number of lines
      int nc= image.cols ; // number of columns
      // allocate output image if necessary
      result.create(image.rows,image.cols,image.type());
      // created images have no padded pixels
      nc= nc*nl; 
      nl= 1;  // it is now a 1D array
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
              
      for (int j=0; j<nl; j++) {
          uchar* data= result.ptr<uchar>(j);
          const uchar* idata= image.ptr<uchar>(j);
          for (int i=0; i<nc; i++) {
 
            // process each pixel ---------------------
                 
            *data++= (*idata++)&mask + div/2;
            *data++= (*idata++)&mask + div/2;
            *data++= (*idata++)&mask + div/2;
 
            // end of pixel processing ----------------
 
          } // end of line                   
      }
}
// using overloaded operators
void colorReduce13(cv::Mat &image, int div=64) {
    
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0));
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
      // perform color reduction
      image=(image&cv::Scalar(mask,mask,mask))+cv::Scalar(div/2,div/2,div/2);
}
```
★emouse 思·睿博客文章★原创文章转载请注明：http://emouse.cnblogs.com
