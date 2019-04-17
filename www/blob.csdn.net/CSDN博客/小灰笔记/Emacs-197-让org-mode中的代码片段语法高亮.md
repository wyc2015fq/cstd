# Emacs-197-让org-mode中的代码片段语法高亮 - 小灰笔记 - CSDN博客





2018年03月25日 23:34:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：432








       默认情况下，org-mode嵌入的代码是没有语法高亮的。如果要开启语法高亮，可以采用如下配置：

(setq org-src-fontify-natively t)

       把上面这段配置加入到emacs的配置文件init.el或者.emacs中，令其生效。之后在org-mode中编辑插入的代码片段的时候就会有语法的高亮。

效果：

![](https://img-blog.csdn.net/20180325233416310)




