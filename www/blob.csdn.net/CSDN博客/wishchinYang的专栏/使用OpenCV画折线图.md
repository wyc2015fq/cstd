# 使用OpenCV画折线图 - wishchinYang的专栏 - CSDN博客
2015年09月10日 15:22:53[wishchin](https://me.csdn.net/wishchin)阅读数：5871
      使用OpenCV画直方图是一件轻松的事情，画折线图就没有那么Easy了，还是使用一个库吧：
```cpp
GraphUtils 源代码添加入工程
```
       原文链接：[http://www.360doc.com/content/14/0424/12/16961511_371674057.shtml](http://www.360doc.com/content/14/0424/12/16961511_371674057.shtml)
       使用的画图代码：
```cpp
//使用一个OpenCV的画图库
#include "GraphUtils.h"
//显示文本里面的模式曲线
void CPicToolsDlg::OnBnClickedButtonShowpattern()
{
	
	//打开目录文件
	CFileDialog FileDlg(
		TRUE, _T("*.pcd"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("*.txt|*.txt| *.pcd|*.pcd| All Files (*.*) |*.*||"), NULL);//选取约定！	
	FileDlg.m_ofn.lpstrTitle = _T("打开待显示模式文本！！！");	// 打开文件对话框的标题名
	if (FileDlg.DoModal() != IDOK)	{
		MessageBox("打开文件失败！！！请重新检测", "提示", 0);
		return ;
	}
	CString FilePath = FileDlg.GetPathName();
	std::string MyFileName = FilePath;
	//读取文本到序列
	std::vector<cv::Point3f >   Seq(0);
	this->readTxt2Seq(MyFileName,Seq);
	//显示文本为模式
    //使用三维显示
	this->showFeature3f(Seq );
	return;
}
//使用OpenCV显示三维序列
//暂时显示一维，三维的太困难了！
//显示一维曲线，最后显示多个，人工分析模式
bool CPicToolsDlg::showFeature3f(std::vector<cv::Point3f >  &Seq )
{
	//画一个曲线//使用三色桌布吧，同时画出x,y,z
	cv::Mat  CanvasM(480,800,CV_8UC3);
	//CanvasM.
	CanvasM= cv::Mat::zeros(480,800,CV_8UC3);//全黑背景
	IplImage C =CanvasM;
	cvNot(&C,&C);//反色
	cv::Mat  Canvas(&C);
	CanvasM =Canvas.clone();//克隆
	std::string BarName="特征的折线图";
	int    nArrayLength = Seq.size();
	float *Array = new float[nArrayLength];
	int delay_ms = 10;
	for (int i=0;i< nArrayLength;++i)
	{
		Array[i] = 479 -Seq[i].x ;
	}
	drawLineGraph(BarName,Array,  nArrayLength,  delay_ms);
	return true;
}
//使用空白画图板
int CPicToolsDlg::drawLineGraph(
	std::string BarName,float *array, int nArrayLength, int delay_ms)
{
	//使用画图工具
	const  char *name     =  BarName.c_str();
	const float *arraySrc =  array;
	IplImage *background  =  NULL;
	//nArrayLength =100;
	showFloatGraph(name, arraySrc, nArrayLength, delay_ms, background);
	return 1;
}
//读取文本浮点数到三元组 序列
bool CPicToolsDlg::readTxt2Seq(
	std::string TxtName, std::vector<cv::Point3f >  &Seq )
{
	std::string TestTxt(TxtName);  //  
	freopen(TestTxt.c_str(),"r",stdin);// 打开文件！  
	//三元组，使用三元组逐个写入
	float ViewValue,ViewValue2,ViewValue3;  
	cv::Point3f P(0,0,0);
	Seq.resize(0);
	while (std::cin)
	{
		std::cin >>ViewValue;  // 利用 重定向输入 浮点值  
		std::cin >>ViewValue2; 
		std::cin >>ViewValue3; 
		P.x = ViewValue;
		P.y = ViewValue2;
		P.z = ViewValue3;
		Seq.push_back(P);
	}
	
	return true;
}
```
最终的简单显示效果：
![](https://img-blog.csdn.net/20150910152116486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

