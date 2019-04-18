# Emacs-204-company popup功能失效 - 小灰笔记 - CSDN博客





2018年03月27日 18:53:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：76
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       尝试使用Emacs管理工程，结果导致了Compamy的popup功能有点失效。只有少数的匹配词能够实现补全。

       归功于Git的版本管理，通过版本之间的对比发现其实问题出现了Emacs自带的语义分析模块中。通过GUI的模式默认开启了sematic-mode之后，补全功能就失效了。于是，在自动生成的Emacs配置中找到sematic-mode的配置，将其清除。

       重启Emacs之后，自动补全的功能恢复。

       看起来，这个语义分析的功能还是有一点不够好用。如果是能够用于类似Source Insight这样的语义分析功能的话自然是很好的，在这方面还是得继续探索下去。




