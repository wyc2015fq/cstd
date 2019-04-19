# JAVA 几种常见的时间格式转换 - sxf_123456的博客 - CSDN博客
2018年10月22日 18:12:46[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：256
```
一、时间戳转化为yyyy-MM-dd HH:mm:ss日期格式
import java.text.SimpleDateFormat;
import java.util.Date;
```
```
Long curr = System.currentTimeMillis(); //获取当前时间戳 (毫秒)
System.out.println(curr); //1540202972921
Date date = new Date(curr);
SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
System.out.println(simpleDateFormat.format(date)); //2018-10-22 18:09:32
```
二、`时间戳转化为yyyy-MM-dd HH:mm:ss日期格式`
Long curr = new Date().getTime();//获取当前时间戳 (毫秒)
System.out.println(curr); //1540202972921
SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
System.out.println(simpleDateFormat.format(curr)); //2018-10-22 18:09:32
