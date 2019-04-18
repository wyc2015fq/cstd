# 如何计算padding大小 - 李鑫o_O - CSDN博客

2017年07月10日 16:53:14[hustlx](https://me.csdn.net/HUSTLX)阅读数：2465
个人分类：[深度学习](https://blog.csdn.net/HUSTLX/article/category/7018158)



                
根据tensorflow中的conv2d函数，我们先定义几个基本符号

1、输入矩阵 W×W，这里只考虑输入宽高相等的情况，如果不相等，推导方法一样，不多解释。

2、filter矩阵 F×F，卷积核

3、stride值 S，步长

4、输出宽高为 new_height、new_width

当然还有其他的一些具体的参数，这里就不再说明了。

我们知道，padding的方式在tensorflow里分两种，一种是VALID，一种是SAME，下面分别介绍这两种方式的实际操作方法。

1、如果padding = ‘VALID’

new_height = new_width = (W – F + 1) / S （结果向上取整）

也就是说，conv2d的VALID方式不会在原有输入的基础上添加新的像素（假定我们的输入是图片数据，因为只有图片才有像素），输出矩阵的大小直接按照公式计算即可。

2、如果padding = ‘SAME’

new_height = new_width = W / S （结果向上取整）

在高度上需要pad的像素数为

pad_needed_height = (new_height – 1)  × S + F - W

根据上式，输入矩阵上方添加的像素数为

pad_top = pad_needed_height / 2  （结果取整）

下方添加的像素数为

pad_down = pad_needed_height - pad_top

以此类推，在宽度上需要pad的像素数和左右分别添加的像素数为

pad_needed_width = (new_width – 1)  × S + F - W

pad_left = pad_needed_width  / 2 （结果取整）

pad_right = pad_needed_width – pad_left

至此，关于tensorflow的卷积padding操作介绍完毕，下面是关于此操作的源码（Get2dOutputSizeVerbose函数的部分节选），我也不会用MarkDown，索性直接截图了，以供参考

![](http://upload-images.jianshu.io/upload_images/2228224-9fb296be5beee6be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

