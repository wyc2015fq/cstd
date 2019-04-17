# matlab normalized pixel Intensities - 家家的专栏 - CSDN博客





2011年11月01日 10:29:35[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1246标签：[matlab																[image](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)
个人分类：[图像处理算法																[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)](https://blog.csdn.net/yihaizhiyan/article/category/715109)







matlab code:

grayImage = imread('gray.bmp');

originalMinValue = double(min(min(grayImage)))

originalMaxValue = double(max(max(grayImage)))

originalRange = originalMaxValue - originalMinValue;


% Get a double image in the range 0 to +255

desiredMin = 0;

desiredMax = 255;

desiredRange = desiredMax - desiredMin;

dblImageS255 = desiredRange * (double(grayImage) - originalMinValue) /

originalRange + desiredMin;


% Get a double image in the range 0 to +1

desiredMin = 0;

desiredMax = 1;

desiredRange = desiredMax - desiredMin;

dblImageS1 = desiredRange * (double(grayImage) - originalMinValue) /

originalRange + desiredMin; 





