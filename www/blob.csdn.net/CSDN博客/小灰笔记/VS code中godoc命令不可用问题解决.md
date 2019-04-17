# VS code中godoc命令不可用问题解决 - 小灰笔记 - CSDN博客





2017年12月09日 17:20:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1352








       折腾了VIM和Emacs之后，又转向了VS，同时又学起了Go语言。使用VS Code的时候遇到了 The “godoc”command is not available的提示，显示如下：

![](https://img-blog.csdn.net/20171209171841429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       看了信息超链接的说明，没有看完就因为不好理解关掉了，转战Github，倒是在Github上看到了有人对此的描述以及自己的解决方案。具体如下：

![](https://img-blog.csdn.net/20171209171855113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       大意就是说，自己创建了一个包含上面代码的文本放到了相应的文件夹，这只是一个临时的解决方法。

       我按照上面的说法在我的电脑上解决，创建了文件并放到了如下位置：

![](https://img-blog.csdn.net/20171209171913579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       文件中的代码如下：



```
#!/usr/bin/bash
go doc $
```


       重启了VS Code，但是问题并没有解决。思考了一下，上面的截图中，解决方案是在UNIX的超级用户下进行的，而bin中一般都是可执行文件。再说，文本中分明就是一个小小的shell脚本，看来得让这个文件成为一个Windows下的批处理文件才能够解决。那么接下来的处理也不复杂了，把这个文件加个.bat的后缀名即可。

![](https://img-blog.csdn.net/20171209172011766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       重启VS Code问题果然得到了解决而且在编码的时候会不断出现编码用法的提示，确实比较人性化。使用时候的截图如下：

![](https://img-blog.csdn.net/20171209172024111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






