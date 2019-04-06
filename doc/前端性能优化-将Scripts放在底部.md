# 前端性能优化-将Scripts放在底部

#  

# 简介

在上面篇文章中我已经提到，web页面中将CSS放在底部会导致载网速低的情况下页面逐渐呈现阻塞，导致“白屏”，应该将CSS引用放在顶部。而对于JavaScript文件，相反，应该放在底部。

## 原理

HTML中script有两个属性，async是异步执行，表示下载完js马上异步执行js，不阻塞浏览器呈现。defer表示延迟执行，需要等页面资源下载完成后执行，相当于放在尾部。既没有async和defer的属性，页面将在下载后阻塞呈现立即执行。  相信看到这篇文章的大部分同学，都没有使用到defer和async。这种情况下JavaScript文件放在顶部会导致页面呈现阻塞。

同样，来看看天猫首页的脚本情况





![img](https://upload-images.jianshu.io/upload_images/3020614-055e52e10fa3e823.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

天猫在html中相当一部分使用了async的，这里有他特殊的使用之处，基本是数据的获取，不会对页面布局有改动，我对它的使用不是清楚，如果谁知道，望告知我。



![img](https://upload-images.jianshu.io/upload_images/3020614-c0f9492df38c5564.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/930)

上面我们说了script的async和defer两个属性，以及没有这两个属性的情况下，脚本阻塞页面呈现，导致“白屏”。下面我们重点看下没有这个两个属性的情况。

引用http://stevesouders.com/hpws/move-scripts.php的例子（demo是前辈大神的，我只是借来用用）。

页面中嵌了2个iframe，左边的是把脚本放在top的情况，右边是脚本放在底部的情况。





![img](https://upload-images.jianshu.io/upload_images/3020614-ae6a75972c438903.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)





![img](https://upload-images.jianshu.io/upload_images/3020614-5394de5522380610.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)

上面两张图可见脚本放在顶部会导致页面呈现阻塞，在网速慢的时候导致“白屏”，直到脚本下载完毕，页面才继续呈现。脚本放在底部则可以让页面尽快呈现。前端的性能优化必须对这些细节非常注意，变量引起质变，如果每个地方你都极尽最佳性能，你开发出来的网站性能将会尤其突出。