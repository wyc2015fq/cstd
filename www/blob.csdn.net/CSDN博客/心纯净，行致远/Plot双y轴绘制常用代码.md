# Plot双y轴绘制常用代码 - 心纯净，行致远 - CSDN博客





2019年02月27日 10:28:12[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：190








代码：

```
%绘制双y图;
x=0:0.001:5;
figure('color',[1 1 1]);    %将背景设置为白色
[hAx,hLine1,hLine2]=plotyy(x,SRFlateral,x,Dy); %这里的SRFlateral,Dy是从simulink导出到workspace的变量
title('The numerical changes of SRFlateral and Dy','Fontsize',30,'Fontname', 'Times New Rome')
xlabel('Time unit: s','Fontsize', 30,'Fontname', 'Times New Rome');
ylabel(hAx(1),'SRFlateral','Fontsize',30, 'Fontname', 'Times New Rome');
ylabel(hAx(2),'Dy unit:m','Fontsize',30,'Fontname', 'Times New Rome');
box off;
set(hLine1,'linewidth',2);%设置第一条线的线宽
set(hLine2,'linewidth',2);
set(hAx,'FontName','Times New Roman')%设置x轴、左y轴、右y轴、标题的刻度字号和字型
```

绘制的图形如下：

![](https://img-blog.csdnimg.cn/20190227102423896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

其中，代码的最后一行
`set(hAx,'FontName','Times New Roman')%设置x轴、左y轴、右y轴刻度字号和字型`
可以设置所有的字号大小，如果写成
`set(hAx,'frontsize',20,'FontName','Times New Roman')%设置x轴、左y轴、右y轴刻度字号和字型`
则所有的字号大小都会变成20.

### 常用的绘制函数的用法如下

> 
set(get(gca,'title'),'FontSize',10,'FontName','宋体');%设置标题字体大小，字型

set(get(gca,'XLabel'),'FontSize',10,'FontName','Times New Roman');%设置X坐标标题字体大小，字型

set(get(gca,'YLabel'),'FontSize',10,'FontName','Times New Roman');%设置Y坐标标题字体大小，字型

set(gca,'FontName','Times New Roman','FontSize',10)%设置坐标轴字体大小，字型

text(0.3,1.2,'(a)','FontSize',10,'FontName','Times New Roman'); %设置文本字型字号

set(gca, 'XTick', [0 10 20 30 40 50 60 70 80 90]) %设置X坐标轴刻度数据点位置

set(gca,'XTickLabel',{'0','10','20','30','40','50','60','70','80','90'}) %设置X坐标轴刻度处显示的字符

set(gca, 'YTick', [-15 -10 -5 0 5 10 15]) %设置X坐标轴刻度数据点位置

set(gca,'YTickLabel',{'-15','-10','-5','0','5','10','15'}) %设置Y坐标轴刻度处显示的字符

axis([0,90,-20,20])

set(gca,'YTickLabel',[]); %只显示y坐标轴刻度，不显示y坐标轴的值；

set(gca,'XTickLabel',[]); %只显示x坐标轴刻度，不显示x坐标轴的值；

set(gca,'ytick',[]); %y轴的坐标值和刻度均不显示；

set(gca,'xtick',[]); %x轴的坐标值和刻度均不显示；

figure;

set (gcf,'Position',[400,300,600,200]); %设定plot输出图片的尺寸。参数含义为：xmin,ymin,width,height

%plot的默认参数为[232,246,560,420],Position的单位可以用units属性制定，units属性的值可以是下列字符串中的任何一

%种：pixel（像素，缺省值）、normalized（相对单位）、inches（英寸）、centimeters（厘米）、points（磅）。

%指定大小后，在figure中用text输出的文字大小，设置的是多大，在WORD中显示的就是多大。

set (gcf,'units','centimeters','Position',[4,3,6,2]); %指定fiugre的尺寸为6cm宽，2cm高。

%也可以直接用下属语句：

fiure('units','centimeters','Position',[4,3,6,2]);

%改变ylabel离坐标轴的距离

hc=findobj(allchild(gcf), 'Type', 'axes');

hc2=findobj(allchild(hc), 'Type', 'text');

set(hc2(3), 'Position', [0 0 0]);

%改变xlabel离坐标轴的距离

hc=findobj(allchild(gcf), 'Type', 'axes');

hc2=findobj(allchild(hc), 'Type', 'text');

set(hc2(4), 'Position', [0 0 0]);

%双y轴坐标图形调整

[AX,H1,H2]=plotyy(f*1e-12,abs(T1.*T1),f*1e-12,angle(T1.*T1));

set(AX,'FontSize',14,'FontName','Times New Roman')%设置x轴、左y轴、右y轴刻度字号和字型

set(AX(1),'Xcolor','k','Ycolor','k')%设置x轴、左y轴刻度字体为黑色；

set(AX(2),'Xcolor','k','Ycolor','k')%设置x轴、右y轴刻度字体为黑色；

set(AX,'Xlim',[207.5 217.1],'xtick',[208:1:217])%设置x轴数据范围（207.5到217.1），刻度显示（208，209,210...217）

set(AX(1),'ylim',[0,0.5],'ytick',[0:0.1:0.5])%设置左y轴数据范围（0到0.5），刻度显示（0,0.1,0.2...0.5）

set(AX(2),'ylim',[0,3],'ytick',[0:1:3])%设置右y轴数据范围（0到3），刻度显示（0,1,2,3）

set(H1,'Linestyle','-','color','r','Linewidth',2.5);%设置第一条曲线的线型、颜色、粗细

line(x,y,'linestyle','-','color','r','Linewidth',2.5,'parent',AX(1));%在第一条曲线上再画一条曲线。

set(H2,'Linestyle','-.','color','b','Linewidth',2.5);%设置第二条曲线的线型、颜色、粗细

set(get(AX(1),'Ylabel'),'string','Amplitude(a.u.)','FontSize',10,'FontName','Times New Roman','Position',[0 0 0 ]);%设置左y轴标题字号字型

set(get(AX(2),'Ylabel'),'string','Phase(rad)','FontSize',10,'FontName','Times New Roman');%设置右y轴标题字号字型

set(get(AX(1),'Xlabel'),'string','Opticalfrequency(THz)','FontSize',10,'FontName','Times New Roman');%设置x坐标标题字体大小，字型




一、线型设置：

t=0:.1:8;

[ax,[h1](https://www.baidu.com/s?wd=h1&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd),h2]=plotyy(t,sin(t),t,cos(t)); % plotyy（X1,Y1,X2,Y2）：以左、右不同纵轴绘制X1-Y1、X2-Y2两条曲线。

set(h1,'linestyle','-','marker','o','color','r');

set(h2,'linestyle',':','marker','x','color','b');

![](https://img-blog.csdn.net/20141221200511187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDU1NTY4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://blog.csdn.net/u010555688/article/details/37723559)

二、加注图例：



x=linspace(0,2*pi,40);

[ax,h1,h2]=plotyy(x,sin(x)+cos(x),x,exp(x));

set(h1,'linestyle','-')

set(h2,'linestyle','-')

set(h1,'marker','o')

set(h2,'marker','+')

hold on

x=linspace(0,2*pi,40);

hh=line(x,cos(x));

set(hh,'linestyle','-')

set(hh,'marker','s')

hold on

hhf=line(x,sin(x));

set(hhf,'color','r')

set(hhf,'linestyle','-')

set(hhf,'marker','*')

legend([h1,h2,hh,hhf],'sin(x)+cos(x)','exp(x)','cos(x)','sin(x)',0); %加注多条线的图例

![](https://img-blog.csdn.net/20141221200534374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDU1NTY4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

用法：linspace(x1,x2,N)　　

功能：linspace是Matlab中的一个指令，用于产生x1,x2之间的N点行矢量。其中x1、x2、N分别为起始值、中止值、元素个数。若缺省N，默认点数为100。



三、坐标轴标注：

figure;

t=0:.1:3*pi;

[H,Ha,Hb]=plotyy(t,sin(t),t,exp(t));

d1=get(H(1),'ylabel');

set(d1,'string','yayacpf');

d2=get(H(2),'ylabel');

set(d2,'string','bbs from hit','fontsize',18);

![](https://img-blog.csdn.net/20141221200539484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDU1NTY4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

四、双坐标轴

X=[10,30,50,70,100,150,200];

Y1=[0.0501,0.1847,0.1663,0.235,0.2724,0.3491,0.3856];

Y2=[0.0239,0.0545,0.1165,0.1003,0.1413,0.2381,0.2433];

[AX,H1,H2]=plotyy(X,Y1,X,Y2,'plot','plot'); %双y轴图像

xlabel('User ID');

set(get(AX(1),'ylabel'),'string','Average Trust Value');  %对坐标标注

set(get(AX(2),'ylabel'),'string','Normalized NTR, NBTR, NREJ');

set(AX(1),'ytick',[0:0.1:1]);               %控制左边的y轴的刻度标注

set(AX(2),'ytick',[0:0.1:1]);  

set(H1,'marker','*');

set(H2,'marker','o');

set(H2,'LineStyle','none')

legend('Average Trust Value','NTR');

![](https://img-blog.csdn.net/20140712161515604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDU1NTY4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



