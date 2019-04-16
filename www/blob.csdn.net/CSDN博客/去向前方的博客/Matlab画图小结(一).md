# Matlab画图小结(一) - 去向前方的博客 - CSDN博客





2018年09月18日 20:22:08[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：1490








#前言

Matlab作为常用的科学计算软件，其自带的画图足以支持我们日常的画图所学。

##图中子图作法

第一种：magnify是个动态放大镜，固化后可以用tools>edit plot移动小图，能选取多个局部图，这个方法不错
用法：打开figure图，输入magnify，左键动态选取查看，ctrl+左键固化，也可右键固化，‘<’和‘>’缩放方法范围，‘+’和‘-’缩放放大比例。

```
Ever wish MATLAB had a magnifying glass so you could look at the details of a small region of your plots without having to zoom in and out again and again? Just run 'magnify.m' with the figure of interest as the current figure, then use the left mouse button to bring up a magnified veiw that you control. Or use the 'Ctrl' key while clicking to bring up a magnifying glass that 'locks' onto the figure when released (for use when copying and printing figures). Use the '<' and '>' keys to make the magnifying glass smaller or larger. Use '-' and '+' to decrease or increase the magnification level.
```

网上一般有三种方法在MATLAB中做图中图，但是我感觉使用magnify是magnify是个动态放大镜，固化后可以用tools>edit plot移动小图，能选取多个局部图。下面详细讲解其使用方法：

1、下载magnify，官网下载地址http://www.mathworks.com/matlabcentral/fileexchange/5961-magnify

2、下载后将文件放到当前figure目录下（也就是当前工作区间中）。

3、打开figure图，输入magnify，然后右键选中想要放大的区域（按着右键不要动），然后可以使用‘<’和‘>’缩放方法范围，‘+’和‘-’缩放放大比例，看着放大的小图满意后松开右键即可。（这一过程网上都管他们叫固化）

4、然后用tools>edit plot就可以移动小图到你想放在的地方了。

```
f1=figure; 
         plot(1:100,randn(1,100),(1:300)/3,rand(1,300)), grid on, 
         magnify(f1) 
         % You must complete the amplification in the figure before you run the next code.Assuming we just magnify once. 
         d=get(f1,'Children'); 
         d(1).Position=[0 0.5 d(1).Position(3) d(1).Position(4)];%Move the magnified figure to the specific position 
         axis = d(2); 
         rectangle('Position',[d(1).XLim(1),d(1).YLim(1),d(1).XLim(2)-d(1).XLim(1),d(1).YLim(2)-d(1).YLim(1)],'LineWidth',2,'LineStyle','--','EdgeColor','r')%Mark the magnified part in the original figure.

http://cn.mathworks.com/matlabcentral/fileexchange/5961-magnify
```

第二种：用起来也很方便，缺点是只能框选一处，不能选取多个。

美国学者Duane HanselmanandBruce Littlefield编写的MasteringMATLAB7上的例子，实现图中图缩放功能，使用了3个函数，这三个函数在附件中，当然也可以到网站http://www.eece.maine.edu/mm/去下载。

函数简单介绍：

getn()将get()函数的输出参数简化为单个变量；

getbox()实现矩形区域的选择，并捕捉该区域的横纵标的范围

mmzoom创建一个缩放坐标轴。

试图做了例子，传上来一起分享。

例子：
```
x = -pi:pi/12:pi;
      y = tan(sin(x)) - sin(tan(x));
      plot(x,y,'-- ro','LineWidth',2,'MarkerEdgeColor','k','MarkerFaceColor','g',
      'MarkerSize',7.5)
      mmzoom
```

第三种：也可编个小程序，分别在两个图形句柄里画图，可以借鉴下.

```
代码;
    figure(1);
    h2=axes('position',[0 0 1 1]);
    axis(h2);
    x2=0:pi/50:2*pi;
    y2=sin(x2);
    h3=plot(x2,y2,'b-');
    h1=axes('position',[0.3 0.2 0.4 0.4]);
    axis(h1);
    x1=0:pi/50:2*pi;
    y1=cos(x1);
    h4=plot(x1,y1,'r-');
    hold on
    h=[h3; h4];
    str=['大图中的曲线';'小图中的曲线'];
    legend(h,str);
```

![在这里插入图片描述](https://img-blog.csdn.net/20180918201349966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

看下来，还是倾向第三种使用axes方法放大局部图像的方法，关于axes的详细解释见
[http://5460521xp.blog.163.com/blog/static/6908756920122210253741/](http://5460521xp.blog.163.com/blog/static/6908756920122210253741/)

```
figure;% 生成新的图形窗口           
plot(t,y);axis('equal'); % 绘制整体曲线图    

axes('Position',[0.18,0.62,0.28,0.25]); % 生成子图   

plot(t1,y1); % 绘制局部曲线图                         
xlim([min(t1),max(t1)]); % 设置坐标轴范围
```

axes函数后的参数说明，以横纵坐标量程为基准：

例中0.18为子图左下角的归一化横坐标位置：

```
（该位置的横坐标-横坐标起始值）/横坐标量程=0.18，
```

同理，0.62为子图左下角的归一化纵坐标位置：

```
（该位置的纵坐标-纵坐标起始值）/纵坐标量程=0.62，
```

0.28为子图的归一化宽度，0.25为子图的归一化高度，具体计算过程同上

```
下面通过绘制 y=1/(t-3) 的曲线举例说明该函数的使用方法。
程序如下：   
```                                              
clc;clear;close all;                                                                                                               
t=linspace(0,6,300);% 生成曲线的整体离散坐标值t
t1=linspace(2.8,3.2,300); % 生成曲线的局部离散坐标值t1                                                                                                                                
y=sin(1./[t-3]);% 生成曲线的整体离散函数值y
y1=sin(1./[t1-3]); % 生成曲线的局部离散函数值y1                                                                                                                                  
figure;% 生成新的图形窗口            
plot(t,y);axis('equal'); % 绘制整体曲线图                                                                                            
axes('Position',[0.18,0.62,0.28,0.25]); % 生成子图                                                                           
plot(t1,y1); % 绘制局部曲线图                                                                                                                
xlim([min(t1),max(t1)]); % 设置坐标轴范围           
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180918201445708?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 途中标记

`  ``

a=2;

b=3;

t=0:0.01:10;

y=exp(-a*t).*sin(b*t);[y_max,i_max]=max(y);t_text=[‘t=’,num2str(t(i_max))];%生成最大值点的横坐标字符串y_text=[‘y=’,num2str(y_max)];%生成最大值的纵坐标字符串max_text=char(‘maxinum’,t_text,y_text);%生成标志最大值点的字符串tit=['y=exp(-2t)*sin(3*t)’];%生成标志图名的字符串plot(t,zeros(size(t)),‘k’);%画纵坐标为0的基准线hold on;plot(t,y,‘b’);%用蓝色画y(t)曲线plot(t(i_max),y_max,‘r.’,‘MarkerSize’,20),%用大红点标最大值点text(t(i_max)+0.3,y_max+0.05,max_text);title(tit),xlabel(‘t’),ylabel(‘y’),hold off;```![在这里插入图片描述](https://img-blog.csdn.net/20180918201756201?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)##细节设置```%横纵坐标的表示xlim([0 20])ylim([8900 9250])xlabel(’\itt\rm(s)’)%表示斜体和分离开ylabel(’\itn\rm_1(r/min)’)%横纵坐标的显示set(gca,‘XTickMode’,‘manual’,‘XTick’,[0:2:20]);set(gca,‘YTickMode’,‘manual’,‘YTick’,[8900:50:9250]);set(gca,‘Fontname’,‘Times New Roman’,‘FontSize’,6,‘LineWidth’,1);%坐标轴字体设置set(gcf,‘color’,‘white’);%坐标轴颜色设置box off  %有无外框Width=4.72;Height=3.55;%单位为厘米！！！这里根据需求更改。。。ScreenSize=14.1; % 屏幕大小，单位为英寸，且应该注意该值通常指对角线的长度，需根据勾股定理计算宽高ScreenSizeInCM=ScreenSize*2.45; %1英寸等于2.45厘米，长度换算scrsz = get(0,‘ScreenSize’);  %得到屏幕分辨率ScreenWidth=ScreenSizeInCM/sqrt(1+(scrsz(4)/scrsz(3))^2);%屏幕宽，单位为厘米ScreenHeight=ScreenWidthscrsz(4)/scrsz(3);%屏幕高，单位厘米WidthRatio=Width/ScreenWidth;%图形的期望宽度与屏幕宽度的比值HeightRatio=Height/ScreenHeight;%图形的期望高度与屏幕高度的比值set(gcf,‘Unit’,‘Normalized’,‘Position’,[0.1 0.1 WidthRatio HeightRatio]);%设置绘图的大小，无需再到word里再调整大小```![在这里插入图片描述](https://img-blog.csdn.net/20180918202009637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)```**






