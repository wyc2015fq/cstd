# OpenCV学习笔记（二十五）——OpenCV图形界面设计Qt+VS2008

2011年11月14日 15:40:08

yang_xian521

阅读数：34434

 								标签： 																[qt																](https://so.csdn.net/so/search/s.do?q=qt&t=blog)[图形																](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[image																](https://so.csdn.net/so/search/s.do?q=image&t=blog)[数据结构																](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)[application																](https://so.csdn.net/so/search/s.do?q=application&t=blog)[mfc																](https://so.csdn.net/so/search/s.do?q=mfc&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/yang_xian521/article/category/910716) 							

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

一直对MFC对OpenCV的支持不好而耿耿于怀，了解了Qt对OpenCV支持很好，但网上这方面的资料很少。大部分的图形交互的设计都是基于OpenCV2.0之前的数据结构lpImage进行的。最近得到了一本好书《OpenCV 2 Computer Vision Application Programming Cookbook》，下载的链接为<http://ishare.iask.sina.com.cn/f/20485520.html?retcode=0>，2011年5月出版，全书都是基于OpenCV2.2版本的实现，采用了新的数据结构。我这里强烈建议利用C++开发的朋友们不要再使用老版本的数据结构了，实在影响开发效率。至于大家最熟悉的参考书《learning  OpenCV》和《OpenCV教程——基础篇》这两本广为流传的书，我的看法是已经远远不能满足OpenCV新版本的学习了。真的希望这本新书的中文版尽快出炉。

参考这本书的利用Qt创建GUI，不过书中的办法是在Qt Creator下实现的，我这里通过VS2008+Qt实现。下面结合一个例程介绍一下如何在Qt的GUI环境下开发OpenCV。

先新建工程Qt Project --> Qt  Application，点击finish完成工程的创建。然后在项目属性里的连接器的附加依赖项里面添加opencv的lib文件。工程创建好之后，一个后缀名为.ui的文件就是关于界面设计的。首先创建两个按钮，拖拽两个Push  Buttons到Form中去，修改其属性，一个名字为Open Image，一个为Process。右键按钮选择connect  signal为其添加响应函数，选择clicked（）。对应的cpp文件多出了on_OpenImage_clicked函数，代码如下：



```cpp
#include "qttest1.h"
 
qttest1::qttest1(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}
 
qttest1::~qttest1()
{
 
}
 
 
void qttest1::on_OpenImage_clicked()
{
 
}
```

 接下来要在对应的头文件中添加显示图片的必要代码，添加QFileDialog类的声明，OpenCV必要的include头文件包含，在类中声明一个cv :: Mat成员变量。代码如下： 





```cpp
#ifndef QTTEST1_H
#define QTTEST1_H
 
#include <QtGui/QMainWindow>
#include <QFileDialog>
#include "ui_qttest1.h"
 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
 
class qttest1 : public QMainWindow
{
	Q_OBJECT
 
public:
	qttest1(QWidget *parent = 0, Qt::WFlags flags = 0);
	~qttest1();
 
private:
	Ui::qttest1Class ui;
	cv::Mat image;	// cv图片
 
private slots:
	void on_OpenImage_clicked();
};
 
#endif // QTTEST1_H
```



接下来添加on_OpenImage_clicked的函数内容。代码如下：



```cpp
void qttest1::on_OpenImage_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
	image = cv::imread(fileName.toAscii().data());
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);
}
```

 类似的办法添加Process按钮的相应代码： 





```cpp
void qttest1::on_Process_clicked()
{
	cv::flip(image, image, 1);
	cv::namedWindow("Output Image");
	cv::imshow("Output Image", image);
}
```



这样就可以实现一个普通的按钮响应。但我们的目的是将图片显示在对话框form中，这还需要进一步深入学习。与MFC中显示图片需要CvvImage类似，要想在Qt的Form中显示图片，图片的格式也必须为QImage，需要将cv：：Mat的BGR通道顺序变换为RGBA，QImage的格式为Format_RGB32，调用cvtColor实现（这里例程里调用的为BGR2RGB，然后QImage的格式为Format_RGB888，但我试验显示结果是有问题的，故自己调整了一下）。（后经实验，BGR2RGB，Format_RGB888这组参数对于webcam视频是正确的，对于我当时的实验照片是不正确的，格式的问题真是混乱！！！）实现代码如下：



```cpp
	cv::cvtColor(image, image, CV_BGR2RGBA);
	QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB32);
	QLabel *label = new QLabel(this);
	label->move(200, 50);
	label->setPixmap(QPixmap::fromImage(img));
	label->resize(label->pixmap()->size());	
	label->show();
```

 注意打开图片的路径不要有中文，最后的显示结果为 



![img](http://hi.csdn.net/attachment/201111/14/0_1321256497sA7g.gif)

相关代码的下载地址为<http://download.csdn.net/detail/yang_xian521/3793960>