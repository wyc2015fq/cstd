# Numpy中ndim、shape、dtype、astype的用法 - 数据之美的博客 - CSDN博客
2019年02月27日 22:06:47[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：46
本文介绍numpy数组中这四个方法的区别ndim、shape、dtype、astype。
1.ndim
![](https://img-blog.csdn.net/20180531092218231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0RhX3dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ndim返回的是数组的维度，返回的只有一个数，该数即表示数组的维度。
2.shape
shape：表示各位维度大小的元组。返回的是一个元组。
对于一维数组：有疑问的是为什么不是（1，6），因为arr1.ndim维度为1，元组内只返回一个数。
对于二维数组：前面的是行，后面的是列，他的ndim为2，所以返回两个数。
对于三维数组：很难看出，下面打印arr3，看下它是什么结构。
![](https://img-blog.csdn.net/20180531093349226?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0RhX3dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
先看最外面的中括号，包含[[1,2,3],[4,5,6]]和[[7,8,9],[10,11,12]]，假设他们为数组A、B，就得到[A,B]，如果A、B仅仅是一个数字，他的ndim就是2，这就是第一个数。但是A、B是（2，3）的数组。所以结合起来，这就是arr3的shape，为（2，2，3）。
将这种方法类比，也就可以推出4维、5维数组的shape。
3.dtype
![](https://img-blog.csdn.net/20180531092347376?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0RhX3dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
dtype：一个用于说明数组数据类型的对象。返回的是该数组的数据类型。由于图中的数据都为整形，所以返回的都是int32。如果数组中有数据带有小数点，那么就会返回float64。
有疑问的是：整形数据不应该是int吗？浮点型数据不应该是float吗？
解答：int32、float64是Numpy库自己的一套数据类型。
4.astype
![](https://img-blog.csdn.net/20180531153910939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0RhX3dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
astype：转换数组的数据类型。
int32 --> float64        完全ojbk
float64 --> int32        会将小数部分截断
string_ --> float64        如果字符串数组表示的全是数字，也可以用astype转化为数值类型
![](https://img-blog.csdn.net/20180531155929475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0RhX3dhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意其中的float，它是python内置的类型，但是Numpy可以使用。Numpy会将Python类型映射到等价的dtype上。
以上是这四个方法的简单用法，之后若有什么新发现再做补充。
--------------------- 
作者：Da_wan 
来源：CSDN 
原文：https://blog.csdn.net/Da_wan/article/details/80518725 
版权声明：本文为博主原创文章，转载请附上博文链接！
