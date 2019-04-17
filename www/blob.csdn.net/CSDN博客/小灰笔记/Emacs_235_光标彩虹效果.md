# Emacs_235_光标彩虹效果 - 小灰笔记 - CSDN博客





2018年07月24日 21:03:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：122
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       虽说现在对Emacs的使用还算不上熟练，这个光标的彩虹效果也不是什么也别有用的功能。不过，在学到了一定的阶段之后，这些东西能够带给人一点新鲜感。何况，有些高手也在用。

       高手都用，就不要再说他华而不实了。

       能够实现光标彩虹效果的一个插件：beacon

       具体的光标效果不好捕捉，直接引用一个GitHub上的图，同时附上GitHub的链接。

GitHub链接：[https://github.com/Malabarba/beacon](https://github.com/Malabarba/beacon)

效果图：

![](https://img-blog.csdn.net/20180724210348910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       我自己的配置如下：

(setq-default beacon-lighter "")

(setq-default beacon-size 5)

(add-hook 'after-init-hook 'beacon-mode)

(beacon-mode 1)



       配置其实也是purcell以及其他地方的配置组合了。也有了相应的效果，但是效果确实是没有上面这个好看。后期，我倒是有可能把这个功能直接去掉呢！



