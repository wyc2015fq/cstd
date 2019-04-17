# 跨系统如何保持Session存活和Token共享问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年07月28日 18:01:11[boonya](https://me.csdn.net/boonya)阅读数：8751








#### WEB端

##### 问题描述

         WMS系统对监管仓进行访问（监管仓内嵌于WMS系统），但是需要登录监管仓系统才能看到引入WMS系统的界面，否则看不了监管仓。这里涉及到一个监管仓访问超时的问题：如果用户一直在WMS上操作，而对监管仓不闻不问，那么一般在30minutes 之后，再次点击监管仓页面就会发现打不开了；同理，如果用户一直停在监管仓操作，那么30minutes之后，回来再次访问WMS的页面的时候，WMS也会自动退出到登录界面。以上问题说明，两个系统使用的不是同一个session时，在同一个系统中操作时间越长，另一个系统session存活的几率就越小，不被用户操作的系统也最容易导致session死亡，大多数时候只能半开半闭式地访问，而不能灵活地游弋于多个系统。要解决session共享的问题才能实现多系统紧密协作。

##### 处理方案

       可以将session用一个公用的数据存储区块存储起来，如：memcache。利用memcache的数据超时来处理session。同时还可以实现单点登录，如果用户通过验证，但是memcache已经存在此session那么就不再允许用户登录系统。


#### 移动端

##### 问题描述

         历史惊人的相似，在Android和IOS上分别访问WMS和监管仓的时候发现不能共享Token。因为此Token是WMS和监管仓都分别作了生成的，登录时也仅仅是对是否超时作了验证，所以在登录的过程中他们是没有问题的，但是访问其他系统页面后切换回来就是在用其他类系统的Token在访问自己的站点，所以导致Token通不过验证。我们并没有通知对方系统自己的Token已经变了，而是自以为是独立地工作。

##### 处理方案

       方法一：同上；

       方法二：每个系统都提供交换Token的接口，改变了就通知对方系统，这样可以确保用户获取到的token永远都是最新的。

我觉得今天这个标题很有意思，值得大家深入去思考，上面的解决方案是余之鄙见，希冀更多处理方案！

#### Token实现代码

##### TOken实体



```java
package com.wlyd.wms.service.api.data;

import java.io.Serializable;

public class UserToken implements Serializable
{
    private static final long serialVersionUID = -8766321739625153631L;

    private String signature;// 签名
    
    private String timestamp;// 时间戳
    
    private String random;// 随机数
    
    private String authorizationCode;//授权码
    
    public String getSignature()
    {
	return signature;
    }
    
    public void setSignature(String signature)
    {
	this.signature = signature;
    }
    
    public String getTimestamp()
    {
	return timestamp;
    }
    
    public void setTimestamp(String timestamp)
    {
	this.timestamp = timestamp;
    }
    
    public String getRandom()
    {
	return random;
    }
    
    public void setRandom(String random)
    {
	this.random = random;
    }
    
    
    public String getAuthorizationCode()
    {
        return authorizationCode;
    }

    public void setAuthorizationCode(String authorizationCode)
    {
        this.authorizationCode = authorizationCode;
    }

    @Override
    public String toString()
    {
	return "signature=" + signature + "×tamp=" + timestamp + "&random=" + random+ "&authorizationCode=" + authorizationCode;
    }
}
```

##### C#与Java DES加密工具



