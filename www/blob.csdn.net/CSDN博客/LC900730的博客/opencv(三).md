# opencv(三) - LC900730的博客 - CSDN博客
2017年10月10日 14:36:48[lc900730](https://me.csdn.net/LC900730)阅读数：123标签：[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[opencv](https://blog.csdn.net/LC900730/article/category/7199508)
### ptr函数
Mat类提供了ptr函数可以得到图像任意行的首地址。ptr是一个模板函数，它返回了第i行的首地址。
对于彩色图像，每个图像由3部分构成，蓝色通道、绿色通道和红色通道。因此对于一个包含彩色图像的Mat，会返回一个由3个8位数组成的向量。OpenCV将此类型的向量定义为Vec3b，即由三个uchar组成的向量。
```
image.at<Vec3b>(j,i)[channel]=value;
如：
outputImage.at<Vec3b>(i,j)[0]=outputImage.at<Vec3b>(i,j)[0]/div*div+div/2;   //蓝色通道
outputImage.at<Vec3b>(i,j)[1]=outputImage.at<Vec3b>(i,j)[0]/div*div+div/2;   //绿色通道
outputImage.at<Vec3b>(i,j)[2]=outputImage.at<Vec3b>(i,j)[0]/div*div+div/2;   //红色通道
```
