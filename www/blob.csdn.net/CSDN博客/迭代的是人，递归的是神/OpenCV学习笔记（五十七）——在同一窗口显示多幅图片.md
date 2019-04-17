# OpenCV学习笔记（五十七）——在同一窗口显示多幅图片 - 迭代的是人，递归的是神 - CSDN博客





2012年08月28日 15:14:01[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：20645










好久没更新blog里，看到OpenCV官网做的越来越好，心里也是很高兴的，真有些冲动将来加入到这个组织里做些事。估计2.4.3要在国庆左右跟大家见面，让我们多期待一下吧。

闲话少说，今天不介绍复杂的算法了，来个简单的，大家写文章做图经常用Matlab，在Matlab里经常在一个窗口里打开多幅图片。遗憾的是OpenCV没有集成这样的功能，但这难不倒大家，让我试试用ROI来解决这个问题。

没啥好讲的，上代码好了：



```cpp
void imshowMany(const std::string& _winName, const vector<Mat>& _imgs)
{
	int nImg = (int)_imgs.size();
	
	Mat dispImg;

	int size;
	int x, y;

	// w - Maximum number of images in a row 
	// h - Maximum number of images in a column 
	int w, h;
	// scale - How much we have to resize the image
	float scale;
	int max;

	if (nImg <= 0) 
	{
		printf("Number of arguments too small....\n");
		return;
	}
	else if (nImg > 12)
	{
		printf("Number of arguments too large....\n");
		return;
	}
	
	else if (nImg == 1)
	{
		w = h = 1;
		size = 300;
	}
	else if (nImg == 2)
	{
		w = 2; h = 1;
		size = 300;
	}
	else if (nImg == 3 || nImg == 4)
	{
		w = 2; h = 2;
		size = 300;
	}
	else if (nImg == 5 || nImg == 6)
	{
		w = 3; h = 2;
		size = 200;
	}
	else if (nImg == 7 || nImg == 8)
	{
		w = 4; h = 2;
		size = 200;
	}
	else
	{
		w = 4; h = 3;
		size = 150;
	}

	dispImg.create(Size(100 + size*w, 60 + size*h), CV_8UC3);

	for (int i= 0, m=20, n=20; i<nImg; i++, m+=(20+size))
	{
		x = _imgs[i].cols;
		y = _imgs[i].rows;

		max = (x > y)? x: y;
		scale = (float) ( (float) max / size );

		if (i%w==0 && m!=20)
		{
			m = 20;
			n += 20+size;
		}

		Mat imgROI = dispImg(Rect(m, n, (int)(x/scale), (int)(y/scale)));
		resize(_imgs[i], imgROI, Size((int)(x/scale), (int)(y/scale)));
	}

	namedWindow(_winName);
	imshow(_winName, dispImg);
}
```


附上效果图一张：



![](https://img-my.csdn.net/uploads/201208/28/1346138604_4801.png)


工程的下载链接为[http://download.csdn.net/detail/yang_xian521/4531610](http://download.csdn.net/detail/yang_xian521/4531610)



