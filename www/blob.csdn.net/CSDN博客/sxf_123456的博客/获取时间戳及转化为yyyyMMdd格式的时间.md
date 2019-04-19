# 获取时间戳及转化为yyyyMMdd格式的时间 - sxf_123456的博客 - CSDN博客
2018年09月12日 22:39:24[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1885
### 1、获取当前时间戳两种方法:
-  System.currentTimeMillis();  #1536764057392  微秒
-  new Date().getTime();  #1536764057392  微秒
### 2、时间戳转化为yyyyMMdd格式时间
- 
SimpleDateFormat df = new SimpleDateFormat("yyyyMMdd HH:mm:ss");
String date_1 = df.format(System.currentTimeMillis()); # 20180912 22:54:17
String date_2 = df.format(new Date().getTime()); # 20180912 22:54:17
### 3、yyyyMMdd HH:mm:ss 格式时间转化为时间戳
- 
String date_1 = "20180910 22:58:10";
SimpleDateFormat df = new SimpleDateFormat("yyyyMMdd HH:mm:ss");
System.out.println(String.valueOf(df.parse(date_1).getTime())); #1536591490000  微秒
