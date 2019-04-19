# DjangoUeditor项目的集成 - =朝晖= - 博客园
# [DjangoUeditor项目的集成](https://www.cnblogs.com/dhcn/p/7130535.html)
        DjangoUeditor这个项目,出品人已经不再提供维护支持。
       最近在一个使用到aliyun oss的项目里集成了一次这个东西，当然我之前在普通文件上传的北京下已经集成过很多次了。
       主要修改的东西就是在哪个文件上传函数里面，最方便的改法就是文件上传存储相关的操作全部用Django的default_storage替代，主要是两个函数式一个save，一个url函数。这个项目里面用到了一个第三方写的oss storagelib，不过有点小问题，其url函数的实现有问题，于是把包文件copy到自己目录里面hacking修改了一下。
       至于DjangoUeditor项目的修改一个是上面说的文件存储操作的处理，一个就是上传相对目录的生成。
       再其他的集成可能就在比如对Ueditor的富文本内容的过滤，可以自定义一个Forms Field来做.窗口前端的定制就需要自己修改Ueditor的配置或者某些需要hackingueditor的[js](http://lib.csdn.net/base/javascript)代码。

