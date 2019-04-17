# RSA 实现数据加密解密以及Token管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月02日 10:36:03[boonya](https://me.csdn.net/boonya)阅读数：8421








RSA加密解密key都是成对出现的，一个公钥，一个私钥，并且加密解密一般是公钥加密私钥解密。


### 1.RSA加密工具



```java
package com.wlyd.fmcgwms.util.api;

import java.io.ByteArrayOutputStream;
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
 * <p>
 * RSA公钥/私钥/签名工具包
 * </p>
 * <p>
 * 罗纳德·李维斯特（Ron [R]ivest）、阿迪·萨莫尔（Adi [S]hamir）和伦纳德·阿德曼（Leonard [A]dleman）
 * </p>
 * <p>
 * 字符串格式的密钥在未在特殊说明情况下都为BASE64编码格式<br/>
 * 由于非对称加密速度极其缓慢，一般文件不使用它来加密而是使用对称加密，<br/>
 * 非对称加密算法可以用来对对称加密的密钥加密，这样保证密钥的安全也就保证了数据的安全
 * </p>
 * 
 * @author IceWee
 * @date 2012-4-26
 * @version 1.0
 */
public class RSAUtils {

	/**
	 * 加密算法RSA
	 */
	public static final String KEY_ALGORITHM = "RSA";

	/**
	 * 签名算法
	 */
	public static final String SIGNATURE_ALGORITHM = "MD5withRSA";

	/**
	 * 获取公钥的key
	 */
	private static final String PUBLIC_KEY = "RSAPublicKey";

	/**
	 * 获取私钥的key
	 */
	private static final String PRIVATE_KEY = "RSAPrivateKey";

	/**
	 * RSA最大加密明文大小
	 */
	private static final int MAX_ENCRYPT_BLOCK = 117;

	/**
	 * RSA最大解密密文大小
	 */
	private static final int MAX_DECRYPT_BLOCK = 128;

	/**
	 * <p>
	 * 生成密钥对(公钥和私钥)
	 * </p>
	 * 
	 * @return
	 * @throws Exception
	 */
	public static Map<String, Object> genKeyPair() throws Exception {
		KeyPairGenerator keyPairGen = KeyPairGenerator
				.getInstance(KEY_ALGORITHM);
		keyPairGen.initialize(1024);
		KeyPair keyPair = keyPairGen.generateKeyPair();
		RSAPublicKey publicKey = (RSAPublicKey) keyPair.getPublic();
		RSAPrivateKey privateKey = (RSAPrivateKey) keyPair.getPrivate();
		Map<String, Object> keyMap = new HashMap<String, Object>(2);
		keyMap.put(PUBLIC_KEY, publicKey);
		keyMap.put(PRIVATE_KEY, privateKey);
		return keyMap;
	}

	/**
	 * <p>
	 * 用私钥对信息生成数字签名
	 * </p>
	 * 
	 * @param data
	 *            已加密数据
	 * @param privateKey
	 *            私钥(BASE64编码)
	 * 
	 * @return
	 * @throws Exception
	 */
	public static String sign(byte[] data, String privateKey) throws Exception {
		byte[] keyBytes = Base64Utils.decode(privateKey);
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		PrivateKey privateK = keyFactory.generatePrivate(pkcs8KeySpec);
		Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
		signature.initSign(privateK);
		signature.update(data);
		return Base64Utils.encode(signature.sign());
	}

	/**
	 * <p>
	 * 校验数字签名
	 * </p>
	 * 
	 * @param data
	 *            已加密数据
	 * @param publicKey
	 *            公钥(BASE64编码)
	 * @param sign
	 *            数字签名
	 * 
	 * @return
	 * @throws Exception
	 * 
	 */
	public static boolean verify(byte[] data, String publicKey, String sign)
			throws Exception {
		byte[] keyBytes = Base64Utils.decode(publicKey);
		X509EncodedKeySpec keySpec = new X509EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		PublicKey publicK = keyFactory.generatePublic(keySpec);
		Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
		signature.initVerify(publicK);
		signature.update(data);
		return signature.verify(Base64Utils.decode(sign));
	}

	/**
	 * <P>
	 * 私钥解密
	 * </p>
	 * 
	 * @param encryptedData
	 *            已加密数据
	 * @param privateKey
	 *            私钥(BASE64编码)
	 * @return
	 * @throws Exception
	 */
	public static byte[] decryptByPrivateKey(byte[] encryptedData,
			String privateKey) throws Exception {
		byte[] keyBytes = Base64Utils.decode(privateKey);
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key privateK = keyFactory.generatePrivate(pkcs8KeySpec);
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.DECRYPT_MODE, privateK);
		int inputLen = encryptedData.length;
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		int offSet = 0;
		byte[] cache;
		int i = 0;
		// 对数据分段解密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_DECRYPT_BLOCK) {
				cache = cipher
						.doFinal(encryptedData, offSet, MAX_DECRYPT_BLOCK);
			} else {
				cache = cipher
						.doFinal(encryptedData, offSet, inputLen - offSet);
			}
			out.write(cache, 0, cache.length);
			i++;
			offSet = i * MAX_DECRYPT_BLOCK;
		}
		byte[] decryptedData = out.toByteArray();
		out.close();
		return decryptedData;
	}

	/**
	 * <p>
	 * 公钥解密
	 * </p>
	 * 
	 * @param encryptedData
	 *            已加密数据
	 * @param publicKey
	 *            公钥(BASE64编码)
	 * @return
	 * @throws Exception
	 */
	public static byte[] decryptByPublicKey(byte[] encryptedData,
			String publicKey) throws Exception {
		byte[] keyBytes = Base64Utils.decode(publicKey);
		X509EncodedKeySpec x509KeySpec = new X509EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key publicK = keyFactory.generatePublic(x509KeySpec);
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.DECRYPT_MODE, publicK);
		int inputLen = encryptedData.length;
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		int offSet = 0;
		byte[] cache;
		int i = 0;
		// 对数据分段解密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_DECRYPT_BLOCK) {
				cache = cipher
						.doFinal(encryptedData, offSet, MAX_DECRYPT_BLOCK);
			} else {
				cache = cipher
						.doFinal(encryptedData, offSet, inputLen - offSet);
			}
			out.write(cache, 0, cache.length);
			i++;
			offSet = i * MAX_DECRYPT_BLOCK;
		}
		byte[] decryptedData = out.toByteArray();
		out.close();
		return decryptedData;
	}

	/**
	 * <p>
	 * 公钥加密
	 * </p>
	 * 
	 * @param data
	 *            源数据
	 * @param publicKey
	 *            公钥(BASE64编码)
	 * @return
	 * @throws Exception
	 */
	public static byte[] encryptByPublicKey(byte[] data, String publicKey)
			throws Exception {
		byte[] keyBytes = Base64Utils.decode(publicKey);
		X509EncodedKeySpec x509KeySpec = new X509EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key publicK = keyFactory.generatePublic(x509KeySpec);
		// 对数据加密
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.ENCRYPT_MODE, publicK);
		int inputLen = data.length;
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		int offSet = 0;
		byte[] cache;
		int i = 0;
		// 对数据分段加密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_ENCRYPT_BLOCK) {
				cache = cipher.doFinal(data, offSet, MAX_ENCRYPT_BLOCK);
			} else {
				cache = cipher.doFinal(data, offSet, inputLen - offSet);
			}
			out.write(cache, 0, cache.length);
			i++;
			offSet = i * MAX_ENCRYPT_BLOCK;
		}
		byte[] encryptedData = out.toByteArray();
		out.close();
		return encryptedData;
	}

	/**
	 * <p>
	 * 私钥加密
	 * </p>
	 * 
	 * @param data
	 *            源数据
	 * @param privateKey
	 *            私钥(BASE64编码)
	 * @return
	 * @throws Exception
	 */
	public static byte[] encryptByPrivateKey(byte[] data, String privateKey)
			throws Exception {
		byte[] keyBytes = Base64Utils.decode(privateKey);
		PKCS8EncodedKeySpec pkcs8KeySpec = new PKCS8EncodedKeySpec(keyBytes);
		KeyFactory keyFactory = KeyFactory.getInstance(KEY_ALGORITHM);
		Key privateK = keyFactory.generatePrivate(pkcs8KeySpec);
		Cipher cipher = Cipher.getInstance(keyFactory.getAlgorithm());
		cipher.init(Cipher.ENCRYPT_MODE, privateK);
		int inputLen = data.length;
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		int offSet = 0;
		byte[] cache;
		int i = 0;
		// 对数据分段加密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_ENCRYPT_BLOCK) {
				cache = cipher.doFinal(data, offSet, MAX_ENCRYPT_BLOCK);
			} else {
				cache = cipher.doFinal(data, offSet, inputLen - offSet);
			}
			out.write(cache, 0, cache.length);
			i++;
			offSet = i * MAX_ENCRYPT_BLOCK;
		}
		byte[] encryptedData = out.toByteArray();
		out.close();
		return encryptedData;
	}

	/**
	 * <p>
	 * 获取私钥
	 * </p>
	 * 
	 * @param keyMap
	 *            密钥对
	 * @return
	 * @throws Exception
	 */
	public static String getPrivateKey(Map<String, Object> keyMap)
			throws Exception {
		Key key = (Key) keyMap.get(PRIVATE_KEY);
		return Base64Utils.encode(key.getEncoded());
	}

	/**
	 * <p>
	 * 获取公钥
	 * </p>
	 * 
	 * @param keyMap
	 *            密钥对
	 * @return
	 * @throws Exception
	 */
	public static String getPublicKey(Map<String, Object> keyMap)
			throws Exception {
		Key key = (Key) keyMap.get(PUBLIC_KEY);
		return Base64Utils.encode(key.getEncoded());
	}

}
```



