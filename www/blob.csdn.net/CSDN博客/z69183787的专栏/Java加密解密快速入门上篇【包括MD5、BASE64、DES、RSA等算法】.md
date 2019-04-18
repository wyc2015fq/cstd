# Java加密解密快速入门上篇【包括MD5、BASE64、DES、RSA等算法】 - z69183787的专栏 - CSDN博客
2017年05月22日 18:10:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：298
个人分类：[工具-加解密](https://blog.csdn.net/z69183787/article/category/5974293)

 搞Java的攻城狮应该比较欣喜若狂的，不仅Sun公司的JDK提供了庞大的类库，而且还有众多的开源组织和个人不断地丰富着Java的生态系统。没错的，诸如MD5、BASE64、DES、RSA等经典加密解密算法，JDK都有提供专门的API来封装相关操作。程序猿只需要了解算法的思想，以及学习如何使用API即可。![吻](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-kiss.gif)当然，针对BASE64算法，JDK仅仅提供了非常标准的实现，在某些场景下无法满足应用需求。在最后一部分，将学习Apache提供的Commons
 Codec![大笑](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-laughing.gif)对BASE64算法的扩展实现，以满足实践的需要。
<一>. MD5加密算法：
       消息摘要算法第五版(Message Digest Algorithm)，是一种单向加密算法，只能加密、无法解密。然而MD5加密算法已经被中国山东大学王小云教授成功破译![哭](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-cry.gif)，但是在安全性要求不高的场景下，MD5加密算法仍然具有应用价值。
 1. 创建md5对象： 
Java代码 
- MessageDigest md5 = MessageDigest.getInstance("md5");  
 2.  进行加密操作： 
Java代码 
- byte[] cipherData = md5.digest(plainText.getBytes());  
 3.  将其中的每个字节转成十六进制字符串：byte类型的数据最高位是符号位，通过和0xff进行与操作，转换为int类型的正整数。 
Java代码 
- String toHexStr = Integer.toHexString(cipher & 0xff);  
 4. 如果该正数小于16(长度为1个字符)，前面拼接0占位：确保最后生成的是32位字符串。 
Java代码 
- builder.append(toHexStr.length() == 1 ? "0" + toHexStr : toHexStr);  
 5. 加密转换之后的字符串为：c0bb4f54f1d8b14caf6fe1069e5f93ad 
 6. 完整的MD5算法应用如下所示： 
Java代码 
- /**
-  * 功能简述: 测试MD5单向加密.
-  * @throws Exception
-  */
- @Test
- publicvoid test01() throws Exception {  
-     String plainText = "Hello , world !";  
-     MessageDigest md5 = MessageDigest.getInstance("md5");  
- byte[] cipherData = md5.digest(plainText.getBytes());  
-     StringBuilder builder = new StringBuilder();  
- for(byte cipher : cipherData) {  
-         String toHexStr = Integer.toHexString(cipher & 0xff);  
-         builder.append(toHexStr.length() == 1 ? "0" + toHexStr : toHexStr);  
-     }  
-     System.out.println(builder.toString());  
- //c0bb4f54f1d8b14caf6fe1069e5f93ad
- }  
<二>. 使用BASE64进行加密/解密：
        使用BASE64算法通常用作对二进制数据进行加密，加密之后的数据不易被肉眼识别。严格来说，经过BASE64加密的数据其实没有安全性可言![保密](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-sealed.gif)，因为它的加密解密算法都是公开的，典型的防菜鸟不防程序猿的呀。 经过标准的BASE64算法加密后的数据， 通常包含/、+、=等特殊符号，不适合作为url参数传递，幸运的是Apache的Commons
 Codec模块提供了对BASE64的进一步封装。  (参见最后一部分的说明)
 1. 使用BASE64加密： 
Java代码 
- BASE64Encoder encoder = new BASE64Encoder();  
- String cipherText = encoder.encode(plainText.getBytes());  
  2. 使用BASE64解密： 
Java代码 
- BASE64Decoder decoder = new BASE64Decoder();  
- plainText = new String(decoder.decodeBuffer(cipherText));  
  3. 完整代码示例： 
Java代码 
- /**
-  * 功能简述: 使用BASE64进行双向加密/解密.
-  * @throws Exception
-  */
- @Test
- publicvoid test02() throws Exception {  
-     BASE64Encoder encoder = new BASE64Encoder();  
-     BASE64Decoder decoder = new BASE64Decoder();  
-     String plainText = "Hello , world !";  
-     String cipherText = encoder.encode(plainText.getBytes());  
-     System.out.println("cipherText : " + cipherText);  
- //cipherText : SGVsbG8gLCB3b3JsZCAh
-     System.out.println("plainText : " +   
- new String(decoder.decodeBuffer(cipherText)));  
- //plainText : Hello , world !
<三>. 使用DES对称加密/解密：
         数据加密标准算法(Data Encryption Standard)，和BASE64最明显的区别就是有一个工作密钥，该密钥既用于加密、也用于解密，并且要求密钥是一个长度至少大于8位的字符串。使用DES加密、解密的核心是确保工作密钥的安全性。![叫喊](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-yell.gif)
 1. 根据key生成密钥： 
Java代码 
- DESKeySpec keySpec = new DESKeySpec(key.getBytes());  
- SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("des");  
- SecretKey secretKey = keyFactory.generateSecret(keySpec);  
  2. 加密操作： 
Java代码 
- Cipher cipher = Cipher.getInstance("des");  
- cipher.init(Cipher.ENCRYPT_MODE, secretKey, new SecureRandom());  
- byte[] cipherData = cipher.doFinal(plainText.getBytes());  
  3. 为了便于观察生成的加密数据，使用BASE64再次加密： 
Java代码 
- String cipherText = new BASE64Encoder().encode(cipherData);  
     生成密文如下：PtRYi3sp7TOR69UrKEIicA== 
  4. 解密操作： 
Java代码 
- cipher.init(Cipher.DECRYPT_MODE, secretKey, new SecureRandom());  
- byte[] plainData = cipher.doFinal(cipherData);  
- String plainText = new String(plainData);  
  5. 完整的代码demo： 
Java代码 
- /**
-  * 功能简述: 使用DES对称加密/解密.
-  * @throws Exception
-  */
- @Test
- publicvoid test03() throws Exception {  
-     String plainText = "Hello , world !";  
-     String key = "12345678";    //要求key至少长度为8个字符
- 
-     SecureRandom random = new SecureRandom();  
-     DESKeySpec keySpec = new DESKeySpec(key.getBytes());  
-     SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("des");  
-     SecretKey secretKey = keyFactory.generateSecret(keySpec);  
- 
-     Cipher cipher = Cipher.getInstance("des");  
-     cipher.init(Cipher.ENCRYPT_MODE, secretKey, random);  
- byte[] cipherData = cipher.doFinal(plainText.getBytes());  
-     System.out.println("cipherText : " + new BASE64Encoder().encode(cipherData));  
- //PtRYi3sp7TOR69UrKEIicA==
- 
-     cipher.init(Cipher.DECRYPT_MODE, secretKey, random);  
- byte[] plainData = cipher.doFinal(cipherData);  
-     System.out.println("plainText : " + new String(plainData));  
- //Hello , world !
- }  
<四>. 使用RSA非对称加密/解密：
        RSA算法是非对称加密算法的典型代表，既能加密、又能解密。和对称加密算法比如DES的明显区别在于用于加密、解密的密钥是不同的。使用RSA算法，只要密钥足够长(一般要求1024bit)，加密的信息是不能被破解的。![天真](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-innocent.gif)用户通过https协议访问服务器时，就是使用非对称加密算法进行数据的加密、解密操作的。
       服务器发送数据给客户端时使用私钥（private key）进行加密，并且使用加密之后的数据和私钥生成数字签名（digital signature）并发送给客户端。客户端接收到服务器发送的数据会使用公钥（public key）对数据来进行解密，并且根据加密数据和公钥验证数字签名的有效性，防止加密数据在传输过程中被第三方进行了修改。
       客户端发送数据给服务器时使用公钥进行加密，服务器接收到加密数据之后使用私钥进行解密。
 1. 创建密钥对KeyPair：
Java代码 
- KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("rsa");  
- keyPairGenerator.initialize(1024);  //密钥长度推荐为1024位.
- KeyPair keyPair = keyPairGenerator.generateKeyPair();  
  2. 获取公钥/私钥：
Java代码 
- PublicKey publicKey = keyPair.getPublic();  
- PrivateKey privateKey = keyPair.getPrivate();  
  3. 服务器数据使用私钥加密：
Java代码 
- Cipher cipher = Cipher.getInstance("rsa");  
- cipher.init(Cipher.ENCRYPT_MODE, privateKey, new SecureRandom());  
- byte[] cipherData = cipher.doFinal(plainText.getBytes());  
  4. 用户使用公钥解密：
Java代码 
- cipher.init(Cipher.DECRYPT_MODE, publicKey, new SecureRandom());  
- byte[] plainData = cipher.doFinal(cipherData);  
  5. 服务器根据私钥和加密数据生成数字签名：
Java代码 
- Signature signature  = Signature.getInstance("MD5withRSA");  
- signature.initSign(privateKey);  
- signature.update(cipherData);  
- byte[] signData = signature.sign();  
  6. 用户根据公钥、加密数据验证数据是否被修改过：
Java代码 
- signature.initVerify(publicKey);  
- signature.update(cipherData);  
- boolean status = signature.verify(signData);  
  7. RSA算法代码demo：![尴尬](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-embarassed.gif)
Java代码 
- /**
-  * 功能简述: 使用RSA非对称加密/解密.
-  * @throws Exception
-  */
- @Test
- publicvoid test04() throws Exception {  
-     String plainText = "Hello , world !";  
- 
-     KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("rsa");  
-     keyPairGenerator.initialize(1024);  
-     KeyPair keyPair = keyPairGenerator.generateKeyPair();  
- 
-     PublicKey publicKey = keyPair.getPublic();  
-     PrivateKey privateKey = keyPair.getPrivate();  
- 
-     Cipher cipher = Cipher.getInstance("rsa");  
-     SecureRandom random = new SecureRandom();  
- 
-     cipher.init(Cipher.ENCRYPT_MODE, privateKey, random);  
- byte[] cipherData = cipher.doFinal(plainText.getBytes());  
-     System.out.println("cipherText : " + new BASE64Encoder().encode(cipherData));  
- //gDsJxZM98U2GzHUtUTyZ/Ir/NXqRWKUJkl6olrLYCZHY3RnlF3olkWPZ35Dwz9BMRqaTL3oPuyVq
- //sehvHExxj9RyrWpIYnYLBSURB1KVUSLMsd/ONFOD0fnJoGtIk+T/+3yybVL8M+RI+HzbE/jdYa/+
- //yQ+vHwHqXhuzZ/N8iNg=
- 
-     cipher.init(Cipher.DECRYPT_MODE, publicKey, random);  
- byte[] plainData = cipher.doFinal(cipherData);  
-     System.out.println("plainText : " + new String(plainData));  
- //Hello , world !
- 
-     Signature signature  = Signature.getInstance("MD5withRSA");  
-     signature.initSign(privateKey);  
-     signature.update(cipherData);  
- byte[] signData = signature.sign();  
-     System.out.println("signature : " + new BASE64Encoder().encode(signData));  
- //ADfoeKQn6eEHgLF8ETMXan3TfFO03R5u+cQEWtAQ2lRblLZw1DpzTlJJt1RXjU451I84v3297LhR
- //co64p6Sq3kVt84wnRsQw5mucZnY+jRZNdXpcbwh2qsh8287NM2hxWqp4OOCf/+vKKXZ3pbJMNT/4
- ///t9ewo+KYCWKOgvu5QQ=
- 
-     signature.initVerify(publicKey);  
-     signature.update(cipherData);  
- boolean status = signature.verify(signData);  
-     System.out.println("status : " + status);  
- //true
- }  
        Ok，这个blog的内容够长的了，还是在下一篇博客中对加密解密封装成一个工具类吧，然后简单介绍一下Commons Codec对BASE64的扩展支持！![微笑](http://www.iteye.com/javascripts/tinymce/plugins/emotions/img/smiley-smile.gif)
