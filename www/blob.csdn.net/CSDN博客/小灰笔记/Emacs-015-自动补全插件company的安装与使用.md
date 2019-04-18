# Emacs-015-自动补全插件company的安装与使用 - 小灰笔记 - CSDN博客





2018年01月03日 22:56:30[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2783








Emacs-015-自动补全插件company的安装与使用

       驱动我从VIM切换到Emacs，想在Emacs上做一下尝试的一个很大的原因就是VIM上的自动补全工具安装调试起来太麻烦！我花了好几天的时间没能够把YCM的配置实现。最后我的VIM被我弄到了Bug频发，实在是不愿意继续下去。

       而时不时看看RMS等人的动向是我的乐趣，看着他们使用Emacs的感觉很好，于是我也就开始了我的Emacs之旅。

       稳扎稳打，从头开始！今天终于走到了自动补全这一关。很快锁定了一个可以使用的插件——company。其实这是一个缩写，是complete anything的缩写。才发现Emacs内置了一个插件管理工具，插件的安装原来还是比较简单的，跟Notepad++的简单度有的一拼。以下是进入插件管理器的方式：

![](https://img-blog.csdn.net/20180103225514360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       进入之后找到company，然后鼠标左键单击。

![](https://img-blog.csdn.net/20180103225532282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




鼠标点击后会出现下面截图中显示的另一个buffer，点击里面的install。

![](https://img-blog.csdn.net/20180103225543856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       伴随着emacs lisp文件的编译，company也就安装完成。

![](https://img-blog.csdn.net/20180103225553962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




M-x，然后输入company-mode既可以在当前的buffer中激活相应的功能。如果要在Emacs启动的时候就开启相应的功能，可以对配置文件做一下修改，添加以下配置：

(add-hook 'after-init-hook'global-company-mode)

       使用的具体示例如下：

![](https://img-blog.csdn.net/20180103225607497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       如果有可以自动实现补全的可能，会自动弹出相应的提示。Emacs中编程语言相应的配置会略有不同，因此在使用Emacs对C语言等变成语言进行编码的时候可能会有一点小问题，解决一下才能够继续使用。



