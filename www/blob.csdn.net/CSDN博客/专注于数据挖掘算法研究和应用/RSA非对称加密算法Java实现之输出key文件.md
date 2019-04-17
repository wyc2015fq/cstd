# RSA非对称加密算法Java实现之输出key文件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月26日 13:03:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2191








场景：Java实现RSA，将公钥和秘钥分别输出文件，公钥用于加密，私钥用于解密。

重点要关注解密时，不能直接传String，要用byte[]，所以需要加二者转换函数。

参考代码如下：



```java
package sk.ml;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.security.Key;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;


public class RSAAsymmetricEncryption {
	/**
	 * 公钥加密，私钥解密(非对称加密)
	 * 
	 */
	public static void main(String[] args) throws Exception {
		//生成公钥文件和秘钥文件并保存
		generateKey();
		//导入公钥加密
		String express="13366668888";
		byte[] cipherText=publicEnrypy(express);
		String strCiphertext=byteToString(cipherText);//密文byte[]转化成string
		System.out.println(strCiphertext);
		//导入私钥解密
		byte[] getcipherText=stringToByte(strCiphertext);//密文string转化成byte[]
		byte[] retExpress=privateEncode(getcipherText);
		System.out.println(new String(retExpress));
    }
	//生成公钥和秘钥
	public static void generateKey() throws Exception{
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");       
        KeyPair keyPair = keyPairGenerator.generateKeyPair();// 生成钥匙对       
        Key publicKey = keyPair.getPublic();// 得到公钥         
        String puckeyFile="D:\\tmp\\rsa_pubkey.dat";
        saveKey(publicKey,puckeyFile);//保存公钥
        Key privateKey = keyPair.getPrivate();// 得到私钥    
        String prikeyFile="D:\\tmp\\rsa_prikey.dat";
        saveKey(privateKey,prikeyFile);//把私钥保存到硬盘上 
	}
    //保存公钥和秘钥
	private static void saveKey(Key rsaKey,String filename) throws Exception{
        ObjectOutputStream outputStream = new ObjectOutputStream( new FileOutputStream(new File(filename)));
        outputStream.writeObject(rsaKey);
    }
	//导出公钥和秘钥
	private static Key loadKey(String filename) throws Exception {
        ObjectInputStream inputStream = new ObjectInputStream( new FileInputStream(new File(filename)));
        Key rsaKey = (Key) inputStream.readObject();
        return rsaKey;
    }
	
    /**
     * 加密的方法,使用公钥进行加密
     * @throws Exception
     */
    public static byte[] publicEnrypy(String express) throws Exception {
        Cipher cipher = Cipher.getInstance("RSA");
        String puckeyFile="D:\\tmp\\rsa_pubkey.dat";
        Key pubkey=loadKey(puckeyFile);
        cipher.init(Cipher.ENCRYPT_MODE, pubkey);// 设置为加密模式      
        byte[] result = cipher.doFinal(express.getBytes());// 对数据进行加密  
        return result;//返回密文
    }

    /**
     * 解密的方法，使用私钥进行解密
     * @throws Exception
     */
    public static byte[] privateEncode(byte[] cipherText) throws Exception {
        Cipher cipher = Cipher.getInstance("RSA"); 
        String prikeyFile="D:\\tmp\\rsa_prikey.dat";
        Key prikey = loadKey(prikeyFile);// 从硬盘中读取私钥    
        cipher.init(Cipher.DECRYPT_MODE, prikey);//设置为解密模式，用私钥解密   
        byte[] result = cipher.doFinal(cipherText);//对加密后的数据进行解密
        return result ;//返回明文 
    }
    
    //  把byte[]元素之间添加空格，并转化成字符串返回，  
    public static String byteToString(byte[] resouce){
    	StringBuffer sb = new StringBuffer(); 
    	for (int i = 0; i < resouce.length; i++) { 
    		if (i == resouce.length-1) { 
    			sb.append(Byte.toString(resouce[i]));
    		}else{  
    			sb.append(Byte.toString(resouce[i]));
    			sb.append(" ");  
    		}  
    	} 
    	return sb.toString();  
    } 
    
    //  把字符串按照空格进行拆分成数组，然后转化成byte[],返回  
    public static byte[] stringToByte(String resouce){  
    	String[] strArr = resouce.split(" ");
    	int len = strArr.length;
    	byte[] clone = new byte[len];
    	for (int i = 0; i < len; i++) { 
    		clone[i] = Byte.parseByte(strArr[i]);
    	}  
    	return clone;        
    }  
}
```








