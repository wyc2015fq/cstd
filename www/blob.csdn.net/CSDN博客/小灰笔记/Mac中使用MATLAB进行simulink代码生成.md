# Mac中使用MATLAB进行simulink代码生成 - 小灰笔记 - CSDN博客





2017年02月19日 15:37:49[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1253








说起来这仅仅算是一点点尝试，多年前我尝试在Ubuntu Linux试用了一下MATLAB感觉体验不是很好，后来我确定Windows平台是学习使用MATLAB的最佳平台。至于Linux和Mac，只能是作其他的考虑使用。不过，那时候我还没有Mac。

前阵子在微博上看到了学生党们都已经在Mac上使用MATLAB，最近在MathWorks的官网上看到的图片中也看到了在Mac上运行着的MATLAB，于是想找个简单版本在Mac上试用一下。

对我来说，只要是代码生成的环节能够过了其他的都还好说。只要这一关过了，我觉得现在的汽车电子所用的基本开发框架移植到Mac上来用荧光不会有什么大的问题。

简单找来之前的模型测试，模型都是在Windows平台中测试过的。一个假单的查表功能：

![](https://img-blog.csdn.net/20170219153742642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20170219153755637?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




代码生成的功能使用没有障碍，只有命令窗中的部分提示有所不同。如此看来，在Mac上使用应该不仅是没有问题，加上Mac独有的一种情愫，很多人如此使用应该是一种赏心悦目的体验。

不过，简单的使用中Mac平台上的MATLAB还是有那么一点问题，那就是中文目录的识别问题。使用中发现，如果目录中存在汉字在目录切换上就会存在问题，使用ls查看带有汉字的目录或者文件看到的也是乱码。目前还不知道如何解决，不过类似的问题从Windows拷贝文件到Linux的时候我是遇到过的。此外，在64位的XP上使用MATLAB的时候似乎也遇到过。猜测，Mac上出现这种现象的原因应该还是在于字符的编码问题。至于如何解决，以后再找时间研究一下。




