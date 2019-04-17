# mapreduce中报内部类classnotfound - zj360202的专栏 - CSDN博客





2013年03月15日 14:18:33[zj360202](https://me.csdn.net/zj360202)阅读数：1322








```java
必须要加上这句，不然会在执行这个mr的时候找不到内部类中的mapper或reducer
```

```java
job1.setJarByClass(PlatFormMemberVisitTimeForDetailPVMR.class);
```




