# Spring设计安全的Restful接口-无用户状态的安全 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月16日 15:51:08[boonya](https://me.csdn.net/boonya)阅读数：2457








设计安全的Restful接口：无用户交互状态的接口安全设计。我们需要实现UR拦截请求拦截，接口接入授权验证和请求验重。本文中不涉及任何用户校验。

## 设计原理

1.UR拦截请求拦截：通过URL进行拦截过滤；

2.接入授权验证：验证请求头Token；

3.请求验重：验证请求序列Sequence；

接口设计文档请参考：[WMS系统对外开放接口设计文档-V.1.0.docx](http://download.csdn.net/detail/boonya/9783390)

注：文档禁用于商业用途！

## 安全过滤器



```java
package com.wlyd.fmcgwms.util.security;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.alibaba.fastjson.JSONObject;
import com.wlyd.fmcgwms.util.Log;
import com.wlyd.fmcgwms.util.SAASTokenManager;
import com.wlyd.fmcgwms.util.StaticProperty;
import com.wlyd.fmcgwms.util.api.RSAUtils;
import com.wlyd.fmcgwms.util.ehcache.EhcacheUtil;
import com.wlyd.fmcgwms.util.sysinit.InitSysProperties;
/**
 * 接口安全过滤器
 * 
 * @package com.wlyd.fmcgwms.util.security.SecurityFilter
 * @date   2017年3月15日  下午1:57:18
 * @author pengjunlin
 * @comment   
 * @update
 */
public class SecurityFilter implements Filter {
	
	private static String API_ISENABLE ;// WMS是否开通对外接口
	
	private static String API_PALTFORM;// WMS开放的平台编码
	
	private static String API_MEMBERCODE;//WMS开放的服务商编码
	
	private static String publicKey;// 公钥
	
	private static String privateKey;// 私钥
	

	@Override
	public void destroy() {
		// TODO Auto-generated method stub
	}
	
	/**
	 * 验证配置是否规范
	 * 
	 * @MethodName: isConfiged 
	 * @Description: 
	 * @param platformCode
	 * @param memberCode
	 * @return
	 * @throws
	 */
	private boolean isConfiged(String platformCode,String memberCode){
        if(API_ISENABLE==null||!API_ISENABLE.equals("true")){
			return false;
		}
        if(API_PALTFORM==null||platformCode==null||!platformCode.equals(API_PALTFORM)){
        	return false;
        }
		if (API_MEMBERCODE==null||memberCode == null||! memberCode.equals(API_MEMBERCODE)) {
			return false;
		}
		return true;
	}
	
	/**
	 * 验证token是否有效
	 * 
	 * @MethodName: validateToken 
	 * @Description: 
	 * @param token
	 * @return
	 * @throws
	 */
	private boolean validateToken(String token){
		if(token==null||token.equals("")){
			return false;
		}
		String params[]=SAASTokenManager.decryptToken(privateKey, token, "&");
		if(params==null||params.length<3){
			return false;
		}
		long now=System.currentTimeMillis();
		// Token超时验证20s
		long seconds=(now-Long.valueOf(params[2]))/1000;
		if(!API_PALTFORM.equals(params[0])||!API_MEMBERCODE.equals(params[1])||seconds>20){
			return false;
		}
		return true;
	}
	
	/**
	 * 验证请求是否重复
	 * 
	 * @MethodName: validateSequece 
	 * @Description: 
	 * @param sequence
	 * @return
	 * @throws
	 */
	public boolean validateSequece(String sequence){
		if(sequence==null||sequence.equals("")){
			return false;
		}
		String requestSequence=(String) EhcacheUtil.get(StaticProperty.REQUESTCACHE, sequence);
		// 请求序列相同验证失败
		if(requestSequence!=null&&sequence.equals(requestSequence)){
			return false;
		}
		return true;
	}

	@Override
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		PrintWriter out = null;
		HttpServletRequest req = (HttpServletRequest) request;
		HttpServletResponse res = (HttpServletResponse) response;
		String platformCode = req.getHeader("PlatformCode");
		String memberCode = req.getHeader("MemberCode");
		String token = req.getHeader("Token");
		String sequence = req.getHeader("Sequence");
		//String path = req.getServletPath();
		try {
			byte [] bytes=SAASTokenManager.generateBytesToken(publicKey, platformCode, memberCode,"&");
			token = RSAUtils.bytesToString(bytes);
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		
		// 验证接口是否配置正确 
		if(!isConfiged(platformCode, memberCode)){
			JSONObject json = new JSONObject();
			json.put("IsSuccess", "false");
			json.put("OperationDesc", "API parameters are not configed right! ");
			json.put("ResultCode", ResultCode.OPEN_API_CONFIG_ERROR);
			try {
				out = res.getWriter();
				out.write(json.toJSONString());
			} catch (Exception e) {
				e.printStackTrace();
			}
			return;
		}
		
		// 验证Token是否合法
		if(!validateToken(token)){
			JSONObject json = new JSONObject();
			json.put("IsSuccess", "false");
			json.put("OperationDesc", "Unauthorized:Token is invalid!");
			json.put("ResultCode", ResultCode.OPEN_API_TOKEN_INVALID);
			try {
				out = res.getWriter();
				out.write(json.toJSONString());
			} catch (Exception e) {
				e.printStackTrace();
			}
			return;
		}
		
		// 验证Sequence是否合法
		if(!validateSequece(sequence)){
			JSONObject json = new JSONObject();
			json.put("IsSuccess", "false");
			json.put("OperationDesc", "Refused:request API too frequently!");
			json.put("ResultCode", ResultCode.OPEN_API_REQUEST_REQUENTLY);
			try {
				out = res.getWriter();
				out.write(json.toJSONString());
			} catch (Exception e) {
				e.printStackTrace();
			}
			return;
		}

		chain.doFilter(request, response);

	}

	@Override
	public void init(FilterConfig arg0) throws ServletException {
		Log.getLogger(getClass()).info(">>>SecurityFilter invoke init method。。。。。。。。。START!"); 
		API_ISENABLE = InitSysProperties.getLowerCaseFromEhcache(StaticProperty.WMS_OPEN_API_ISENABLE);
		API_PALTFORM = InitSysProperties.getUpperCaseFromEhcache(StaticProperty.WMS_OPEN_API_PLATFORM);
		API_MEMBERCODE = InitSysProperties.getUpperCaseFromEhcache(StaticProperty.WMS_OPEN_API_MEMBERCODE);
		publicKey=EhcacheUtil.get(StaticProperty.WMS_OPEN_API_RSA_PUBLIC_KEY).toString();
		privateKey=EhcacheUtil.get(StaticProperty.WMS_OPEN_API_RSA_PRIVATE_KEY).toString();
		Log.getLogger(getClass()).info(">>>SecurityFilter invoke init method。。。。。。。。。SUCCESS!"); 
	}

}
```

注：需要通过RSA工具生成密钥对（公钥&私钥）。
## web.xml配置过滤器



```
<filter>
		<filter-name>SecurityFilter</filter-name>
		<filter-class>com.wlyd.fmcgwms.util.security.SecurityFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>SecurityFilter</filter-name>
		<url-pattern>/openapi/*</url-pattern>
	</filter-mapping>
```


## 控制层openapi接口


```java
package com.wlyd.fmcgwms.controller.security;

import java.util.HashMap;
import java.util.Map;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import com.wlyd.fmcgwms.controller.BaseController;
import com.wlyd.fmcgwms.util.Tools;
/**
 * 开放API控制层
 * 
 * @package com.wlyd.fmcgwms.controller.platform.OpenAPIController
 * @date   2017年3月14日  下午4:52:12
 * @author pengjunlin
 * @comment   
 * @update
 */
@Controller
@RequestMapping("/openapi")
public class OpenAPIController extends BaseController{
	
	/**
	 * 未授权
	 * 
	 * @MethodName: unauthenticated 
	 * @Description: 
	 * @return
	 * @throws
	 */
	@RequestMapping("/unauthenticated")
	@ResponseBody
	public String unauthenticated(){
		Map<String,Object> map=new HashMap<String, Object>();
		map.put("IsSuccess", "false");
		map.put("OperationDesc", "Unauthenticated:Please contact to WMS developers!");
		return Tools.toJson(map);
	}
	
	/**
	 * 授权成功
	 * 
	 * @MethodName: success 
	 * @Description: 
	 * @return
	 * @throws
	 */
	@RequestMapping("/success")
	@ResponseBody
	public String success(){
		Map<String,Object> map=new HashMap<String, Object>();
		map.put("IsSuccess", "true");
		map.put("OperationDesc", "Authenticated!");
		return Tools.toJson(map);
	}

}
```


没有其它的方法。



## RSA加密工具



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
	
	/**
	 * 将加密后的字节数组转换为对象
	 * 
	 * @MethodName: bytesToString 
	 * @Description: 
	 * @param encrytpByte
	 * @return
	 * @throws
	 */
	public static String bytesToString(byte[] encrytpByte) {
	     String result = "";
	     for (Byte bytes : encrytpByte) {
	         result += bytes.toString() + " ";
	     }
	     return result;
	 }
	
	/**
	 * 公钥加密
	 * 
	 * @MethodName: encrypt 
	 * @Description: 
	 * @param publicKey
	 * @param obj
	 * @return
	 * @throws
	 */
	public static byte[] encrypt(RSAPublicKey publicKey, byte[] obj) {
	     if (publicKey != null) {
	         try {
	             Cipher cipher = Cipher.getInstance(KEY_ALGORITHM);
	             cipher.init(Cipher.ENCRYPT_MODE, publicKey);
	             return cipher.doFinal(obj);
	         } catch (Exception e) {
	             e.printStackTrace();
	         }
	     }
	     return null;
	 }
	
	/**
	 * 私钥加密
	 * 
	 * @MethodName: decrypt 
	 * @Description: 
	 * @param privateKey
	 * @param obj
	 * @return
	 * @throws
	 */
	public static byte[] decrypt(RSAPrivateKey privateKey, byte[] obj) {
	     if (privateKey != null) {
	         try {
	             Cipher cipher = Cipher.getInstance(KEY_ALGORITHM);
	             cipher.init(Cipher.DECRYPT_MODE, privateKey);
	             return cipher.doFinal(obj);
	         } catch (Exception e) {
	             e.printStackTrace();
	         }
	     }
	     return null;
	 }

}
```


## Token管理工具



```java
package com.wlyd.fmcgwms.util;

