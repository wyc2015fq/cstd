
# 让DOSBox启动后自动执行命令 - 迂者-贺利坚的专栏 - CSDN博客

2018年07月03日 09:04:05[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1028


使用DOSBox，可以在win下模拟DOS，自些好玩的工作。例如，学习8086汇编。
每次启动DOSBox后，都要挂载、转盘符。遇上调试的程序老死，就不好玩了。
可以想想办法，让这些固定“套路”自动化。
注意到DOSBox初启时，有一个窗口，如下显示：
![这里写图片描述](https://img-blog.csdn.net/20180703085829587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180703085829587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
就这个文件，掌管DOSBox启动后执行的命令。
找到这个文件。
![这里写图片描述](https://img-blog.csdn.net/20180703090016614?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180703090016614?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
用记事本就可以编辑。
拉到最下面，找到[autoexec]部分，补充命令如下：
![这里写图片描述](https://img-blog.csdn.net/20180703090211652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180703090211652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后重启DOSBox就行了。
截屏？不给。
自己做吧！

[
](https://img-blog.csdn.net/20180703090211652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4aGVsaWppYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
