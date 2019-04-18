# Java 使用Calendar 工具类时 设置时区 - z69183787的专栏 - CSDN博客
2014年02月19日 16:20:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1161
```java
TimeZone.setDefault(TimeZone.getTimeZone("GMT+8"));
			Calendar c = Calendar.getInstance(TimeZone.getTimeZone("GMT+8"));
			c.setTime(date);
```
