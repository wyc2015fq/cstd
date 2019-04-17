# RSA Data must start with zero问题解决方法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月17日 10:10:55[boonya](https://me.csdn.net/boonya)阅读数：6853








RSA工具使用公钥加密之后生成byte[] ，直接解密是没有问题的，但将byte[]通过Base64.encode(byte[])之后得到的字符串重新getBytes()去解密是会出现RSA Data must start with zero的问题的。

解决方法：



```java
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
	 * 私钥解密
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
```
测试方法：



```java
@Test
	public void testRSA() throws Exception{
		
		String publicKey=EhcacheUtil.get(StaticProperty.WMS_OPEN_API_RSA_PUBLIC_KEY).toString();
		
		String privateKey=EhcacheUtil.get(StaticProperty.WMS_OPEN_API_RSA_PRIVATE_KEY).toString();
		
		String platformCode="1234",tenantCode="10000";
	    
		byte [] bytes=SAASTokenManager.generateBytesToken(publicKey, platformCode, tenantCode);
		
		System.out.println("RSA bites 加密:"+new String(bytes)); 
		
		System.out.println("RSA bites 解密:"+new String(RSAUtils.decryptByPrivateKey(bytes, privateKey)));
		
		// 将bytes转换为String
		String bytesStr = RSAUtils.bytesToString(bytes);
		
		String[] strArr = bytesStr.split(" ");
        
        int len = strArr.length;
        
        // 转回bytes
        byte[] clone = new byte[len];
         
        for (int i = 0; i < len; i++) {
             clone[i] = Byte.parseByte(strArr[i]);
        }
		
		System.out.println("RSA bites 解密:"+new String(RSAUtils.decryptByPrivateKey(clone, privateKey)));
		
	}
```





