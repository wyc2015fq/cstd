# AES加密CBC模式兼容互通四种编程语言平台【PHP、Javascript、Java、C#】 - 零度的博客专栏 - CSDN博客
2016年11月01日 11:48:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：1122
       密码学中的高级加密标准（Advanced Encryption Standard，AES），又称高级加密标准Rijndael加密法，是美国联邦政府采用的一种区块加密标准。这个标准用来替代原先的DES，已经被多方分析且广为全世界所使用。经过五年的甄选流程，高级加密标准由美国国家标准与技术研究院 （NIST）于2001年11月26日发布于FIPS PUB197，并在2002年5月26日成为有效的标准。2006年，高级加密标准已然成为对称密钥加密中最流行的算法之一。该算法为比利时密码学家Joan
 Daemen和VincentRijmen所设计，结合两位作者的名字，以Rijndael之命名之，投稿高级加密标准的甄选流程。（Rijdael的发音近于 "Rhinedoll"。）
       AES加密模式和填充方式如下：（其实还有还几种填充方式没写上，开始时候也在这里绕了一下）
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/51383012#)[copy](http://blog.csdn.net/z69183787/article/details/51383012#)[print](http://blog.csdn.net/z69183787/article/details/51383012#)[?](http://blog.csdn.net/z69183787/article/details/51383012#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1756937/fork)
- 算法/模式/填充                16字节加密后数据长度        不满16字节加密后长度  
- AES/CBC/NoPadding             16                          不支持  
- AES/CBC/PKCS5Padding          3216
- AES/CBC/ISO10126Padding       3216
- AES/CFB/NoPadding             16                          原始数据长度  
- AES/CFB/PKCS5Padding          3216
- AES/CFB/ISO10126Padding       3216
- AES/ECB/NoPadding             16                          不支持  
- AES/ECB/PKCS5Padding          3216
- AES/ECB/ISO10126Padding       3216
- AES/OFB/NoPadding             16                          原始数据长度  
- AES/OFB/PKCS5Padding          3216
- AES/OFB/ISO10126Padding       3216
- AES/PCBC/NoPadding            16                          不支持  
- AES/PCBC/PKCS5Padding         3216
- AES/PCBC/ISO10126Padding      3216
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
算法/模式/填充                16字节加密后数据长度        不满16字节加密后长度
AES/CBC/NoPadding             16                          不支持
AES/CBC/PKCS5Padding          32                          16
AES/CBC/ISO10126Padding       32                          16
AES/CFB/NoPadding             16                          原始数据长度
AES/CFB/PKCS5Padding          32                          16
AES/CFB/ISO10126Padding       32                          16
AES/ECB/NoPadding             16                          不支持
AES/ECB/PKCS5Padding          32                          16
AES/ECB/ISO10126Padding       32                          16
AES/OFB/NoPadding             16                          原始数据长度
AES/OFB/PKCS5Padding          32                          16
AES/OFB/ISO10126Padding       32                          16
AES/PCBC/NoPadding            16                          不支持
AES/PCBC/PKCS5Padding         32                          16
AES/PCBC/ISO10126Padding      32                          16
```
      更多关于加密模式内容：[http://blog.sina.com.cn/s/blog_679daa6b0100zmpp.html](http://blog.sina.com.cn/s/blog_679daa6b0100zmpp.html)
      看到这么多模式，已经有点头晕了，那我的目标是希望找到 PHP、Javascript、Java、C# 的AES加密模式一个交集；又经过一轮查找，资讯了百度谷歌这两位老师之后，找到了一篇关于PHP和Java的AES互通兼容加密文章，看完之后发现了原来PHP的AES加密填充只有ZeroPadding（补零 - 因为数据长度不是16的整数倍就需要填充），而Java是没有这种填充模式，杯具的只能自己写一个了，那Java的填充模式就用NoPadding（不填充内容）；
Java端代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/51383012#)[copy](http://blog.csdn.net/z69183787/article/details/51383012#)[print](http://blog.csdn.net/z69183787/article/details/51383012#)[?](http://blog.csdn.net/z69183787/article/details/51383012#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1756937/fork)
- /*
-  * To change this template, choose Tools | Templates
-  * and open the template in the editor.
-  */
- 
- /**
-  *
-  * @author Jacker
-  */
- 
- import javax.crypto.Cipher;  
- import javax.crypto.spec.IvParameterSpec;  
- import javax.crypto.spec.SecretKeySpec;  
- import sun.misc.BASE64Decoder;  
- 
- publicclass Encryption  
- {  
- publicstaticvoid main(String args[]) throws Exception {  
-         System.out.println(encrypt());  
-         System.out.println(desEncrypt());  
-     }  
- 
- publicstatic String encrypt() throws Exception {  
- try {  
-             String data = "Test String";  
-             String key = "1234567812345678";  
-             String iv = "1234567812345678";  
- 
-             Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");  
- int blockSize = cipher.getBlockSize();  
- 
- byte[] dataBytes = data.getBytes();  
- int plaintextLength = dataBytes.length;  
- if (plaintextLength % blockSize != 0) {  
-                 plaintextLength = plaintextLength + (blockSize - (plaintextLength % blockSize));  
-             }  
- 
- byte[] plaintext = newbyte[plaintextLength];  
-             System.arraycopy(dataBytes, 0, plaintext, 0, dataBytes.length);  
- 
-             SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");  
-             IvParameterSpec ivspec = new IvParameterSpec(iv.getBytes());  
- 
-             cipher.init(Cipher.ENCRYPT_MODE, keyspec, ivspec);  
- byte[] encrypted = cipher.doFinal(plaintext);  
- 
- returnnew sun.misc.BASE64Encoder().encode(encrypted);  
- 
-         } catch (Exception e) {  
-             e.printStackTrace();  
- returnnull;  
-         }  
-     }  
- 
- publicstatic String desEncrypt() throws Exception {  
- try
-         {  
-             String data = "2fbwW9+8vPId2/foafZq6Q==";  
-             String key = "1234567812345678";  
-             String iv = "1234567812345678";  
- 
- byte[] encrypted1 = new BASE64Decoder().decodeBuffer(data);  
- 
-             Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");  
-             SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");  
-             IvParameterSpec ivspec = new IvParameterSpec(iv.getBytes());  
- 
-             cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec);  
- 
- byte[] original = cipher.doFinal(encrypted1);  
-             String originalString = new String(original);  
- return originalString;  
-         }  
- catch (Exception e) {  
-             e.printStackTrace();  
- returnnull;  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author Jacker
 */
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import sun.misc.BASE64Decoder;
public class Encryption
{
    public static void main(String args[]) throws Exception {
        System.out.println(encrypt());
        System.out.println(desEncrypt());
    }
    public static String encrypt() throws Exception {
        try {
            String data = "Test String";
            String key = "1234567812345678";
            String iv = "1234567812345678";
            Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");
            int blockSize = cipher.getBlockSize();
            byte[] dataBytes = data.getBytes();
            int plaintextLength = dataBytes.length;
            if (plaintextLength % blockSize != 0) {
                plaintextLength = plaintextLength + (blockSize - (plaintextLength % blockSize));
            }
            byte[] plaintext = new byte[plaintextLength];
            System.arraycopy(dataBytes, 0, plaintext, 0, dataBytes.length);
            
            SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");
            IvParameterSpec ivspec = new IvParameterSpec(iv.getBytes());
            cipher.init(Cipher.ENCRYPT_MODE, keyspec, ivspec);
            byte[] encrypted = cipher.doFinal(plaintext);
            return new sun.misc.BASE64Encoder().encode(encrypted);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
    public static String desEncrypt() throws Exception {
        try
        {
            String data = "2fbwW9+8vPId2/foafZq6Q==";
            String key = "1234567812345678";
            String iv = "1234567812345678";
            
            byte[] encrypted1 = new BASE64Decoder().decodeBuffer(data);
            
            Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");
            SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");
            IvParameterSpec ivspec = new IvParameterSpec(iv.getBytes());
            
            cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec);
 
            byte[] original = cipher.doFinal(encrypted1);
            String originalString = new String(original);
            return originalString;
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
```
       这里需要强调的就是Java的填充模式是NoPadding，用自己的编写的补零填充内容；
       最后，就是javascript端的实现，这个是最杯具的，花的时间是最多，也是难倒了很多刚入门的小菜；一开始我是先想到在os找一插件快速解决（CryptoJS），但是结果并不如意，加密解密后的内容总是乱码不对，最后找啊找，看了很多的国外的资料，翻樯去google论坛和stackoverflow等网站，最后得到了一些零星的资料，终于解决掉问题，原来是密匙的编码导致；（中间试了很多很多代码，翻了很多资料，筛选了无数资料，原来坚持是有回报的）
         Javascript端代码：
