# java加解密之DES多种使用方式 - z69183787的专栏 - CSDN博客
2017年09月20日 17:47:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：339
个人分类：[工具-加解密](https://blog.csdn.net/z69183787/article/category/5974293)

最近逆向分析中，常常遇到加解密的内容，接触得比较多的加解密算法一般有对称的DES和AES，非对称的RSA，单向的MD5等。
这里就DES的使用研究进行一个随笔记录，并不会说其实现原理，原理可以自行百度。
DES作为56位秘钥加密目前已经被容易破解了，尽管如此，在一些简单而又不是很敏感的数据加密中还是有它的身影。
在Android逆向分析中，在被混淆的代码里，往往你能看到一个方法a的参数是byte[ ]，返回值也是byte[ ]，而且在里面又有“DES”的字符串，如果不继续分析其内容而直接猜测的话，这个方法a 究竟是DES的什么工作模式？不妨猜测一下：
首先，加密还是解密？用的是 Cypher.ENCRYPT 还是 Cypher.DECRYPT？ x 2
接着，秘钥的生成方式？用的是SecretKeyFactory 还是 KeyGenerator 抑或 直接用 SecretKeySpec ？ x3
最后，块加密的模式？ ECB，CBC，OFB，CFB等？是否有填充操作？ x4+
x2x3x4+=24+
24种以上的结果~
虽然逆向分析中只需要知道其用法即可，细节的研究对其分析并不会有多大的效率上的帮助，但我还是去折腾了一番~
----------------------开始分割线-----------------------
这里，我先贴出一种比较常用的调用方式：DES     使用SecretKeyFactory 的秘钥     选择CBC模式     进行加解密。
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- publicclass DESCryptography {  
- 
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- 
-         String content="aaaaaaaabbbbbbbbaaaaaaaa";  
-         String key="01234567";  
- 
-         System.out.println("加密前："+byteToHexString(content.getBytes()));  
- byte[] encrypted=DES_CBC_Encrypt(content.getBytes(), key.getBytes());  
-         System.out.println("加密后："+byteToHexString(encrypted));  
- byte[] decrypted=DES_CBC_Decrypt(encrypted, key.getBytes());  
-         System.out.println("解密后："+byteToHexString(decrypted));  
-     }  
- 
- publicstaticbyte[] DES_CBC_Encrypt(byte[] content, byte[] keyBytes){        
- try {  
-             DESKeySpec keySpec=new DESKeySpec(keyBytes);  
-             SecretKeyFactory keyFactory=SecretKeyFactory.getInstance("DES");              
-             SecretKey key=keyFactory.generateSecret(keySpec);         
- 
-             Cipher cipher=Cipher.getInstance("DES/CBC/PKCS5Padding");  
-             cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(keySpec.getKey()));             
- byte[] result=cipher.doFinal(content);  
- return result;  
-         } catch (Exception e) {  
- // TODO Auto-generated catch block
-             System.out.println("exception:"+e.toString());  
-         }  
- returnnull;  
-     }  
- 
- publicstaticbyte[] DES_CBC_Decrypt(byte[] content, byte[] keyBytes){        
- try {  
-             DESKeySpec keySpec=new DESKeySpec(keyBytes);  
-             SecretKeyFactory keyFactory=SecretKeyFactory.getInstance("DES");  
-             SecretKey key=keyFactory.generateSecret(keySpec);  
- 
-             Cipher cipher=Cipher.getInstance("DES/CBC/PKCS5Padding");  
-             cipher.init(Cipher.DECRYPT_MODE, key, new IvParameterSpec(keyBytes));  
- byte[] result=cipher.doFinal(content);  
- return result;  
-         } catch (Exception e) {  
- // TODO Auto-generated catch block
-             System.out.println("exception:"+e.toString());  
-         }  
- returnnull;  
-     }  
- 
- publicstatic String byteToHexString(byte[] bytes) {  
-         StringBuffer sb = new StringBuffer(bytes.length);  
-         String sTemp;  
- for (int i = 0; i < bytes.length; i++) {  
-             sTemp = Integer.toHexString(0xFF & bytes[i]);  
- if (sTemp.length() < 2)  
-                 sb.append(0);  
-             sb.append(sTemp.toUpperCase());  
-         }  
- return sb.toString();  
-     }  
- 
- privatestaticbyte toByte(char c) {  
- byte b = (byte) "0123456789ABCDEF".indexOf(c);  
- return b;  
-      }  
- }  
运行结果：
![](https://img-blog.csdn.net/20160811115724562)
这是一个标准的使用方法~
-----------------------结束分割线----------------------------
若是这么简单就好了，对于有强迫症的我，感觉没把它完全弄清楚，感觉就是不爽~~~
然而，我还是讲一下其它方法的研究吧。
1、  加密还是解密。
只需要在Cypher.init初始化时设置Mode即可，即Cypher.ENCRYPT 或 Cypher.DECRYPT。
2、  秘钥的产生。
这个有点恶心， 我研究了一下 KeyGenerator， KeyPairGenerator，KeyFactory，SecretKeyFactory这四个类，是有区别的。
根据 Oracle 的 [Standard Algorithm Name Documentation](http://docs.oracle.com/javase/7/docs/technotes/guides/security/StandardNames.html#SecretKeyFactory)提供的说明：
KeyGenerator和SecretKeyFactory，都是javax.crypto包的，生成的key主要是提供给AES，DES，3DES，MD5，SHA1等 对称 和 单向 加密算法。
KeyPairGenerator和KeyFactory，都是java.security包的，生成的key主要是提供给DSA，RSA， EC等 非对称加密算法。
在这里，因为是DES算法，所以我主要是研究上面那两个类，那么，Generator 和 Factory 有什么区别呢？
额。。。鉴于水平有限，只能说，两者都是对key进行加工，生成新的key，只是生成的方法不一样，生成的结果也不一样。
顺便一提，还有一个同时实现了Key和KeySpec接口的SecretKeySpec类，该类可以直接用keyBytes生成原始的key，即没有加工过，不同上面的Factory和Generator。
我将3种生成key的方法和结果贴出来：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- publicstaticvoid printKey(byte[] keyBytes){  
- try{  
- //第一种，Factory
-         DESKeySpec keySpec=new DESKeySpec(keyBytes);  
-         SecretKeyFactory keyFactory=SecretKeyFactory.getInstance("DES");              
-         SecretKey key1=keyFactory.generateSecret(keySpec);    
- 
- //第二种, Generator
-         KeyGenerator keyGenerator=KeyGenerator.getInstance("DES");  
-         keyGenerator.init(56, new SecureRandom(keyBytes));//key为8个字节，实际用了56位； 后面随机数用key作为种子seed生成
-         SecretKey key2=keyGenerator.generateKey();  
- 
- //第三种， SecretKeySpec
-         SecretKey key3=new SecretKeySpec(keyBytes, "DES");//SecretKeySpec类同时实现了Key和KeySpec接口
- 
- //打印
-         System.out.println("key1："+byteToHexString(key1.getEncoded()));  
-         System.out.println("key2："+byteToHexString(key2.getEncoded()));  
-         System.out.println("key3："+byteToHexString(key3.getEncoded()));  
- 
-     }catch(Exception e){  
-         System.out.println(e.toString());  
-     }     
- }  
打印结果：
![](https://img-blog.csdn.net/20160811140042975)
就结果看来，key3是原始的，没有加工过；key1是经过Factory生成的，只是将其稍微加工了一下，加了一些置换操作；而key2是经过Generator通过SecureRandom生成的，所以大幅度变形。
3、块加密的模式以及数据填充。
看下面的一行代码：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- Cipher cipher=Cipher.getInstance("DES/CBC/NoPadding");  
Cipher加密器初始化需要一个字符串，字符串里提供了三种设置。
一是，加解密算法；二是，加解密模式；三是，是否需要填充。
常见的模式有四种：
ECB（电码本模式），CBC（加密块链模式），OFB（输出反馈模式），CFB（加密反馈模式），这些模式的原理在网上都有很多，这里不讲。
ECB模式简单，缺点是块加密的内容容易重复，会被统计分析攻击；
CBC,  OFB,  CFB三个模式，都是根据前面加密块的内容，对key进行新一轮处理后再，再对下一数据块进行处理，如此类推下去，这样一来，加密的强度也有所增强。他们都需要用到初始化向量IV，英文大概是Initialization Vector的缩写吧。
填充：
对于加密，因为DES是块加密，数据长度必须是8的倍数，然而实际上加密前的明文getBytes()后基本不会恰好是8的倍数，所以一般需要进行填充，填充的规则这里不说，想知道的百度吧，反正这个只需要设置参数 PKCS5Padding ，JDK就帮你填充了，若不填充，且数据长度不是8倍数，则会抛异常；
对于解密，一般来说加密的数据长度本身就是8的倍数，所以只需要NoPadding就可以了，若加密的数据长度不是8，就需要用PKCS5Padding，否则解密出来后的明文尾巴的会比原明文的尾巴多出好几位填充数据。（实测其它模式是，会抛 Given final block not properly padded的异常，这个要结合实际切换一下NoPadding 和 PKCS5Padding）
最上面已经把CBC模式的代码贴出来了，我就不贴ECB，OFB，CFB模式的了，因为只需要改动两行代码就OK
ECB模式：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- Cipher cipher=Cipher.getInstance("DES/ECB/PKCS5Padding");  
- cipher.init(Cipher.ENCRYPT_MODE, key);  
CBC模式：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- Cipher cipher=Cipher.getInstance("DES/CBC/PKCS5Padding");  
- cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(keyBytes));//直接用keyBytes初始化IV
OFB模式：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- Cipher cipher=Cipher.getInstance("DES/OFB/PKCS5Padding");  
- cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(keyBytes));//直接用keyBytes初始化IV
CFB模式：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52180768#)[copy](http://blog.csdn.net/qq_18870023/article/details/52180768#)
- Cipher cipher=Cipher.getInstance("DES/CFB/PKCS5Padding");  
- cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(keyBytes));//直接用keyBytes初始化IV
写法基本是一样的，只是改一下参数即可。
最后调用doFinal方法即可~
