# java实现对称加密AES和DES的加密、解密 - 零度的博客专栏 - CSDN博客
2016年11月01日 11:41:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：8262
        目前主流的加密方式有：1、对称加密：AES、DES      2、非对称加密：RSA、DSA。
        本文主要讲解java实现调用AES/DES加密算法包，调用过程最精要的就是下面两句话：
```
Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
cipher.init(Cipher.ENCRYPT_MODE, key, zeroIv);
```
        其中，DES是采用的算法，CBC是工作模式，DES一共有电子密码本模式（[ECB](http://www.hudong.com/wiki/ECB)）、加密分组链接模式（[CBC](http://www.hudong.com/wiki/CBC)）、加密反馈模式（[CFB](http://www.hudong.com/wiki/CFB)）和输出反馈模式（[OFB](http://write.blog.csdn.net/postedit)）四种模式，PKCS5Padding是填充模式，还有其它的填充模式。
       而方法cipher.init();一共有三个参数：Cipher.ENCRYPT_MODE, key, zeroIv，Cipher.ENCRYPT_MODE是加密/解密工作模式，key是密钥（模式） zeroIv就是初始化向量。工作模式、填充模式、初始化向量这三种因素一个都不能少。否则，如果你不指定的话，那么就要程序就要调用默认实现。
       下面分别实现对称加密/解密的代码。
### 加密：
加密方式： AES128(CBC/PKCS5Padding) + Base64,  私钥：lianghuilonglong，要加密的字符串：abcdefg。
     实现如下：
```
public String encrypt(){
　　  String text = "abcdefg";   //要加密的字符串
　　  String key = "lianghuilonglong";　 //私钥： AES固定格式为128/192/256 bits.即：16/24/32bytes。DES固定格式为128bits，即8bytes。
　　  String iv   = "aabbccddeeffgghh";　　//初始化向量参数，AES 为16bytes. DES 为8bytes.
　　  Key keySpec = new SecretKeySpec(key.getBytes(), "AES");    //两个参数，第一个为私钥字节数组， 第二个为加密方式 AES或者DES
　　  IvParameterSpec ivSpec = new IvParameterSpec(iv.getBytes);  // 初始化向量  
　　  Cipher cipher = Cipher.getIntance("AES/CBC/PKCS5Padding");　 // 实例化加密类，参数为加密方式，要写全
　　  cipher.init(Cipher.ENCRYPT_MODE,  keySpec, ivSpec);　　      // 初始化，此方法可以采用三种方式，按服务器要求来添加。
                                                                  //（1）无第三个参数
                                                                  //（2）第三个参数为SecureRandom random = new SecureRandom();中random对象，随机数。(AES不可采用这种方法)
                                                                  //（3）采用此代码中的IVParameterSpec
　　//cipher.init(Cipher.ENCRYPT_MODE, keySpec);
　　//SecureRandom random = new SecureRandom();
　　//cipher.init(Cipher.ENCRYPT_MODE, keySpec, random);
　　  byte [] b = cipher.doFinal(text.getBytes());//加密操作,返回加密后的字节数组，然后需要编码。主要编解码方式有Base64, HEX, UUE,7bit等等。
　　
     String ret = Base64.encode(b);   //Base64、HEX等编解码
}
```
### 解密：
     解密逻辑：将服务器返回的加密字符串，先用Base64、HEX等解码成byte[]，再用加密时相同的加密方式及key进行解密。加密与解密代码几乎相同。唯一区别为在Cipher类init时，工作模式为Cipher.DECRYPT_MODE。
     加密方式： AES128(CBC/PKCS5Padding) + Base64, 私钥：lianghuilonglong
代码实现：
```
public String deCiphering(){
　　String keySpec = "lianghuilonglong";
　　String textDeCipher = "UstyI8JoQOty8egSMFQfig==";   //从服务器返回的加密字符串，需要解密的字符串
　　byte [] byte = Base64.decode(textDeCipher);　　      //先用Base64解码
　　IvParaterSpec ivSpec = new IvParaterSpec("abcdefghabcdefgh".getBytes());       
　　Key key = new SecretKeySpec(keySpec.getBytes(), "AES");
　　Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
　　cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);    //与加密时不同MODE:Cipher.DECRYPT_MODE
　　byte [] ret = cipher.doFinal(byte);
　　return new String(ret, "utf-8");
}
```