```java
package com.wlyd.wms.util.api;

import java.security.Key;
import java.security.spec.AlgorithmParameterSpec;
import java.util.Calendar;
import java.util.Date;
import java.util.Random;
import javax.crypto.Cipher;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;
import javax.crypto.spec.IvParameterSpec;
import com.wlyd.wms.service.api.data.UserToken;
import com.wlyd.wms.util.Log;
import Decoder.BASE64Decoder;
import Decoder.BASE64Encoder;

public class JCSharpDESUtil
{
    // DES加密的私钥，必须是8位长的字符串
    private static final byte[] DESkey = "11111111".getBytes();// 设置密钥
    
    private static final byte[] DESIV = "12345678".getBytes();// 设置向量
    
    static AlgorithmParameterSpec iv = null;// 加密算法的参数接口，IvParameterSpec是它的一个实现
    private static Key key = null;
    
    static
    {
	DESKeySpec keySpec;
	try
	{
	    keySpec = new DESKeySpec(DESkey);// 设置密钥参数
	    iv = new IvParameterSpec(DESIV);// 设置向量
	    SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");// 获得密钥工厂
	    key = keyFactory.generateSecret(keySpec);// 得到密钥对象
	} catch (Exception e)
	{
	    Log.getLogger(JCSharpDESUtil.class).error("");
	}
	
    }
    
    public static String encrypt(String data)
    {
	try
	{
	    Cipher enCipher = Cipher.getInstance("DES/CBC/PKCS5Padding");// 得到加密对象Cipher
	    enCipher.init(Cipher.ENCRYPT_MODE, key, iv);// 设置工作模式为加密模式，给出密钥和向量
	    byte[] pasByte = enCipher.doFinal(data.getBytes("utf-8"));
	    BASE64Encoder base64Encoder = new BASE64Encoder();
	    return base64Encoder.encode(pasByte).replace("\n", "").replace("\r", "");// 去掉换行回车符
	} catch (Exception e)
	{
	    Log.getLogger(JCSharpDESUtil.class).error("加密异常：" + e.getMessage());
	}
	return "";
    }
    
    public static String decrypt(String data)
    {
	try
	{
	    Cipher deCipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
	    deCipher.init(Cipher.DECRYPT_MODE, key, iv);
	    BASE64Decoder base64Decoder = new BASE64Decoder();
	    byte[] pasByte = deCipher.doFinal(base64Decoder.decodeBuffer(data));
	    return new String(pasByte, "UTF-8");
	} catch (Exception e)
	{
	    Log.getLogger(JCSharpDESUtil.class).error("解密异常：" + e.getMessage());
	}
	return "";
    }
    
    public static void main(String[] args) throws Exception
    {
	String data = "/YG1K/6/mh86b5EbjYdZwrNm+MGzGw7DXXzj1mBM3ssuw2BS2Xan9Yk1mBijRLoujGtD9Wo692KvVUoIvZZY9NdpJvgv76Rn";
	
	System.out.println("加密：" + JCSharpDESUtil.encrypt("boonya"));
	
	System.out.println("解密：" + JCSharpDESUtil.decrypt(JCSharpDESUtil.encrypt("boonya")));
	
	System.out.println("CShap data Length " + (data.getBytes().length % 8));
	System.out.println("CShap解密：" + JCSharpDESUtil.decrypt(data));
	
	UserToken encryptedToken = new UserToken();
	encryptedToken.setSignature("wmsadmin");
	encryptedToken.setTimestamp(Calendar.getInstance().getTimeInMillis() + "");
	encryptedToken.setRandom(new Random().nextInt(999999999) + "");
	String text = encryptedToken.toString();
	System.out.println("明文：" + text);
	String encryptData = JCSharpDESUtil.encrypt(text);
	System.out.println("加密：" + encryptData);
	
	System.out.println("解密：" + JCSharpDESUtil.decrypt(JCSharpDESUtil.encrypt(text)));
	
	long time = new Date().getTime();
	System.out.println(Calendar.getInstance().getTimeInMillis());
	System.out.println(new Date(Calendar.getInstance().getTimeInMillis()));
	System.out.println(time);
	
    }
    
}
```

##### TOKEN工具类



