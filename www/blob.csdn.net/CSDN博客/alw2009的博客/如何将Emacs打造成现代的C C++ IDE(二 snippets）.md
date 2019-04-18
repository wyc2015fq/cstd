# 如何将Emacs打造成现代的C/C++ IDE(二 snippets） - alw2009的博客 - CSDN博客
2018年12月26日 22:16:03[traveler_zero](https://me.csdn.net/alw2009)阅读数：92
今天将来尝试snippet功能， 使用这个功能了之后， 编码速度将会得到一个很大的提升。
今天的内容如下：
1.如何启用snippet？
按照之前的步骤配置好之后， 其实还差一步就可以使用snippet了。 因为我们使用的是company的自动补全， 我们需要把company-yasnippet添加到c-c++的backends里。在.spacemacs的user-config部分添加。
```
(add-hook 'c++-mode-hook
            (lambda ()
              (setq company-backends '(company-yasnippet))))
```
2.个性化设置
可以在dotspacemacs-configuration-layers里的auto-completion的auto-completion-private-snippets-directory指定自定义的snippets目录， snippets的写法参考[http://joaotavora.github.io/yasnippet/](http://joaotavora.github.io/yasnippet/)
3.效果演示
![](https://img-blog.csdnimg.cn/20181226220353442.png)
