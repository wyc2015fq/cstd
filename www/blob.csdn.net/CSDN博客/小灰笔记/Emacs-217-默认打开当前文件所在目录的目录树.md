# Emacs-217-默认打开当前文件所在目录的目录树 - 小灰笔记 - CSDN博客





2018年04月09日 08:18:24[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：616








       如果默认情况下，打开一次目录树之后，关闭再打开打开的依然是上次打开的目录树。使用Emacs的时候我会不断切换目录，绝大多数时候我想能够随时切换到当前文件所在的目录树。

       如果这样，可以通过smart的模式来实现。如下是我的neotree的配置：

(global-set-key [f8] 'neotree-toggle)

(setq neo-smart-open t)

       增加如上配置，可以方便地使用F8来快速切换出当前文件所在的目录的目录树。




