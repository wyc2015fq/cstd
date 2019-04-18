# java加解密之AES使用 - z69183787的专栏 - CSDN博客
2017年09月20日 16:45:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2362
上一次的随笔记录了关于DES加解密算法的使用，这次的随笔是关于AES的。
因为各种算法名字，模式名字很多都是三个字母，太容易混淆了，所以单独写成一篇。
AES加密也是用得比较多的，在逆向分析中也经常遇到，加密的强度也是妥妥的，传说，它是为了解决DES的过时而征集的算法~~根源就不说了。
-----------------开始分割线---------------------
同DES那篇博客一样，原理不说，说了也不懂，直接先上一个标准的加解密使用方法：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52183755#)[copy](http://blog.csdn.net/qq_18870023/article/details/52183755#)
- publicclass AESCrptography {  
- 
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- 
-         String content="hello";  
-         String key="aaaaaaaa";  
-         String iv="abcdefghijklmnop";  
- 
-         System.out.println("加密前："+byteToHexString(content.getBytes()));  
- byte[ ] encrypted=AES_CBC_Encrypt(content.getBytes(), key.getBytes(), iv.getBytes());  
-         System.out.println("加密后："+byteToHexString(encrypted));  
- byte[ ] decrypted=AES_CBC_Decrypt(encrypted, key.getBytes(), iv.getBytes());  
-         System.out.println("解密后："+byteToHexString(decrypted));  
-     }  
- 
- publicstaticbyte[] AES_CBC_Encrypt(byte[] content, byte[] keyBytes, byte[] iv){  
- 
- try{  
-             KeyGenerator keyGenerator=KeyGenerator.getInstance("AES");  
-             keyGenerator.init(128, new SecureRandom(keyBytes));  
-             SecretKey key=keyGenerator.generateKey();  
-             Cipher cipher=Cipher.getInstance("AES/CBC/PKCS5Padding");  
-             cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(iv));  
- byte[] result=cipher.doFinal(content);  
- return result;  
-         }catch (Exception e) {  
- // TODO Auto-generated catch block
-             System.out.println("exception:"+e.toString());  
-         }   
- returnnull;  
-     }  
- 
- publicstaticbyte[] AES_CBC_Decrypt(byte[] content, byte[] keyBytes, byte[] iv){  
- 
- try{  
-             KeyGenerator keyGenerator=KeyGenerator.getInstance("AES");  
-             keyGenerator.init(128, new SecureRandom(keyBytes));//key长可设为128，192，256位，这里只能设为128
-             SecretKey key=keyGenerator.generateKey();  
-             Cipher cipher=Cipher.getInstance("AES/CBC/PKCS5Padding");  
-             cipher.init(Cipher.DECRYPT_MODE, key, new IvParameterSpec(iv));  
- byte[] result=cipher.doFinal(content);  
- return result;  
-         }catch (Exception e) {  
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
- }  
运行结果：
![](https://img-blog.csdn.net/20160811170323622)
AES的一个标准使用方法到此为止
-------------------结束分割线--------------------
怎么可能善罢甘休，DES都研究了，AES也得研究研究~~
**首先，加解密。**
跟DES一样，直接设置Cypher.ENCRYPT 或 Cypher.DECRYPT 即可。
**其次，key的生成模式。**
上面的代码用了 KeyGenerator 生成key，并没有用 SecretFactory，原因是 抛异常 SecretFactory 找不到 该算法，为什么？我一脸懵逼~
![](https://img-blog.csdn.net/20160811171134351)
根据 Oracle的文档 [Standard Algorithm Name Documentation](http://docs.oracle.com/javase/7/docs/technotes/guides/security/StandardNames.html#SecretKeyFactory) 里明明写着支持 “AES”
 的啊。。。
![](https://img-blog.csdn.net/20160811171951893)
好吧，那么只能暂时用Generator生成key了，但这里接着又发生了一个问题，KeyGenerator初始化时，为什么只能128位？不是说可以192， 256的吗？
看看jdk先：
![](https://img-blog.csdn.net/20160811173102943)
JDK文档中标注着128，额，好吧，那就128吧~
除了用KeyGenerator生成key之外，也可以直接使用原始key：
**[java]**[view
 plain](http://blog.csdn.net/qq_18870023/article/details/52183755#)[copy](http://blog.csdn.net/qq_18870023/article/details/52183755#)
- SecretKey key2=new SecretKeySpec(keyBytes, "AES");  
不过，需要注意的是，这种方法的key的原始长度必须是128位，不然抛异常~~
而Generator的不会，因为它会根据设置的位数（128）和SecureRandom自动生成相应位数（128）的key。
**最后，块加密模式和填充。**
这里跟上一篇介绍DES的一样，毕竟AES和DES都是对称加密的块加密类型。
**最后的最后，还有一个值得注意的问题。**
Cipher.getInstance("AES")    和    Cipher.getInstance("AES/CBC/PKCS5Padding")   一样，即默认。
附上jdk说明：
![](https://img-blog.csdn.net/20160811175236000)
好了，随笔到此为止~
