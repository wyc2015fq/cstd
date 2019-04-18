# Matlab图形窗口大小的控制，plot窗口大小，figure大小，axis设置，实用 - fighting！！！ - CSDN博客
2019年02月22日 11:00:29[dujiahei](https://me.csdn.net/dujiahei)阅读数：74
 转载自：http://blog.sina.com.cn/s/blog_48c735630101iadu.html
## Matlab中保存图像时，图形窗口大小的控制zz
首先要了解的是Matlab是面向对象的。最高等级的对象是screen，它定义了figure可以用的最大szie。
screen下面是figure。figue就是你画图的时候跳出来的那个新的对话窗口。如果figure变化，screen是不会跟着变化的。但screen变化的话，figure就要跟着变化了。
figure下面是axes。axes是那个窗口里面你要画的东西。axes的大小和位置取决于figure，如果你放大缩小figure的大小的话，里面的图线也会跟着变化的。
**set(gca,'position',[])**
因此，set (gca,'position',[0.1,0.1,0.9,0.9] );的作用是：
设置坐标轴距离画板（图形窗口figure）边距。
[0.1,0.1,0.9,0.9]分别为axes在figure中的左边界，下边界，宽度，高度，最小为0，最大为1（左边界，下边界为0，上边界，右边界为1）
见下面的例子：
-----------------------------------------------------------------------------
figure
 set (gca,'position',[0.1,0.1,0.9,0.9] );
 x=1:0.1:10; 
y=sin(x);
plot(x,y)
-----------------------------------------------------------------------------
**set(gcf,'position',[])**
一般matlab绘出来图的框架（图形窗口）大都是正方形或者近似正方形的矩形，能不能画一些扁的矩形呢？
使用图形的position属性可以做到。
如set(gcf,'unit','normalized','position',[0.2,0.2,0.64,0.32]);的意思是：
对gcf的position进行设置。使其在屏幕上的显示位置是以（0.2，0.2）为原点，长0.64，宽0.32。同gca一样，仍然是左边界，下边界为0，
上边界，右边界为1。
另外，gcf的position也可以不是normalized的。如下面的例子：
-----------------------------------------------------------------------------
x=-2*pi:0.1:2*9i;y=sin(x);figure;set(gcf,'Position',[500,500,500,500], 'color','w') %大小设置plot(x,y,'k-')%节点位移图形输出xlim([min(s(:,2)) max(s(:,2))])grid on
-----------------------------------------------------------------------------
其中，
[500,500,500,500]的意思为：原点的位置x，原点的位置y，宽，高，其坐标为points（详见下面），
现在问题还存在：
如果仅设置position的话，打印的时候还是正方形。可以用下面的方法解决：
通常默认情况下，print命令输出图像为 8*5inches，无视屏幕显示尺寸
**通过命令行修改的话有三步**
1 设置paperposition为manual
set(gcf,'PaperPositionMode', 'manual')
[ auto | {manual} ]
2 设置paperunit   
set(gcf,'PaperUnits','inches')
[ {inches} | centimeters | normalized | points ]
3 设置paperposition
set(gcf,'PaperPosition',[left,bottom,width,height])
例如
set(gcf, 'PaperPositionMode', 'manual');
set(gcf, 'PaperUnits', 'points');
set(gcf, 'PaperPosition', [0 0 640 480]);
还有一个相关命令是**papersize**
paperposition是placement，代表图像在paper（感觉就是屏幕screen的意思？）中的所处位置。left和bottom计算好，就可以使图像在paper中居中
papersize是纸张大小;position要比size小的
PaperPosition
    four-elementrect vector
    Location onprinted page. A rectangle that determines the location of thefigure on the printed page. Specify this rectangle with a vector ofthe form
    rect = [left, bottom, width, height]
    where leftspecifies the distance from the left side of the paper to the leftside of the rectangle and bottom specifies the distance from thebottom of the page to the bottom of the rectangle. Together thesedistances define the lower-left corner of the rectangle. width andheight define the dimensions of the rectangle. The PaperUnitsproperty specifies the units used to define this rectangle.
要使图像比例输出与屏幕显示的一致，可以使用如下命令
屏幕显示图像尺寸可以plot时用 set(gcf,'position',[left bottom width height])调整，或者print之前拖动窗口手动调整
This example exports a figure at screen size to a 24-bit TIFF file,myfigure.tif.
% set(gcf,'position',[80 100 800 600])  %如果手动拖放，则不需要这一行命令
set(gcf, 'PaperPositionMode','auto')   % Use screen size
print -dtiff myfigure
用matlab画了一张图，投稿时要缩小，缩小后字体就会过小或者发虚。我摸索出比较好的方法是如下的代码：%%%%%%%%%%%%%%%%%%%%%%plotyour figure before%%%%%%%%%%%%%%%%%%%%%% figure resize
set(gcf,'Position',[100 100 260 220]);
set(gca,'Position',[.13 .17 .80 .74]);
figure_FontSize=8;
set(get(gca,'XLabel'),'FontSize',figure_FontSize,'Vertical','top');
set(get(gca,'YLabel'),'FontSize',figure_FontSize,'Vertical','middle');
set(findobj('FontSize',10),'FontSize',figure_FontSize);
set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);%%%%%%%%%%%%%%%%%%%%%%%%%%%%解释：set(gcf,'Position',[100100 260 220]);
这句是设置绘图的大小，不需要到word里再调整大小。我给的参数，图的大小是7cmset(gca,'Position',[.13 .17.80 .74]);
这句是设置xy轴在图片中占的比例，可能需要自己微调。figure_FontSize=8;
set(get(gca,'XLabel'),'FontSize',figure_FontSize,'Vertical','top');
set(get(gca,'YLabel'),'FontSize',figure_FontSize,'Vertical','middle');
set(findobj('FontSize',10),'FontSize',figure_FontSize);这4句是将字体大小改为8号字，在小图里很清晰set(findobj(get(gca,'Children'),'LineWidth',0.5),'LineWidth',2);
这句是将线宽改为2