import java.sql.Timestamp;
import java.util.Date;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.wlyd.fmcgwms.persistence.beans.api.AccessToken;
import com.wlyd.fmcgwms.util.api.Base64Utils;
import com.wlyd.fmcgwms.util.api.RSAUtils;
import com.wlyd.fmcgwms.util.ehcache.EhcacheUtil;
import com.wlyd.fmcgwms.util.sysinit.InitSysProperties;

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
	 * Token存储对象，保持100000个并发容量(K-useraccount@corCode,V-token)
	 */
	private static final Map<String, Object> map = new ConcurrentHashMap<String, Object>(
			100000);

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
		String str=platformCode+"$"+tenantCode+"$"+new Date().getTime();
		try {
			byte [] bytes= RSAUtils.encryptByPublicKey(str.getBytes(),publicKey);
		//	return new String( bytes ,"UTF-8");
			return Base64Utils.encode(bytes);
		} catch (Exception e) {
			e.printStackTrace(); 
		}
		return null;
	}
	
	/**
	 * 生成RSA加密 Token
	 * 
	 * @MethodName: generateBytesToken 
	 * @Description: 
	 * @param platformCode
	 * @param tenantCode
	 * @return
	 * @throws
	 */
	public static byte [] generateBytesToken(String publicKey,String platformCode,String tenantCode){
		byte [] bytes=new byte[0];
		String str=platformCode+"$"+tenantCode+"$"+new Date().getTime();
		try {
			bytes= RSAUtils.encryptByPublicKey(str.getBytes(),publicKey);
			return bytes;
		} catch (Exception e) {
			e.printStackTrace(); 
		}
		return null;
	}
	
	/**
	 * 生成RSA加密 Token
	 * 
	 * @MethodName: generateBytesToken 
	 * @Description: 
	 * @param platformCode
	 * @param tenantCode
	 * @param regex
	 * @return
	 * @throws
	 */
	public static byte [] generateBytesToken(String publicKey,String platformCode,String tenantCode,String regex){
		byte [] bytes=new byte[0];
		String str=platformCode+regex+tenantCode+regex+new Date().getTime();
		try {
			bytes= RSAUtils.encryptByPublicKey(str.getBytes(),publicKey);
			return bytes;
		} catch (Exception e) {
			e.printStackTrace(); 
		}
		return null;
	}
	
	/**
	 * 模拟自动生成Token
	 * 
	 * @MethodName: getToken 
	 * @Description: 
	 * @return
	 * @throws
	 */
	public  static String getToken(){
		// 开启线程同步库存数据到OMS
		/*Map<String, Object> keyMap=null;
		try {
			keyMap = RSAUtils.genKeyPair();
		} catch (Exception e) {
			e.printStackTrace();
		}
		String publicKey=null;
		try {
			publicKey = RSAUtils.getPublicKey(keyMap);
		} catch (Exception e) {
			e.printStackTrace();
		}
		String privateKey=null;
		try {
			privateKey = RSAUtils.getPrivateKey(keyMap);
		} catch (Exception e) {
			e.printStackTrace();
		}*/
		String token=null;
		try {
			String publicKey=EhcacheUtil.get("OMS_RSA_PUBLIC_KEY").toString();
			//String privateKey=EhcacheUtil.get("OMS_RSA_PRIVATE_KEY").toString();
			String platformCode = (String) EhcacheUtil.get("WAAS_PLATFORMCODE");
			String memberCode =  InitSysProperties.getUpperCaseFromEhcache(StaticProperty.WAAS_MEMBERCODE);
			token = SAASTokenManager.generateToken(publicKey, platformCode, memberCode);
			if(token!=null){
				token.replaceAll("\\n", "");
				token.replaceAll("\\r", "");
			}
		} catch (Exception e) { 
			Log.getLogger(SAASTokenManager.class).error("Token Error:"+e.getMessage());
			e.printStackTrace();
		}
		return token;
	}
	
	/**
	 * 解密Token
	 * 
	 * @MethodName: decryptToken 
	 * @Description: 
	 * @param privateKey
	 * @param token
	 * @param regex
	 * @return
	 * @throws
	 */
	public static String [] decryptToken(String privateKey,String token,String regex){
		String params[]=new String[0];
		try {
			String[] strArr = token.split(" ");
	        
	        int len = strArr.length;
	        
	        // 转回bytes
	        byte[] clone = new byte[len];
	         
	        for (int i = 0; i < len; i++) {
	             clone[i] = Byte.parseByte(strArr[i]);
	        }
	        String decryptedToken=new String(RSAUtils.decryptByPrivateKey(clone, privateKey));
			params=decryptedToken.split(regex);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return params;
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

		System.out.println("publicKey:\n"+publicKey);
		System.out.println("privateKey:\n"+privateKey);

		String token=generateToken(publicKey,"1234", "10000");
		
		byte [] bitesToken=RSAUtils.encryptByPublicKey(("1234$10000$"+new Date().getTime()).getBytes(), publicKey);
		
		System.out.println("RSA Token:"+token);
		
		System.out.println("RSA bites Token加密:"+new String(bitesToken,"UTF-8")); 
		
		System.out.println("RSA bites Token解密:"+new String(RSAUtils.decryptByPrivateKey(bitesToken, privateKey))); 
		
		System.out.println("加密:"+new String(RSAUtils.encryptByPublicKey("this is data".getBytes(), publicKey),"UTF-8"));
		System.out.println("明文:"+new String(RSAUtils.decryptByPrivateKey(RSAUtils.encryptByPublicKey("this is data".getBytes(), publicKey), privateKey)));
	}

}
```

本文中所述的token我并没有保存下来，而是客户端每次来了都进行验证。
## Ehcache缓存请求30s验重

缓存方案你也可以用Redis来做，这里因为项目使用的是Ehcache故将就之。

ehcahe配置：ehcache.xml





```
<?xml version="1.0" encoding="UTF-8"?>
<!-- 分布式缓存对象同步  采用RMI方式  官方还提供其他同步方式  此处略 -->
<ehcache xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:noNamespaceSchemaLocation="ehcache.xsd"
         updateCheck="true" monitoring="autodetect"
         dynamicConfig="true">

 	<!-- <diskStore path="java.io.tmpdir" /> -->
	<!--1. 指定除自身之外的网络群体中其他提供同步的主机列表，用“|”分开不同的主机 -->
	<!-- <cacheManagerPeerProviderFactory
		class="net.sf.ehcache.distribution.RMICacheManagerPeerProviderFactory"
		properties="peerDiscovery=manual,rmiUrls=//10.100.0.60:40004/metaCache" /> -->

	<!--2. 配宿主主机配置监听程序，来发现其他主机发来的同步请求 -->
	<!-- <cacheManagerPeerListenerFactory
		class="net.sf.ehcache.distribution.RMICacheManagerPeerListenerFactory"
		properties="port=40004,socketTimeoutMillis=120000" />此处默认8000端口 -->
	
	<!-- 默认缓存 -->
	<defaultCache 
		maxElementsInMemory="10000" 
		eternal="false"
		timeToIdleSeconds="1800" 
		overflowToDisk="true"
		diskSpoolBufferSizeMB="30" 
		maxElementsOnDisk="10000000"
		diskPersistent="false" 
		diskExpiryThreadIntervalSeconds="120"
		memoryStoreEvictionPolicy="LRU">
	</defaultCache>
	
	<!-- 缓存 -->
	<cache name="metaCache" 
		maxElementsInMemory="1000" 
		eternal="false"
		timeToIdleSeconds="0" 
		overflowToDisk="false"
		diskSpoolBufferSizeMB="30" 
		maxElementsOnDisk="10000000"
		diskPersistent="false" 
		diskExpiryThreadIntervalSeconds="120"
		memoryStoreEvictionPolicy="LRU">
		<!-- <cacheEventListenerFactory
    			class="net.sf.ehcache.distribution.RMICacheReplicatorFactory"
 			properties="replicateAsynchronously=true, 
 						replicatePuts=false, 
 						replicateUpdates=true,
 						replicateUpdatesViaCopy=true, 
 						replicateRemovals=true "/>
		<bootstrapCacheLoaderFactory
			class="net.sf.ehcache.distribution.RMIBootstrapCacheLoaderFactory"
			properties="bootstrapAsynchronously=false" /> -->
	</cache>

	<cache name="REQUESTCACHE" 
		maxElementsInMemory="100000" 
		eternal="false"
		timeToIdleSeconds="30" 
		timeToLiveSeconds="30" 
		overflowToDisk="false"
		maxElementsOnDisk="0" 
		diskPersistent="false"
		diskExpiryThreadIntervalSeconds="0" 
		memoryStoreEvictionPolicy="LRU">

	</cache>
