# Emacs-200-使用一份org文件导出多个代码文件 - 小灰笔记 - CSDN博客





2018年03月25日 23:36:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：55
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       其实，这个导出方式跟只导出鱼粉代码的方式没有太大的差异。只不过需要对每个不同的代码片段单独写tangle。

       假设有如下的org文件：

* My first org-mode code.



** Here is the code

#+BEGIN_SRC python :tangle 1.py

def Demo():

   print("org-mode")



#+END_SRC



** Here is the code

#+BEGIN_SRC python :tangle 2.py

def Demo():

   print("org-mode")



#+END_SRC



** Here is the code

#+BEGIN_SRC python :tangle 3.py

def Demo():

   print("org-mode")



#+END_SRC



** Here is the code

#+BEGIN_SRC python :tangle 4.py

def Demo():

   print("org-mode")



#+END_SRC

       C-cC-v t

       查看org文件所在的目录会发现，导出了四份不同的代码文件。

       因此，其实代码的导出其实只是做一部分文本信息的提取。而tangle也只是写明了提取写入的对象文件名称。




