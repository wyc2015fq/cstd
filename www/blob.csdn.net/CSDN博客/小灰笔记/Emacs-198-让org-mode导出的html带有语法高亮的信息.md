# Emacs-198-让org-mode导出的html带有语法高亮的信息 - 小灰笔记 - CSDN博客





2018年03月25日 23:35:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：226








       在实现这个功能前，首先得保证开启了org-mode代码片段的语法高亮功能。然后，安装一个支持html语法高亮导出的插件：htmlize

       安装往之后，在emacs的配置文件中加入如下配置并使其生效。之后导出的html文件便会保留语法高亮的信息。

(require 'htmlize)




