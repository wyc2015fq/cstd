# 批量导入数据到oracle - 左直拳的马桶_日用桶 - CSDN博客
2018年01月13日 15:30:17[左直拳](https://me.csdn.net/leftfist)阅读数：301
有一些数据需要录入到数据库。这些数据位于excel和word文档里。
导入数据嘛，这有啥难的？
不过，这些数据并不是从数据库里导出来的，ID还要用序列号生成，我不知道导入的话，该咋整。也许是可以导入的，但我不知道方法。
作为一名程序员，当然不可能一条条地录进去，这会玷污了程序员这个称号。我用了一个方法，可以导进去，记录如下： 
思路：
> 
1、创建一个临时用的表temp  
  2、开启编辑模式，将数据粘贴到temp  
  3、insert into 目标表 select from temp
![这里写图片描述](https://img-blog.csdn.net/20180113152923529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180113152840254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
insert into meta(id,category,parentid,name,p1)
select seq_meta.nextval,9,52,t.name,t.p1 from temp t;
```
