# Emacs-154-指定启动时候默认显示的buffer - 小灰笔记 - CSDN博客





2018年03月16日 23:43:17[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：97
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









![](https://img-blog.csdn.net/20180316234303575)

            initial-buffer-choice的设定值可以指定emacs启动时候读取显示的文件。现在正好在学习emacs的初级阶段，配置文件经常需要修改。接下来直接把配置文件指定为启动时候显示的buffer。

  (setqinitial-buffer-choice "~/.emacs.d/init.el")

            设置完成后的启动效果：

![](https://img-blog.csdn.net/20180316234309713)

            设置成功。

            现在的emacs并没有增加多少配置，不过启动速度似乎慢了很多。具体的原因暂且不去考虑了，目前来看，精准的编辑以及编辑时候的流畅度是我更加关心的。




