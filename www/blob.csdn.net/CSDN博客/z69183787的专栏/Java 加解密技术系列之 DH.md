# Java 加解密技术系列之 DH - z69183787的专栏 - CSDN博客
2017年07月24日 11:28:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：395
[http://m.blog.csdn.net/happylee6688/article/details/45242691](http://m.blog.csdn.net/happylee6688/article/details/45242691)
# 序
上一篇文章中简单的介绍了一种非对称加密算法 — — RSA，今天这篇文章，继续介绍另一种非对称加密算法 — — DH。当然，可能有很多人对这种加密算法并不是很熟悉，不过没关系，希望今天这篇文章能帮助你熟悉他。
# 概念
DH，全称为“Diffie-Hellman”，他是一种确保共享KEY安全穿越不安全网络的方法，也就是常说的密钥一致协议。由公开密钥密码体制的奠基人Diffie和Hellman所提出的一种思想。
简单的说就是允许两名用户在公开媒体上交换信息以生成“一致”的、可以共享的密钥。也就是由甲方产出一对密钥（公钥、私钥），乙方依照甲方公钥产生乙方密钥对（公钥、私钥）。
以此为基线，作为数据传输保密基础，同时双方使用同一种对称加密算法构建本地密钥（SecretKey）对数据加密。这样，在互通了本地密钥（SecretKey）算法后，甲乙双方公开自己的公钥，使用对方的公钥和刚才产生的私钥加密数据，同时可以使用对方的公钥和自己的私钥对数据解密。不单单是甲乙双方两方，可以扩展为多方共享数据通讯，这样就完成了网络交互数据的安全通讯！
# 原理
- 通信方A和通信方B约定一个初始数g，g是公开的，如g=5；
- A生成一个随机数a，a是保密的，如a=6；
- A计算g^a发送给B，g^a=5^6；
- B生成一个随机数b，b是保密的，如b=15；
- B计算g^b发送给A，g^b=5^15；
- A接收到g^b后，再使用保密的a，计算(g^b)^a=g^(a*b)=5^(6*15) ；
- B接收到g^a后，再使用保密的b，计算(g^a)^b=g^(a*b)=5^(6*15)；
- 这样通信方A和B得到一个相同的“密钥”g^(a*b)=5^(6*15)。
整个通信过程中g、g^a、g^b是公开的，但由于g、a、b都是整数，通过g和g^a得到a还是比较容易的，b也是如此，所以最终的“密钥”g^(a*b)还是可以被计算出来的。所以实际的过程还需要在基本原理上加入新的计算——模运算。
- 通信方A和通信方B约定一个初始数g，如g=5，一个质数p，如p=23，g和p是公开的 ；
- A生成一个随机数a，a是保密的，如a=6 ；
- A计算g^a%p发送给B，g^a%p=5^6%23=8 ；
- B生成一个随机数b，b是保密的，如b=15 ；
- B计算g^b%p发送给A，g^b%p=5^15%23=19 ；
- A接收到g^b%p后，再使用保密的a，计算(g^b%p)^a%p=19^6%23=2 ；
- B接收到g^a%p后，再使用保密的b，计算(g^a%p)^b%p=8^15%23=2 ；
- 这样通信方A和B得到一个相同的密钥：2。
(g^b%p)^a%p=(g^a%p)^b%p 证明：
如果a=2：
- (g^b%p)^a%p=(g^b%p)^2%p=(g^b-n*p)^2%p=(g^(2*b)-2*g^b*n*p+(n*p)^2)%p=g^(2*b)%p ；
- 可以看出(g^b-n*p)^2展开后除g^(2*b)外，其它都是p的倍数，所以整个算式的结果是g^(2*b)%p ；
- 同理对(g^b-n*p)^a展开后除g^(a*b)外，其它都是p的倍数，所以整个算式的结果是g^(a*b)%p ；
- 同样可以得出(g^a%p)^b%p=g^(a*b)%p ；
- 所以(g^b%p)^a%p=(g^a%p)^b%p 。
整个通信过程中g、p、g^a%p、g^b%p是公开的，这时通过g、p、g^a%p得到a比较难，同样通过g、p、g^b%p得到b比较难，所以最终的密钥是比较安全的。
以g=5、p=23、g^a%p=8计算a为例，a=log(5, (8+23*n))，这个只能将n的可能值逐个带入公式试验才能得到a的值。如果a、p是比较大的数那么计算更加困难。
需要注意的是，为了防止应用优化算法计算上述问题，质数p不是随便选择的，需要符合一定的条件。随机数a、b的生成算法也必需注意，应使结果尽可能随机，不能出现可预测的规律，否则会使破解变的容易。
# 代码实现
```java
<span style="font-family:Comic Sans MS;font-size:12px;">package com.test.dh;
import com.google.common.collect.Maps;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;
import javax.crypto.*;
import javax.crypto.interfaces.DHPrivateKey;
import javax.crypto.interfaces.DHPublicKey;
import javax.crypto.spec.DHParameterSpec;
import java.security.*;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Map;
/**
 * Created by xiang.li on 2015/3/4.
 * DH 加解密工具类
 */
public class DH {
    /**
     * 定义加密方式
     */
    private static final String KEY_DH = "DH";
    /**
     * 默认密钥字节数
     */
    private static final int KEY_SIZE = 1024;
    /**
     * DH加密下需要一种对称加密算法对数据加密，这里我们使用DES，也可以使用其他对称加密算法
     */
    private static final String KEY_DH_DES = "DES";
    private static final String KEY_DH_PUBLICKEY = "DHPublicKey";
    private static final String KEY_DH_PRIVATEKEY = "DHPrivateKey";
    /**
     * 初始化甲方密钥
     * @return
     */
    public static Map<String, Object> init() {
        Map<String, Object> map = null;
        try {
            KeyPairGenerator generator = KeyPairGenerator.getInstance(KEY_DH);
            generator.initialize(KEY_SIZE);
            KeyPair keyPair = generator.generateKeyPair();
            // 甲方公钥
            DHPublicKey publicKey = (DHPublicKey) keyPair.getPublic();
            // 甲方私钥
            DHPrivateKey privateKey = (DHPrivateKey) keyPair.getPrivate();
            map = Maps.newHashMap();
            map.put(KEY_DH_PUBLICKEY, publicKey);
            map.put(KEY_DH_PRIVATEKEY, privateKey);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return map;
    }
    /**
     * 初始化乙方密钥
     * @param key 甲方密钥
     * @return
     */
    public static Map<String, Object> init(String key) {
        Map<String, Object> map = null;
        try {
            // 解析甲方密钥
            byte[] bytes = decryptBase64(key);
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(bytes);
            KeyFactory factory = KeyFactory.getInstance(KEY_DH);
            PublicKey publicKey = factory.generatePublic(keySpec);
            // 由甲方公钥构建乙方密钥
            DHParameterSpec spec = ((DHPublicKey) publicKey).getParams();
            KeyPairGenerator generator = KeyPairGenerator.getInstance(KEY_DH);
            generator.initialize(spec);
            KeyPair keyPair = generator.generateKeyPair();
            // 乙方公钥
            DHPublicKey dhPublicKey = (DHPublicKey) keyPair.getPublic();
            // 乙方私钥
            DHPrivateKey dhPrivateKey = (DHPrivateKey) keyPair.getPrivate();
            map = Maps.newHashMap();
            map.put(KEY_DH_PUBLICKEY, dhPublicKey);
            map.put(KEY_DH_PRIVATEKEY, dhPrivateKey);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return map;
    }
    /**
     * DH 加密
     * @param data 带加密数据
     * @param publicKey 甲方公钥
     * @param privateKey 乙方私钥
     * @return
     */
    public static byte[] encryptDH(byte[] data, String publicKey, String privateKey) {
        byte[] bytes = null;
        try {
            // 生成本地密钥
            SecretKey secretKey = getSecretKey(publicKey, privateKey);
            // 数据加密
            Cipher cipher = Cipher.getInstance(secretKey.getAlgorithm());
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            bytes = cipher.doFinal(data);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        }
        return bytes;
    }
    /**
     * DH 解密
     * @param data 待解密数据
     * @param publicKey 乙方公钥
     * @param privateKey 甲方私钥
     * @return
     */
    public static byte[] decryptDH(byte[] data, String publicKey, String privateKey) {
        byte[] bytes = null;
        try {
            // 生成本地密钥
            SecretKey secretKey = getSecretKey(publicKey, privateKey);
            // 数据解密
            Cipher cipher = Cipher.getInstance(secretKey.getAlgorithm());
            cipher.init(Cipher.DECRYPT_MODE, secretKey);
            bytes = cipher.doFinal(data);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        }
        return bytes;
    }
    /**
     * 取得私钥
     * @param map
     * @return
     */
    public static String getPrivateKey(Map<String, Object> map) {
        String str = "";
        try {
            Key key = (Key) map.get(KEY_DH_PRIVATEKEY);
            str = encryptBase64(key.getEncoded());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }
    /**
     * 取得公钥
     * @param map
     * @return
     */
    public static String getPublicKey(Map<String, Object> map) {
        String str = "";
        try {
            Key key = (Key) map.get(KEY_DH_PUBLICKEY);
            str = encryptBase64(key.getEncoded());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }
    /**
     * 构建本地密钥
     * @param publicKey 公钥
     * @param privateKey 私钥
     * @return
     */
    private static SecretKey getSecretKey(String publicKey, String privateKey) {
        SecretKey secretKey = null;
        try {
            // 初始化公钥
            byte[] publicBytes = decryptBase64(publicKey);
            KeyFactory factory = KeyFactory.getInstance(KEY_DH);
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(publicBytes);
            PublicKey localPublicKey = factory.generatePublic(keySpec);
            // 初始化私钥
            byte[] privateBytes = decryptBase64(privateKey);
            PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(privateBytes);
            PrivateKey localPrivateKey = factory.generatePrivate(spec);
            KeyAgreement agreement = KeyAgreement.getInstance(factory.getAlgorithm());
            agreement.init(localPrivateKey);
            agreement.doPhase(localPublicKey, true);
            // 生成本地密钥
            secretKey = agreement.generateSecret(KEY_DH_DES);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return secretKey;
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
        // 生成甲方密钥对
        Map<String, Object> mapA = init();
        String publicKeyA = getPublicKey(mapA);
        String privateKeyA = getPrivateKey(mapA);
        System.out.println("甲方公钥:\n" + publicKeyA);
        System.out.println("甲方私钥:\n" + privateKeyA);
        // 由甲方公钥产生本地密钥对
        Map<String, Object> mapB = init(publicKeyA);
        String publicKeyB = getPublicKey(mapB);
        String privateKeyB = getPrivateKey(mapB);
        System.out.println("乙方公钥:\n" + publicKeyB);
        System.out.println("乙方私钥:\n" + privateKeyB);
        String word = "abc";
        System.out.println("原文: " + word);
        // 由甲方公钥，乙方私钥构建密文
        byte[] encWord = encryptDH(word.getBytes(), publicKeyA, privateKeyB);
        // 由乙方公钥，甲方私钥解密
        byte[] decWord = decryptDH(encWord, publicKeyB, privateKeyA);
        System.out.println("解密: " + new String(decWord));
    }
}</span><span style="font-family: 微软雅黑; font-size: 14px;">
</span>
```
# 结果
![](https://img-blog.csdn.net/20150424095513709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlsZWU2Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150424095543146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlsZWU2Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 结束语
这个机制的巧妙在于需要安全通信的双方可以用这个方法确定对称密钥。然后可以用这个密钥进行加密和解密。但是注意，这个密钥交换协议/算法只能用于密钥的交换，而不能进行消息的加密和解密。双方确定要用的密钥后，要使用其他对称密钥操作加密算法实际加密和解密消息。
