# Emacs-205-Emacs的管理模块化 - 小灰笔记 - CSDN博客





2018年03月27日 18:54:24[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：86








       模块化的主要技巧就是如何实现对其他文件的调用。

       在elisp中，只要一个文件用(provide ‘block_name)的形式在最后“声明”一下这个模块，并且将相应的文件加到加载目录中。这样，就可以使用require引用相应的配置。

       一个配置示例如下：

```
(global-company-mode 1)
(provide 'g-company)
```

       把g-company.el放入到一个可以让emacs找到的家在目录，这样就能够使用require引用这个配置模块。

       这个并没有进行什么理论性学习，纯粹是从别人的配置中魔方学来的。通过几个模块的尝试，这种管理方式能够比较简单的实现配置的模块化。




