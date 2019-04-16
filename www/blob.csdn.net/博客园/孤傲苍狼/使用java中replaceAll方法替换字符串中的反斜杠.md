# 使用java中replaceAll方法替换字符串中的反斜杠 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [使用java中replaceAll方法替换字符串中的反斜杠](https://www.cnblogs.com/xdp-gacl/p/3912784.html)



　　今天在项目中使用java中replaceAll方法将字符串中的反斜杠("\")替换成空字符串("")，结果出现如下的异常：

```
1 java.util.regex.PatternSyntaxException: Unexpected internal error near index 1 \^
```

　　上网找了一下错误的原因：**在regex中"\\"表示一个"\"，在java中一个"\"也要用"\\"表示。这样，前一个"\\"代表regex中的"\"，后一个"\\"代表java中的"\"。**所以要想使用replaceAll方法将字符串中的反斜杠("\")替换成空字符串("")，则需要这样写：str.replaceAll("\\\\","");

　　写一段测试代码演示上面出现的异常：

```
1 String s="C:\盘";
2 **s.replaceAll("\\","");**
```

　　使用上面的代码会导致

```
1 java.util.regex.PatternSyntaxException: **Unexpected internal error near index 1 \^**
```

　　要想将"C:\盘"中的"\"替换成空字符串，正确的写法是：

```
1 **s.replaceAll("\\\\","");**
```

　　这样就可以正常替换了。









