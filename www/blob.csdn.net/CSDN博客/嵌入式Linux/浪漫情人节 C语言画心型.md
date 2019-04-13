
# 浪漫情人节|C语言画心型 - 嵌入式Linux - CSDN博客

2019年02月14日 11:52:43[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：215



# 1.前言
新年第一天上班，先祝大家新年快乐，巧的是，今天刚好又是情人节，所以想了下用C实现画心形符号~
# 2.心形符号数学公式
[http://mathworld.wolfram.com/HeartCurve.html](http://mathworld.wolfram.com/HeartCurve.html)
这个网站里面的公式，我随便挑了一个
![](https://img-blog.csdnimg.cn/20190214113407685.png)
公式如下
$$
(x^2+y^2-1)^3 - x^2y^3 = 0
$$

# 3.代码如下
代码里面有注释，不理解的可以自己运行调试
easyx 下载链接如下
[https://www.easyx.cn/](https://www.easyx.cn/)
`#include <stdio.h>
#include <easyx.h>
#include <math.h>
#define X 480
#define Y 480
int main()
{
	int i, j;
	/*初始化X,Y缓冲区*/
	initgraph(X, Y);
	/*设置背景颜色*/
	setbkcolor(WHITE);
	/*用于清除图形屏幕*/
	cleardevice();
	for (i = 0; i<X; i++)
	{
		for (j = 0; j<Y; j++)
		{
			/*i,j就是坐标轴，但是因为没有负数，所以需要做平移操作*/
			double t1 = j / 100.0 - 2, t2 = -(i / 100.0 - 2.5);
			/*判断是否在区间范围内*/
			if(pow(pow(t1,2) +pow(t2,2) -1,3) -pow(t1,2)*pow(t2,3) < 0.0)
				putpixel(j, i, RED);
		}
	}
	getchar();
	return 0;
}`
# 4.运行结果
![](https://img-blog.csdnimg.cn/20190214114117978.png)

[
  ](https://img-blog.csdnimg.cn/20190214113407685.png)