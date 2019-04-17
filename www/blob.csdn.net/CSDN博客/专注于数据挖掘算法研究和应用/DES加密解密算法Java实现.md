# DES加密解密算法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月16日 13:58:58[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2158








DES 使用一个 56 位的密钥以及附加的 8 位奇偶校验位，产生最大 64 位的分组大小。这是一个迭代的分组密码，使用称为 Feistel 的技术，其中将加密的文本块分成两半。使用子密钥对其中一半应用循环功能，然后将输出与另一半进行“异或”运算；接着交换这两半，这一过程会继续下去，但最后一个循环不交换。DES 使用 16 个循环，使用异或，置换，代换，移位操作四种基本运算。




DES（Data Encryption Standard）是发明最早的最广泛使用的分组对称加密算法。DES算法的入口参数有三个：Key、Data、Mode。其中Key为8个字节共64位，是DES算法的工作密钥；Data也为8个字节64位，是要被加密或被解密的数据；Mode为DES的工作方式，有两种：加密或解密。





参考代码如下：



```java
package com.gddx.des;

import java.io.IOException;
import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;


/**
 * DES加密 解密算法
 * 
 * @author Fangjs
 * @date 2017-03-16 
 */
public class DesUtil {

    private final static String DES = "DES";
    private final static String ENCODE = "UTF-8";
    private final static String defaultKey = "gddxbdhp";//8字节key长度

    /**
     * 使用 默认key加密
     */
    public static String encrypt(String express) throws Exception {
    	if (express == null) return null;
        byte[] bCiphertext = encrypt(express.getBytes(ENCODE), defaultKey.getBytes(ENCODE));
        String ciphertext = new sun.misc.BASE64Encoder().encode(bCiphertext);
        return ciphertext;//返回密文
    }

    /**
     * 使用 默认key 解密
     */
    public static String decrypt(String ciphertext) throws IOException, Exception {
        if (ciphertext == null) return null;
        sun.misc.BASE64Decoder decoder = new sun.misc.BASE64Decoder();
        byte[] buf = decoder.decodeBuffer(ciphertext);
        byte[] bExpress = decrypt(buf, defaultKey.getBytes(ENCODE));
        return new String(bExpress, ENCODE);//返回明文
    }

    /**
     * Description 根据键值进行加密
     * @param data
     * @param key 加密键byte数组
     * @return
     * @throws Exception
     */
    public static String encrypt(String express, String key) throws Exception {
    	if (express == null) return null;
        byte[] bCiphertext = encrypt(express.getBytes(ENCODE), key.getBytes(ENCODE));
        String ciphertext = new sun.misc.BASE64Encoder().encode(bCiphertext);
        return ciphertext;//返回密文
    }

    /**
     * Description 根据键值进行解密
     * @param data
     * @param key 加密键byte数组
     * @return
     * @throws IOException
     * @throws Exception
     */
    public static String decrypt(String ciphertext, String key) throws IOException, Exception {
    	if (ciphertext == null) return null;
        sun.misc.BASE64Decoder decoder = new sun.misc.BASE64Decoder();
        byte[] buf = decoder.decodeBuffer(ciphertext);
        byte[] bExpress = decrypt(buf, key.getBytes(ENCODE));
        return new String(bExpress, ENCODE);
    }

    /**
     * @param data
     * @param key  加密键byte数组
     * @return
     * @throws Exception
     */
    private static byte[] encrypt(byte[] data, byte[] key) throws Exception {
        // 生成一个可信任的随机数源
        SecureRandom sr = new SecureRandom();
        // 从原始密钥数据创建DESKeySpec对象
        DESKeySpec dks = new DESKeySpec(key);
        // 创建一个密钥工厂，然后用它把DESKeySpec转换成SecretKey对象
        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(DES);
        SecretKey securekey = keyFactory.generateSecret(dks);
        // Cipher对象实际完成加密操作
        Cipher cipher = Cipher.getInstance(DES);
        // 用密钥初始化Cipher对象
        cipher.init(Cipher.ENCRYPT_MODE, securekey, sr);
        return cipher.doFinal(data);
    }

    /**
     * Description 根据键值进行解密
     * @param data
     * @param key  加密键byte数组
     * @return
     * @throws Exception
     */
    private static byte[] decrypt(byte[] data, byte[] key) throws Exception {
        // 生成一个可信任的随机数源
        SecureRandom sr = new SecureRandom();
        // 从原始密钥数据创建DESKeySpec对象
        DESKeySpec dks = new DESKeySpec(key);
        // 创建一个密钥工厂，然后用它把DESKeySpec转换成SecretKey对象
        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(DES);
        SecretKey securekey = keyFactory.generateSecret(dks);
        // Cipher对象实际完成解密操作
        Cipher cipher = Cipher.getInstance(DES);
        // 用密钥初始化Cipher对象
        cipher.init(Cipher.DECRYPT_MODE, securekey, sr);
        return cipher.doFinal(data);
    }
    
    
    public static void main(String[] args) throws Exception {
        String express = "DES加密解密算法";
        String ciphertext=encrypt(express,"12345678");
        System.out.println(ciphertext);
        String rexpress=decrypt(ciphertext,"12345678");
        System.out.println(express);
    }

}
```


执行结果：



```
1IBx2S1YBzHb0TYdu93sDdwL+MZKsOYw
DES加密解密算法
```





