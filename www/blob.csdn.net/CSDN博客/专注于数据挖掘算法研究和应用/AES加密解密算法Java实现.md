# AES加密解密算法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月16日 15:01:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3294








AES加密算法是密码学中的高级加密标准（Advanced Encryption Standard，AES），又称Rijndael加密法，是美国联邦政府采用的一种区块加密标准。这个标准用来替代原先的DES，已经被多方分析且广为全世界所使用。


AES 是一个新的可以用于保护电子数据的加密算法。明确地说，AES 是一个迭代的、对称密钥分组的密码，它可以使用128、192 和 256 位密钥，并且用 128 位（16字节）分组加密和解密数据。与公共密钥密码使用密钥对不同，对称密钥密码使用相同的密钥加密和解密数据。通过分组密码返回的加密数据 的位数与输入数据相同。迭代加密使用一个循环结构，在该循环中重复置换（permutations ）和替换(substitutions）输入数据。





参考代码如下：



```java
package com.gddx.des;

import java.security.Key;
import java.security.SecureRandom;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;

public class AesUtil {
	private final static String AES = "AES";
    private final static String charset = "UTF-8";
    private final static String defKey = "gddxbdhp";//8字节key长度
    /***
     * 加密
     * @param plain
     * @param keySeed
     * @param charSet
     * @return 输出密文 16进制
     */
    public static String encrypt(String plain, String keySeed, String charSet) {
    	try {
    		byte[] byteContent = plain.getBytes(charSet);
    		Cipher cipher = Cipher.getInstance(AES);
    		cipher.init(Cipher.ENCRYPT_MODE, getKey(keySeed));

            byte[] buf = cipher.doFinal(byteContent);
            // byte[]转16进制
            StringBuffer sb = new StringBuffer();
            for (int i = 0; i < buf.length; i++) {
                String hex = Integer.toHexString(buf[i] & 0xFF);
                if (hex.length() == 1) {
                    hex = '0' + hex;
                }
                sb.append(hex.toUpperCase());
            }
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    
    /***
     * 解密 输入16进制的字符串
     * 
     * @param layer
     * @param keySeed
     * @param charSet
     * @return 原文
     */
    public static String decrypt(String layer, String keySeed, String charSet) {
        try {
            byte[] byteContent = new byte[layer.length() / 2];
            if (layer.length() < 1) {
                return null;
            }
            // 将16进制转换为二进制
            for (int i = 0; i < layer.length() / 2; i++) {
                int high = Integer.parseInt(layer.substring(i * 2, i * 2 + 1), 16);
                int low = Integer.parseInt(layer.substring(i * 2 + 1, i * 2 + 2), 16);
                byteContent[i] = (byte) (high * 16 + low);
            }

            Cipher cipher = Cipher.getInstance(AES);
            cipher.init(Cipher.DECRYPT_MODE, getKey(keySeed));

            byte[] buf = cipher.doFinal(byteContent);
            return new String(buf, charSet);

        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public static Key getKey(String keySeed) {
        if (keySeed == null) {
            keySeed = System.getenv("AES_SYS_KEY");
        }
        if (keySeed == null) {
            keySeed = System.getProperty("AES_SYS_KEY");
        }
        if (keySeed == null || keySeed.trim().length() == 0) {
            keySeed = "abcd1234!@#$";// 默认种子
        }
        try {
            SecureRandom secureRandom = SecureRandom.getInstance("SHA1PRNG");
            secureRandom.setSeed(keySeed.getBytes());
            KeyGenerator generator = KeyGenerator.getInstance("AES");
            generator.init(secureRandom);
            return generator.generateKey();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        String express="AES加密解密算法";
        long start=System.currentTimeMillis();
        String layer = AesUtil.encrypt(express,defKey,charset);
        long end =System.currentTimeMillis();
        System.out.println("加密时长:"+String.valueOf(end-start));
        System.out.println("密文："+layer);
        String plain = AesUtil.decrypt(layer,defKey,charset);
        System.out.println("原文："+plain);
    }
}
```


执行结果如下：



```
加密时长:358
密文：D21DE80BAFDDE82CC14EEE3F2D7DBC2E87C4230E49575DD1E97174F21E3A2D0A
原文：AES加密解密算法
```


实际上，我的需求是设计一个对称可逆的加密解密算法，这个算法加密时间快生成的密文字节少，绝对的轻量而且安全。
            


