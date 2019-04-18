# Matlab设置字体大小 - fighting！！！ - CSDN博客
2018年09月08日 09:52:02[dujiahei](https://me.csdn.net/dujiahei)阅读数：1384
转自：[https://www.cnblogs.com/hxsyl/p/5366941.html](https://www.cnblogs.com/hxsyl/p/5366941.html)
1.  设置坐标轴上下限：axis([xmin,xmax,ymin,ymax]);
2.  设置图片大小：set(gcf,'Position',[x1,y1,dx,dy]);
     x1和y1是图的左下角坐标，
     dx和dy是图沿着x和y方向的大小，
     比如：set(gcf,'Position',[347,162,380,300]);
3.  图例的相关设置：
     图例中字体及大小：legend('FontName','Times New Roman','FontSize',7,LineWidth',1.5);
     图例中各个量及位置：legend('y','zc','location','SouthEast');
4.  坐标轴的名称的相关设置：
     x轴的名称及字体和大小：xlabel('x(m)','FontName','Times New Roman','FontSize',7);
     y轴的名称及字体和大小：ylabel('zc(m) and y(m)','FontName','Times New Roman','FontSize',7,LineWidth',1.5);
5.  坐标轴的字体及大小设置
     set(gca,'FontName','Times New Roman','FontSize',7,'LineWidth',1.5);
6.  图名的字体及大小设置
     title('yslice:600m;zslice:150m','FontName','Times New Roman','FontSize',7);
7.  坐标轴反向
    set(gca,'zdir','reverse'); %将坐标轴数值反向
　　用matlab绘图如何定义坐标轴上的数字字体大小？
　　方法一：在画完图的Figure窗口里，通过菜单Edit-Axes Properties....-Font-Font Size即可设置。
　　　　=================================================================
　　方法二：用plot命令画完图后，用如下命令：set(gca,'FontSize',20);即可完成菜单同样效果。
　　总结一句话，如果不懂，就在里面设置FontSize属性。
