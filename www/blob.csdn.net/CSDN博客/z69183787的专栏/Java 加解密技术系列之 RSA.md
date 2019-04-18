# Java 加解密技术系列之 RSA - z69183787的专栏 - CSDN博客
2017年07月24日 11:25:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：348
个人分类：[工具-加解密](https://blog.csdn.net/z69183787/article/category/5974293)
[http://m.blog.csdn.net/happylee6688/article/details/45218801](http://m.blog.csdn.net/happylee6688/article/details/45218801http://)
# 序
距离上一次写博客感觉已经很长时间了，先吐槽一下，这个月以来，公司一直在加班，又是发版、上线，又是新项目太紧，具体的就不多说了，想听我吐槽的小伙伴，可以私信给我(*^__^*) 。上一篇文章，已经把对称加密的算法讲完了。从今天开始，要说说非对称加密了。因为，非对称加密真的是太重要了，我们的日常生活中，都离不开非对称加密。
# 概念
在说 RSA 之前，首先聊聊什么是非对称加密。在讲对称加密的时候，就曾经说过，对称加密算法在加密和解密时使用的是同一个秘钥，加解密双方必须使用同一个密钥才能进行正常的沟通。而非对称加密则不然，非对称加密算法需要两个密钥来进行加密和解密，分别是公钥和私钥。
需要注意的一点，这个公钥和私钥必须是一对的，如果用公钥对数据进行加密，那么只有使用对应的私钥才能解密，反之亦然。由于加密和解密使用的是两个不同的密钥，因此，这种算法叫做非对称加密算法。
# 工作过程
如下图，甲乙之间使用非对称加密的方式传输数据。
- 乙方生成一对密钥（公钥和私钥）并将公钥向其它方公开；
- 得到该公钥的甲方使用该密钥对机密信息进行加密后再发送给乙方；
- 乙方再用自己保存的另一把专用密钥（私钥）对加密后的信息进行解密；
- 乙方只能用其专用密钥（私钥）解密由对应的公钥加密后的信息；
- 在传输过程中，即使攻击者截获了传输的密文，并得到了乙的公钥，也无法破解密文，因为只有乙的私钥才能解密密文。
![](https://img-blog.csdn.net/20150423105825489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlsZWU2Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在非对称加密中使用的主要算法有：RSA、Elgamal、背包算法、Rabin、D-H、ECC（椭圆曲线加密算法）等。今天主要是介绍 RSA ，至于其他的算法，后续会选择几个进行介绍。
# RSA
其实，在早在 1978 年的时候，RSA就已经出现了，它是第一个既能用于数据加密也能用于数字签名的算法。它易于理解和操作，也很流行。其原理就如上面的工作过程所述。
RSA 算法基于一个十分简单的数论事实：将两个大素数相乘十分容易，但是想要对其乘积进行因式分解却极其困难，因此可以将乘积公开作为加密密钥。
# 代码实现
下面来看一下具体的代码实现。
```java
<span style="font-family:Comic Sans MS;font-size:12px;">package com.test.rsa;
import com.google.common.collect.Maps;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;
import javax.crypto.Cipher;
import java.security.*;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Map;
/**
 * Created by xiang.li on 2015/3/3.
 * RSA 加解密工具类
 */
public class RSA {
    /**
     * 定义加密方式
     */
    private final static String KEY_RSA = "RSA";
    /**
     * 定义签名算法
     */
    private final static String KEY_RSA_SIGNATURE = "MD5withRSA";
    /**
     * 定义公钥算法
     */
    private final static String KEY_RSA_PUBLICKEY = "RSAPublicKey";
    /**
     * 定义私钥算法
     */
    private final static String KEY_RSA_PRIVATEKEY = "RSAPrivateKey";
    /**
     * 初始化密钥
     * @return
     */
    public static Map<String, Object> init() {
        Map<String, Object> map = null;
        try {
            KeyPairGenerator generator = KeyPairGenerator.getInstance(KEY_RSA);
            generator.initialize(1024);
            KeyPair keyPair = generator.generateKeyPair();
            // 公钥
            RSAPublicKey publicKey = (RSAPublicKey) keyPair.getPublic();
            // 私钥
            RSAPrivateKey privateKey = (RSAPrivateKey) keyPair.getPrivate();
            // 将密钥封装为map
            map = Maps.newHashMap();
            map.put(KEY_RSA_PUBLICKEY, publicKey);
            map.put(KEY_RSA_PRIVATEKEY, privateKey);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return map;
    }
    /**
     * 用私钥对信息生成数字签名
     * @param data 加密数据
     * @param privateKey 私钥
     * @return
     */
    public static String sign(byte[] data, String privateKey) {
        String str = "";
        try {
            // 解密由base64编码的私钥
            byte[] bytes = decryptBase64(privateKey);
            // 构造PKCS8EncodedKeySpec对象
            PKCS8EncodedKeySpec pkcs = new PKCS8EncodedKeySpec(bytes);
            // 指定的加密算法
            KeyFactory factory = KeyFactory.getInstance(KEY_RSA);
            // 取私钥对象
            PrivateKey key = factory.generatePrivate(pkcs);
            // 用私钥对信息生成数字签名
            Signature signature = Signature.getInstance(KEY_RSA_SIGNATURE);
            signature.initSign(key);
            signature.update(data);
            str = encryptBase64(signature.sign());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }
    /**
     * 校验数字签名
     * @param data 加密数据
     * @param publicKey 公钥
     * @param sign 数字签名
     * @return 校验成功返回true，失败返回false
     */
    public static boolean verify(byte[] data, String publicKey, String sign) {
        boolean flag = false;
        try {
            // 解密由base64编码的公钥
            byte[] bytes = decryptBase64(publicKey);
            // 构造X509EncodedKeySpec对象
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(bytes);
            // 指定的加密算法
            KeyFactory factory = KeyFactory.getInstance(KEY_RSA);
            // 取公钥对象
            PublicKey key = factory.generatePublic(keySpec);
            // 用公钥验证数字签名
            Signature signature = Signature.getInstance(KEY_RSA_SIGNATURE);
            signature.initVerify(key);
            signature.update(data);
            flag = signature.verify(decryptBase64(sign));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return flag;
    }
    /**
     * 私钥解密
     * @param data 加密数据
     * @param key 私钥
     * @return
     */
    public static byte[] decryptByPrivateKey(byte[] data, String key) {
        byte[] result = null;
        try {
            // 对私钥解密
            byte[] bytes = decryptBase64(key);
            // 取得私钥
            PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(bytes);
            KeyFactory factory = KeyFactory.getInstance(KEY_RSA);
            PrivateKey privateKey = factory.generatePrivate(keySpec);
            // 对数据解密
            Cipher cipher = Cipher.getInstance(factory.getAlgorithm());
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            result = cipher.doFinal(data);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
    /**
     * 私钥解密
     * @param data 加密数据
     * @param key 公钥
     * @return
     */
    public static byte[] decryptByPublicKey(byte[] data, String key) {
        byte[] result = null;
        try {
            // 对公钥解密
            byte[] bytes = decryptBase64(key);
            // 取得公钥
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(bytes);
            KeyFactory factory = KeyFactory.getInstance(KEY_RSA);
            PublicKey publicKey = factory.generatePublic(keySpec);
            // 对数据解密
            Cipher cipher = Cipher.getInstance(factory.getAlgorithm());
            cipher.init(Cipher.DECRYPT_MODE, publicKey);
            result = cipher.doFinal(data);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
    /**
     * 公钥加密
     * @param data 待加密数据
     * @param key 公钥
     * @return
     */
    public static byte[] encryptByPublicKey(byte[] data, String key) {
        byte[] result = null;
        try {
            byte[] bytes = decryptBase64(key);
            // 取得公钥
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(bytes);
            KeyFactory factory = KeyFactory.getInstance(KEY_RSA);
            PublicKey publicKey = factory.generatePublic(keySpec);
            // 对数据加密
            Cipher cipher = Cipher.getInstance(factory.getAlgorithm());
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            result = cipher.doFinal(data);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
    /**
     * 私钥加密
     * @param data 待加密数据
     * @param key 私钥
     * @return
     */
    public static byte[] encryptByPrivateKey(byte[] data, String key) {
        byte[] result = null;
        try {
            byte[] bytes = decryptBase64(key);
            // 取得私钥
            PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(bytes);
            KeyFactory factory = KeyFactory.getInstance(KEY_RSA);
            PrivateKey privateKey = factory.generatePrivate(keySpec);
            // 对数据加密
            Cipher cipher = Cipher.getInstance(factory.getAlgorithm());
            cipher.init(Cipher.ENCRYPT_MODE, privateKey);
            result = cipher.doFinal(data);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
    /**
     * 获取公钥
     * @param map
     * @return
     */
    public static String getPublicKey(Map<String, Object> map) {
        String str = "";
        try {
            Key key = (Key) map.get(KEY_RSA_PUBLICKEY);
            str = encryptBase64(key.getEncoded());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }
    /**
     * 获取私钥
     * @param map
     * @return
     */
    public static String getPrivateKey(Map<String, Object> map) {
        String str = "";
        try {
            Key key = (Key) map.get(KEY_RSA_PRIVATEKEY);
            str = encryptBase64(key.getEncoded());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }
    /**
     * BASE64 解密
     * @param key 需要解密的字符串
     * @return 字节数组
     * @throws Exception
     */
    public static byte[] decryptBase64(String key) throws Exception {
        return (new BASE64Decoder()).decodeBuffer(key);
    }
    /**
     * BASE64 加密
     * @param key 需要加密的字节数组
     * @return 字符串
     * @throws Exception
     */
    public static String encryptBase64(byte[] key) throws Exception {
        return (new BASE64Encoder()).encodeBuffer(key);
    }
    /**
     * 测试方法
     * @param args
     */
    public static void main(String[] args) {
        String privateKey = "";
        String publicKey = "";
        // 生成公钥私钥
        Map<String, Object> map = init();
        publicKey = getPublicKey(map);
        privateKey = getPrivateKey(map);
        System.out.println("公钥: \n\r" + publicKey);
        System.out.println("私钥： \n\r" + privateKey);
        System.out.println("公钥加密--------私钥解密");
        String word = "你好，世界！";
        byte[] encWord = encryptByPublicKey(word.getBytes(), publicKey);
        String decWord = new String(decryptByPrivateKey(encWord, privateKey));
        System.out.println("加密前: " + word + "\n\r" + "解密后: " + decWord);
        System.out.println("私钥加密--------公钥解密");
        String english = "Hello, World!";
        byte[] encEnglish = encryptByPrivateKey(english.getBytes(), privateKey);
        String decEnglish = new String(decryptByPublicKey(encEnglish, publicKey));
        System.out.println("加密前: " + english + "\n\r" + "解密后: " + decEnglish);
        System.out.println("私钥签名——公钥验证签名");
        // 产生签名
        String sign = sign(encEnglish, privateKey);
        System.out.println("签名:\r" + sign);
        // 验证签名
        boolean status = verify(encEnglish, publicKey, sign);
        System.out.println("状态:\r" + status);
    }
}</span>
```
# 加解密结果
![](https://img-blog.csdn.net/20150423110555322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlsZWU2Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 结束语
其实，看似很复杂的过程，用一句话就可以描述：使用公钥加密、私钥解密，完成了乙方到甲方的一次数据传递，通过私钥加密、公钥解密，同时通过私钥签名、公钥验证签名，完成了一次甲方到乙方的数据传递与验证，两次数据传递完成一整套的数据交互。
非对称加密算法的出现，就是为了解决只有一把密钥的加解密，只要这一把密钥丢失或者被公开，那么加密数据就很容易被攻击。同时，也正是由于非对称加密算法的出现，才有了后面的数字签名、数字证书等等。
好了，今天就到这吧，下一篇继续非对称加密，至于哪一种，到时候就知道了，这里先保密，(*^__^*) 嘻嘻。
