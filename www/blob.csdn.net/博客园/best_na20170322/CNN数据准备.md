# CNN数据准备 - best_na20170322 - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/believe-in-me/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/best_na20170322)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/believe-in-me/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [CNN数据准备](https://www.cnblogs.com/believe-in-me/p/6646927.html)





参考：http://blog.csdn.net/chengzhongxuyou/article/details/50717543

# 数据准备

这部分是最耗时耗力的了，自己自行准备，注意准备的图片的大小最好是一样的，免得麻烦。我使用64*64的尺寸大小。给出train文件夹图像库的列表示意。假设我们已经准备好了数据，每一个分类中有100张train图片，有20张test图片。将这里面的400张train图片放到train文件夹，80张test图片放到test文件夹。

![](http://img.blog.csdn.net/20160222164524449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后我们需要制作标签(label)，将图片路径与图片分类作为键值对，因为我们需要对四种不同的图像进行分类，所以我们可以将第一种图像的标签设为0，第二种设为1，第三种设为2，第四种设为3.

这些“键值对”被保存到.txt中。对应的train中的label为：

![](http://img.blog.csdn.net/20160222165010531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](http://img.blog.csdn.net/20160222165052801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如果图片数据库非常大，直接写一个小程序，自动生成标签label的.txt。注意label包括两个部分，一个是train,一个是test。这里不再赘述test的label的生成。

那么我们为什么要花时间去做label呢，当然是通过程序读取label的.txt，每一行为一个键值对，一个键值对对应一幅图像的路径与标签。注意，是图像的路径而不是图像的名称。因为我的数据库与程序在同级目录，所以这里的图片名称即为图片相对路径。












