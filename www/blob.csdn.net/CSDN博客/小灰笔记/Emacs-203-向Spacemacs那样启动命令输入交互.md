# Emacs-203-向Spacemacs那样启动命令输入交互 - 小灰笔记 - CSDN博客





2018年03月26日 18:22:49[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：115








       其实，Spacemacs中的命令输入交互激活就是通过把相应的功能绑定到了特定的按键，也就是SPC SPC。如果我们安装了leader-key的插件之后，也能够比较轻松的实现类似的功能。

       比如，我发现helm-M-x的命令跟Spacemacs的命令交互行为最为相似，我可以在Leader的按键绑定中加入如下的配置：

```
(evil-leader/set-key
 "SPC" 'helm-M-x)
```


       自然，这个也需要安装了helm。配置完了并使其生效之后，SPC SPC即可激活命令输入了。




