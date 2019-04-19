# Java对大文件MD5解密详解 - 零度的博客专栏 - CSDN博客
2017年05月23日 16:12:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：4061
        MD5是常用的加密[算法](http://lib.csdn.net/base/datastructure)，也经常用于校验信息完整，如文件的完整性。用术语讲，MD5是一种**消息摘要算法(Message Digest Algorithm)**。另外还有一种常用的消息摘要算法SHA1。如果想了解这些的话，可以去百度百科：MD5、SHA1、消息摘要算法。
[Java](http://lib.csdn.net/base/javase)已经实现了MD5、SHA1算法。利用**java.security.MessageDigest**类就可以获取字符串和文件的MD5以及SHA1结果。
1.字符串的MD5（下面的代码有详细注释）
**[java]**[view
 plain](http://blog.csdn.net/xiao__gui/article/details/8148203#)[copy](http://blog.csdn.net/xiao__gui/article/details/8148203#)
- publicstatic String stringMD5(String input) {  
- 
- try {  
- 
- // 拿到一个MD5转换器（如果想要SHA1参数换成”SHA1”）
- 
-       MessageDigest messageDigest =MessageDigest.getInstance("MD5");  
- 
- 
- // 输入的字符串转换成字节数组
- 
- byte[] inputByteArray = input.getBytes();  
- 
- 
- 
- // inputByteArray是输入字符串转换得到的字节数组
- 
-       messageDigest.update(inputByteArray);  
- 
- 
- 
- // 转换并返回结果，也是字节数组，包含16个元素
- 
- byte[] resultByteArray = messageDigest.digest();  
- 
- 
- 
- // 字符数组转换成字符串返回
- 
- return byteArrayToHex(resultByteArray);  
- 
- 
- 
-    } catch (NoSuchAlgorithmException e) {  
- 
- returnnull;  
- 
-    }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
![](http://static.blog.csdn.net/images/save_snippets.png)
   //下面这个函数用于将字节数组换成成16进制的字符串
**[java]**[view
 plain](http://blog.csdn.net/xiao__gui/article/details/8148203#)[copy](http://blog.csdn.net/xiao__gui/article/details/8148203#)
- publicstatic String byteArrayToHex(byte[] byteArray) {  
- 
- // 首先初始化一个字符数组，用来存放每个16进制字符
- 
- char[] hexDigits = {'0','1','2','3','4','5','6','7','8','9', 'A','B','C','D','E','F' };  
- 
- 
- 
- // new一个字符数组，这个就是用来组成结果字符串的（解释一下：一个byte是八位二进制，也就是2位十六进制字符（2的8次方等于16的2次方））
- 
- char[] resultCharArray =newchar[byteArray.length * 2];  
- 
- 
- 
- // 遍历字节数组，通过位运算（位运算效率高），转换成字符放到字符数组中去
- 
- int index = 0;  
- 
- for (byte b : byteArray) {  
- 
-       resultCharArray[index++] = hexDigits[b>>> 4 & 0xf];  
- 
-       resultCharArray[index++] = hexDigits[b& 0xf];  
- 
-    }  
- 
- 
- 
- // 字符数组组合成字符串返回
- 
- returnnew String(resultCharArray);  
[]()
从上面代码可以看出，使用MessageDigest对字符串进行MD5算法的步骤是，先将字符串转换成字节数组，在进行MD5算法，最后返回的也是一个字节数组，要我们自己转成32位的字符串。
2.文件MD5
对文件进行MD5也可以像字符串MD5一样的，首先要把文件转成字节数组，后面和字符串MD5完全一样。
但是如果是一个特别大的文件，一下子把一个文件的数组全部读到内存中，那么估计内存也吃不消。
对于大文件，可以使用DigestInputStream。
**[java]**[view
 plain](http://blog.csdn.net/xiao__gui/article/details/8148203#)[copy](http://blog.csdn.net/xiao__gui/article/details/8148203#)
- publicstatic String fileMD5(String inputFile) throws IOException {  
- 
- 
- 
- // 缓冲区大小（这个可以抽出一个参数）
- 
- int bufferSize = 256 * 1024;  
- 
-       FileInputStream fileInputStream = null;  
- 
-       DigestInputStream digestInputStream = null;  
- 
- 
- 
- try {  
- 
- // 拿到一个MD5转换器（同样，这里可以换成SHA1）
- 
-          MessageDigest messageDigest =MessageDigest.getInstance("MD5");  
- 
- 
- 
- // 使用DigestInputStream
- 
-          fileInputStream = new FileInputStream(inputFile);  
- 
-          digestInputStream = new DigestInputStream(fileInputStream,messageDigest);  
- 
- 
- 
- // read的过程中进行MD5处理，直到读完文件
- 
- byte[] buffer =newbyte[bufferSize];  
- 
- while (digestInputStream.read(buffer) > 0);  
- 
- 
- 
- // 获取最终的MessageDigest
- 
-          messageDigest= digestInputStream.getMessageDigest();  
- 
- 
- 
- // 拿到结果，也是字节数组，包含16个元素
- 
- byte[] resultByteArray = messageDigest.digest();  
- 
- 
- 
- // 同样，把字节数组转换成字符串
- 
- return byteArrayToHex(resultByteArray);  
- 
- 
- 
-       } catch (NoSuchAlgorithmException e) {  
- 
- returnnull;  
- 
-       } finally {  
- 
- try {  
- 
-             digestInputStream.close();  
- 
-          } catch (Exception e) {  
- 
-          }  
- 
- try {  
- 
-             fileInputStream.close();  
- 
-          } catch (Exception e) {  
- 
-          }  
- 
-       }  
- 
-    }  
![](http://static.blog.csdn.net/images/save_snippets.png)
![](http://static.blog.csdn.net/images/save_snippets.png)
   上面的方法本人亲测过大小约4G的文件，得出的MD5值和网上下载的一个MD5小工具得到的MD5值一样，说明上面的方式没有什么问题。不过取大文件的MD5很慢，4G的文件跑一下要一分钟（I5处理器 6G内存 64位XP系统 本本）。
附1：我在网上还看到一种给文件MD5的方式
**[java]**[view
 plain](http://blog.csdn.net/xiao__gui/article/details/8148203#)[copy](http://blog.csdn.net/xiao__gui/article/details/8148203#)
- publicstatic String getFileMD5String(File file) throws IOException{  
-  FileInputStream in = new FileInputStream(file);  
-  FileChannel ch =in.getChannel();  
-  MappedByteBuffer byteBuffer =ch.map(FileChannel.MapMode.READ_ONLY, 0,file.length());  
-  messagedigest.update(byteBuffer);  
- return byteArrayToHex (messagedigest.digest());  
- }  
[]()
[]()
我也尝试过这样的方式，但是如果文件大于2G，那么这种方式会出现异常。所以不推荐。
附2：[测试](http://lib.csdn.net/base/softwaretest)文件MD5的main方法
**[java]**[view
 plain](http://blog.csdn.net/xiao__gui/article/details/8148203#)[copy](http://blog.csdn.net/xiao__gui/article/details/8148203#)
- publicstaticvoid main(String[] args) {  
- 
- long startTime = System.currentTimeMillis();  
- 
- try {  
-     System.out.println(fileMD5("E:/软件/VS2008ProEdition90DayTrialCHSX1435983.iso"));  
-   } catch (IOException e) {  
-     e.printStackTrace();  
-   }  
- 
- long endTime = System.currentTimeMillis();  
- 
-   System.out.println((endTime - startTime)/1000);  
- }  
[]()
[]()