</ehcache>
```


Ehcache工具类：





```java
package com.wlyd.fmcgwms.util.ehcache;

import net.sf.ehcache.Cache;
import net.sf.ehcache.CacheManager;
import net.sf.ehcache.Element;

/**
 * ehcache 缓存工具类
 * 
 * @packge com.wlyd.fmcgwms.util.ehcache.EhcacheUtil
 * @date 2016年4月28日 下午4:11:27
 * @author pengjunlin
 * @comment cacheName在ehcache.xml中配置
 * @update 添加注释，代码重构删除原有类com。wlyd.fmcgwms.util.ehcache.EhcacheUtilOverWrite
 */
public class EhcacheUtil {

	public static CacheManager manager = CacheManager.create();// 缓存管理

	public static String cacheName = "metaCache";// 缓存名称

	/**
	 * 获取缓存对象
	 * 
	 * @MethodName: get
	 * @Description:
	 * @param key
	 * @return
	 * @throws
	 */
	public static Object get(Object key) {
		Cache cache = manager.getCache(cacheName);
		if (cache != null) {
			Element element = cache.get(key);
			if (element != null) {
				return element.getObjectValue();
			}
		}
		return null;
	}

	/**
	 * 获取缓存对象
	 * 
	 * @MethodName: get
	 * @Description:
	 * @param cacheName
	 * @param key
	 * @return
	 * @throws
	 */
	public static Object get(String cacheName, Object key) {
		Cache cache = manager.getCache(cacheName);
		if (cache != null) {
			Element element = cache.get(key);
			if (element != null) {
				return element.getObjectValue();
			}
		}
		return null;
	}