```java
package com.wlyd.wms.util;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;

import com.wlyd.wms.service.api.data.UserToken;
import com.wlyd.wms.util.api.JCSharpDESUtil;

public class TokenUtil
{
    private static ConcurrentHashMap<String, UserToken> map = new ConcurrentHashMap<String, UserToken>();
    
    /**
     * 生成加密Token
     * 
     * @param username
     * @return
     */
    public static UserToken generateToken(String username)
    {
	UserToken userToken = new UserToken();
	// 设置签名
	userToken.setSignature(username);
	// 设置时间戳
	userToken.setTimestamp(getTimeStamp());
	// 设置随机数
	userToken.setRandom(getRandom());
	// 设置授权码
	userToken.setAuthorizationCode(getAuthorizationCode(username));
	
	return userToken;
    }
    
    /**
     * 判定是否已经登录
     * 
     * @param signature
     * @return
     */
    public static boolean whetherUserHasSignedIn(String signature)
    {
	if(map.containsKey(signature))
	{
	    return true;
	}
	return false;
    }
    
    /**
     * 创建用户Token
     * 
     * @param username
     * @return
     */
    public static String createToken(String username)
    {
	UserToken userToken = generateToken(username);
	// 保存用户Token
	String token=encryptUserToken(userToken);
	
	map.put(encryptSignature(userToken.getSignature()), userToken);
	
	return token;
    }
    
    /**
     * 设置自定义的Token
     * 
     * @param username
     * @param userToken
     */
    public static void setSelfToken(String username,UserToken userToken)
    {
	map.put(username, userToken);
    }
    
    /**
     * 【重新生成】更新Token
     * 
     * @param token
     * @return
     */
    public static String reCreateToken(String token)
    {
	UserToken userToken=TokenUtil.analyseEncryptedUserToken(token);
	return createToken(userToken.getSignature());
    }
    
    /**
     * 加密签名
     * 
     * @param username
     * @return
     */
    public static String encryptSignature(String username)
    {
	return JCSharpDESUtil.encrypt(username);
    }
    
    /**
     * 解密签名
     * 
     * @param signature
     * @return
     */
    public static String decryptSignature(String signature)
    {
	return JCSharpDESUtil.decrypt(signature);
    }
    
    /**
     * 生成时间戳
     * @return
     */
    public static String getTimeStamp()
    {
	return Calendar.getInstance().getTimeInMillis() + "";
    }
    
    /**
     * 生成随机数
     * 
     * @return
     */
    public static String getRandom()
    {
	return new Random().nextInt(999999999) + "";
    }
    
    /**
     * 加密用户Token
     * 
     * @param userToken
     * @return
     */
    public static String encryptUserToken(UserToken userToken)
    {
	return JCSharpDESUtil.encrypt(userToken.toString());
    }
    
    /**
     * 解析加密用户Token
     * 
     * @param token
     * @return
     */
    public static UserToken decryptUserToken(String token)
    {
	String decryptToken=JCSharpDESUtil.decrypt(token);
	String[] params = decryptToken.split("&");
	// 分析用户提交过来的Token
	UserToken userToken = new UserToken();
	for (int i = 0, j = params.length; i < j; i++)
	{
	    String[] currentParams = params[i].split("=");
	    String param = currentParams[0];
	    switch (param)
	    {
		case "signature":
		    userToken.setSignature(currentParams[1]);
		    break;
		case "timestamp":
		    userToken.setTimestamp(currentParams[1]);
		    break;
		case "random":
		    userToken.setRandom(currentParams[1]);
		    break;
		case "authorizationCode":
		    userToken.setAuthorizationCode(currentParams[1]);
		    break;
		default:
		    break;
	    }
	}
	return userToken;
    }
    
    /**
     * 【解密用户Token】分析用户Token
     * 
     * @param token
     * @return
     */
    public static UserToken analyseEncryptedUserToken(String token)
    {
	UserToken userToken=decryptUserToken( token);
	return userToken;
    }
    
    /**
     * 验证用户Token
     * 
     * @param token
     * @param userToken
     * @return
     */
    public static boolean verifyUserToken(String token)
    {
	// 分析用户需要验证的Token
	UserToken userToken=decryptUserToken(token);
	// 给用户加密签名
	String encryptSignature=encryptSignature(userToken.getSignature());
	// 判定是否包含此Token
	if(!map.containsKey(encryptSignature))
	{
	    return false;
	}
	// 获取登录的用户Token
	UserToken userTokenIner=(UserToken) map.get(encryptSignature);
	// 验证是否存在此用户登录的Token
	if (userTokenIner!=null&&(userToken.toString()).equals(userTokenIner.toString()))
	{
	    // 判定时间戳是否过期
	    long currentTime = Calendar.getInstance().getTimeInMillis();
	    long timestamp = Long.valueOf(userToken.getTimestamp());
	    // Token有效时间为30分钟
	    long verifyTime = 30 * 60 * 1000;
	    if (currentTime - timestamp > verifyTime)
	    {
		// 移除过期的Token
		map.remove(encryptSignature);
		return false;
	    }
	    return true;
	}
	 return false;
    }
    
    /**
     * 获取已经签入的用户Token
     * 
     * @return
     */
    public static UserToken getUserSignedToken(String signature)
    {
	if(map.containsKey(signature))
	{
	    return map.get(signature);
	}
	return null;
    }
    
    /**
     * 根据用户名获取MD5Token
     * 
     * @param username
     * @return
     */
    public static String getAuthorizationCode(String username)
    {
	SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
	String date = sdf.format(new Date());
	String token = Md5.getPassSalt("username=" + username + "&date=" + date, Md5.getMD5Str(username));
	return token;
    }
    
    public static void main(String[] args) 
    { 
	String	token=TokenUtil.createToken("boonya");
	System.out.println("token:"+token);
	
	System.out.println("Original token:"+TokenUtil.analyseEncryptedUserToken(token));
	
	boolean result=TokenUtil.verifyUserToken(token);
	System.out.println("verify reulst:"+result);
    }

    
    /**
     * 创建用户Token
     * 
     * @param username
     * @return
     */
    public static String createTokenIOS(String username){
		UserToken userToken = generateToken(username);
		map.put(encryptSignature(userToken.getSignature()), userToken);
		return userToken.toString();
    }
    
    
    public static UserToken decryptUserTokenIOS(String token)
    {
	String[] params = token.split("&");
	// 分析用户提交过来的Token
	UserToken userToken = new UserToken();
	for (int i = 0, j = params.length; i < j; i++)
	{
	    String[] currentParams = params[i].split("=");
	    String param = currentParams[0];
	    switch (param)
	    {
		case "signature":
		    userToken.setSignature(currentParams[1]);
		    break;
		case "timestamp":
		    userToken.setTimestamp(currentParams[1]);
		    break;
		case "random":
		    userToken.setRandom(currentParams[1]);
		    break;
		case "authorizationCode":
		    userToken.setAuthorizationCode(currentParams[1]);
		    break;
		default:
		    break;
	    }
	}
	return userToken;
    }
    
    /**
     * 解析token信息非加密
     * @MethodName: verifyUserTokenIOS 
     * @Description: 
     * @param token
     * @return
     * @throws
     */
    public static boolean verifyUserTokenIOS(String token)
    {
	// 分析用户需要验证的Token
	UserToken userToken=decryptUserTokenIOS(token);
	// 给用户加密签名
	String encryptSignature=encryptSignature(userToken.getSignature());
	// 判定是否包含此Token
	if(!map.containsKey(encryptSignature))
	{
	    return false;
	}
	// 获取登录的用户Token
	UserToken userTokenIner=(UserToken) map.get(encryptSignature);
	// 验证是否存在此用户登录的Token
	if (userTokenIner!=null&&(userToken.toString()).equals(userTokenIner.toString()))
	{
	    // 判定时间戳是否过期
	    long currentTime = Calendar.getInstance().getTimeInMillis();
	    long timestamp = Long.valueOf(userToken.getTimestamp());
	    // Token有效时间为30分钟
	    long verifyTime = 30 * 60 * 1000;
	    if (currentTime - timestamp > verifyTime)
	    {
		// 移除过期的Token
		map.remove(encryptSignature);
		return false;
	    }
	    return true;
	}
	 return false;
    }
    
    
}
```

