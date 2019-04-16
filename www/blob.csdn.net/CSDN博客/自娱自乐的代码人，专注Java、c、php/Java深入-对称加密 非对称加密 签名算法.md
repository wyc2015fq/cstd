# Java深入 - 对称加密/非对称加密/签名算法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年05月12日 10:18:09[initphp](https://me.csdn.net/initphp)阅读数：3815
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









## 对称加密：

对称加密是指，加密方和解密方使用同样的秘钥来进行加密和解密。

![](https://img-blog.csdn.net/20140512101643625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




### 常用对称加密：

#### 1. DES算法

DES算法的入口参数有三个：Key、Data、Mode。其中Key为8个字节共64位，是DES算法的工作密钥；Data也为8个字节64位，是要被加密或被解密的数据；Mode为DES的工作方式，有两种：加密或解密。DES算法是这样工作的：如Mode为加密，则用Key 去把数据Data进行加密， 生成Data的密码形式（64位）作为DES的输出结果；如Mode为解密，则用Key去把密码形式的数据Data解密，还原为Data的明码形式（64位）作为DES的输出结果。DES算法具有极高安全性，到目前为止，除了用穷举搜索法对DES算法进行攻击外，还没有发现更有效的办法。

java实现：



```java
package com.xxx.controller.api;


import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class Test {

    /**
     * 入口函数
     * @param args
     */
    public static void main(String[] args) throws Exception{
        String test = desEncrypt("helloworld", "123456asdsadad");
        System.out.println("加密后：" + test);
        String data = desDecrypt(test, "123456asdsadad");
        System.out.println("解密后：" + data);
    }

    /**
     * DES加密算法-加密
     * @param data
     * @param key
     * @return
     * @throws Exception
     */
    public static String desEncrypt(String data, String key) throws Exception {
        SecureRandom sr = new SecureRandom(); //强随机数
        DESKeySpec dks = new DESKeySpec(key.getBytes("UTF-8"));
        //密钥工厂用来将密钥（类型 Key 的不透明加密密钥）转换为密钥规范（底层密钥材料的透明表示形式）
        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
        SecretKey securekey = keyFactory.generateSecret(dks);
        //DES加密
        Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding"); //DES算法
        cipher.init(Cipher.ENCRYPT_MODE, securekey, sr);
        byte[] bt =  cipher.doFinal(data.getBytes());
        return new BASE64Encoder().encode(bt); //base64处理     
    }
    
    /**
     * DES加密算法-解密
     * @param data
     * @param key
     * @return
     * @throws Exception
     */
    public static String desDecrypt(String data, String key) throws Exception {
        byte[] buf = new BASE64Decoder().decodeBuffer(data); //base64处理     
        
        //强随机数
        SecureRandom sr = new SecureRandom(); 
        
        //密钥工厂用来将密钥（类型 Key 的不透明加密密钥）转换为密钥规范（底层密钥材料的透明表示形式）
        DESKeySpec dks = new DESKeySpec(key.getBytes("UTF-8"));
        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
        SecretKey securekey = keyFactory.generateSecret(dks);
        
        //DES解密
        Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding"); //DES算法
        cipher.init(Cipher.DECRYPT_MODE, securekey, sr);
        byte[] bt =  cipher.doFinal(buf);
        return new String(bt); 
    }

}
```



#### 2. AES算法

AES 是一个迭代的、对称密钥分组的密码，它可以使用128、192 和 256 位密钥，并且用 128 位（16字节）分组加密和解密数据。与公共密钥密码使用密钥对不同，对称密钥密码使用相同的密钥加密和解密数据。通过分组密码返回的加密数据的位数与输入数据相同。迭代加密使用一个循环结构，在该循环中重复置换和替换输入数据。


java实现：



```java
package com.xxx.controller;


import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class test {

    /**
     * 入口函数
     * @param args
     */
    public static void main(String[] args) throws Exception {
        String test = aesEncrypt("helloworld", "123456asdsadad");
        System.out.println("加密后：" + test);
        String data = aesDecrypt(test, "123456asdsadad");
        System.out.println("解密后：" + data);
    }

    /**
     * AES加密算法-加密
     * @param data
     * @param key
     * @return
     * @throws Exception
     */
    public static String aesEncrypt(String data, String key) throws Exception {
        //构造秘钥生成器
        KeyGenerator kgen = KeyGenerator.getInstance("AES");
        kgen.init(128, new SecureRandom(key.getBytes()));
        //产生原始对称密钥
        SecretKey secretKey = kgen.generateKey();
        byte[] enCodeFormat = secretKey.getEncoded();
        //生成密钥
        SecretKeySpec keySpec = new SecretKeySpec(enCodeFormat, "AES");
        //AES加密算法
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        byte[] byteContent = data.getBytes("utf-8");
        cipher.init(Cipher.ENCRYPT_MODE, keySpec);
        byte[] result = cipher.doFinal(byteContent);
        //base64处理
        return new BASE64Encoder().encode(result);
    }

    /**
     * AES加密算法-解密
     * @param data
     * @param key
     * @return
     * @throws Exception
     */
    public static String aesDecrypt(String data, String key) throws Exception {
        byte[] buf = new BASE64Decoder().decodeBuffer(data); //base64处理     

        //构造秘钥生成器
        KeyGenerator kgen = KeyGenerator.getInstance("AES");
        kgen.init(128, new SecureRandom(key.getBytes()));
        //产生原始对称密钥
        SecretKey secretKey = kgen.generateKey();
        byte[] enCodeFormat = secretKey.getEncoded();
        //生成密钥
        SecretKeySpec keySpec = new SecretKeySpec(enCodeFormat, "AES");
        //AES加密算法
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, keySpec);
        byte[] result = cipher.doFinal(buf);
        //base64处理
        return new String(result);
    }

}
```





### 常用非对称加密：



非对称加密算法实现机密信息交换的基本过程是：甲方生成一对密钥并将其中的一把作为公用密钥向其它方公开；得到该公用密钥的乙方使用该密钥对机密信息进行加密后再发送给甲方；甲方再用自己保存的另一把专用密钥对加密后的信息进行解密。甲方只能用其专用密钥解密由其公用密钥加密后的任何信息。

![](https://img-blog.csdn.net/20140512140651265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


#### 1. RSA算法



```java
import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import java.util.HashMap;
import java.util.Map;

import javax.crypto.Cipher;

/**
 * RSA安全编码组件
 * 
 * @author 梁栋
 * @version 1.0
 * @since 1.0
 */
public abstract class RSACoder extends Coder {
	public static final String KEY_ALGORITHM = "RSA";
	public static final String SIGNATURE_ALGORITHM = "MD5withRSA";

	private static final String PUBLIC_KEY = "RSAPublicKey";
	private static final String PRIVATE_KEY = "RSAPrivateKey";

	/**
	 * 用私钥对信息生成数字签名
	 * 
	 * @param data
	 *            加密数据
	 * @param privateKey
	 *            私钥
	 * 
	 * @return
	 * @throws Exception
	 */
	public static String sign(byte[] data, String privateKey) throws Exception {
		// 解密由base64编码的私钥
		byte[] keyBytes = decryptBASE64(privateKey);

		// 构造PKCS8EncodedKeySpec对象
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(keyBytes);

		// KEY_ALGORITHM 指定的加密算法
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);

		// 取私钥匙对象
		PrivateKey priKey = keyFactory.generatePrivate(pkcs8KeySpec);

		// 用私钥对信息生成数字签名
		Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
		signature.initSign(priKey);
		signature.update(data);

		return encryptBASE64(signature.sign());
	}

	/**
	 * 校验数字签名
	 * 
	 * @param data
	 *            加密数据
	 * @param publicKey
	 *            公钥
	 * @param sign
	 *            数字签名
	 * 
	 * @return 校验成功返回true 失败返回false
	 * @throws Exception
	 * 
	 */
	public static boolean verify(byte[] data, String publicKey, String sign)
			throws Exception {

		// 解密由base64编码的公钥
		byte[] keyBytes = decryptBASE64(publicKey);

		// 构造X509EncodedKeySpec对象
		X509EncodedKeySpec keySpec = new X509EncodedKeySpec(keyBytes);

		// KEY_ALGORITHM 指定的加密算法
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);

		// 取公钥匙对象
		PublicKey pubKey = keyFactory.generatePublic(keySpec);

		Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
		signature.initVerify(pubKey);
		signature.update(data);

		// 验证签名是否正常
		return signature.verify(decryptBASE64(sign));
	}

	/**
	 * 解密<br>
	 * 用私钥解密
	 * 
	 * @param data
	 * @param key
	 * @return
	 * @throws Exception
	 */
	public static byte[] decryptByPrivateKey(byte[] data, String key)
			throws Exception {
		// 对密钥解密
		byte[] keyBytes = decryptBASE64(key);

		// 取得私钥
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key privateKey = keyFactory.generatePrivate(pkcs8KeySpec);

		// 对数据解密
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.DECRYPT_MODE, privateKey);

		return cipher.doFinal(data);
	}

	/**
	 * 解密<br>
	 * 用公钥解密
	 * 
	 * @param data
	 * @param key
	 * @return
	 * @throws Exception
	 */
	public static byte[] decryptByPublicKey(byte[] data, String key)
			throws Exception {
		// 对密钥解密
		byte[] keyBytes = decryptBASE64(key);

		// 取得公钥
		X509EncodedKeySpec x509KeySpec = new X509EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key publicKey = keyFactory.generatePublic(x509KeySpec);

		// 对数据解密
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.DECRYPT_MODE, publicKey);

		return cipher.doFinal(data);
	}

	/**
	 * 加密<br>
	 * 用公钥加密
	 * 
	 * @param data
	 * @param key
	 * @return
	 * @throws Exception
	 */
	public static byte[] encryptByPublicKey(byte[] data, String key)
			throws Exception {
		// 对公钥解密
		byte[] keyBytes = decryptBASE64(key);

		// 取得公钥
		X509EncodedKeySpec x509KeySpec = new X509EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key publicKey = keyFactory.generatePublic(x509KeySpec);

		// 对数据加密
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.ENCRYPT_MODE, publicKey);

		return cipher.doFinal(data);
	}

	/**
	 * 加密<br>
	 * 用私钥加密
	 * 
	 * @param data
	 * @param key
	 * @return
	 * @throws Exception
	 */
	public static byte[] encryptByPrivateKey(byte[] data, String key)
			throws Exception {
		// 对密钥解密
		byte[] keyBytes = decryptBASE64(key);

		// 取得私钥
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key privateKey = keyFactory.generatePrivate(pkcs8KeySpec);

		// 对数据加密
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.ENCRYPT_MODE, privateKey);

		return cipher.doFinal(data);
	}

	/**
	 * 取得私钥
	 * 
	 * @param keyMap
	 * @return
	 * @throws Exception
	 */
	public static String getPrivateKey(Map<String, Object> keyMap)
			throws Exception {
		Key key = (Key) keyMap.get(PRIVATE_KEY);

		return encryptBASE64(key.getEncoded());
	}

	/**
	 * 取得公钥
	 * 
	 * @param keyMap
	 * @return
	 * @throws Exception
	 */
	public static String getPublicKey(Map<String, Object> keyMap)
			throws Exception {
		Key key = (Key) keyMap.get(PUBLIC_KEY);

		return encryptBASE64(key.getEncoded());
	}

	/**
	 * 初始化密钥
	 * 
	 * @return
	 * @throws Exception
	 */
	public static Map<String, Object> initKey() throws Exception {
		KeyPairGenerator keyPairGen = KeyPairGenerator
				.getInstance(KEY_ALGORITHM);
		keyPairGen.initialize(1024);

		KeyPair keyPair = keyPairGen.generateKeyPair();

		// 公钥
		RSAPublicKey publicKey = (RSAPublicKey) keyPair.getPublic();

		// 私钥
		RSAPrivateKey privateKey = (RSAPrivateKey) keyPair.getPrivate();

		Map<String, Object> keyMap = new HashMap<String, Object>(2);

		keyMap.put(PUBLIC_KEY, publicKey);
		keyMap.put(PRIVATE_KEY, privateKey);
		return keyMap;
	}
}
```





## 签名方法：



在开发后端服务的过程中，为了确保接口数据的准确性和不被修改，我们一般会对传递的参数进行一个签名。

假如请求数据：appKey=test&a=xxxx&b=ssss&e=sss&d=sss&time=xxxxx&sign=sapodpasdpa

这个请求参数中，appKey为api请求方的key（请求方和服务端还保存一份secret秘钥），sign为数据的签名，其余为传递的参数。

sign的签名算法是对传递的参数进行键值升序排序，例如排序成这样：

a=xxxx&appKey=test&b=ssss&d=sss&e=sss&time=xxxxx

请求方对这个参数进行签名：sign = MD5（a=xxxx&appKey=test&b=ssss&d=sss&e=sss&time=xxxxx + 秘钥）

然后服务端拿到请求的sign，同时也会拿到appKey，就可以找到对应的secret秘钥，同样对请求的参数进行升序排序后进行md5的签名，再比较sign值是否相同，如果相同则表示数据未被修改。

同时为了接口的安全性，我们可以看到请求参数中有time字段，我们还可以根据time时间来做请求的失效性处理，例如time和当前服务器时间比较 大小返回在5分钟之内还是有效，超出了说明这个请求的数据过期，可以拒绝请求。

PHP方法：



```php
/**
	 * 方法库-sign签名方法
	 * @param $array 需要加密的参数 
	 * @param $secret 秘钥
	 * @param $signName sign的名称，sign不会进行加密
	 */
	public function sign($array, $secret, $signName = "sign") {
		if (count($array) == 0) {
			return "";
		}
		ksort($array); //按照升序排序
		$str = "";
		foreach ($array as $key => $value) {
			if ($signName == $key) continue;
			$str .= $key . "=" . $value . "&";
		}
		$str = rtrim($str, "&");
		return md5($str . $secret);
	}
```










