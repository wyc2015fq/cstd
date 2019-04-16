# Matlab学习笔记：画图多重设置 - 去向前方的博客 - CSDN博客





2018年07月27日 10:08:16[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：173








# 前言

画图时遇到了一些问题，因此，上网搜索了一下，总结如下。

# matlab中坐标轴的刻度如何设置？

用matlab绘图函数plot,plot3等绘制的图形，其坐标轴的刻度可以通过手动方式修改，请问有没有什么函数可以实现坐标轴刻度的修改？

```
str='E:\cj\f'
x=[0:0.1:1];
xlab=[1:11];
for i=1:10
      plot([0 1],[0 1]);hold on;plot([0 1],[1 2]);
      set(gca,'xtick',x);
      set(gca,'xticklabel',xlab);
      hold off;
      saveas(gcf,[str num2str(i) '.bmp']);
end
```

补充一点，刻度不限制于数字。比如：

```bash
Day = {'Sun','Mon','Tue','Wed','Thu','Fri','Sat'}；
plot(1:7,1:7);
set(gca,'xticklabel',Day);
```

![这里写图片描述](https://img-blog.csdn.net/20180727095640433?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

EXAMPLE: Matlab中Tick(坐标轴刻度)的不同风格 
```
x=1:8;

subplot(2,2,1)
plot(x)
%tick style 0(auto)

subplot(2,2,2)
plot(x)
set(gca,'xtick',[1 3 6 8]);%style 1
set(gca,'ytick',[]);%style 2

subplot(2,2,3)
plot(x)
set(gca,'xtick',[1 3 6 8]);
set(gca,'xticklabel',sprintf('.4f|',get(gca,'xtick')));%style 3
set(gca,'ytick',[2 4 5 7]);
set(gca,'yticklabel',{'Two','Four','Five','Seven'});%style 4

subplot(2,2,4)
plot(x)
set(gca,'xminortick','on');%style 5
set(gca,'ticklength',[0.05 0.025]);%style 6
set(gca,'tickdir','out');%style 7

x=[1 1.53 4];
y=[1 2 3];
plot(x,y)
set(gca,'XTick',x)
set(gca,'XTickLabel',sprintf('%3.4f|',x))
set(gca,'YTick',y)
set(gca,'YTickLabel',sprintf('%+1.2f|',y))
```

![这里写图片描述](https://img-blog.csdn.net/20180727095935365?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
set(gca,'xtick'，[x起始坐标:间隔:x终点坐标]);   %设置x轴
set(gca,'ytick'，[y起始坐标:间隔:y终点坐标]);   %设置y轴
```

matlab非等长坐标

```bash
set(gca,'xtick',[0:10:100,140:40:400])
```

![这里写图片描述](https://img-blog.csdn.net/20180727100155880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 关于label

大家好，有个问题想向各位请教一下，在此先行谢过。

是这样的，当我在采用plot3等三维作图函数时，调用xlabel，ylabel，zlabel等作用后的结果是x轴和y轴的文字沿着水平方向，如果文字的方向能够设置得跟轴的方向一致的话就美观多了。

我目前采用的方法是:

```
close all;
figure;
plot3(10, 20, 30, 'Marker', 'o', 'MarkerSize', 20);
xlabel('中华人民共和国');
set(get(gca, 'XLabel'), 'Rotation', 15);
set(get(gca, 'XLabel'), 'Color', 'r');
ylabel('中华人民共和国');
set(get(gca, 'YLabel'), 'Rotation', -23);
set(get(gca, 'YLabel'), 'Color', 'g');
zlabel('中华人民共和国');
set(get(gca, 'ZLabel'), 'Color', 'b');
```

其中旋转的角度15和-23是我凭感觉试出来的，感觉很笨拙，不知道有没有什么更好的办法，能够一步准确的设置到位？希望能够得到热心朋友的帮助，万分感谢。

# 关于竖向或倾斜文本

plot(0:pi/20:2*pi,sin(0:pi/20:2*pi)) 

text(pi,0,’ \leftarrow sin(\pi)’,’FontSize’,18,’rotation’,90)

![这里写图片描述](https://img-blog.csdn.net/2018072710043122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

刚在一个matlab牛人的博客上看到一个axes函数中用’position’参数问题， 

大概是在一个图中，不同位置画一些图形

```matlab
EXAMPLE：
x = 0 : 0.01 : 2 * pi;
y1 = sin(x); y2 = cos(x); y3 = sin(x) + cos(x);
plot(x, y1)
axes('position', [0.55 0.65 0.3 0.2])
plot(x, y2)
xlabel('y2 = cos(x)');
axes('position', [0.15 0.15 0.3 0.2])
plot(x, y3)
xlabel(' y3 = sin(x) + cos(x)');
```

![这里写图片描述](https://img-blog.csdn.net/20180727100457282?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
set(gca,'Position',[left bottom width height]
set(gca,'Position',[0.15 0.15 0.75 0.75])       %设置图像的位置
```

```
position设置文字的位置

EXAMPLE：

x=0:0.01:2*pi;

y=sin(x);

plot(x,y);

xlabel('XLABEL','position',[2 -1.15 1]);

ylabel('YLABEL','position',[-0.5 0 1]);

title('TITLE:Y=SIN(X)','position',[5 0.6 1],'rotation',45);
```

![这里写图片描述](https://img-blog.csdn.net/20180727100512234?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以scrsz=get(0,’ScreenSize’)检查LZ荧幕的pixel数

h0=figure(‘toolbar’,’none’,    ‘position’,[198 56 350 300],    ‘name’,’实例01’); 

这个position具体的范围是多少　 

198 56 350 300这几个数字是什么意思 

h1=axes(‘parent’,h0,   ‘visible’,’off’); 

这句有什么作用 

请各位朋友帮忙指点一下！！ 

答： 

[物体（此为figure）距离参照物（此为屏幕）左下角的x轴距离，y轴距离，物体的宽度，物体的高度] 

h1=axes(‘parent’,h0,   ‘visible’,’off’); 

应该是在h0中创建一个axes对象，且设置其visible属性为off 

自定义设置坐标轴内容
请教各位大虾：

matlab 画出柱状图以后，想用字符串表示横坐标，对应那些柱，怎么做？

比如1,2,3,4,5想改成B,A,BA,BCD,CD 

自动生成的全是数字，改了半天也没改好，郁闷

找到命令set(gca,’xtick’,[ ])可以把x轴刻度清空,但怎么在原来柱状图显示1 2 3 4 5 的地方显示上 AB CD EF GH IJ 呢????

```
答：set(gca,'xtick',['AB','CD']);
EXAMPLE:

x=20:10:20000;
y=rand(size(x));
semilogx(x,y);
set(gca,'XLim',[20 20000]);
set(gca,'XMinorTick','off');
set(gca,'XTick',[20 31.5 63 125 250 500 1000 2000 4000 8000 16000]);
set(gca,'XGrid','on');
set(gca,'XMinorGrid','off');

ezplot('x^3+x^2+x+5',[1,100])
set(gca,'xtick',1:100,'xticklabel',mod(1:10,10)) %xtick就是你要在哪些地方显示刻度，xticklabel就是显示什么
```

# 文字格式设置

```
figure_FontSize=12;
set(get(gca,'XLabel'),'FontSize',figure_FontSize,'Vertical','top');
set(get(gca,'YLabel'),'FontSize',figure_FontSize,'Vertical','middle');
set(findobj('FontSize',12),'FontSize',figure_FontSize);

%这4句是将字体大小改为8号字，在小图里很清晰

%set(gcf,'Position',[100 100 260 220]);
%这句是设置绘图的大小，不需要到word里再调整大小。我给的参数，图的大小是7cm

%set(gca,'Position',[.13 .17 .80 .74]);
%这句是设置xy轴在图片中占的比例，可能需要自己微调。

%set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);

%这句是将线宽改为2

set(gca, 'Fontname', 'Times newman', 'Fontsize', 12);

%设置图片的字体类型和字号大小的。
```





