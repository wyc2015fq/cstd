# QImage Color Convert to Gray 转为灰度图 - Grandyang - 博客园







# [QImage Color Convert to Gray 转为灰度图](https://www.cnblogs.com/grandyang/p/4663262.html)







在Qt中，我们有时需要把QImage类的彩色图片转为灰度图，一开始我想的是用QImage的成员函数convertToFormat()，但是试了好多参数，返现转化的图片都有问题，不是我们想要的灰度图，如果谁用成员函数成功转化成了灰度图，请在评论区将您的方法写下，让博主学习一下。那么还有一种笨办法，就是手动将R,G,B的值平均一下，然后用平均值更新每一个像素值，也能得到灰度图。参见代码如下：



```
// image is the input color image, img is output grey image
QImage img(image);
for (int i = 0; i < img.width(); ++i) {
    for (int j = 0; j < img.height(); ++j) {
        int gray = qGray(img.pixel(i, j));
        img.setPixel(i, j, QColor(gray, gray, gray).rgb());
    }
}
```














