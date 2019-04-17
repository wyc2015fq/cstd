# Emacs_237_spacemacs中使用ag - 小灰笔记 - CSDN博客





2018年07月24日 23:28:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：106








       在我自己的配置之中成功实现了projectile的grep以及ag，使用起来倒是还凑合。这可以说是我一直不想切换Spacemacs的一个理由。因为在Spacemacs中使用工程管理一直没有成功。

       今天又做了因此尝试居然成功了，原来ag在Spacemacs中并不是shell中的ag，而是需要一个专门的ag插件。

       安装方法如下：

![](https://img-blog.csdn.net/201807242327557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       按照上面截图，增加安装的插件。然后重启Emacs安装插件，安装后工程即可正常使用。

       调用方式： C-c pss

       测试了一下，这个搜索速度果真比较快。



