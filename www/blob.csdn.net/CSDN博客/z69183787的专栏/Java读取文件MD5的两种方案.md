# Java读取文件MD5的两种方案 - z69183787的专栏 - CSDN博客
2018年02月10日 19:57:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：128
个人分类：[文件处理-读写																[工具-加解密](https://blog.csdn.net/z69183787/article/category/5974293)](https://blog.csdn.net/z69183787/article/category/2229757)
[http://blog.csdn.net/wangqiuyun/article/details/22941433](http://blog.csdn.net/wangqiuyun/article/details/22941433)
http://blog.csdn.net/sike_java/article/details/78538762
最近要弄一下Java读取文件MD5，注意这里是小文件，网上找了一下，有两种解决方案，一种是用MessageDigest来实现，另种是用org.apache.commons.codec.digest来实现，代码如下：
[java][view
 plain](http://blog.csdn.net/wangqiuyun/article/details/22941433#)[copy](http://blog.csdn.net/wangqiuyun/article/details/22941433#)
- package cdm;  
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileNotFoundException;  
- import java.io.IOException;  
- import java.math.BigInteger;  
- import java.nio.MappedByteBuffer;  
- import java.nio.channels.FileChannel;  
- import java.security.MessageDigest;  
- 
- import org.apache.commons.codec.digest.*;   
- import org.apache.commons.io.IOUtils;  
- 
- publicclass testMD5 {  
- 
- publicstatic String getMd5ByFile(File file) throws FileNotFoundException {  
-             String value = null;  
-             FileInputStream in = new FileInputStream(file);  
- try {  
-             MappedByteBuffer byteBuffer = in.getChannel().map(FileChannel.MapMode.READ_ONLY, 0, file.length());  
-             MessageDigest md5 = MessageDigest.getInstance("MD5");  
-             md5.update(byteBuffer);  
-             BigInteger bi = new BigInteger(1, md5.digest());  
-             value = bi.toString(16);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         } finally {  
- if(null != in) {  
- try {  
-                     in.close();  
-                 } catch (IOException e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
- return value;  
-         }  
- 
- publicstaticvoid main(String[] args) throws IOException {  
- 
-         String path="E:\\commons-codec-1.9-bin.zip";  
- 
-         String v = getMd5ByFile(new File(path));  
-         System.out.println("MD5:"+v.toUpperCase());  
- 
-         FileInputStream fis= new FileInputStream(path);    
-         String md5 = DigestUtils.md5Hex(IOUtils.toByteArray(fis));    
-         IOUtils.closeQuietly(fis);    
-         System.out.println("MD5:"+md5);   
- 
- //System.out.println("MD5:"+DigestUtils.md5Hex("WANGQIUYUN"));
-     }  
- 
- }  
运行：
![](https://img-blog.csdn.net/20140404160025078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3FpdXl1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结果在官网提供的MD5一致。
附commons-codec-1.9.jar下载地址：[http://commons.apache.org/proper/commons-codec/download_codec.cgi](http://commons.apache.org/proper/commons-codec/download_codec.cgi)
[](http://commons.apache.org/proper/commons-codec/download_codec.cgi)
如果要处理过G的文件，请参照：[http://www.cnblogs.com/yaowukonga/p/3523668.html](http://www.cnblogs.com/yaowukonga/p/3523668.html)