##### 交换公钥工具类



```java
package com.wlyd.wms.util.api;

import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;

import com.wlyd.wms.service.api.data.UserToken;

public class UserSecurityManager
{
    private static ConcurrentHashMap<String, UserToken> TOKEN = new ConcurrentHashMap<String, UserToken>();
    
    /**
     * <k,v>:<sessionId,clientPublicKey>
     */
    private static ConcurrentHashMap<String, String> CLIENT_PUBLIC = new ConcurrentHashMap<String, String>();
    
    /**
     * <k,v>:<sessionId,serverPublicKey>
     */
    private static ConcurrentHashMap<String, String> SERVER_PUBLIC = new ConcurrentHashMap<String, String>();
    
    /**
     * <k,v>:<serverPublicKey,serverPrivateKey>
     */
    private static ConcurrentHashMap<String, String> SERVER_PUBLIC_PRIVATE = new ConcurrentHashMap<String, String>();
    
    
    public static String getSessionId(String clientPublicKey)
    {
	if(CLIENT_PUBLIC.containsValue(clientPublicKey))
	{
	    for (Entry<String, String > entry: CLIENT_PUBLIC.entrySet())
	    {
		if(entry.getValue().equals(clientPublicKey))
		{
		    return entry.getKey();
		}
	    }
	}
	return null;
    }
    
    public static ConcurrentHashMap<String, UserToken> getTOKEN()
    {
        return TOKEN;
    }

    public static void rememberUserToken(String key,UserToken token)
    {
	TOKEN.put(key, token);
    }
    
    public static void removeUserToken(String key)
    {
	if(TOKEN.containsKey(key))
	{
	    TOKEN.remove(key);
	}
    }
    
    public static void rememberClientPublicKey(String sessionId,String clientPublicKey)
    {
	CLIENT_PUBLIC.put(sessionId, clientPublicKey);
    }
    
    public static String getClientPublicKey(String sessionId)
    {
	if(CLIENT_PUBLIC.containsKey(sessionId))
	{
	   return CLIENT_PUBLIC.get(sessionId);
	}
	return null;
    }
    
    public static void removeClientPublicKey(String sessionId)
    {
	if(CLIENT_PUBLIC.containsKey(sessionId))
	{
	    CLIENT_PUBLIC.remove(sessionId);
	}
    }
    
    public static void rememberServerPublicKey(String sessionId,String serverPublicKey)
    {
	SERVER_PUBLIC.put(sessionId, serverPublicKey);
    }
    
    public static String getServerPublicKey(String sessionId)
    {
	if(SERVER_PUBLIC.containsKey(sessionId))
	{
	    return SERVER_PUBLIC.get(sessionId);
	}
	return null;
    }
    
    public static String getServerPrivateKey(String sessionId)
    {
	if(SERVER_PUBLIC.containsKey(sessionId))
	{
	    String serverPublicKey= SERVER_PUBLIC.get(sessionId);
	    if(SERVER_PUBLIC_PRIVATE.containsKey(serverPublicKey))
	    {
		SERVER_PUBLIC_PRIVATE.get(serverPublicKey);
	    }
	}
	return null;
    }
    
    public static void removeServerPublicKey(String sessionId)
    {
	if(SERVER_PUBLIC.containsKey(sessionId))
	{
	    SERVER_PUBLIC.remove(sessionId);
	}
    }
    
    public static void rememberServerPublicAndPrivateKey(String serverPublicKey,String serverPrivateKey)
    {
	SERVER_PUBLIC_PRIVATE.put(serverPublicKey, serverPrivateKey);
    }
    
    public static void removeServerPublicAndPrivateKey(String serverPublicKey)
    {
	if(serverPublicKey==null||serverPublicKey.equals("")) return;
	if(SERVER_PUBLIC_PRIVATE.containsKey(serverPublicKey))
	{
	    SERVER_PUBLIC_PRIVATE.remove(serverPublicKey);
	}
    }
    
    
}
```










