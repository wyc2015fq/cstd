# Emacs-123-使用init.el作为启动配置文件 - 小灰笔记 - CSDN博客





2018年03月09日 19:29:45[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：729








       默认生成的自动加载文件是.emacs，这个我用过。配置文件管理用的文件夹是.emacs.d。如果是使用spacemacs，似乎还有一个.spacemacs可以用。

       自动加载的文件还可以使用init.el，使用的方式是直接把这个文件放到.emacs.d文件夹中即可。


![](https://img-blog.csdn.net/20180309192932458)



       具体就像上面，只需要两步：

1.    home目录创建.emacs.d；

2.    .emacs.d中创建init.el。

小结：

1.    init.el不可以为空，否则报错；

2.    相比其他的方式，这种方式只有一个目录方便管理以及备份。





