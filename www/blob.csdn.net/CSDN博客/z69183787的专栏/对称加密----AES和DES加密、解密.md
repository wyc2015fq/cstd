# 对称加密----AES和DES加密、解密 - z69183787的专栏 - CSDN博客
2016年09月09日 17:39:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3480
[https://blog.csdn.net/qq_18870023/article/details/52183755](https://blog.csdn.net/qq_18870023/article/details/52183755)
目前主流的加密方式有：（对称加密）AES、DES        （非对称加密）RSA、DSA
调用AES/DES加密算法包最精要的就是下面两句话：
Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
cipher.init(Cipher.ENCRYPT_MODE, key, zeroIv);
CBC是工作模式，DES一共有电子密码本模式（[ECB](http://www.hudong.com/wiki/ECB)）、加密分组链接模式（[CBC](http://www.hudong.com/wiki/CBC)）、加密反馈模式（[CFB](http://www.hudong.com/wiki/CFB)）和输出反馈模式（[OFB](http://write.blog.csdn.net/postedit)）四种模式，
PKCS5Padding是填充模式，还有其它的填充模式：
然后，cipher.init（)一共有三个参数：Cipher.ENCRYPT_MODE, key, zeroIv，zeroIv就是初始化向量。
工作模式、填充模式、初始化向量这三种因素一个都不能少。否则，如果你不指定的话，那么就要程序就要调用默认实现。
代码：
1.加密：
例：加密方式： AES128(CBC/PKCS5Padding) + Base64, 私钥：lianghuilonglong,要加密的字符串abcdefg
public String encrypt(){
　　String text = "abcdefg";   //要加密的字符串
　　String key = "lianghuilonglong";　 //私钥   AES固定格式为128/192/256 bits.即：16/24/32bytes。DES固定格式为128bits，即8bytes。
　　String iv   = "aabbccddeeffgghh";　　//初始化向量参数，AES 为16bytes. DES 为8bytes.
**     如果使用Java环境，则需要打JCE的补丁，以支持长度超过8位的Key     JCE补丁修复方式：将“local_policy.jar”及“US_export_policy.jar”拷贝到指定{JAVA_HOME}\jre\lib\security路径下。     下载路径：JDK7 [http://www.oracle.com/technetwork/java/javase/downloads/jce-7-download-432124.html](http://www.oracle.com/technetwork/java/javase/downloads/jce-7-download-432124.html)**
　　Key keySpec = new SecretKeySpec(key.getBytes(), "AES");    //两个参数，第一个为私钥字节数组， 第二个为加密方式 AES或者DES
　　IvParameterSpec ivSpec = new IvParameterSpec(iv.getBytes);     
　　Cipher cipher = Cipher.getIntance("AES/CBC/PKCS5Padding");　  //实例化加密类，参数为加密方式，要写全
　　cipher.init(Cipher.ENCRYPT_MODE,  keySpec, ivSpec);　　           //初始化，此方法可以采用三种方式，按服务器要求来添加。（1）无第三个参数（2）第三个参数为SecureRandom random = new SecureRandom();中random对象，随机数。(AES不可采用这种方法)（3）采用此代码中的IVParameterSpec
　　//cipher.init(Cipher.ENCRYPT_MODE, keySpec);
　　//SecureRandom random = new SecureRandom();
　　//cipher.init(Cipher.ENCRYPT_MODE, keySpec, random);
　　byte [] b = cipher.doFinal(text.getBytes());　                   //加密操作,返回加密后的字节数组，然后需要编码。主要编解码方式有Base64, HEX, UUE, 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　7bit等等。此处看服务器需要什么编码方式
　　String ret = Base64.encode(b);                                       //Base64、HEX等编解码
}
2.解密：
逻辑： 将服务器返回的加密字符串，先用Base64、HEX等解码成byte[]，再用加密时相同的加密方式及key进行解密。加密与解密代码几乎相同。唯一区别为在Cipher类init时，工作模式为Cipher.DECRYPT_MODE。代码：
//加密方式： AES128(CBC/PKCS5Padding) + Base64, 私钥：lianghuilonglong
public String deCiphering(){
　　String keySpec = "lianghuilonglong";
　　String textDeCipher = "UstyI8JoQOty8egSMFQfig==";   //从服务器返回的加密字符串，需要解密的字符串
　　byte [] byte = Base64.decode(textDeCipher);　　         //先用Base64解码
　　IvParaterSpec ivSpec = new IvParaterSpec("abcdefghabcdefgh".getBytes());       
　　Key key = new SecretKeySpec(keySpec.getBytes(), "AES");
　　Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
　　cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);               //与加密时不同MODE:Cipher.DECRYPT_MODE
　　byte [] ret = cipher.doFinal(byte);
　　return new String(ret, "utf-8");
}
-----------------------------------------------------------
上一次的随笔记录了关于DES加解密算法的使用，这次的随笔是关于AES的。
因为各种算法名字，模式名字很多都是三个字母，太容易混淆了，所以单独写成一篇。
AES加密也是用得比较多的，在逆向分析中也经常遇到，加密的强度也是妥妥的，传说，它是为了解决DES的过时而征集的算法~~根源就不说了。
-----------------开始分割线---------------------
同DES那篇博客一样，原理不说，说了也不懂，直接先上一个标准的加解密使用方法：
[java][view plain](https://blog.csdn.net/qq_18870023/article/details/52183755#)[copy](https://blog.csdn.net/qq_18870023/article/details/52183755#)
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
首先，加解密。
跟DES一样，直接设置Cypher.ENCRYPT 或 Cypher.DECRYPT 即可。
其次，key的生成模式。
上面的代码用了 KeyGenerator 生成key，并没有用 SecretFactory，原因是 抛异常 SecretFactory 找不到 该算法，为什么？我一脸懵逼~
![](https://img-blog.csdn.net/20160811171134351)
根据 Oracle的文档 [Standard Algorithm Name Documentation](http://docs.oracle.com/javase/7/docs/technotes/guides/security/StandardNames.html#SecretKeyFactory) 里明明写着支持 “AES” 的啊。。。
![](https://img-blog.csdn.net/20160811171951893)
好吧，那么只能暂时用Generator生成key了，但这里接着又发生了一个问题，KeyGenerator初始化时，为什么只能128位？不是说可以192， 256的吗？
看看jdk先：
![](https://img-blog.csdn.net/20160811173102943)
JDK文档中标注着128，额，好吧，那就128吧~
除了用KeyGenerator生成key之外，也可以直接使用原始key：
[java][view plain](https://blog.csdn.net/qq_18870023/article/details/52183755#)[copy](https://blog.csdn.net/qq_18870023/article/details/52183755#)
- SecretKey key2=new SecretKeySpec(keyBytes, "AES");  
不过，需要注意的是，这种方法的key的原始长度必须是128位，不然抛异常~~
而Generator的不会，因为它会根据设置的位数（128）和SecureRandom自动生成相应位数（128）的key。
最后，块加密模式和填充。
这里跟上一篇介绍DES的一样，毕竟AES和DES都是对称加密的块加密类型。
最后的最后，还有一个值得注意的问题。
Cipher.getInstance("AES")    和    Cipher.getInstance("AES/CBC/PKCS5Padding")   一样，即默认。
附上jdk说明：
![](https://img-blog.csdn.net/20160811175236000)
好了，随笔到此为止~
