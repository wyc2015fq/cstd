# matlab 绘制 阴影 柱状图 条形图 - fighting！！！ - CSDN博客
2019年02月26日 11:25:51[dujiahei](https://me.csdn.net/dujiahei)阅读数：112
转自：[https://blog.csdn.net/u013608300/article/details/80028109](https://blog.csdn.net/u013608300/article/details/80028109)
 在科技文献写作等过程中，我们往往需要粘贴柱状图或者条形图，图中以不同颜色对不同数据进行区分，然而当黑白打印时，色彩颜色往往难以区分，故而需要绘制带有黑白阴影的柱状图或者条形图。对比效果见图1。下面介绍具体实现方法，本程序主体由Ben Hinkle完成，我只是个中间搬运工。
![](https://img-blog.csdnimg.cn/2019022611254619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
顶层代码与分析如下，完整代码见链接：https://download.csdn.net/download/u013608300/10364534
https://pan.baidu.com/s/1NqzRupJMD4Dka57MJrxJng
%名称：main_top
%功能：绘制阴影柱状图、条形图
%注释：冰三点水
%备注：设置阴影格式'/', '\', '|', '-', '+', 'x', '.'
%data：2018-0421
clear;
close all;
data=[40 9;24 10; 12 3];
bar(data,1);
title('Hello world','FontName','Times New Roman','FontSize',15);   %图名
xlabel('Hey girl','fontsize',15,'FontName','Times New Roman');     %横坐标内容及字体
ylabel('Hey man','fontsize',15,'FontName','Times New Roman');      %纵坐标内容及字体
axis([0 4 0 50]);                                           %修改横坐标范围
legend('AA','BB','Square');                                 %修改图例
set(gca,'XTickLabel',{'Img1','Img2','Img3'},'FontSize',15,'FontName','Times New Roman');    %修改横坐标名称、字体
applyhatch(gcf,'\.x.');                                     %设置阴影格式'/', '\', '|', '-', '+', 'x', '.'
--------------------- 
作者：冰三点水 
来源：CSDN 
原文：https://blog.csdn.net/u013608300/article/details/80028109 
版权声明：本文为博主原创文章，转载请附上博文链接！
