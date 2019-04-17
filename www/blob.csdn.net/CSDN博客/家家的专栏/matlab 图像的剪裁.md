# matlab 图像的剪裁 - 家家的专栏 - CSDN博客





2011年02月21日 21:29:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2562








第一种方法：

I2 = imcrop(I,[75 68 130 112]); 



例子：A=imread('18.bmp');
A1=imcrop(A,[3,4,677,89]);
imshow(A1);





 第二种方法：

F = getframe(h,[left bottom width height]) 
后面4个参数分别你想截的图的左下角坐标，以及你想截的图的宽度和高度
F = getframe(h,rect) specifies a rectangular area from which to copy the pixmap。
rect is relative to the lower left corner of the figure or axes h, in pixel units. rect is a four-element vector in the form [left bottom width height], where width and height define the dimensions of the rectangle.


