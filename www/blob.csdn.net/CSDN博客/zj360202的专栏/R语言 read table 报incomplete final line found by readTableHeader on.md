# R语言 read.table 报incomplete final line found by readTableHeader on - zj360202的专栏 - CSDN博客





2013年08月19日 17:37:35[zj360202](https://me.csdn.net/zj360202)阅读数：6986








```java
> read.table("d:/tt_2",header=FALSE,quote="")
    V1
1 null
2 null
3 0est
4    0
警告信息：
In read.table("d:/tt_2", header = FALSE, quote = "") :
  incomplete final line found by readTableHeader on 'd:/tt_2'
```




数据文件内容



```java
null
null
0est
0
```
报错的主要原因是最后一行没有换行，所以将数据文件改成





```java

```
null
null
0est
0

```java

```



就不报错了





```java
> read.table("d:/tt_2",header=FALSE,quote="")
    V1
1 null
2 null
3 0est
4    0
```








