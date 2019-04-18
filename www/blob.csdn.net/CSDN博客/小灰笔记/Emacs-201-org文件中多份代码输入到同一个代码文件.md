# Emacs-201-org文件中多份代码输入到同一个代码文件 - 小灰笔记 - CSDN博客





2018年03月26日 18:21:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：50
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       多份代码提取到同一个文件，操作也比较简单。只需要在tangle中把每一份的代码文件名称写成相同，无须关系是否会覆盖，这个写入并不会覆盖。

       有如下org文件：

```
* My first org-mode code.
** Here is the code
#+BEGIN_SRC python :tangle 1.py
def Demo():
   print("org-mode")
#+END_SRC

** Here is the code
#+BEGIN_SRC python :tangle 1.py
def Demo():
   print("org-mode")
#+END_SRC

** Here is the code
#+BEGIN_SRC python :tangle 1.py
def Demo():
   print("org-mode")
#+END_SRC

** Here is the code
#+BEGIN_SRC python :tangle 1.py
def Demo():
   print("org-mode")
#+END_SRC
```


       C-cC-v t

       之后，查看org文件所在目录，生成的1.py中有上面的4份代码。尽管他们内容相同，但是依然被提取到了同样的文件中。