	/**
	 * 添加缓存对象
	 * 
	 * @MethodName: put
	 * @Description:
	 * @param key
	 * @param value
	 * @throws
	 */
	public static void put(Object key, Object value) {
		Cache cache = manager.getCache(cacheName);
		if (cache != null) {
			cache.put(new Element(key, value));
		}
	}

	/**
	 * 添加缓存对象
	 * 
	 * @MethodName: put
	 * @Description:
	 * @param cacheName
	 * @param key
	 * @param value
	 * @throws
	 */
	public static void put(String cacheName, Object key, Object value) {
		Cache cache = manager.getCache(cacheName);
		if (cache != null) {
			cache.put(new Element(key, value));
		}
	}

	/**
	 * 移出缓存对象
	 * 
	 * @MethodName: remove
	 * @Description:
	 * @param key
	 * @return
	 * @throws
	 */
	public static boolean remove(Object key) {
		Cache cache = manager.getCache(cacheName);
		if (cache != null) {
			return cache.remove(key);
		}
		return false;
	}

	/**
	 * 移除缓存对象
	 * 
	 * @MethodName: remove
	 * @Description:
	 * @param cacheName
	 * @param key
	 * @return
	 * @throws
	 */
	public static boolean remove(String cacheName, Object key) {
		Cache cache = manager.getCache(cacheName);
		if (cache != null) {
			return cache.remove(key);
		}
		return false;
	}
}
```







测试方法：





```java
package fmcgwms;


