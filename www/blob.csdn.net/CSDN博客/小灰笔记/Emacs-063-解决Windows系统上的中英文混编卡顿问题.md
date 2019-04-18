# Emacs-063-解决Windows系统上的中英文混编卡顿问题 - 小灰笔记 - CSDN博客





2018年01月16日 23:31:37[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：952








                
Emacs-063-解决系统上的中英文混编卡顿问题
其实，最初这个问题的发现很奇怪。曾经让我以为是Emacs或者是操作系统的问题。因为最初我学习Emacs的时候是用的Mac，那时候进行中英文的混合编辑虽说显示效果一般，但是没有卡顿问题。
后来，我把问题定位到字符集的编码上。因为在知乎上看到了一个类似的问题，而解决的方法居然是把Emacs的默认字体进行修改，修改成中文。曾经，我也这样使用了一段时间。然而，近期的编码中，类似问题再次出现的时候Emacs出现了字符编码格式的提示。此时开始考虑到是不是需要改成UTF-8，加上使用VSCode编码的时候，遇到了这样的类似问题。VSCode默认对一些文件格式使用UTF-8来存储，这样处理的比较悄无声息。
网络上搜索到了把Buffer按照UTF-8进行编码的配置，具体如下：

(set-language-environment "UTF-8")
尝试增加了上面的这个配置，并且使其生效。再次进行类似的混合编辑，结果卡顿现象消失。具体的效果倒是有点类似于在Mac上的体验了，显示有一点不自然。这个后续继续改进吧，目前看来已然是改进的不错了！

现在的效果如下：

![](https://img-blog.csdn.net/20180116233108235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


不是很理想，但是让我有了继续坚持使用Emacs的兴趣了！因为卡顿消失后，一切变得舒心多了。
            


