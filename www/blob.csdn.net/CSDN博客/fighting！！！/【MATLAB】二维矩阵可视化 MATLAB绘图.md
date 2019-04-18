# 【MATLAB】二维矩阵可视化 MATLAB绘图 - fighting！！！ - CSDN博客
2018年10月17日 08:47:35[dujiahei](https://me.csdn.net/dujiahei)阅读数：310
转自：[https://blog.csdn.net/CSDN___CSDN/article/details/82110579?utm_source=blogxgwz9](https://blog.csdn.net/CSDN___CSDN/article/details/82110579?utm_source=blogxgwz9)
基本绘图函数
1、plot(y)
如果y是向量，则采用向量的索引值作为横坐标值，以向量元素的值作为纵坐标值。
如果y是实数矩阵，则相当于对y的每一列进行二维绘图。
如果y是复数组成的向量，则相当于plot(real(y),imag(y))。
2、plot(x,y)
当x和y都是矢量，如果其长度相等，则以x为横坐标，y为纵坐标。
当x为矢量，y是有一维与x同维的矩阵时，则以x为共同的横坐标绘制多条不同颜色的曲线，曲线的条数由y的另一维决定。
当x，y是同维的矩阵时，则以x和y对应的列元素为横纵坐标分别绘制曲线，曲线条数等于矩阵的列数。
clear all;
x=1:7;
y=magic(7);
plot(x,y);
clear all;
x=1:7;
y=magic(7);
plot(x,y)
 3、plot(x,y,s)
s是对二维绘图的线型和颜色等进行设置。
二维绘图常用设置
线型    说明    标记符    说明    颜色    说明
-    实线（默认）    +    加号符    r    红色
--    虚线    o    空心圆    g    绿色
：    点线    *    星号    b    蓝色
-.    点划线    .    实心圆    c    青绿色
          x    叉号符    m    洋红色
          s    正方形    y    黄色
          d    菱形    k    黑色
          ^    上三角形    w    白色
v
下三角形          
          >    右三角形          
          <    左三角形          
          p    正五边形          
          h    六边形          
clear all;
x=0.01:0.3:2*pi;
y=cos(x+0.5)+2;
subplot(2,2,1);
plot(x,y,'k');
subplot(2,2,2);
plot(x,y,'k.');
subplot(2,2,3);
plot(x,y,'k-^');%表示用黑色的实线（默认）连接各点，各点采用上三角标记。
subplot(2,2,4);
plot(x,y,'k^');
函数plot()绘图的PropertyName
PropertyName    说明    取值
LineWidth    线宽    数字，单位是points
MarkerEdgeColor    标记点边框的颜色    颜色字符
MarkerFaceColor    标记点内部填充颜色    颜色字符
MarkerSize    标记点大小    数字，单位是points
%如左下图
clear all;
x=-pi:pi/10:pi;
y=tan(sin(x))-sin(tan(x));
plot(x,y,'--rs','LineWidth',2,'MarkerEdgeColor','k','MarkerFaceColor','g','MarkerSize',10);
4、plot(x1,y1,s1,x2,y2,s2,...)
%如右上图
clear all;
x=-pi:pi/10:pi;
y=sin(x);
z=cos(x);
figure;
plot(x,y,'r:',x,z,'k-');
 子图绘制
subplot进行子图绘制。
subplot(m,n,p)：该函数将当前窗口分成m x n个绘图区。子绘图区的编号按行优先从左到右编号。
clear all;
x=-pi:pi/10:pi;
figure;
subplot(221);
plot(x,sin(x),'r--');
subplot(223);
plot(x,cos(x),'b:*');
subplot(2,2,[2,4]);
plot(x,sin(x)+cos(x),'b-.^');
叠加图绘制
hold函数启动或关闭图形保持功能。
hold on：启动图形保持功能，允许在同一坐标轴上绘制多个图
hold off：关闭图形保持功能。
hold：在hold on 和 hold off之间切换。
设置坐标轴
axis([xmin xmax ymin ymax])
axis([xmin xmax ymin ymax zmin zmax])
axis off ：取消坐标轴的一切设置
axis on ：恢复坐标轴的一切设置
网格线和边框
通过函数gird()给坐标轴加网格线
grid on ：给当前坐标轴添加网格线
grid off ：取消当前坐标轴的网格线
grid ：在grid on 和 grid off 之间切换
clear all;
x=-pi:pi/10:pi;
figure;
plot(x,sin(x)+cos(x),'b-.^');
grid on
box()函数给坐标轴加边框
box on ：该函数给当前坐标轴添加框线。
box off ：该函数取消当前坐标轴的边框线。
box ：不带参数时，在box on 和box off 之间切换。
%见右上图
clear all;
x=-pi:pi/10:pi;
figure;
plot(x,sin(x)+cos(x),'b-.^');
box off
数据光标
datacursormode()显示鼠标所选的图形上某点的坐标值。
datacursormode on ：该函数打开图形的数据光标功能。
datacursormode off ： 该函数关闭图形的数据光标功能。
datacursormode：在datacursormode on 和 datacursormode off 之间转换。
%见左下图
clear all;
x=0.1:0.1:2*pi;
y=sin(x);
figure;
plot(x,y,'--r');
datacursormode on
极坐标绘图
polar(theta,rho)：弧度为theta，半径为rho
polar(theta,rho,s)：参数s对曲线的线型、标记和颜色等信息进行设置。
%见右上图
clear all;
x=0:0.02:2*pi;
y=sin(2*x).*cos(2*x);
figure;
polar(x,y,'--r');
[x,y]=pol2cart(theta,rho)：将极坐标转换为直角坐标
[x,y,z]=pol2cart(theta,rho,Z)
[theta,rho]：cart2pol(x,y)：将直角坐标转换为极坐标
[theta,rho,Z]：cart2pol(x,y,z)
clear all;
theta=0:pi/40:4*pi;
rho=sin(theta);
figure;
subplot(211);
polar(theta,rho);
[x,y]=pol2cart(theta,rho);
subplot(212);
plot(x,y);
axis equal
对数和半对数坐标系绘图
semilogx()：x轴采用对数坐标，y轴采用等比例刻度坐标。
semilogy()：y轴采用对数坐标，x轴采用等比例刻度坐标。
loglog()：x，y 轴采用对数坐标。
双纵轴绘图
plotyy(x1,y1,x2,y2)：(x1,y1)采用左侧y轴，(x2,y2)采用右侧y轴
plotyy(x1,y1,x2,y2,'function')：function可以是"plot","semilogx","semilogy","loglog"
plotyy(x1,y1,x2,y2,'function1','function2')：function1绘制(x1,y1)，function2绘制(x2,y2)
clear all;
x=0.1:0.1:2*pi;
y=sin(x);
z=10.^x;
figure;
plotyy(x,y,x,z,'plot','semilogy');
--------------------- 
作者：CSDN___CSDN 
来源：CSDN 
原文：https://blog.csdn.net/CSDN___CSDN/article/details/82110579?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
