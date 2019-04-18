# javax.crypto.BadPaddingException: Given final block not properly padded解决方案 - z69183787的专栏 - CSDN博客
2017年09月27日 18:34:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3987
JAVA的AES加密解密在windows上测试一切正常，上传到空间上在解密时就出现错误。空间是Linux系统
查看日志发现出现此异常
**javax.crypto.BadPaddingException: Given final block not properly padded**
后面百度了一下终于解决了，在生成key的时候出现错误的
原来的代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
private Key initKeyForAES(String key) throws NoSuchAlgorithmException {
        if (null == key || key.length() == 0) {
            throw new NullPointerException("key not is null");
        }
        SecretKeySpec key2 = null;try {
            KeyGenerator kgen = KeyGenerator.getInstance("AES");
            kgen.init(128, new SecureRandom(key.getBytes()));
            SecretKey secretKey = kgen.generateKey();
            byte[] enCodeFormat = secretKey.getEncoded();
            key2 = new SecretKeySpec(enCodeFormat, "AES");
        } catch (NoSuchAlgorithmException ex) {
            throw new NoSuchAlgorithmException();
        }
        return key2;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
主要是红色部分的问题
修改后代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
private Key initKeyForAES(String key) throws NoSuchAlgorithmException {
        if (null == key || key.length() == 0) {
            throw new NullPointerException("key not is null");
        }
        SecretKeySpec key2 = null;
        SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
        random.setSeed(key.getBytes());
        try {
            KeyGenerator kgen = KeyGenerator.getInstance("AES");
            kgen.init(128, random);
            SecretKey secretKey = kgen.generateKey();
            byte[] enCodeFormat = secretKey.getEncoded();
            key2 = new SecretKeySpec(enCodeFormat, "AES");
        } catch (NoSuchAlgorithmException ex) {
            throw new NoSuchAlgorithmException();
        }
        return key2;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
其实就是SecureRandom创建的方式不同而引起的错误，具体原理我也不懂，因为加密解密代码都是网上搜的，具体没研究过这个。总之能解决问题就好。
我是从这找到解决方案的：http://wenku.baidu.com/link?url=wOibKHENi2Z5gFOL5prjGBE8RES1dZEZlrvfY1NTl89QJWtTwXUNLmgEXVYWGBGXR25oRvOKPJTI5M3o95KW0yIHwgFVEnJiZt1-0YvRQua
随心地记录一些东西，以备不时之需。