import org.apache.http.client.methods.HttpGet;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import com.alibaba.fastjson.JSONObject;
import com.wlyd.fmcgwms.util.Md5;
import com.wlyd.fmcgwms.util.SAASTokenManager;
import com.wlyd.fmcgwms.util.StaticProperty;
import com.wlyd.fmcgwms.util.api.APIHttpClient;
import com.wlyd.fmcgwms.util.api.RSAUtils;
import com.wlyd.fmcgwms.util.ehcache.EhcacheUtil;
/**
 * 开放接口对外模拟测试
 * @package fmcgwms.OpenApiTest
 * @date   2017年3月15日  下午5:51:03
 * @author pengjunlin
 * @comment   
 * @update
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class OpenApiTest {
```

```java
@Test
	public void testEchcacheToTimeout(){
		
		
		String requestSequence=Md5.getMD5Str("localhost&interface&opt_type&uniquecode");
		
		System.out.println(">>>>>原始MD5串："+requestSequence); 
		
		EhcacheUtil.put(StaticProperty.REQUESTCACHE, requestSequence, requestSequence);
		
		System.out.println(">>>>>缓存："+EhcacheUtil.get(StaticProperty.REQUESTCACHE, requestSequence)); 
		
		try {
			Thread.sleep(31000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(">>>>>30s后缓存："+EhcacheUtil.get(StaticProperty.REQUESTCACHE, requestSequence)); 
		
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		
	}
```




Ehcache的输出：



```
>>>>>原始MD5串：cae76d65fc5c47875730ed35a262abb4
>>>>>缓存：cae76d65fc5c47875730ed35a262abb4
2017-03-16 15:13:49,647 [net.sf.ehcache.CacheManager@2f1a74d1] INFO  [net.sf.ehcache.util.UpdateChecker] - This is the latest GA release.
2017-03-16 15:14:00,016 [SpringJobSchedulerFactoryBean_Worker-1] ERROR [com.wlyd.fmcgwms.persistence.beans.platform.job.CycleFetchInterfaceInfoTaskJob] - >>>WMS-WAAS接口定时任务<<启动失败---fetch----:请设置WAAS_API_ISENABLE=true
2017-03-16 15:14:00,016 [SpringJobSchedulerFactoryBean_Worker-2] ERROR [com.wlyd.fmcgwms.persistence.beans.platform.job.CycleSendInterfaceInfoTaskJob] - >>>WMS-WAAS定时任务<<启动失败----send---:请设置WAAS_API_ISENABLE=true
>>>>>30s后缓存：null
```





