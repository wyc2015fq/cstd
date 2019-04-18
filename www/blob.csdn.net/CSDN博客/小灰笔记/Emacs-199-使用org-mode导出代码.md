# Emacs-199-使用org-mode导出代码 - 小灰笔记 - CSDN博客





2018年03月25日 23:36:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：80
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       编辑如下org文件：

* My first org-mode code.



** Here is the code

#+BEGIN_SRC python :tangle 1.py

def Demo():

   print("org-mode")



#+END_SRC

       org-mode代码导出成为tangle，在上面的代码片段头部加上了一个tangle标签并写明文件名称。

       C-cC-v t，执行这个命令可以实现代码的导出。导出的提示在minibuffer中有相应的提示显示。此时，查看org文件所在的目录就能够看到生生的文件。




