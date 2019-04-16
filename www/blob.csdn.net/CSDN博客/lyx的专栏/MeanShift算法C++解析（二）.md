# MeanShift算法C++解析（二） - lyx的专栏 - CSDN博客





2015年11月03日 21:27:04[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1210








这回呢主要说说鼠标回调函数和目标初始化函数。

当在视频流界面按下按键“P”的时候呢，画面就会停止，点击两下鼠标，分别作为追踪目标选择的左上角和右下角，如此，就可确定追踪目标。在鼠标选择完毕之后，回调函数onMouse会调用init_target函数。这是一个目标初始化函数，也差不多包括了MeanShift算法的一部分核心内容了吧。至于什么是回调函数，那我只能简单的说一下。回调函数是事件驱动的编程方法中的一种函数。函数调用时间不确定，只有特定事件发生，例如按下某个按键，函数才会激活被调用，在Qt内部机制里面叫槽函数（实现有点不一样），在MFC里面就是大量的回调函数，包括eclipse里面也是。有点类似于嵌入式编程里面的中断函数吧。​

void onMouse(intevent,intx,inty,intflags,void*param)

{
if(pause)
{
//=======================================
if(event==CV_EVENT_LBUTTONDOWN)
{
CvPointpt=cvPoint(x,y);
if(chooseEndFlag==MOUSE2PT)
{
drawing_box.width=abs(pt1st.x-pt.x);
drawing_box.height=abs(pt1st.y-pt.y);//设置目标矩形的宽和高
cvRectangle(current,pt1st,pt,CV_RGB(255,0,0),2);
cvShowImage("Meanshift",current);//绘制带目标矩形的图像
//目标初始化
m_wei=(double*)malloc(sizeof(double)*drawing_box.height*drawing_box.width);
init_target(hist1,m_wei,current);
is_tracking=true;
//break;
chooseEndFlag=MOUSE1PT;
return;
}
else
{
chooseEndFlag=MOUSE2PT;
pt1st=cvPoint(pt.x,pt.y);
drawing_box.x=pt.x;
drawing_box.y=pt.y;
}
}
}


}


在onMouse函数里面调用了init_target函数，这个函数 好好解析一下首先是内部变量，


​如下面的示意图所示（有点丑），


​int t_h,t_w,t_x,t_y;这四个量分别用来描述选择的跟踪目标的矩形框，依次是：高，宽，左上角横坐标，左上角纵坐标。


double h,dist;这两个变量是比较重要的，h是整个算法的带宽，就是目标矩形中点到四个顶点的长度的平方，（h=pow(((double)t_w)/2,2)+pow(((double)t_h)/2,2);），dist的话，是distance的意思，就是某一点到中点的距离（dist=pow(i-(double)t_h/2,2)+pow(j-(double)t_w/2,2);//d=x^2+y^2）
int i,j;用来循环计数的变量
int q_r,q_g,q_b,q_temp;由于是彩色图像，所以要分成RGB三个通道来完成，然后计算出最终用来表示颜色的数值（q_temp=q_r*256+q_g*16+q_b;）


[![](http://s16.sinaimg.cn/mw690/002y1HhYgy6Rmqbplkbdf)](http://photo.blog.sina.com.cn/showpic.html#blogid=&url=http://album.sina.com.cn/pic/002y1HhYgy6Rmqbplkbdf)






hist1呢就是一个直方图，横坐标从0,1,2,3,4一直到4096，因为有三个通道，每个通道16位。init_target的本质任务，就是填好这张名字叫做hist1的直方图。


最简单的填法就是，从目标矩形第一个像素开始，是哪种颜色就对属于那种颜色的那个数值加一。​但是这样对于像素同学们太一视同仁了。我们往往认为处于目标矩形中间的像素有更加多的话语权。所以我们利用之间计算的h和dist计算出一个权重矩阵


m_wei[i*t_w+j]=1-dist/h;


​C+=m_wei[i*t_w+j];

然后把所有权值求和，就和的目的等会儿就知道啦。​
hist1[q_temp]=hist1[q_temp]+m_wei[(i-t_y)*t_w+(j-t_x)];这是直方图的计算公式，q_temp是当前像素对应的0-4096的色彩值。仔细一想，发现，本质就是，每一个目标矩形里面的像素点都有一张选取票，票上写着权值矩阵中对应的大小，然后开始投票，于是直方图就形成了。
//归一化直方图
for(i=0;i<</span>4096;i++)
{
hist1[i]=hist1[i]/C;


}


直方图的所有值加起来要等于100%，初中的孩子都知道哦。所以需要用C这样一个东西。不过作为一个小本，装着逼要说这叫直方图的归一化。





void init_target(double*hist1,double*m_wei,IplImage*current)
{//此函数无需考虑时间复杂度
int t_h,t_w,t_x,t_y;//目标矩形信息
double h,dist;
int i,j;
int q_r,q_g,q_b,q_temp;
t_h=drawing_box.height;//选区高
t_w=drawing_box.width;//选区宽
t_x=drawing_box.x;//选区左上角x坐标
t_y=drawing_box.y;//选区左上角y坐标
h=pow(((double)t_w)/2,2)+pow(((double)t_h)/2,2);//带宽,（不明白原来的注释为什么是带宽，感觉就是用来归一化的，最大的离中心点的距离）
//初始化权值矩阵和目标直方图
for(i=0;i<</span>t_w*t_h;i++)
{
m_wei[i]=0.0;
}
for(i=0;i<</span>4096;i++)//4098=16*16*16
{
hist1[i]=0.0;
}
for(i=0;i<</span>t_h;i++)
{
for(j=0;j<</span>t_w;j++)
{
dist=pow(i-(double)t_h/2,2)+pow(j-(double)t_w/2,2);//d=x^2+y^2
m_wei[i*t_w+j]=1-dist/h;
C+=m_wei[i*t_w+j];//统计权值矩阵的总和？
}
}
//计算目标权值直方
for(i=t_y;i<</span>t_y+t_h;i++)
{
for(j=t_x;j<</span>t_x+t_w;j++)
{
//rgb颜色空间量化为16*16*16bins
q_r=((u_char)current->imageData[i*current->widthStep+j*3+2])/16;//char*imageData;
q_g=((u_char)current->imageData[i*current->widthStep+j*3+1])/16;
q_b=((u_char)current->imageData[i*current->widthStep+j*3+0])/16;
q_temp=q_r*256+q_g*16+q_b;
hist1[q_temp]=hist1[q_temp]+m_wei[(i-t_y)*t_w+(j-t_x)];
}
}
//归一化直方图
for(i=0;i<</span>4096;i++)
{
hist1[i]=hist1[i]/C;
}
}


