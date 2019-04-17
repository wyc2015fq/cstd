# matlab 图形锐化 滤波 - 家家的专栏 - CSDN博客





2011年08月24日 17:15:28[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：6855








help imread

help  fspecial  imfilt

帮助稳定中有较多的示例


fspecial 函数

功能：产生预定义滤波器

格式：H=fspecial(type)

        H=fspecial('gaussian',n,sigma)         高斯低通滤波器

         H=fspecial('sobel')                          Sobel 水平边缘增强滤波器

        H=fspecial('prewitt')                       Prewitt 水平边缘增强滤波器

        H=fspecial('laplacian',alpha)             近似二维拉普拉斯运算滤波器

        H=fspecial('log',n,sigma)                 高斯拉普拉斯（LoG）运算滤波器

        H=fspecial('average',n)                   均值滤波器

        H=fspecial('unsharp',alpha)             模糊对比增强滤波器

 说明：对于形式 H=fspecial(type) ，fspecial 函数产生一个由 type 指定的二维滤波器 H ，返回的 H 常与其它滤波器搭配使用。



imfilter函数

格式：B=imfilter(A,h)

说明：将原始图像 A 按指定的滤波器 h 进行滤波增强处理，增强后的图像 B 与 A 的尺寸和类型相同

参考资料：[http://hi.baidu.com/ouyang80/blog/item/742f565078e8c95b1138c216.html](http://hi.baidu.com/ouyang80/blog/item/742f565078e8c95b1138c216.html)



