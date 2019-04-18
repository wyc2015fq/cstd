# 从技术角度深入PKI体系之四消息摘要算法之MD - weixin_33985507的博客 - CSDN博客
2018年02月12日 13:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
# MD家族
总所周知，MD5算法是目前典型的消息摘要算法，其前身有MD2、MD4算法，它们会产生一个128位的信息摘要。如果将这个128位的二进制摘要信息换算成十六进制，可以得到一个32位的字符串，这就是数字指纹。
|算法|长度|实现|
|----|----|----|
|MD2|128|JDK|
|MD4|128|第三方|
|MD5|128|JDK|
这里的第三方指的是非JDK实现方式，依赖第三方扩展实现，如：Bouncy Castle或者apache codec等。他们除了可以实现JDK的方法，还扩展了JDK没有的MD算法。
# MD算法实现
## JDK实现MD5、MD2
```
//MD5
MessageDigest md5 = MessageDigest.getInstance("MD5");
byte[] md5Res = md5.digest(src.getBytes());
//MD2
MessageDigest md2 = MessageDigest.getInstance("MD2");
byte[] md2Res = md2.digest(src.getBytes());
```
JDK自带方式，加密结果为128位byte[]，需要转为32位的16进制字符串（字节数组 转 16进制）。
## Bouncy Castle实现
```
//bc实现 md2
//Digest digest = new MD2Digest();
//bc实现 md5
//Digest digest = new MD5Digest();
//bc实现 md4
Digest digest = new MD4Digest();
digest.update(src.getBytes(),0,src.length());
byte[] mdres = new byte[digest.getDigestSize()];
digest.doFinal(mdres,0);
String mdStr = org.bouncycastle.util.encoders.Hex.toHexString(mdres);
```
Bouncy Castle这个就非常叼了，基本上都可以实现，最后的mdStr 就是16进制字符串了。
## apache codec实现
```
String mdHex = DigestUtils.md5Hex(src);
String mdHex = DigestUtils.md2Hex(src)
```
至此，MD算法基本上就这些了。这几种方式，可以说Bouncy Castle最全了，需要添加他们各自的依赖包。根据项目的实际需求，选择实现方式。
> 
上述示例代码中src表示需要摘要的原文。类型为String。
加密方法也有许多重载，也只是传入不同类型的数据。大家可以深入研究。
