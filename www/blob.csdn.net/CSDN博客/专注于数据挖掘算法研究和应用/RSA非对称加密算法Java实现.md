# RSA非对称加密算法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月21日 09:26:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1972
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









RSA适用场景：

1）乙方生成两把密钥（公钥和私钥）。公钥是公开的，任何人都可以获得，私钥则是保密的。

2）甲方获取乙方的公钥，然后用它对信息加密。

3）乙方得到加密后的信息，用私钥解密。





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
import javax.crypto.Cipher;


public class RSAAsymmetricEncryption {
	/**
	 * 公钥加密，私钥解密(非对称加密)
	 * 
	 */
	public static void main(String[] args) throws Exception {
		//公钥加密
		String express="abc";
		publicEnrypy(express);      
        //私钥解密
        privateEncode();
    }

    /**
     * 加密的方法,使用公钥进行加密
     * @throws Exception
     */
    public static void publicEnrypy(String express) throws Exception {

        Cipher cipher = Cipher.getInstance("RSA");
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");       
        KeyPair keyPair = keyPairGenerator.generateKeyPair();// 生成钥匙对       
        Key publicKey = keyPair.getPublic();// 得到公钥      
        Key privateKey = keyPair.getPrivate();// 得到私钥    
        saveKey(privateKey);//把私钥保存到硬盘上 
        cipher.init(Cipher.ENCRYPT_MODE, publicKey);// 设置为加密模式      
        byte[] result = cipher.doFinal(express.getBytes());// 对数据进行加密  
        saveData(result);//把加密后的数据保存到硬盘上      
        System.out.println(result.toString());//显示密文
    }

    /**
     * 解密的方法，使用私钥进行解密
     * @throws Exception
     */
    public static void privateEncode() throws Exception {
        Cipher cipher = Cipher.getInstance("RSA");     
        Key privateKey = loadKey();// 从硬盘中读取私钥    
        cipher.init(Cipher.DECRYPT_MODE, privateKey);//设置为解密模式，用私钥解密    
        byte[] data = loadData();// 从硬盘中读取加密后的数据        
        byte[] result = cipher.doFinal(data);//对加密后的数据进行解密，返回解密后的结果
        System.out.println( new String(result) );//显示明文
    }

    /**
     * 从硬盘中加载加密后的文件
     * @return
     * @throws FileNotFoundException
     * @throws IOException
     */
    private static byte[] loadData() throws FileNotFoundException, IOException {
        FileInputStream fileInputStream = new FileInputStream(new File("D:\\tmp\\rsa.dat"));
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int len = 0;
        while ((len = fileInputStream.read(buffer)) != -1) {
            outputStream.write(buffer, 0, len);
        }
        fileInputStream.close();
        return outputStream.toByteArray();
    }
    
    /**
     * 把加密后的密文保存到硬盘上
     * @param result
     * @throws FileNotFoundException
     * @throws IOException
     */
    private static void saveData(byte[] result) throws FileNotFoundException, IOException {
        FileOutputStream fileOutputStream = new FileOutputStream(new File("D:\\tmp\\rsa.dat"));
        fileOutputStream.write(result);
    }

    /**
     * 从硬盘中加载私钥
     * @return
     * @throws IOException
     * @throws FileNotFoundException
     * @throws ClassNotFoundException
     */
    private static Key loadKey() throws IOException, FileNotFoundException, ClassNotFoundException {
        ObjectInputStream inputStream = new ObjectInputStream( new FileInputStream(new File("D:\\tmp\\rsa_prikey.dat")));
        Key privateKey = (Key) inputStream.readObject();
        return privateKey;
    }

    
    /**
     * 把私钥保存到硬盘上
     * @param privateKey
     * @throws IOException
     * @throws FileNotFoundException
     */
    private static void saveKey(Key privateKey) throws IOException, FileNotFoundException {
        ObjectOutputStream outputStream = new ObjectOutputStream( new FileOutputStream(new File("D:\\tmp\\rsa_prikey.dat")));
        outputStream.writeObject(privateKey);
    }
}
```











