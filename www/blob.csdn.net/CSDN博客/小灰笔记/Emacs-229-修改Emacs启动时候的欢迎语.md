# Emacs-229-修改Emacs启动时候的欢迎语 - 小灰笔记 - CSDN博客





2018年04月11日 21:33:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：351








     最早的时候看陈斌的配置，下载了一份使用看到他的启动欢迎语被修改了。我觉得很神奇，以后我也想要做一份类似的。

       今天使用purcell的配置，也看到了类似的功能。忍不住了，找了一个这个功能的实现。本以为会是一个插件，我还写了脚本搜索了全部的文件内容。最后借助Google找到了设置方案，原来是如此简单！

       修改配置：

(setq initial-scratch-message "Happyhacking and Grey ♥ you!")

       启动效果：

![](https://img-blog.csdn.net/20180411213312499)




