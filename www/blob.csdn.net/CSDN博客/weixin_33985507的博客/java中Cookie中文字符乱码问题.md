# java中Cookie中文字符乱码问题 - weixin_33985507的博客 - CSDN博客
2014年11月21日 10:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
如果Cookie中的Value 中有中文字符出现，在加入Cookie的时候，会出现下面的错误：
　 java.lang.IllegalArgumentException: Control character in cookie value or attribute.
当我们设定Cookie的Value的值得时候：
　　cookie.setValue(ret); 改为如下方式尽心编码！　　
　　cookie.setValue(URLEncoder.encode(ret, "utf-8"));使用指定的编码机制将字符串转换为 `application/x-www-form-urlencoded` 格式，中文字符是两个字节。
当我们在取出Cookie的Value的值时，同样利用相应的解码：
　　String val = cookie.getValue();
　　val = URLDecoder.decode(val, "utf-8"); 使用指定的编码机制对 `application/x-www-form-urlencoded` 字符串解码。
