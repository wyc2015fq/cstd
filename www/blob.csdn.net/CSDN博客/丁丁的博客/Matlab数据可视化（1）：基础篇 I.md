
# Matlab数据可视化（1）：基础篇 I - 丁丁的博客 - CSDN博客


2013年05月21日 05:32:16[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：6873


本文在《[MATLAB Graphics and Data Visualization Cookbook](http://it-ebooks.info/book/1577/)》一书的基础上，介绍Matlab数据可视化方法。 我们假设读者已经具备Matlab的基本知识（了解Matlab的语法，能够使用Matlab进行数据的读取、运算等操作），并能熟练地查阅相关的Matlab函数。未经特殊说明，所有图像都在Matlab
 2013a 环境下绘制（源码[下载](https://code.google.com/p/dk-matlab-visualization/)）。
## 一. 绘制散列点和直线：
（源代码basic.m）
1、首先，生成一组增加了高斯噪声的模拟数据

```python
% 生成模拟数据
time = 1:2:60;
temperature = time*2+randn(1,30)*3+20;
```
2、然后，绘制出这些数据的散点图（图1）

```python
% 创建绘图窗口
figure;
%绘制散点图
plot(time,temperature,'.');
%散点图可以直接用scatter函数成生，如下：
%scatter(time,temperature,'.');
% 增加坐标说明和图像标题
xlabel('时间');ylabel('温度');title('散点图');
```
![](https://img-blog.csdn.net/20130520144504209)
图1
3、用生成的模拟数据作线性拟合，在原图上绘制出拟合后的直线（图2）

```python
% 线性拟合
p =  polyfit(time,temperature,1);
y = polyval(p,time);
% 在同一个窗口绘图
hold on;
plot(time,y,'r--');
title('散点图+拟合曲线');
% 添加图例
legend({'数据','拟合'},'Location','NorthWest');
```
![](https://img-blog.csdn.net/20130520144712111)
图2
4、最后，将拟合直线连续地画出（图3）

```python
set(findobj(gca,'color',[1,0,0]),'Linestyle','-','Linewidth',1.5);
title('散点图+拟合直线（连续）');
```
![](https://img-blog.csdn.net/20130520145008027)
图3

## 二. 绘制误差条（error bar）
（源代码errorbars.m）（注：绘图所用原始数据（14cancer.xtrain和14cancer.ytrain）可以从这里[下载](http://www-stat.stanford.edu/~tibs/ElemStatLearn/data.html)。数据包括14种癌症的198个样本的对16,062组基因表达水平的测量值，每列一个样本，共16,062行，198列）
1、读取第1111号基因的表达水平，并用直线+误差条的形式用图表表达，并添加标注（图4）

```python
% 读取数据
load 14cancer.mat

% 显示第1111基因表达水平的bar、error bar及相关标注
% 计算每种癌症的均值和方差
expressionLevel = [xtrain(1111,:) xtest(1111,:)];
cancerTypes = [ytrain,ytest];
for i = 1:14
    indexes = expressionLevel(cancerTypes == i);
    meanExpressionLevel(i) = median(indexes);
    stdExpressionLevel(i) = 3*std(indexes);
end
errorbar(1:14,meanExpressionLevel,stdExpressionLevel,stdExpressionLevel);
% 添加标注
ylabel('1111号基因表达水平');
xlabel('癌症类型');
title({'直线代表中位水平','条型代表中值上下3倍\sigma范围','#1111基因在14种癌症的198个样本中的表达水平'},'Color',[1,0,0]);
set(gca,'Fontsize',11,'XTick',1:14,'XTickLabel',classLabels);
```
![](https://img-blog.csdn.net/20130520163355031)
图4 （可以看到，x轴上标注完全重叠在一起）
2、调整x轴标注角度，避免重叠（图5）
```python
rotateXLabels(gca,45);
```
此处，将当前坐标轴（由gca（）函数得到）的标注，倾斜45度。（定义rotateXLabels的m文件可以从这里[下载](http://www.mathworks.com/matlabcentral/fileexchange/27812-rotate-x-axis-tick-labels/content/rotateXLabels.m)。）
![](https://img-blog.csdn.net/20130521052919997)
图5
## 三. 为图像添加注解
（源代码annotations.m）此处以绘制标准正态分布的图像为例。
1、绘制正态分布图（图6）

```python
% 生成(-3,3)的高斯分布累积曲线数据
x = -3:.01:3;
y = exp(1).^(-x.^2/2)/sqrt(2*pi);
plot(x,y);
line([0 0],get(gca,'ylim'));
```
![](https://img-blog.csdn.net/20130520182741094)
图6
2、添加注解（图7）

```python
[xnfu,ynfu] = ds2nfu([.5,0],[.15,.05]);
%[xnfu,ynfu] = dsxy2figxy([.5,0],[.15,.05]);
annotation('textarrow',xnfu,ynfu,'String','期望');
```
其中，ds2nfu将相对于图像x、y轴的坐标转化为annotation函数需要的归一化坐标（normalize figure units，左下角为(0,0)，右上角为(1,1)）。定义ds2nfu函数的m文件可以从这里[下载](http://www.mathworks.com/matlabcentral/fileexchange/10656-data-space-to-figure-units-conversion/all_files)；dsxy2figxy函数功能相似，源代码可以从这里[下载](http://www.mathworks.com/matlabcentral/fileexchange/30347-sigplot/content/sigplot/sigplot/BasicFunctions/dsxy2figxy.m)。
![](https://img-blog.csdn.net/20130520183150508)
图7
当然，标注也可以在交互模式下，手工插入，如图8所示。
![](https://img-blog.csdn.net/20130520183249178)
图8
添加的注解的位置不会随着图像的大小而改变，但如果坐标轴位置变化，注解则不能随之移动。如果要使注解相对于坐标轴也固定，可以如图9选择“相对坐标轴固定”。
![](https://img-blog.csdn.net/20130520183609734)
图9
此处，我们所加的标注是带箭头的文本框，Matlab还有其他多种注解格式（文本框、箭头、矩形、椭圆……）可供选择，不再一一赘言。
注：关于注解的位置参数的选择：可以在交互模式下手动调整注解位置，然后查看属性，获取位置参数。对于其他需要设定位置的对象（如坐标轴、子图、标题、图例……）也可以采用这种方式。之后，便可以方便的使用命令和设置对象位置，而无需手动调整。
## 四. 为图像添加网格，增强可读性
（源代码grids.m）
1、按族群提取收入水平信息
（所用数据来自[Marketing](http://www-stat.stanford.edu/~tibs/ElemStatLearn/data.html)）

```python
load MarketingData
% 初始化 y
y = NaN(length(incomel),length(ethnic));
% y中每个点代表在符合指定的收入水平和种群的人数
for i = 1:length(ethnic)
    thisGroup = find(data(:,13)==i);
    for j = 1:length(incomel)
        y(j,i) = length(find(data(thisGroup,1)==j));
    end
end
```
2、绘制堆叠条形图，选择合适的颜色，并添加相应标注（图10）
![](https://img-blog.csdn.net/20130520211255261)
图10

```python
% 指定图像位置
figure('units','normalized',...
    'position',[0.3474 0.3481 0.2979 0.5565]);
axes('position',[0.1300 0.2240 0.6505 0.6816]);

% 绘制条型图
bar(y,.4,'stacked','linestyle','none');

% 设置colormap
colormap('summer');

% 为图像添加标注
set(gca,'Fontsize',11,'Xtick',[1:9]-.5,'XTickLabel',[num2str(incomel') repmat(' 到 ',9,1) num2str(incomeu')]);
rotateXLabels(gca, 45);
ylabel('人数','Fontsize',11);
xlabel('收入群体','Fontsize',11);
title({'居民收入按族群分布情况','使用默认网格'});
box on;

% 为颜色条添加标注
h = colorbar;
set(h,'Fontsize',11,'ytick',1:8,'yticklabel',...
    ethnic);
ylabel(h,'族群','Fontsize',11);
set(gcf,'Color',[1 1 1]);
```
3、打开自动网格，显示默认格式（图11）

```python
grid on;
```

![](https://img-blog.csdn.net/20130520211353478)
图11

4、将自动网格设置为minor格式（图12）

```python
grid minor;
title({'居民收入按族群分布情况','使用minor网格'});
```

![](https://img-blog.csdn.net/20130520211551911)
图12
5、关闭自动网格，并添加自定义网格（图13）

```python
% 设置坐标轴范围
xlim([0 10]);ylim([0 1800]);
% Set y grid positions and draw lines (no x grid lines)设置y轴网格线位置（x轴不画线）
YgridPos = [0:200:1800];
set(gca,'ytick',YgridPos,'yticklabel',YgridPos);
xLimits = get(gca,'xlim');
line([xLimits(1)*ones(size(YgridPos),1); ...
    xLimits(2)*ones(size(YgridPos),1)],...
    [YgridPos; YgridPos],'Color',[.7 .7 .7],...
    'LineStyle','-');
XgridPos = [.5:9.5];
yLimits = get(gca,'ylim');
% 作两条平行于y轴的分隔线
line([XgridPos([2 9]); XgridPos([2 9])],...
    [yLimits(1)*ones(2,1) yLimits(2)*ones(2,1)]',...
    'Color',[.4 .4 .4],'LineStyle','-','Linewidth',2);
```
![](https://img-blog.csdn.net/20130520211705753)
图13

6、清除多余的刻度，完善标注（图14）

```python
% 清除边框，得到Tufte风格的条形图
line([xLimits(1) xLimits(2)]',[YgridPos(1);...
    YgridPos(1)],'Color',[1 1 1],'LineStyle','-');
line([xLimits(1) xLimits(1)]',[YgridPos(1);...
    YgridPos(end)], 'Color',[1 1 1],'LineStyle','-');
line([xLimits(end) xLimits(end)]', [YgridPos(1);...
    YgridPos(end)],'Color',[1 1 1],'LineStyle','-');
line([xLimits(1) xLimits(2)]',[YgridPos(end); ...
    YgridPos(end)],'Color',[1 1 1],'LineStyle','-');
%去掉x轴上多余的刻度
set(gca,'XTick',[]);
% 添加箭头和箭框注解
[xmeannfu ymeannfu]= ds2nfu([4.4,1.5],[1600,1600]);
annotation('textarrow',xmeannfu,ymeannfu,...
    'String',{'无边框网格线','分隔各个类别'});
[xmeannfu ymeannfu]=ds2nfu([5.8,8.5],[1600,1600]);
annotation('arrow',xmeannfu,ymeannfu);
title('居民收入按族群分布情况');
% 清除y轴上末尾多余的刻度标注
for i = 1:length(YgridPos)-1;
    cellticks{i} = num2str(YgridPos(i));
end
cellticks{i+1} = '';
set(gca,'ytick',YgridPos,'YTicklabel',cellticks);
% 去掉颜色条的边框
axes(h);box off;
```

![](https://img-blog.csdn.net/20130520211801282)
图14



