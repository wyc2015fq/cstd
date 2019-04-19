# 人脸识别实战之用QT做点事（八）：QT小知识点（1） - sinat_31425585的博客 - CSDN博客
2018年08月19日 19:11:33[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：208
1、opencv中Mat类型图片转Qt的QImage类型图片
```cpp
QImage Mat2QImage(Mat& Img){
	QImage qImg;
	if (Img.channels() == 3)                             //3 channels color image
	{
		cv::cvtColor(Img, Img, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(Img.data),
			Img.cols, Img.rows,
			Img.cols * Img.channels(),
			QImage::Format_RGB888);
	}
	else if (Img.channels() == 1)                    //grayscale image
	{
		qImg = QImage((const unsigned char*)(Img.data),
			Img.cols, Img.rows,
			Img.cols * Img.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(Img.data),
			Img.cols, Img.rows,
			Img.cols * Img.channels(),
			QImage::Format_RGB888);
	}
	return qImg;
}
```
2、设置应用程序图标
    首先，创建.ico图标文件，并放置在源码所在路径；然后，新建一个txt文本文件，并更名为myico.rc，保存；其次，输入以下文档内容：
```cpp
IDI_ICON1 ICON DISCARDABLE "myico.ico"
```
  注意，myico.ico为创建的.ico文件的文件名；最后，在工程的.pro文件中添加下面这行代码：
```cpp
RC_FILE += myico.rc
```
  最后运行以下，就可以看到效果。
3、让label中内容自适应label大小
```cpp
ui->label->setScaledContent(true);
```
自适应缩放待显示图像尺寸，使之与label大小相匹配。
4、设置窗口标题
  使用setWindowTitle函数，里面字符串就是设置的窗口名。
```cpp
setWindowTitle(tr(""));
```
5、设置按钮背景颜色
```cpp
ui->pushButton->setStyleSheet("background:green");
```
这样就将pushButton对应按钮背景色设置成了绿色。
还可以通过更改样式表，来实现背景色和前景色的设置：
```cpp
QPushButton{
	color: red;
	background-color: rgb(0, 85, 255);
}
```
这样就将前景色设置成了红色，背景色设置成了蓝色。
6、设置背景图片
还是在样式表中，设置主界面的背景，可以设置background-image和border-image，然而，由于背景图片和控件可能大小不一致，导致图片不能完全显示，或者背景图片重复出现，所以一般采用border-image，设置方法如下：
```cpp
/**********主界面背景************/
QMainWindow{
/*背景图片*/
	border-image: url(:/image/images/background.jpg);
}
```
效果如下：
![](https://img-blog.csdn.net/2018082213043613?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7、设置label底色，及设置透明
设置底色可以通过更改样式表：
```cpp
QLabel{
	background-color: rgb(128, 128, 128);
}
```
设置成透明，可以通过setAttribute()实现：
```cpp
setAttribute(Qt::WA_TranslucentBackground)
```
界面如下：
![](https://img-blog.csdn.net/20180824214348429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
8、定时器
      通常一个任务是由多个模块构成，比如，要进行人脸识别跟踪，首先需要进行分类器学习，然后进行分类器分类，最后进行人脸跟踪。整个任务执行过程中，需要进行模块切换（学习模块->识别模块->跟踪模块），模块切换过程，可以考虑用定时器来实现。 
```cpp
QTimer    *timer1;         // 声明定时器
    timer1 = new QTimer(this); 
    timer1->start(33);
    connect(timer1, SIGNAL(timeout()),  // 时间到，读取当前摄像头信息
            this, SLOT(recognize()));
    void MainWindow::recognize()
    {
        ...........
    ｝
    timer1->stop();
```
    流程： 定时器变量声明-> 定时器变量赋值 -> 启动定时器 -> 将定时器与槽函数连接起来->关闭定时器。