### 2.Token管理工具



```java
package com.wlyd.fmcgwms.util;

import java.sql.Timestamp;
import java.util.Date;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.wlyd.fmcgwms.persistence.beans.api.AccessToken;
import com.wlyd.fmcgwms.util.api.RSAUtils;

/**
 * SAAS Token管理工具
 * 
 * @packge com.wlyd.wmscloud.util.SAASTokenManager
 * @date 2016年5月6日 上午10:20:01
 * @author pengjunlin
 * @comment
 * @update
 */
public class SAASTokenManager {
	

	/**
	 * Token存储对象，保持5000个并发容量(K-useraccount@corCode,V-token)
	 */
	public static Map<String, Object> map = new ConcurrentHashMap<String, Object>(
			5000);

	/**
	 * 获取用户Token
	 * 
	 * @MethodName: getToken
	 * @Description:
	 * @param key
	 * @return
	 * @throws
	 */
	public static AccessToken getToken(String key) {
		if (map.containsKey(key)) {
			return (AccessToken) map.get(key);
		}
		return null;
	}

	/**
	 * 添加用户token
	 * 
	 * @MethodName: putToken
	 * @Description:
	 * @param key
	 *            useraccount@corCode
	 * @param accessToken
	 * @throws
	 */
	public static void putToken(String key, AccessToken accessToken) {
		map.put(key, accessToken);
	}

	/**
	 * 移除token
	 * 
	 * @MethodName: removeToken
	 * @Description:
	 * @param key
	 *            useraccount@corCode
	 * @throws
	 */
	public static void removeToken(String key) {
		if (map.containsKey(key)) {
			map.remove(key);
		}
	}

	/**
	 * 验证Token是否过期
	 * 
	 * @MethodName: isVlidateToken
	 * @Description:
	 * @param key
	 *            useraccount@corCode
	 * @return
	 * @throws
	 */
	public static boolean isVlidateToken(String key) {
		if (map.containsKey(key)) {
			AccessToken accessToken = (AccessToken) map.get(key);
			long currentTimestamp = new Date().getTime();
			// 有效时间两小时
			if (accessToken.getLongTime() - currentTimestamp > 2 * 3600 * 1000) {
				return false;
			}
			return true;
		}
		return false;
	}

	/**
	 * 更新Token
	 * 
	 * @MethodName: reputToken
	 * @Description:
	 * @param key
	 *            useraccount@corCode
	 * @param accessToken
	 * @return
	 * @throws
	 */
	public static void reputToken(String key, AccessToken accessToken) {
		if (map.containsKey(key)) {
			putToken(key, accessToken);
		}
	}

	/**
	 * 更新Token
	 * 
	 * @MethodName: reputToken
	 * @Description:
	 * @param key
	 *            useraccount@corCode
	 * @param tokenStr
	 * @return
	 * @throws
	 */
	public static void reputToken(String key, String tokenStr) {
		if (map.containsKey(key)) {
			AccessToken accessToken = new AccessToken();
			accessToken.setToken(tokenStr);
			accessToken.setTimestamp(new Timestamp(new Date().getTime()));
			putToken(key, accessToken);
		}
	}
	
	/**
	 * 是否包含用户token
	 * @MethodName: iscontainKey 
	 * @Description: 
	 * @param key
	 *          useraccount@corCode
	 * @return
	 * @throws
	 */
	public static boolean iscontainKey(String key){
		return map.containsKey(key);
	}
	
	/**
	 * 生成RSA加密 Token
	 * 
	 * @MethodName: generateToken 
	 * @Description: 
	 * @param platformCode
	 * @param tenantCode
	 * @return
	 * @throws
	 */
	public static String generateToken(String publicKey,String platformCode,String tenantCode){
		String str=platformCode+tenantCode+new Date().getTime();
		try {
			byte [] bytes= RSAUtils.encryptByPublicKey(str.getBytes(),publicKey);
			return new String( bytes ,"UTF-8");
		} catch (Exception e) {
			e.printStackTrace(); 
		}
		return null;
	}

	/**
	 * @throws Exception 
	 * 测试函数入口
	 * 
	 * @MethodName: main
	 * @Description:
	 * @param args
	 * @throws
	 */
	public static void main(String[] args) throws Exception {
		System.out.println(Md5.getMD5Str("123456"));
		String key = "wmsadmin@10000"; 
		AccessToken accessToken = new AccessToken();
		accessToken.setToken("token==xxjisifdihfifdds");
		accessToken.setTimestamp(new Timestamp(new Date().getTime()));
		putToken(key, accessToken);
		AccessToken accessToken2 = getToken(key);
		System.out.println("token:" + accessToken2.getToken());
		System.out.println("isValidate:" + isVlidateToken(key));
		
		Map<String, Object> keyMap=RSAUtils.genKeyPair();
		String publicKey=RSAUtils.getPublicKey(keyMap);
		String privateKey=RSAUtils.getPrivateKey(keyMap);
		
		String token=generateToken(publicKey,"abcdefghijklmnopqrstuvwxyz", "10000");
		
		System.out.println("RSA Token:"+token);
		
		System.out.println("加密:"+new String(RSAUtils.encryptByPublicKey("this is data".getBytes(), publicKey),"UTF-8"));
		System.out.println("明文:"+new String(RSAUtils.decryptByPrivateKey(RSAUtils.encryptByPublicKey("this is data".getBytes(), publicKey), privateKey)));
	}

}
```



