# OpenCV学习笔记（四十二）——Mat数据操作之普通青年、文艺青年、暴力青年

2011年12月29日 11:47:18

yang_xian521

阅读数：44642

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

首先还是要感谢箫鸣朋友在我[《OpenCV学习笔记（四十）——再谈OpenCV数据结构Mat详解》](http://blog.csdn.net/yang_xian521/article/details/7107786)的留言，告诉我M.at<float>(3, 3)在Debug模式下运行缓慢，推荐我使用M.ptr<float>(i)此类方法。这不禁勾起了我测试一下的冲动。下面就为大家奉上我的测试结果。

我这里测试了三种操作Mat数据的办法，套用流行词，普通青年，文艺青年，为啥第三种我不叫2b青年，大家慢慢往后看咯。

> 普通青年的操作的办法通常是M.at<float>(i,  j)
>
> 文艺青年一般会走路线M.ptr<float>(  i )[ j ]
>
> 暴力青年通常直接强制使用我第40讲提到的M.data这个指针

实验代码如下：



```cpp
	t = (double)getTickCount();
	Mat img1(1000, 1000, CV_32F);
	
	for (int i=0; i<1000; i++)
	{
		for (int j=0; j<1000; j++)
		{
			img1.at<float>(i,j) = 3.2f;
		}
	}
	t = (double)getTickCount() - t;
	printf("in %gms\n", t*1000/getTickFrequency());
	//***************************************************************
	t = (double)getTickCount();
	Mat img2(1000, 1000, CV_32F);
 
	for (int i=0; i<1000; i++)
	{
		for (int j=0; j<1000; j++)
		{
			img2.ptr<float>(i)[j] = 3.2f;
		}
	}
	t = (double)getTickCount() - t;
	printf("in %gms\n", t*1000/getTickFrequency());
	//***************************************************************
	t = (double)getTickCount();
	Mat img3(1000, 1000, CV_32F);
	float* pData = (float*)img3.data;
 
	for (int i=0; i<1000; i++)
	{
		for (int j=0; j<1000; j++)
		{
			*(pData) = 3.2f;
			pData++;
		}
	}
	t = (double)getTickCount() - t;
	printf("in %gms\n", t*1000/getTickFrequency());
	//***************************************************************
	t = (double)getTickCount();
	Mat img4(1000, 1000, CV_32F);
 
	for (int i=0; i<1000; i++)
	{
		for (int j=0; j<1000; j++)
		{
			((float*)img3.data)[i*1000+j] = 3.2f;
		}
	}
	t = (double)getTickCount() - t;
	printf("in %gms\n", t*1000/getTickFrequency());
```

 最后两招可以都看成是暴力青年的方法，因为反正都是指针的操作，局限了各暴力青年手段就不显得暴力了。 



在Debug、Release模式下的测试结果分别为：

 

|           | Debug    | Release |
| --------- | -------- | ------- |
| 普通青年  | 139.06ms | 2.51ms  |
| 文艺青年  | 66.28ms  | 2.50ms  |
| 暴力青年1 | 4.95ms   | 2.28ms  |
| 暴力青年2 | 5.11ms   | 1.37ms  |

根据测试结果，我觉得箫铭说的是很可信的，普通青年的操作在Debug模式下果然缓慢，他推荐的文艺青年的路线确实有提高。值得注意的是本来后两种办法确实是一种比较2b青年的做法，因为at操作符或者ptr操作符，其实都是有内存检查的，防止操作越界的，而直接使用data这个指针确实很危险。不过从速度上确实让人眼前一亮，所以我不敢称这样的青年为2b，尊称为暴力青年吧。

不过在Release版本下，几种办法的速度差别就不明显啦，都是很普通的青年。所以如果大家最后发行程序的时候，可以不在意这几种操作办法的，推荐前两种哦，都是很好的写法，操作指针的事还是留给大神们用吧。就到这里吧~~

补充：箫铭又推荐了两种文艺青年的处理方案，我也随便测试了一下，先贴代码，再贴测试结果：



```cpp
	/*********加强版********/
	t = (double)getTickCount();
	Mat img5(1000, 1000, CV_32F);
	float *pData1;
	for (int i=0; i<1000; i++) 
	{ 
		pData1=img5.ptr<float>(i);
		for (int j=0; j<1000; j++) 
		{ 
			pData1[j] = 3.2f; 
		} 
	} 
	t = (double)getTickCount() - t;
	printf("in %gms\n", t*1000/getTickFrequency());
	/*******终极版*****/
	t = (double)getTickCount();
	Mat img6(1000, 1000, CV_32F);
	float *pData2;
	Size size=img6.size();
	if(img2.isContinuous())
	{
		size.width = size.width*size.height;
		size.height = 1;
	}
	size.width*=img2.channels();
	for(int i=0; i<size.height; i++)
	{
		pData2 = img6.ptr<float>(i);
		for(int j=0; j<size.width; j++)
		{
			pData2[j] = saturate_cast<float>(3.2f);
		}
	}
	t = (double)getTickCount() - t;
	printf("in %gms\n", t*1000/getTickFrequency());
```

 测试结果： 



 

|                | Debug   | Release |
| -------------- | ------- | ------- |
| 加强版文艺青年 | 5.74ms  | 2.43ms  |
| 终极版文艺青年 | 40.12ms | 2.34ms  |

 我的测试结果感觉这两种方案只是锦上添花的效果，也使大家的操作有了更多的选择，但感觉在速度上并没有数量级的提升，再次感谢箫铭对我blog的支持。后来箫铭说saturate_cast才把速度降下来，我很同意，就不贴上去测试结果了。但我查看资料了解了一下

saturate_cast

的作用。

可以看成是类型的强制转换，比如对于saturate_cast<uchar>来说，就是把数据转换成8bit的0~255区间，负值变成0，大于255的变成255。如果是浮点型的数据，变成round最近的整数

，还是很有用处的函数，推荐大家在需要的时候尝试。 