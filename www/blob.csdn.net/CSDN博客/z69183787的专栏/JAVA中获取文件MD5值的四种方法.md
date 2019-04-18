# JAVA中获取文件MD5值的四种方法 - z69183787的专栏 - CSDN博客
2018年02月10日 19:58:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：729
[http://blog.csdn.net/sike_java/article/details/78538762](http://blog.csdn.net/sike_java/article/details/78538762)
　　JAVA中获取文件MD5值的四种方法其实都很类似，因为核心都是通过JAVA自带的MessageDigest类来实现。获取文件MD5值主要分为三个步骤，第一步获取文件的byte信息，第二步通过MessageDigest类进行MD5加密，第三步转换成16进制的MD5码值。几种方法的不同点主要在第一步和第三步上。具体可以看下面的例子：
方法一、
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 　　private final static String[] strHex = { "0", "1", "2", "3", "4", "5",
 2             "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
 3 
 4     public static String getMD5One(String path) {
 5         StringBuffer sb = new StringBuffer();
 6         try {
 7             MessageDigest md = MessageDigest.getInstance("MD5");
 8             byte[] b = md.digest(FileUtils.readFileToByteArray(new File(path)));
 9             for (int i = 0; i < b.length; i++) {
10                 int d = b[i];
11                 if (d < 0) {
12                     d += 256;
13                 }
14                 int d1 = d / 16;
15                 int d2 = d % 16;
16                 sb.append(strHex[d1] + strHex[d2]);
17             }
18         } catch (NoSuchAlgorithmException e) {
19             e.printStackTrace();
20         } catch (IOException e) {
21             e.printStackTrace();
22         }
23         return sb.toString();
24     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　方法一是比较原始的一种实现方法，首先将文件一次性读入内存，然后通过MessageDigest进行MD5加密，最后再手动将其转换为16进制的MD5值。
方法二、
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 　　public static String getMD5Two(String path) {
 2         StringBuffer sb = new StringBuffer("");
 3         try {
 4             MessageDigest md = MessageDigest.getInstance("MD5");
 5             md.update(FileUtils.readFileToByteArray(new File(path)));
 6             byte b[] = md.digest();
 7             int d;
 8             for (int i = 0; i < b.length; i++) {
 9                 d = b[i];
10                 if (d < 0) {
11                     d = b[i] & 0xff;
12                     // 与上一行效果等同
13                     // i += 256;
14                 }
15                 if (d < 16)
16                     sb.append("0");
17                 sb.append(Integer.toHexString(d));
18             }
19         } catch (NoSuchAlgorithmException e) {
20             e.printStackTrace();
21         } catch (IOException e) {
22             e.printStackTrace();
23         }
24         return sb.toString();
25     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　方法二与方法一不同的地方主要是在步骤三，这里借助了Integer类的方法实现16进制的转换，比方法一更简洁一些。PS：JAVA中byte是有负数的，代码中&0xff的操作与计算机中数据存储的原理有关，即负数存储的是二进制的补码，有兴趣的童鞋可以挖一下，这里不展开说。
方法三、
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 　　public static String getMD5Three(String path) {
 2         BigInteger bi = null;
 3         try {
 4             byte[] buffer = new byte[8192];
 5             int len = 0;
 6             MessageDigest md = MessageDigest.getInstance("MD5");
 7             File f = new File(path);
 8             FileInputStream fis = new FileInputStream(f);
 9             while ((len = fis.read(buffer)) != -1) {
10                 md.update(buffer, 0, len);
11             }
12             fis.close();
13             byte[] b = md.digest();
14             bi = new BigInteger(1, b);
15         } catch (NoSuchAlgorithmException e) {
16             e.printStackTrace();
17         } catch (IOException e) {
18             e.printStackTrace();
19         }
20         return bi.toString(16);
21     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　方法三与前面两个方法相比，在读入文件信息上有点不同。这里是分多次将一个文件读入，对于大型文件而言，比较推荐这种方式，占用内存比较少。步骤三则是通过BigInteger类提供的方法进行16进制的转换，与方法二类似。
方法四、
```
1 　　DigestUtils.md5Hex(new FileInputStream(path));
```
　　方法四应该是最便捷的吧，哈哈，好东西要留在最后，如果你只需要使用标准的MD5，其实一行代码就够了，JAVA自带的commons-codec包就提供了获取16进制MD5值的方法。其底层实现上，也是分多次将一个文件读入，类似方法三。所以性能上也不错。
总结：其实方法都是类似的，推荐使用方法四，简洁且性能不错，当然，如果要做一些调整什么的，可以根据自己的需求进行方法的选择。
PS：其实还有一个重点，就是如何知道自己生成的MD5值是否正确呢？
　　方法很多，其实有一个挺简单的方法，不需要另外安装什么软件。使用windows自带的命令即可：certutil -hashfile [文件路径] MD5，例子如下：
![](http://images2017.cnblogs.com/blog/711223/201710/711223-20171024174436723-2012526484.png)
