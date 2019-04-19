# 非接触CPU卡外部认证步骤 - xqhrs232的专栏 - CSDN博客
2017年06月17日 22:41:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：936
原文地址::[http://blog.csdn.net/xinxinsky/article/details/52288859](http://blog.csdn.net/xinxinsky/article/details/52288859)
相关文章
1、CPU卡认证与操作----[https://wenku.baidu.com/view/67777a5b011ca300a6c3907d.html](https://wenku.baidu.com/view/67777a5b011ca300a6c3907d.html)
最近做一款CPU卡发卡[测试](http://lib.csdn.net/base/softwaretest)工具，由于好长时间没动CPU卡，再拿起来后发现都忘掉了，
外部认证这块还是记一下吧。
现在用的是复旦微的FM1216卡，之前用的是握奇的卡，先说下握奇的外部认证步骤
    1、选择IC卡MF目录 00A4 0000 02 3F00 00
    2、给IC卡发送命令取随机数  0084000008 获取8位随机数1
    3、选择PSAM卡MF目录 00A4000002 3F00 00
    4、选择PSAM卡应用目录 00A4000002 1001 00
    5、PSAM卡初始化DES加密 801A 27 01 08 8位IC卡序列号
    6、PSAM卡DES加密 80FA 00 00 08 8位随机数1
    7、IC卡外部认证 0082 0001 08 8位随机数1
这是之前刷卡设备的认证步骤，卡的结构已经建立好了，现在用的是FM1216空卡，
而且我在数据手册上也未找到SAM卡的DES加密指令，这就尴尬了，怎么进行外部认证？
幸好是用的C#做PC端开发，有DES加密接口，空卡默认密钥是8个0xFF,那么直接用程序计算吧。。。
结果竟然认证通过了。。。
下面说下步骤
1、选择IC卡MF目录
2、给IC卡发送取8字节随机数命令
3、使用8个0xFF做密钥，8字节随机数做加密源用C#的系统API进行DES加密
4、将加密后的8字节随机数使用外部认证命令发送到IC卡，验证通过。
这里要说下C#里面DES加密是不允许弱密钥的，8个字节的0xFF会被视为弱密钥报警编译不通过，通过上网查找可通过反射进行强行加密解密
代码：
**[csharp]**[view
 plain](http://blog.csdn.net/xinxinsky/article/details/52288859#)[copy](http://blog.csdn.net/xinxinsky/article/details/52288859#)
- /// <summary>
- /// DES加密字节数组,可以使用弱密钥
- /// </summary>
- /// <param name="source"></param>
- /// <param name="key"></param>
- /// <returns></returns>
- publicstaticbyte[] Encrypt(byte[] source, byte[] key)  
- {  
-     DESCryptoServiceProvider des = new DESCryptoServiceProvider();  
-     des.Padding = PaddingMode.None;  
- 
-     Type type = Type.GetType("System.Security.Cryptography.CryptoAPITransformMode");  
- object obj = type.GetField("Encrypt", BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.DeclaredOnly).GetValue(type);  
- 
-     MethodInfo mi = des.GetType().GetMethod("_NewEncryptor", BindingFlags.Instance | BindingFlags.NonPublic);  
-     ICryptoTransform desCrypt = (ICryptoTransform)mi.Invoke(des, newobject[] { key, CipherMode.ECB, null, 0, obj });  
- 
- return desCrypt.TransformFinalBlock(source, 0, source.Length);  
- }  
- 
- /// <summary>
- /// DES字节数组解密，可以使用弱密钥
- /// </summary>
- /// <param name="source"></param>
- /// <param name="key"></param>
- /// <returns></returns>
- publicstaticbyte[] Decrypt(byte[] source, byte[] key)  
- {  
-     DESCryptoServiceProvider des = new DESCryptoServiceProvider();  
-     des.Padding = PaddingMode.None;  
- 
-     Type type = Type.GetType("System.Security.Cryptography.CryptoAPITransformMode");  
- object obj = type.GetField("Decrypt", BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.DeclaredOnly).GetValue(type);  
- 
-     MethodInfo mi = des.GetType().GetMethod("_NewEncryptor", BindingFlags.Instance | BindingFlags.NonPublic);  
-     ICryptoTransform desCrypt = (ICryptoTransform)mi.Invoke(des, newobject[] { key, CipherMode.ECB, null, 0, obj });  
- 
- return desCrypt.TransformFinalBlock(source, 0, source.Length);  
- }  
