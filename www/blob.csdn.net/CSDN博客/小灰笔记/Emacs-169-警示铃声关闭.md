# Emacs-169-警示铃声关闭 - 小灰笔记 - CSDN博客





2018年03月19日 20:46:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：227








![](https://img-blog.csdn.net/20180319204614859)



       操作不成功的警铃声音十分烦人，尤其是在Windows系统下，上面是关闭这个的方法。不过有意思的是我在实践的过程中发现把上面的这个值设置为nil并不能够起到禁用的作用，起作用必须把值设置为0。

       具体的配置代码很简单了：

(setq visible-bell 0)





