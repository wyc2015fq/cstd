# 消息摘要算法-HMAC算法 - z69183787的专栏 - CSDN博客
2017年10月30日 15:29:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2445
[http://blog.csdn.net/feiyangxiaomi/article/details/34445005](http://blog.csdn.net/feiyangxiaomi/article/details/34445005)
一、简述
  mac（Message Authentication Code，消息认证码算法）是含有密钥散列函数算法，兼容了MD和SHA算法的特性，并在此基础上加上了密钥。因此MAC算法也经常被称作HMAC算法。关于hmac算法的详情可以参看RFC 2104(http://www.ietf.org/rfc/rfc2104.txt)，这里包含了HmacMD5算法的C语言实现。
  这里需要说明的是经过mac算法得到的摘要值也可以使用十六进制编码表示，其摘要值得长度与实现算法的摘要值长度相同。例如 HmacSHA算法得到的摘要长度就是SHA1算法得到的摘要长度，都是160位二进制数，换算成十六进制的编码为40位。
二、模型分析
甲乙双方进行数据交换可以采取如下流程完成
1、甲方向乙方公布摘要算法（就是指定要使用的摘要算法名）
2、甲乙双方按照约定构造密钥，双方拥有相同的密钥（一般是一方构造密钥后通知另外一方，此过程不需要通过程序实现，就是双方约定个字符串，但是这个字符串可不是随便设定的，也是通过相关算法获取的）
3、甲方使用密钥对消息做摘要处理，然后将消息和生成的摘要消息一同发送给乙方
4、乙方收到消息后，使用甲方已经公布的摘要算法+约定好的密钥 对收到的消息进行摘要处理。然后比对自己的摘要消息和甲方发过来的摘要消息。甄别消息是否是甲方发送过来的
三、MAC系列算法支持表
|算法|摘要长度（位）|备注|
|----|----|----|
|HmacMD5|128|BouncyCastle实现|
|HmacSHA1|160（20个字节）|BouncyCastle实现|
|HmacSHA256|256|BouncyCastle实现|
|HmacSHA384|384|BouncyCastle实现|
|HmacSHA512|512|JAVA6实现|
|HmacMD2|128|BouncyCastle实现|
|HmacMD4|128|BouncyCastle实现|
|HmacSHA224|224|BouncyCastle实现|
```java
package com.main;
import java.security.Security;
import javax.crypto.KeyGenerator;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.util.encoders.Hex;
/**
 * MAC消息摘要组件
 * @author kongqz
 * */
public class HAMC {
///////////////////////////HmacMD5///////////////////////////////
	/**
	 * 初始化HmacMD5的密钥
	 * @return byte[] 密钥
	 * 
	 * */
	public static byte[] initHmacMD5Key() throws Exception{
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacMD5");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacMD5消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacMD5(byte[] data,byte[] key) throws Exception{
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacMD5");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	/**
	 * HmacMD2Hex消息摘要
	 * @param data 待做消息摘要处理的数据
	 * @param String 密钥
	 * @return byte[] 消息摘要
	 * */
	public static String encodeHmacMD5Hex(byte[] data,byte[] key) throws Exception{
		//执行消息摘要处理
		byte[] b=encodeHmacMD5(data,key);
		//做十六进制转换
		return new String(Hex.encode(b));
	}
	
///////////////////////////////HmacSHA1//////////////////////////////////
	/**
	 * 初始化HmacSHA1的密钥
	 * @return byte[] 密钥
	 * 
	 * */
	public static byte[] initHmacSHAKey() throws Exception{
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacSHA1");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacSHA1消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacSHA(byte[] data,byte[] key) throws Exception{
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacSHA1");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	
///////////////////////////////HmacSHA256//////////////////////////////////
	/**
	 * 初始化HmacSHA256的密钥
	 * @return byte[] 密钥
	 * 
	 * */
	public static byte[] initHmacSHA256Key() throws Exception{
		Security.addProvider(new BouncyCastleProvider());
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacSHA256");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacSHA256消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacSHA256(byte[] data,byte[] key) throws Exception{
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacSHA256");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	
///////////////////////////////HmacSHA384//////////////////////////////////
	/**
	 * 初始化HmacSHA384的密钥
	 * @return byte[] 密钥
	 * 
	 * */
	public static byte[] initHmacSHA384Key() throws Exception{
		Security.addProvider(new BouncyCastleProvider());
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacSHA384");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacSHA384消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacSHA384(byte[] data,byte[] key) throws Exception{
		Security.addProvider(new BouncyCastleProvider());
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacSHA384");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	
///////////////////////////////HmacSHA512//////////////////////////////////
	/**
	 * 初始化HmacSHA512的密钥
	 * @return byte[] 密钥
	 * 
	 * */
	public static byte[] initHmacSHA512Key() throws Exception{
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacSHA512");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacSHA512消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacSHA512(byte[] data,byte[] key) throws Exception{
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacSHA512");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
///////////////////////////////HmacMD2-BouncyCastle才支持的实现//////////////////////////////////
	/**
	 * 初始化HmacMD2的密钥
	 * @return byte[] 密钥
	 * */
	public static byte[] initHmacMD2Key() throws Exception{
		
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacMD2");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacMD2消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacMD2(byte[] data,byte[] key) throws Exception{
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacMD2");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	/**
	 * HmacMD2Hex消息摘要
	 * @param data 待做消息摘要处理的数据
	 * @param String 密钥
	 * @return byte[] 消息摘要
	 * */
	public static String encodeHmacMD2Hex(byte[] data,byte[] key) throws Exception{
		//执行消息摘要处理
		byte[] b=encodeHmacMD2(data,key);
		//做十六进制转换
		return new String(Hex.encode(b));
	}
	
///////////////////////////////HmacMD4-BouncyCastle才支持的实现//////////////////////////////////
	/**
	 * 初始化HmacMD2的密钥
	 * @return byte[] 密钥
	 * */
	public static byte[] initHmacMD4Key() throws Exception{
		
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacMD4");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacMD4消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacMD4(byte[] data,byte[] key) throws Exception{
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacMD4");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	/**
	 * HmacMD4Hex消息摘要
	 * @param data 待做消息摘要处理的数据
	 * @param String 密钥
	 * @return byte[] 消息摘要
	 * */
	public static String encodeHmacMD4Hex(byte[] data,byte[] key) throws Exception{
		//执行消息摘要处理
		byte[] b=encodeHmacMD4(data,key);
		//做十六进制转换
		return new String(Hex.encode(b));
	}
///////////////////////////////HmacSHA224-BouncyCastle才支持的实现//////////////////////////////////
	/**
	 * 初始化HmacSHA224的密钥
	 * @return byte[] 密钥
	 * */
	public static byte[] initHmacSHA224Key() throws Exception{
		
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//初始化KeyGenerator
		KeyGenerator keyGenerator=KeyGenerator.getInstance("HmacSHA224");
		//产生密钥
		SecretKey secretKey=keyGenerator.generateKey();
		//获取密钥
		return secretKey.getEncoded();
	}
	/**
	 * HmacSHA224消息摘要
	 * @param data 待做摘要处理的数据
	 * @param key 密钥
	 * @return  byte[] 消息摘要
	 * */
	public static byte[] encodeHmacSHA224(byte[] data,byte[] key) throws Exception{
		//加入BouncyCastleProvider的支持
		Security.addProvider(new BouncyCastleProvider());
		//还原密钥，因为密钥是以byte形式为消息传递算法所拥有
		SecretKey secretKey=new SecretKeySpec(key,"HmacSHA224");
		//实例化Mac
		Mac mac=Mac.getInstance(secretKey.getAlgorithm());
		//初始化Mac
		mac.init(secretKey);
		//执行消息摘要处理
		return mac.doFinal(data);
	}
	/**
	 * HmacSHA224Hex消息摘要
	 * @param data 待做消息摘要处理的数据
	 * @param String 密钥
	 * @return byte[] 消息摘要
	 * */
	public static String encodeHmacSHA224Hex(byte[] data,byte[] key) throws Exception{
		//执行消息摘要处理
		byte[] b=encodeHmacSHA224(data,key);
		//做十六进制转换
		return new String(Hex.encode(b));
	}
	/**
	 * 进行相关的摘要算法的处理展示
	 * @throws Exception 
	 * **/
	public static void main(String[] args) throws Exception {
		String str="123000000000090989999999999999999999999999999999999999999999999999999999999999999999111" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"23000000000090989999999999999999999999999999999999999999999999999999999999999999999" +
				"230000000000909899999999999999999999999999999999999999999999999999999999999999999991";
		//初始化密钥
		byte[] key1=HAMC.initHmacMD5Key();
		//获取摘要信息
		byte[] data1=HAMC.encodeHmacMD5(str.getBytes(), key1);
		String datahex1=new String(Hex.encode(data1));
		
		System.out.println("原文："+str);
		System.out.println();
		
		System.out.println("Bouncycastle HmacMD5的密钥内容和长度:"+key1.toString()+"--"+key1.length);
		System.out.println("Bouncycastle HmacMD5算法摘要："+data1.toString());
		System.out.println("Bouncycastle HmacMD5算法摘要HEX："+datahex1.toString());
		System.out.println();
		
		//初始化密钥
		byte[] key2=HAMC.initHmacSHA256Key();
		//获取摘要信息
		byte[] data2=HAMC.encodeHmacSHA256(str.getBytes(), key2);
		String datahex2=new String(Hex.encode(data2));
		System.out.println("Bouncycastle HmacSHA256的密钥:"+key2.toString());
		System.out.println("Bouncycastle HmacSHA256算法摘要："+data2.toString());
		System.out.println("Bouncycastle HmacSHA256算法摘要HEX："+datahex2);
		System.out.println();
		
		
		//初始化密钥
		byte[] key3=HAMC.initHmacSHAKey();
		//获取摘要信息
		byte[] data3=HAMC.encodeHmacSHA(str.getBytes(), key3);
		String datahex3=new String(Hex.encode(data3));
		System.out.println("Bouncycastle HmacSHA1的密钥:"+key3.toString());
		System.out.println("Bouncycastle HmacSHA1算法摘要："+data3.toString());
		System.out.println("Bouncycastle HmacSHA1算法摘要HEX："+datahex3);
		System.out.println();
		
		
		//初始化密钥
		byte[] key4=HAMC.initHmacSHA384Key();
		
		//获取摘要信息
		byte[] data4=HAMC.encodeHmacSHA384(str.getBytes(), key4);
		String datahex4=new String(Hex.encode(data4));
		System.out.println("Bouncycastle HmacSHA384的密钥:"+key4.toString());
		System.out.println("Bouncycastle HmacSHA384算法摘要："+data4.toString());
		System.out.println("Bouncycastle HmacSHA384算法摘要HEX："+datahex4);
		System.out.println();
		
		
		//初始化密钥
		byte[] key5=HAMC.initHmacSHA512Key();
		//获取摘要信息
		byte[] data5=HAMC.encodeHmacSHA512(str.getBytes(), key5);
		System.out.println("HmacSHA512的密钥:"+key5.toString());
		System.out.println("HmacSHA512算法摘要："+data5.toString());
		System.out.println();
		
		System.out.println("================以下的算法支持是bouncycastle支持的算法，sun java6不支持=======================");
		//初始化密钥
		byte[] key6=HAMC.initHmacMD2Key();
		//获取摘要信息
		byte[] data6=HAMC.encodeHmacMD2(str.getBytes(), key6);
		String datahex6=HAMC.encodeHmacMD2Hex(str.getBytes(), key6);
		System.out.println("Bouncycastle HmacMD2的密钥:"+key6.toString());
		System.out.println("Bouncycastle HmacMD2算法摘要："+data6.toString());
		System.out.println("Bouncycastle HmacMD2Hex算法摘要："+datahex6.toString());
		System.out.println();
		
		//初始化密钥
		byte[] key7=HAMC.initHmacMD4Key();
		//获取摘要信息
		byte[] data7=HAMC.encodeHmacMD4(str.getBytes(), key7);
		String datahex7=HAMC.encodeHmacMD4Hex(str.getBytes(), key7);
		System.out.println("Bouncycastle HmacMD4的密钥:"+key7.toString());
		System.out.println("Bouncycastle HmacMD4算法摘要："+data7.toString());
		System.out.println("Bouncycastle HmacMD4Hex算法摘要："+datahex7.toString());
		System.out.println();
		
		//初始化密钥
		byte[] key8=HAMC.initHmacSHA224Key();
		//获取摘要信息
		byte[] data8=HAMC.encodeHmacSHA224(str.getBytes(), key8);
		String datahex8=HAMC.encodeHmacSHA224Hex(str.getBytes(), key8);
		System.out.println("Bouncycastle HmacSHA224的密钥:"+key8.toString());
		System.out.println("Bouncycastle HmacSHA224算法摘要："+data8.toString());
		System.out.println("Bouncycastle HmacSHA224算法摘要："+datahex8.toString());
		System.out.println();
	}
}
```
运行结果是：
```java
原文：123000000000090989999999999999999999999999999999999999999999999999999999999999999999111230000000000909899999999999999999999999999999999999999999999999999999999999999999992300000000009098999999999999999999999999999999999999999999999999999999999999999999923000000000090989999999999999999999999999999999999999999999999999999999999999999999230000000000909899999999999999999999999999999999999999999999999999999999999999999992300000000009098999999999999999999999999999999999999999999999999999999999999999999923000000000090989999999999999999999999999999999999999999999999999999999999999999999230000000000909899999999999999999999999999999999999999999999999999999999999999999992300000000009098999999999999999999999999999999999999999999999999999999999999999999923000000000090989999999999999999999999999999999999999999999999999999999999999999999230000000000909899999999999999999999999999999999999999999999999999999999999999999992300000000009098999999999999999999999999999999999999999999999999999999999999999999923000000000090989999999999999999999999999999999999999999999999999999999999999999999230000000000909899999999999999999999999999999999999999999999999999999999999999999992300000000009098999999999999999999999999999999999999999999999999999999999999999999923000000000090989999999999999999999999999999999999999999999999999999999999999999999230000000000909899999999999999999999999999999999999999999999999999999999999999999991
Bouncycastle HmacMD5的密钥内容和长度:[B@2fb212f0--64
Bouncycastle HmacMD5算法摘要：[B@53f96306
Bouncycastle HmacMD5算法摘要HEX：771093e83aeb659d125100fa465bd34e
Bouncycastle HmacSHA256的密钥:[B@450b9338
Bouncycastle HmacSHA256算法摘要：[B@7aee64f1
Bouncycastle HmacSHA256算法摘要HEX：2d211a42cb3fb68cd36255f9af4ef614eea6c8b93634183d333bf3e6e0b79d87
Bouncycastle HmacSHA1的密钥:[B@592bf469
Bouncycastle HmacSHA1算法摘要：[B@54bb473b
Bouncycastle HmacSHA1算法摘要HEX：c07963727b485cf6579d103a822382f3159bc6ee
Bouncycastle HmacSHA384的密钥:[B@1527171d
Bouncycastle HmacSHA384算法摘要：[B@395e7bc4
Bouncycastle HmacSHA384算法摘要HEX：cae75e5a2ce20d18bf0862215896f8dbe060ac10806925fa5d2bb15cc983543a1d0e53743f1e84ee53ad7db128547cde
HmacSHA512的密钥:[B@5689a400
HmacSHA512算法摘要：[B@666e2862
================以下的算法支持是bouncycastle支持的算法，sun java6不支持=======================
Bouncycastle HmacMD2的密钥:[B@fca1ca
Bouncycastle HmacMD2算法摘要：[B@49e1d547
Bouncycastle HmacMD2Hex算法摘要：2fad1ddef0a1d4d9e7bed45c4b714ae5
Bouncycastle HmacMD4的密钥:[B@2ebaa4a4
Bouncycastle HmacMD4算法摘要：[B@5f8f22f3
Bouncycastle HmacMD4Hex算法摘要：3f41ccbf594e0170f7073e8c756aa039
Bouncycastle HmacSHA224的密钥:[B@100a384
Bouncycastle HmacSHA224算法摘要：[B@50ef2f9f
Bouncycastle HmacSHA224算法摘要：f7d0e74f5c965f7981155206c05deccf0867e9cf348a1e9569b07353
```
运行结论是：
HMAC的特点，任意的消息大小；固定的输出长度；
```java
HmacMD5算法摘要HEX：771093e83aeb659d125100fa465bd34e//32*4=128位
```
```java
Bouncycastle HmacSHA1算法摘要HEX：c07963727b485cf6579d103a822382f3159bc6ee//40*4=160位
```
```java
Bouncycastle HmacSHA256算法摘要HEX：2d211a42cb3fb68cd36255f9af4ef614eea6c8b93634183d333bf3e6e0b79d87//64*4=256
```
还有个问题就是，我们在使用HMAC的时候使用了hash函数并且加入了密钥key，那么这个key是怎么加入进去的？
A=[msg || key]还是B=[key || msg]，前缀和后缀的做法，前缀不可缺，有前缀的攻击，参考《深入浅出密码学——常用加密技术原理与应用》马小婷的，后缀也有一种构建冲突攻击参考《深入浅出密码学——常用加密技术原理与应用》马小婷。
HMAC方案是由一个内部哈希和一个外部哈希组成，他是Mihir Bellare、Ran Canetti和Hugo Krawczyk于1996年提出的。最大的一个有点就是存在安全证明。
![](https://img-blog.csdn.net/20140626104730828)
------------------------------------------结束线----------------------------------------------------------------------------
