# matlab figure属性设置 - KinboSong的博客 - CSDN博客
2018年03月29日 08:22:48[songjinbo3](https://me.csdn.net/KinboSong)阅读数：2686
figure属性设置： 
（1）设置坐标轴的范围： 
axis（[xmin xmax ymin ymax]）; 
（2）设置坐标轴的间距： 
set(gca, ‘XTick’, [xmin：间距：xmax])； 
set(gca, ‘YTick’, [ymin：间距：ymax])； 
（2）设置有格子： 
grid on 
（3）如果想取消x或者y轴的格子，可以设置： 
set(gca, ‘Xgrid’, ‘off’)默认值为on，设置关掉x轴上的格子 
（4）去掉坐标的边框： 
box off 
（5）设置坐标轴的名字： 
xlabel(‘xname’); 
（6）设置图例： 
legend(‘lenname1’,’lenname2’); 
（7）设置图例的位置： 
在figure图上放好图例，然后右键生成代码即可。 
（8）设置figure的名字： 
figure(‘name’,”) 
（9）在一个figure画不同的plot图： 
使用方法：subplot（m,n,p）或者subplot（m n p）。 
subplot是将多个图画到一个平面上的工具。其中，m表示是图排成m行，n表示图排成n列，也就是整个figure中有n个图是排成一行的，一共m行，如果m=2就是表示2行图。p表示图所在的位置，p=1表示从左到右从上到下的第一个位置。 
eg： 
把绘图窗口分成两行两列四块区域，然后在每个区域分别作图，基本步骤： 
subplot(2,2,1); % 2、2、1之间没有空格也可以 
在第一块绘图 
subplot(2,2,2); 
在第二块绘图 
subplot(2,2,3); 
在第三块绘图 
subplot(2,2,4); 
在第四块绘图 
（10）一个图中画多条曲线 
plot(x1,y1,”,”,x2,y2,”,”) ”表示分别设置属性
