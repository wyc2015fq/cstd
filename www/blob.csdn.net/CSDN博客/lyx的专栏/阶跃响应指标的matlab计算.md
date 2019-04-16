# 阶跃响应指标的matlab计算 - lyx的专栏 - CSDN博客





2015年11月08日 15:08:36[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3805
个人分类：[课程作业](https://blog.csdn.net/qtlyx/article/category/5936697)











**原文地址：**[阶跃响应指标的matlab计算](http://blog.sina.com.cn/s/blog_6163bdeb0100qiwe.html)**作者：**[了凡春秋](http://blog.sina.com.cn/u/1633926635)


最近师兄让帮忙计算阶跃响应的指标，就是改定实验数据或仿真数据，求响应指标（概念见程序中）。编程代码和效果如下

%% 求阶跃响应的典型指标

function main_GetPerformanceOfStepResponse

clc

clear all

close all

global gTolerance

gTolerance = 0.05; % 调整时间的偏差容许范围

%% test

wn = 1;

xi = 0.3;

g = tf(wn^2, [1, 2*xi*wn, wn^2]);

t = 0:0.01:15;

y = step(g,t);

%% 计算阶跃响应的指标

stepvalue = 1;

[OverShoot, RiseTime, PeakTime, AdjustTime, SteadyStateError] =
GetPerformanceOfStepResponse(t, y, stepvalue);

% 绘图

figure

plot(t,y)

grid on

line([PeakTime, PeakTime], [0, (1 + OverShoot/100)*stepvalue],
'color', 'r')

text(PeakTime, stepvalue*0.05, sprintf('峰值时间%.2f',PeakTime))

text(PeakTime, (1 + OverShoot/100 + 0.05)*stepvalue,
sprintf('超调量%.2f%%',OverShoot))

line([RiseTime, RiseTime], [0, stepvalue], 'color', 'r')

text(RiseTime, -stepvalue*0.05, sprintf('上升时间%.2f',RiseTime))

line([AdjustTime, AdjustTime], [0, stepvalue*(1 + gTolerance)],
'color', 'r')

text(AdjustTime, stepvalue*0.05,
sprintf('调整时间%.2f',AdjustTime))

line([AdjustTime t(end)], stepvalue*[(1 - gTolerance), (1 -
gTolerance)], 'color', 'r', 'linestyle', '--')

text(AdjustTime, stepvalue*(1 - gTolerance-0.05),
sprintf('容许范围%.2f', 1 - gTolerance))

line([AdjustTime t(end)], stepvalue*[(1 + gTolerance), (1 +
gTolerance)], 'color', 'r', 'linestyle', '--')

text(AdjustTime, stepvalue*(1 + gTolerance+0.05),
sprintf('容许范围%.2f', 1 + gTolerance))

text(t(end)*0.9, stepvalue*1.05, sprintf('稳态误差%f',
SteadyStateError))

end

%% 求阶跃响应的典型指标

function [OverShoot, RiseTime, PeakTime, AdjustTime,
SteadyStateError] = GetPerformanceOfStepResponse(t, y,
stepvalue)
%
超调量Mp:最大超调量规定为在暂态期间输出超过对应于输入的终值的最大偏离量

% 上升时间tr:在暂态过程中，输出第一次达到对应于输入的终值的时间（从t=0开始计时）

% 峰值时间tp:对应于最大超调量发生的时间（从t=0开始计时）

%
调整时间ts:输出与其对应于输入的终值之间的偏差达到容许范围（一般取5%或2%）所经历的暂态过程时间（从t=0开始计时）

% 稳态误差err:给定输入与稳态输出的差值

global gTolerance

% 超调量和峰值时间

[OSValue, OSIndex] = max(y);

OverShoot = (OSValue - stepvalue)/stepvalue*100;

PeakTime = t(OSIndex);

% 上升时间

index = find(y >= stepvalue, 1, 'first');

RiseTime = t(index);

% 调整时间和稳态误差

index1 = find(y <= stepvalue*(1 - gTolerance), 1,
'last'); % 容许范围由全局变量指定

index2 = find(y >= stepvalue*(1 + gTolerance), 1,
'last');

if isempty(index2) % 如果没有超调量，此值为空

    index =
index1;

else

    index =
max(index1, index2);

end



index = max(index1, index2);

AdjustTime = t(index);

SteadyStateError = mean(y(index:end)) - stepvalue; %
这里的稳态误差计算为调整时间后的数据平均值与给定输入的差，概念上是最后时刻的值与给定输入的差

end



运行结果为：

[![image](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)](http://s5.sinaimg.cn/middle/6163bdeb4a11b9c706ed4&amp;690)

改变容许范围偏差为0.02的结果

[![image](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)](http://s3.sinaimg.cn/middle/6163bdeb4a11b9c840792&amp;690)






