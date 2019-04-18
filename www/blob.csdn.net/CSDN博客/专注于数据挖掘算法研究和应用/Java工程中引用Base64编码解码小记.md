# Java工程中引用Base64编码解码小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月26日 15:24:32[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8713








Base64是网络上最常见的用于传输8Bit字节代码的编码方式之一。


Base64编码说明
　　Base64编码要求把3个8位字节（3*8=24）转化为4个6位的字节（4*6=24），之后在6位的前面补两个0，形成8位一个字节的形式。 如果剩下的字符不足3个字节，则用0填充，输出字符使用'='，因此编码后输出的文本末尾可能会出现1或2个'='。


1）Base64编码解码包下载：http://commons.apache.org/proper/commons-codec/

      下载commons-codec-1.9.jar里面有base64类org.apache.commons.codec.binary.Base64；基础代码如下：




```java
/** 
     * 解密 
     */  
    public static String decodeStr(String pwd)  
    {  
        Base64 base64 = new Base64();  
        byte[] debytes = base64.decodeBase64(new String(pwd).getBytes());  
        return new String(debytes);  
    }  
  
    /** 
     * 加密 
     */  
    public static String encodeStr(String pwd)  
    {  
        Base64 base64 = new Base64();  
        byte[] enbytes = base64.encodeBase64Chunked(pwd.getBytes());  
        return new String(enbytes);  
    }
```








2）如果用JRE默认的sun.misc.BASE64Decoder和sun.misc.BASE64Encoder，没有识别出包，则先移除工程的JRE包再重新加上，然后clean工程即可。 基础代码如下：



```java
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public static void main(String[] args) throws Exception {
        String str = "java12345";
        String ret = null;
        ret = new BASE64Encoder().encode(str.getBytes());
        System.out.println("加密前:"+str+" 加密后:"+ret);
        str = "amF2YTEyMzQ1";
        try {
            ret = new String(new BASE64Decoder().decodeBuffer(str));
        } catch (IOException e) {e.printStackTrace();}
    	System.out.println("解密前:"+str+" 解密后:"+ret);
}
```




   3）用it.sauronsoftware.base64包，可到http://www.sauronsoftware.it/projects/javabase64/manual.php下载，基础代码如下：



```java
byte[] source = ...; // load your data here
byte[] encoded = Base64.encode(source);
byte[] decoded = Base64.decode(encoded);
```










