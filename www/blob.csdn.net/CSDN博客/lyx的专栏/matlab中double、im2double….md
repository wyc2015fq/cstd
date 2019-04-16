# matlab中double、im2double… - lyx的专栏 - CSDN博客





2015年11月08日 15:09:19[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：597










**原文地址：**[matlab中double、im2double、mat2gray区别--原创](http://blog.sina.com.cn/s/blog_6c41e2f30101559d.html)**作者：**[mimi](http://blog.sina.com.cn/u/1816257267)


****************假设某图像数据A（uint8格式）**********************

A =

  235 
200  89  20

>>
double(A)                
%返回与原矩阵数值相同但类型为double的矩阵；

ans =

  
235  
200 89   20

>>
im2double(A)              


%返回矩阵类型：double；数值范围[0 1] ,0对应uint8中的0；1对应uint8中的255；

ans =

   
0.9216   
0.7843  
0.3490   
0.0784

>>
mat2gray(A)               
%对原矩阵归一化

ans =

   
1.0000   
0.8372  
0.3209     
0


****************假设矩阵A为一般二维数组，非图像数据（double格式）**********************

A =

  
235  
200   
89   
20 

>> double(A)

ans =

  
235  
200   89   
20

>> im2double(A)

ans =

  
235  
200   89   
20

>> mat2gray(A)

ans =

   
1.0000   
0.8372   
0.3209     
0 

**********************小结***************************

im2double：如果输入类型是uint8、unit16
、logical，则按照0-->>0，255-->>1，将其值按比例处理成0～1之间的double数值；如果输入类型是double，输出没有处理；

double：返回数值与输入相同的double类型矩阵；


mat2gray：对输入进行归一化处理，最小值-->>0;最大值-->>1，输出类型为double。

在实际的对图像处理过程中，由于我们读入图像是unit8型，而在MATLAB的矩阵运算中要求所有的运算变量为double型（双精度型）。因此通常使用im2double函数将图像数据转换成双精度型数据。






