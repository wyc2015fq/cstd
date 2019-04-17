# Emacs-202-org文件中把代码提取到指定目录的文件中 - 小灰笔记 - CSDN博客





2018年03月26日 18:21:50[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：68








       这个在实践操作中发现需要注意两点：第一，需要在tangle中指明生成文件的目录；第二，指定的目录必须已经存在。

       加入有以下org文件：

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
#+BEGIN_SRC python :tangle ./src/1.py
def Demo():
   print("org-mode")
#+END_SRC
** Here is the code
#+BEGIN_SRC python :tangle 1.py
def Demo():
   print("org-mode")
#+END_SRC
```
C-c C-t

       查看目录，src中的代码文件中只有一份代码而org文件所在目录中的代码中有三代代码。




