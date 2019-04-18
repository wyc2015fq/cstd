# Emacs-037-执行整个Buffer中的elisp语句 - 小灰笔记 - CSDN博客





2018年01月13日 18:28:16[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：127








Emacs-037-执行整个Buffer中的elisp语句

            编辑过很多次Emacs的配置文件了，但是为了让它能够生效我基本上每次都是关闭重启一下。

            之前看过很多人使用Emacs的体会说明，说他们的Emacs基本上是电脑开机就开着一直不关的。如果真是这样，那么Emacs要么是配置好了万年不动，要么是还有比较好的方式能够让配置文件加载生效。类似的加载生效功能我们在VSCode等编辑器中能够接触到，而且比较直观。

            Emacs中其实也能够实现这个功能，只需要调用一个功能来把配置文件中的elisp程序运行一遍。具体命令如下：

evla-buffer

            这个命令能够让当前buffer中所有的elisp程序都执行一下。这样，即使在我们HackEmacs的时候不断更新配置文件也可以不用重启Emacs了。



