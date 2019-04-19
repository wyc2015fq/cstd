# 好用的jar包 - BlueSky - CSDN博客
2014年12月05日 22:52:28[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：510
个人分类：[java编程](https://blog.csdn.net/ASIA_kobe/article/category/2863525)
可以使用commons-io-2.*.jar包，可以非常方便的将读取的文件转换为字符。
比如： File file = new File("d:/index");
       String s  = FileUtils.readFileToString(file);
