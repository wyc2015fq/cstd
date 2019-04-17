# Emacs-164-手动安装emacs主题 - 小灰笔记 - CSDN博客





2018年03月18日 11:16:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：126








            尝试一下手动安装Emacs的主题，之前有过一次不是很成功的尝试。当时，由于配置的不是很成功，不得不在Emacs的程序包中方式了elisp文件。今天以monokai为例子，再尝试一下手动安装。

            创建一个存放主题的目录，我的目录为：

![](https://img-blog.csdn.net/20180318111602605)

            在此放置monokai的主题文件。

            接下来，在配置文件中设置主题的加载目录并加载主题：

  (add-to-list'custom-theme-load-path "~/.emacs.d/packages/theme/monokai")

  (load-theme 'monokait)

            使上面的配置生效后，Emacs的效果如下：

![](https://img-blog.csdn.net/20180318111609158)








