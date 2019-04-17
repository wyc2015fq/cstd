# OpenCV学习笔记（三十五）——用Qt做摄像头读取 - 迭代的是人，递归的是神 - CSDN博客





2011年12月05日 17:36:49[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：18325
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









之前介绍了Qt+OpenCV的图形界面设计[http://blog.csdn.net/yang_xian521/article/details/6968012](http://blog.csdn.net/yang_xian521/article/details/6968012)，那篇里面只是读取了图片，这次再略进一步，再实现一个摄像头视频的读取。

再介绍一下我的开发环境Qt4.7.4+OpenCV2.3.1+VS2008，其实很简单，先在自己的QMainWindow子类里面声明如下变量：



```cpp
public:
	camCapture(QWidget *parent = 0, Qt::WFlags flags = 0);
	~camCapture();
protected:
	void paintEvent(QPaintEvent * e);

private:
	Ui::camCaptureClass ui;
	cv::Mat frame;
	cv::VideoCapture capture;
	QImage *image;
	QTimer *timer;

private slots:
	void nextFrame();
```


paintEvent函数是重载的，目的是为了更新绘图，在其定义中添加：



```cpp
void camCapture::paintEvent(QPaintEvent * e)
{
	// 更新图像
	QPainter painter(this);
	painter.drawImage(QPoint(0, 12), *image);
}
```


camCapture的构造函数里面添加如下初始化代码：



```cpp
// 初始化处理，建立QImage和frame的关联，开启定时器
	capture.open(-1);
	if (capture.isOpened())
	{
		capture >> frame;
		if (!frame.empty())
		{
			cv::cvtColor(frame, frame, CV_BGR2RGB);
			cv::flip(frame, frame, 1);
			image = new QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
			timer = new QTimer(this);
			timer->setInterval(30);
			connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
			timer->start();
		}
	}
```


析构函数里释放timer和image变量。

nextFrame函数实现数据的更新：



```cpp
// 更新数据
	capture >> frame;
	if (!frame.empty())
	{
		cv::cvtColor(frame, frame, CV_BGR2RGB);
		cv::flip(frame, frame, 1);
		this->update();
	}
```

这里我又想起来了我当时做图片读取的时候把参数CV_BGR2RGB、Format_RGB888改为了CV_BGR2RGBA、Format_RGB32，但这次试验发现那组参数可能只对我试验的图片有效，对视频还是CV_BGR2RGB、Format_RGB888这组参数是能用的。

还有一点很不爽，就是添加函数nextFrame和重载paintEvent函数都找不到向导，都是我自己敲进去的，可能是我的开发环境VS对Qt工程的支持不够霸气，以后可能要果断使用QtCreator了。我是Qt方面的真菜鸟，要是有经验的朋友可以给我说说怎么在VS环境里找到添加Qt事件的向导。万分感谢。

欢迎大家下载[http://download.csdn.net/detail/yang_xian521/3882970](http://download.csdn.net/detail/yang_xian521/3882970)