### 3.Token实体对象



```java
package com.wlyd.fmcgwms.persistence.beans.api;

import java.io.Serializable;
import java.sql.Timestamp;

/**
 * SAAS Token WMS管理实体
 * 
 * @packge com.wlyd.wmscloud.persistence.beans.api.AccessToken
 * @date 2016年5月6日 上午10:27:55
 * @author pengjunlin
 * @comment
 * @update
 */
public class AccessToken implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = 4759692267927548118L;

	private String token;// AccessToken字符串

	private Timestamp timestamp;// 时间戳(用于验证token和重新获取token)

	public String getToken() {
		return token;
	}

	public void setToken(String token) {
		this.token = token;
	}

	public Timestamp getTimestamp() {
		return timestamp;
	}

	public void setTimestamp(Timestamp timestamp) {
		this.timestamp = timestamp;
	}
	
	public long getLongTime(){
		if(timestamp!=null){
			return timestamp.getTime();
		}
		return 0;
	}

}
```


RSA使用时记住：公钥加密私钥解密。

## 4.Base64Utils



```java
package com.wlyd.fmcgwms.util.api;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import org.bouncycastle.util.encoders.Base64;

/** 
 * <p> 
 * BASE64编码解码工具包 
 * </p> 
 * <p> 
 * 依赖javabase64-1.3.1.jar 
 * </p> 
 *  
 * @author IceWee 
 * @date 2012-5-19 
 * @version 1.0 
 */  
public class Base64Utils {  
  
    /** 
     * 文件读取缓冲区大小 
     */  
    private static final int CACHE_SIZE = 1024;  
      
    /** 
     * <p> 
     * BASE64字符串解码为二进制数据 
     * </p> 
     *  
     * @param base64 
     * @return 
     * @throws Exception 
     */  
    public static byte[] decode(String base64) throws Exception {  
        return Base64.decode(base64.getBytes());  
    }  
      
    /** 
     * <p> 
     * 二进制数据编码为BASE64字符串 
     * </p> 
     *  
     * @param bytes 
     * @return 
     * @throws Exception 
     */  
    public static String encode(byte[] bytes) throws Exception {  
        return new String(Base64.encode(bytes));  
    }  
      
    /** 
     * <p> 
     * 将文件编码为BASE64字符串 
     * </p> 
     * <p> 
     * 大文件慎用，可能会导致内存溢出 
     * </p> 
     *  
     * @param filePath 文件绝对路径 
     * @return 
     * @throws Exception 
     */  
    public static String encodeFile(String filePath) throws Exception {  
        byte[] bytes = fileToByte(filePath);  
        return encode(bytes);  
    }  
      
    /** 
     * <p> 
     * BASE64字符串转回文件 
     * </p> 
     *  
     * @param filePath 文件绝对路径 
     * @param base64 编码字符串 
     * @throws Exception 
     */  
    public static void decodeToFile(String filePath, String base64) throws Exception {  
        byte[] bytes = decode(base64);  
        byteArrayToFile(bytes, filePath);  
    }  
      
    /** 
     * <p> 
     * 文件转换为二进制数组 
     * </p> 
     *  
     * @param filePath 文件路径 
     * @return 
     * @throws Exception 
     */  
    public static byte[] fileToByte(String filePath) throws Exception {  
        byte[] data = new byte[0];  
        File file = new File(filePath);  
        if (file.exists()) {  
            FileInputStream in = new FileInputStream(file);  
            ByteArrayOutputStream out = new ByteArrayOutputStream(2048);  
            byte[] cache = new byte[CACHE_SIZE];  
            int nRead = 0;  
            while ((nRead = in.read(cache)) != -1) {  
                out.write(cache, 0, nRead);  
                out.flush();  
            }  
            out.close();  
            in.close();  
            data = out.toByteArray();  
         }  
        return data;  
    }  
      
    /** 
     * <p> 
     * 二进制数据写文件 
     * </p> 
     *  
     * @param bytes 二进制数据 
     * @param filePath 文件生成目录 
     */  
    public static void byteArrayToFile(byte[] bytes, String filePath) throws Exception {  
        InputStream in = new ByteArrayInputStream(bytes);     
        File destFile = new File(filePath);  
        if (!destFile.getParentFile().exists()) {  
            destFile.getParentFile().mkdirs();  
        }  
        destFile.createNewFile();  
        OutputStream out = new FileOutputStream(destFile);  
        byte[] cache = new byte[CACHE_SIZE];  
        int nRead = 0;  
        while ((nRead = in.read(cache)) != -1) {     
            out.write(cache, 0, nRead);  
            out.flush();  
        }  
        out.close();  
        in.close();  
    }  
}
```
Base64的工具网上很多，随便找了个用的。











