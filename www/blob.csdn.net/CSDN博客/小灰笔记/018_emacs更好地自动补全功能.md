# 018_emacs更好地自动补全功能 - 小灰笔记 - CSDN博客





2018年11月24日 23:24:37[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：95








         如果我的emacs在这方面的配置再不升级一点，我真有点守不住继续使用emacs的这块阵地了！

         今天尝试了ycmd以及陈斌的配置，总体感觉上不是很好，或许是因为我自己的工作的行业用到的东西稍微特殊一点。

         想来想去，还是扩充clang的自动补全后端。虽说这个一直以来是我首先就会干掉的一个配置，但是至少能够达到类似SI速度的补全效果。

         其实，配置么有什么特殊的了，只是安装clang之后，恢复了company最初的配置。下面展示一下简单的效果吧。

![](https://img-blog.csdnimg.cn/20181124232331730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         我个人感觉，只要是结构体成员这样的信息能够识别正确就算是很大的一个进步了。上面看起来，还是识别成功了的。再看一下demo2的识别。

![](https://img-blog.csdnimg.cn/20181124232331859.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         识别也是成功的。

         接下来，做一个更加复杂的测试，把我用的一个芯片的描述头文件增加进来，看一下具体的代码效果。

![](https://img-blog.csdnimg.cn/20181124232331834.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         或许它还在计算匹配，不过我确实是等了好久没有看到实际的效果。看起来，这个功能与VS Code的简单易上手相比还是差距很大。应付简短的小代码应该还是应付的来。

       似乎，我的确应该准备一下从emacs往VS Code的全面迁移了。



