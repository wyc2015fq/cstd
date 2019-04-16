# MeanShift算法C++解析（三） - lyx的专栏 - CSDN博客





2015年11月03日 21:28:56[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：835









最后是最重要的核心啦，就是MeanShift_Tracking这个函数。同样的，我们先来看看这个函数的内部变量。


int num=0,i=0,j=0;
int t_w=0,t_h=0,t_x=0,t_y=0;同样的，是目标矩形的位置和大小信息
double *w=0;//这个呢，是一个拿来比较的矩阵。
double sum_w=0,x1=0,x2=0,y1=2.0,y2=2.0;这些都是后面计算矩形位置偏移要用到的。第一个长得就很像为了归一化而设置的求和变量。
int q_r,q_g,q_b;


int *q_temp;


w=(double*)malloc(sizeof(double)*4096);


q_temp=(int*)malloc(sizeof(int)*t_w*t_h);


首先是为两个矩阵分配空间，大家设置成数组其实也可以。


while((pow(y2,2)+pow(y1,2)>0.5)&&(num<</span>NUM))​然后是一个条件，分析一下这个条件。当y1^2+y2^2<0.5的时候或者num>NUM的时候。前面一个条件是指已经满足精度要求，可以停止了，再算下去没有什么意义了。后面一个是计算次数，表示再算下去太浪费时间了。


接下来一部分是和之前的目标初始化函数一样的，追踪目标矩形没有变，但是帧变了。用这一变化了的帧计算出了直方图。​


for(i=0;i<</span>4096;i++)
{
if(hist2[i]!=0)//有这样的色彩
{
w[i]=sqrt(hist1[i]/hist2[i]);//其实是比较了两个都有的情况。只要有一个没有这种色彩，那么w的值就是零
}else
{
w[i]=0;
}


}


这是两个直方图的比较函数。可见，只有这种颜色在相邻两帧里面都出现过，那么对应的权值矩阵w才不为零（这里的w不是属于每个像素点的权值矩阵，而是用来度量MeanShift向量的，稍后详细讲一下）。


sum_w=0.0;
x1=0.0;


x2=0.0;


这是对这些用来计算偏移向量的数值的初始化。
for(i=0;i<</span>t_h;i++)
{
for(j=0;j<</span>t_w;j++)
{
sum_w=sum_w+w[q_temp[i*t_w+j]];
x1=x1+w[q_temp[i*t_w+j]]*(i-t_h/2);//ydirection
x2=x2+w[q_temp[i*t_w+j]]*(j-t_w/2);
}
}
y1=x1/sum_w;


y2=x2/sum_w;

这边我就用手绘高清图片来解析吧。


[![](http://s14.sinaimg.cn/mw690/002y1HhYgy6RmNnygt78d)](http://photo.blog.sina.com.cn/showpic.html#blogid=&url=http://album.sina.com.cn/pic/002y1HhYgy6RmNnygt78d)



虽然很神奇，但是还是不知道为什么，不知道理论的问题吧。所以谈一下我直观的感受吧。首先是w矩阵的计算，


for(i=0;i<</span>4096;i++)
{
if(hist2[i]!=0)//有这样的色彩
{
w[i]=sqrt(hist1[i]/hist2[i]);//其实是比较了两个都有的情况。只要有一个没有这种色彩，那么w的值就是零
}else
{
w[i]=0;
}


}


显然，只有当这种颜色在第二帧存在的时候，我们才会有这样的权重不为零。并且，当某种颜色的票数在上一帧很多，而当前帧很少的时候，这个值非常大。


最后看一下偏差的形成​


x1=x1+w[q_temp[i*t_w+j]]*(i-t_h/2);​这是y方向的，本质是一种求和。可见，每一个累加的值有两部分决定，一个是对应像素的w值，一个是像素点的位置信息。最终实现得到x和y两个方向的向量。



