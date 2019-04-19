# iOS,StoryBoard适配实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月09日 09:05:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1610
据说代码界,关于用纯代码,还是Xib,或者故事版(storyboard)来完成项目,一直处于争议状态,前些天还在论坛里看到这方面的大战,小编下面的一席话可能会触怒到纯代码的大神,还望大神见谅,小编毕竟是新手,技术也不厉害.小编开发项目用的就是storyboard,不说别的,项目的效率真是刚刚的,小编没用过纯代码,在这里不发表言论,下边小编说一下storyboard适配的问题,希望对考虑用storyboard的同学有些帮助,
不多说,直接图文解释:
第一,我先来说说里边的几个约束是什么意思,有不正确的还望,留言改正
![](http://upload-images.jianshu.io/upload_images/2165173-e780968a2a9429a4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-43b70b3385957183.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-ec8fda70d3ffdfa7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-36461c45b6195e9c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-b1d0b39e2cdbae50.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二,一般是有四个约束,就能固定住一个控件的位置,如下图,我给图中的button了四个约束,分别是,横向居中,纵向居中,定宽,定高,这样不管机型怎么变这个button都会位于屏幕中间
![](http://upload-images.jianshu.io/upload_images/2165173-db48757f6a5c331c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-0172f61ba5d5b597.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-4b7eaf7ab6b0780a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
还可以这么做,给button上下左右四个方向的约束,这样button的大小就会根据屏幕大小来变大或者变小
![](http://upload-images.jianshu.io/upload_images/2165173-9263317e686aa7de.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2165173-9b085ee8a8ae0d3e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
定宽定高在label中的应用,在实际项目中label的文字并不是确定的,会根据文字的长短来改变label的frame,下边我们就用适配来解决这一问题,首先我们给这个label一个准确的位置,小编在这里给了这个label 上  左,定宽,定高四个约束,(小编你出逗比吧,定宽高能还怎么根据文字改变label的frame)如下图:
![](http://upload-images.jianshu.io/upload_images/2165173-4d9cc424955404a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在右侧有我们对这个label添加的约束,是可编辑的其中点开定宽高的Edit我们可以发现他的这个约束是可以改变的,
![](http://upload-images.jianshu.io/upload_images/2165173-e33ecb767f228d1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下边小编来解释一下,首先"=",这个很好理解,就是定宽,其次"<"就是这个label的宽小于一个值,比如说小宇100,你这个label的文字没有长度是50的话,此时的label的width就是50,文字变成100,此时label的Width就是100,最后是">"label的长度大于一个值,也就是说label的Width有了最小值没有最大值,因此我们队label的width和height的约束坐下更改,是不是就可以实现,label的自适应了呢?
第三,等宽等高宽高等比缩放的应用,这个约束大多用来,横向或者纵向并排几个宽高相等的几个控件.
![](http://upload-images.jianshu.io/upload_images/2165173-dcad517f19505c54.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
如上图,小编对右边的控件2做了上,左,右,宽高等比缩放四个约束,按理说控件2应该确定了,但是还有报错,那是因为控件2的左约束是相对于控件1的,控件1的约束并没有固定,我有给了控件1我这里给了控件1上 左 宽高等比缩放以及和控件2等宽,这里的等宽就和控件2的左右约束确定宽一样的左右,这样就实现了两个控件并排宽高相等,不会出现一个宽一个窄;
文／MaShiYong（简书作者）
原文链接：http://www.jianshu.com/p/a20ea6cf2a70
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
