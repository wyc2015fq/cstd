# Emacs-228-快速选中一片区域的代码 - 小灰笔记 - CSDN博客





2018年04月11日 21:28:51[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：437








       这是从陈斌的配置中找到的一个推荐的配置package：expand-region。

       使用了这个插件之后，可以快速选中一片区域的代码。大致的区域扩展过程：

       字符à 单词 à 句子 à 代码块 à 函数 à 全文件。

       安装package之后，增加的配置：

(require 'expand-region)

(global-set-key (kbd "C-=")'er/expand-region)

       这样，如果使用C-=开始启动这个功能。接下来的区域扩展方式则是不断按=即可。用了一下，确实是比较快捷的编辑方式。




