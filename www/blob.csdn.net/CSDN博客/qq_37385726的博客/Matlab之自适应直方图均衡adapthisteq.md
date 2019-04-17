# Matlab之自适应直方图均衡adapthisteq - qq_37385726的博客 - CSDN博客





2018年09月09日 17:16:57[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：732








转自[这里](https://blog.csdn.net/sousky/article/details/73484691)



## adapthisteq函数执行所谓的对比度受限的自适应直方图均衡

这种方法用直方图匹配方法来逐个处理图像中的较小区域（称为小块）。然后使用双线性内插方法将相邻的小片组合起来，从而消除人口引入的边界。

特别在均匀的灰度区域，可以限制对比度来避免放大噪声。



g = adapthisteq(f, param1, val1, param2, val2, ...)

该函数所用参数：

**'NumTiles'  **一个有正整数组成的两元素向量[r, c]，由向量的行和列指定小片数。r和c都必须至少是2，小片总数等于r*c。默认值是[ 8  8]

**'ClipLimit' **范围是[0 1]内的标量，用于指定对比度增强的限制。较高的值产生较强的对比度。默认值是0.01

'NBins' 针对建立对比度增强变黄所用的直方图容器数目指定的正整数标量。较高的值会在较慢的处理速度下导致较大的动态范围。默认值是256

**'Range' **规定输出图像数据范围的字符串

        'original'——范围被限制到原始图像的范围，[min(f(:))  max(f(:))]

        'full'——使用输出图像类的整个范围。例如，对于uint8类的数据，范围是[0 255]。这是默认值。

**'Distribution' **为图像小片指定期望直方图形状的字符串

        'uniform'——平坦的直方图（默认）

        'rayleigh'——钟形直方图

        'exponential'——曲线直方图

**'Alpha' **适用于瑞利和指数分布的非负标量。默认值为0.4



```python
f = imread('Fig0210(a).tif');
figure(1);
imshow(f);
%adapthisteq的使用
g1 = adapthisteq(f);
figure;
imshow(g1);
g2 = adapthisteq(f, 'NumTiles', [25 25]);
figure;
imshow(g2);
 
g3 = adapthisteq(f, 'NumTiles', [25 25], 'ClipLimit', 0.05);
figure;
imshow(g3);
```
` `
![](https://img-blog.csdn.net/20170620114115331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU291c2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20170620113250560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU291c2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170620113255567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU291c2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170620113300248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU291c2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



