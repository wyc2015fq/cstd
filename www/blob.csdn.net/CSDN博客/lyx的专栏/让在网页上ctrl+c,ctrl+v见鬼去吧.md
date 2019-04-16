# 让在网页上ctrl+c,ctrl+v见鬼去吧~ - lyx的专栏 - CSDN博客





2016年04月01日 14:34:19[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1422









# 1.准备

让我们来看一下XPath的伟大作用。关于XPath的教程，请移步 http://www.phpstudy.net/e/xpath/ 很好地教程哦。

只会xpath还不够，你还需要firefox以及他的插件。点开下面的附加组件，

![](https://img-blog.csdn.net/20160401142040326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后下载这么三个组件，重启你的火狐。开始高效的做dirty work吧。

![](https://img-blog.csdn.net/20160401142057014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 2.开始工作

下面是君联资本官网上的合伙人资料，我们现在要把每个合伙人的名字和职位放到excel表格中，怎么办呢？复制黏贴还不要崩溃！

![](https://img-blog.csdn.net/20160401142049545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在名字上面右击，选择View X Path，然后就是这样的：

![](https://img-blog.csdn.net/20160401142713719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注意上面的路径哦，可能和你学的xpath语法不太一样，但是知道原理的话，一下子就能试出来哦。这个就看经验啦。

如果不是那么简单，一下子就可以解决的话呢，我们右击 查看元素

![](https://img-blog.csdn.net/20160401143126502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这个时候就慢慢改吧。我们看到，每个人都是以li为单位的，所以，我们把上面的xpath语句改成这样：

![](https://img-blog.csdn.net/20160401143232924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后，一下子都出来了，接下来，一起选中复制黏贴吧。

叫我雷锋！



