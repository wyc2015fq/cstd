# Opencv(-) - LC900730的博客 - CSDN博客
2017年09月27日 21:59:47[lc900730](https://me.csdn.net/LC900730)阅读数：109标签：[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[opencv](https://blog.csdn.net/LC900730/article/category/7199508)
声明一个表示图像的变量。 
cv::Mat image; 
这句话创建一个宽高为0的图像。 
cv::Mat的size方法可以获取图像尺寸。方法返回一个结构体，包含宽高。 
image.size().height; 
image.size().width;
image=cv::imread(“img.jpg”);
使用前需要检查图像已经被正确读取，如果文件不存在、文件损坏或者格式无法识别，会出现错误。
```
if(!image.data){
    //图像尚未创建
}
//此时的data实际上是个指向已经分配的内存块的指针，包含图像数据。不含数据时候，被简单置为0；
```
### 显示图像
1.定义窗口； 
cv::namedWindow(“Original Image”); 
2.显示图像； 
cv::imshow(“Original Image”,image);
### 图像变换
cv::Mat result; 
cv::flip(image,result,1); 
//1表示水平反转，0表示垂直反转，负数表示既有水平也有垂直
当图像之间进行辅助，图像数据没有复制，两个对象都指向了一块内存。
### 图像
一张图像是由一个数值组成的矩阵。 
矩阵的每个元素代表一个像素。 
灰度：8位无符号数。0黑色，255白色； 
彩色：每个像素需要3个这样的8位无符号数来表示(红绿蓝)。在这种情况下，矩阵是一个三元数。