**[javascript]**[view plain](http://blog.csdn.net/z69183787/article/details/51383012#)[copy](http://blog.csdn.net/z69183787/article/details/51383012#)[print](http://blog.csdn.net/z69183787/article/details/51383012#)[?](http://blog.csdn.net/z69183787/article/details/51383012#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1756937/fork)
- <script src="aes.js"></script>  
- <script src="pad-zeropadding.js"></script>  
- <script>  
- var data = "Test String";  
- var key  = CryptoJS.enc.Latin1.parse('1234567812345678');  
- var iv   = CryptoJS.enc.Latin1.parse('1234567812345678');  
- 
- //加密
- var encrypted = CryptoJS.AES.encrypt(data,key,{iv:iv,mode:CryptoJS.mode.CBC,padding:CryptoJS.pad.ZeroPadding});  
- 
- document.write(encrypted.ciphertext);  
- document.write('<br/>');  
- document.write(encrypted.key);  
- document.write('<br/>');  
- document.write(encrypted.iv);  
- document.write('<br/>');  
- document.write(encrypted.salt);  
- document.write('<br/>');  
- document.write(encrypted);  
- document.write('<br/>');  
- 
- 
- //解密
- var decrypted = CryptoJS.AES.decrypt(encrypted,key,{iv:iv,padding:CryptoJS.pad.ZeroPadding});  
- console.log(decrypted.toString(CryptoJS.enc.Utf8));  
- </script>  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
<script src="aes.js"></script>
<script src="pad-zeropadding.js"></script>
<script>
var data = "Test String";
var key  = CryptoJS.enc.Latin1.parse('1234567812345678');
var iv   = CryptoJS.enc.Latin1.parse('1234567812345678');
//加密
var encrypted = CryptoJS.AES.encrypt(data,key,{iv:iv,mode:CryptoJS.mode.CBC,padding:CryptoJS.pad.ZeroPadding});
document.write(encrypted.ciphertext);
document.write('<br/>');
document.write(encrypted.key);
document.write('<br/>');
document.write(encrypted.iv);
document.write('<br/>');
document.write(encrypted.salt);
document.write('<br/>');
document.write(encrypted);
document.write('<br/>');
//解密
var decrypted = CryptoJS.AES.decrypt(encrypted,key,{iv:iv,padding:CryptoJS.pad.ZeroPadding});
console.log(decrypted.toString(CryptoJS.enc.Utf8));
</script>
```
      按照官方例子就是失败，核心的aes.js又加密混淆了！唉！想找点线索都难。最后需要提醒总结的是，密匙key和IV需要一致，编码要正确，不然会绕很多冤枉路，希望能帮到以后需要用的人；补充一下，就是全部加密都是 AES/CBC/ZeroPadding 128位模式。 
