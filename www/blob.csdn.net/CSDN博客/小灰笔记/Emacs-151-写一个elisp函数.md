# Emacs-151-写一个elisp函数 - 小灰笔记 - CSDN博客





2018年03月16日 23:37:57[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：138
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









            先看一下简单的代码：

```
(defun my-demo ()
  (message "my first elisp function test")
  )
(my-demo)
```

            执行elisp语句或者函数，需要把光标移动到执行体的后面，然后运行C-xC-e。而上面的程序，现需要执行函数定义，然后执行函数的调用。

            运行方式，先把光标移动到代码第三行最后，然后执行C-xC-e。执行的效果如下：

![](https://img-blog.csdn.net/20180316233624986)



            mini buffer之中显示出函数名称，代表函数定义成功。

之后，把光标移动到第四行代码最后，然后执行C-xC-e。执行的效果如下：

![](https://img-blog.csdn.net/20180316233644109)



            修改一下代码：

```
(defun my-demo ()
  (message "myfirst elisp function test")
  (message "helloworld")
  )
(my-demo)
```

            在最后一行的位置执行C-xC-e以调用my-demo函数。此时显示效果如下：

![](https://img-blog.csdn.net/20180316233732955)



            从上面结果看，加入的新代码没有执行成功。这是我最初学习使用emacs的时候经常犯的错误，因为函数在解析器中的定义没有更新，需要先执行一下函数定义才能够让程序执行的时候有效输出新加入的输出信息。重新操作之后，程序的输出效果如下：
![](https://img-blog.csdn.net/20180316233750391)




