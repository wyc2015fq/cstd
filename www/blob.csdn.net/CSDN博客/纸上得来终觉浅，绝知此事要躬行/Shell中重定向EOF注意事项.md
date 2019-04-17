# Shell中重定向EOF注意事项 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月08日 23:29:03[boonya](https://me.csdn.net/boonya)阅读数：738标签：[EOF](https://so.csdn.net/so/search/s.do?q=EOF&t=blog)
个人分类：[Shell](https://blog.csdn.net/boonya/article/category/8324499)









原文：[https://blog.csdn.net/iamlaosong/article/details/40509399](https://blog.csdn.net/iamlaosong/article/details/40509399)

我们经常在shell脚本程序中用<<EOF重定向输入，将我们输入的命令字符串作为一个执行程序的输入，这样，我们就不需要在那个程序环境中手工输入命令，以便自动执行我们需要的功能，例如：

```bash
sqlplus emssxjk/emssxjk <<EOF
    select count(*) from sncn_yxyj where create_date like sysdate;
    EOF
```



其中的SQL语句相当于在sqlplus程序环境中输入的，这样输入的内容夹在两个EOF之间，可长可短，EOF也可以换成其他任意的字符，大小写不论，只要成对出现即可，例如：

```bash
sqlplus emssxjk/emssxjk <<STD
    select count(*) from sncn_yxyj where create_date like sysdate;
    STD
```



当然这个标志性字符不能用保留字，最常用的还是EOF。

需要注意的是，第一个EOF必须以重定向字符<<开始，第二个EOF必须顶格写，否则会报错。

再看一个自动FTP的例子：



```bash
#ftp data to 10.178.37.244
    rq=`date +%Y%m%d`
    cd /data/product/song
    ftp -i -n <<!
    open 10.178.37.244
    user oracle post
    cd ems
    bin
    put ${rq}.dat
    bye
    !
```





