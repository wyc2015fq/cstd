# Git操作之“时光穿梭” - xiahouzuoxin - CSDN博客





2014年01月10日 10:34:20[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：3207








从之前的两篇博文：

[1] [http://blog.csdn.net/xiahouzuoxin/article/details/9393119](http://blog.csdn.net/xiahouzuoxin/article/details/9393119)

[2] [http://blog.csdn.net/xiahouzuoxin/article/details/9398629](http://blog.csdn.net/xiahouzuoxin/article/details/9398629)

中我们已经能基本地使用git及github了。而版本控制器的真正实用的地方将在本节中展现。

请注意：本节的内容与Github无关，请不要将git与github混为一谈！




***1. 版本回退——时光穿梭（参考[1]）***

我们常常在开发时会遇到这种情况：当对当前的版本不满意，打算从上一个或上几个版本重新开始时，该怎么才能从git中获取上一个版本呢？

不妨先用

**git log**

命令查看提交版本的日志记录吧。

![](https://img-blog.csdn.net/20140110092944796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我已经提交了2个版本，**commit一行后的代码是版本的唯一标识，我们可以通过它到任何一个我们想要的版本**。注意：最新的一个版本是当前版本，上一个版本是指第2个对应的版本。




这时，我们用

**git reset --hard b36e5**

回到上一个版本（可以不用输入整个一大串，git会自动查找，只要能唯一识别版本就行）

![](https://img-blog.csdn.net/20140110093552968)


这个时候再git log看一下吧，

![](https://img-blog.csdn.net/20140110093753234)


只剩下最初提交的一个版本了！上一个版本已经看不到了，而这时我们又后悔了，想回到回退前的版本，怎么办？

还是一样的方法，前提是只要你还记得版本的ID，只要执行

**git reset --hard ab21e**



现在再看看，是不是，回去了！这样就免去在开发过程中为备份不同的版本粘贴复制的麻烦了。

![](https://img-blog.csdn.net/20140110094950218)

我关机了，第二天才后悔，不记得ID了怎么办，git log之后不是已经没有最新版本的ID了吗？

其实还是有办法的，使用

**git reflog**

看看就知道了，git reflog记录了git的每一次版本更新的命令。




**版本回退更方便**：git中使用HEAD表示当前版本，HEAD^表示上一个版本，上上一个版本是HEAD^^，回退到上一个版本只需要执行

**git reset --hard HEAD^**

上上个版本当然是

**git reset --hard HEAD^^**



***2. 版本提交前的回退***

有木有，当你使用完git add之后立马就后悔了，发现add文件太少或太多了。




我们不妨先来理解git add和git commit命令的含义（看下图）：

![](https://img-blog.csdn.net/20140110102249734)


git分为工作区和版本库，我们现在正在编辑的代码就是工作区，版本库里包括了已经使用git commit命令提交的版本，还包括了一个缓存区（Stage）。git add操作就是将修改的代码添加到缓存区的过程，git commit就是将缓存区中更新的部分提交到master正式版本的过程。

我们的第1小节“***版本回退——时光穿梭***”就是想办法如何从git commit执行后回退的过程。而本小节将给出的一个命令则是从git add回退的过程。好吧，请记住，

**git reset HEAD <file> **

下面是一个例子，我将README.md修改add后，再使用git reset HEAD README.md回退的过程。

![](https://img-blog.csdn.net/20140110103159359)





最后，请记住1个帮助命令，

**git help **

非常非常有用，随时查命令。




另附一张常用命令图（非我制作），

![](https://img-blog.csdn.net/20140110103923562)








***参考文献：***

**[1] **廖雪峰Git教程：[http://www.liaoxuefeng.com](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)




