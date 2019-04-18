# java使用smb 远程访问共享文件夹 密码有特殊字符 解决情况 - z69183787的专栏 - CSDN博客
2013年12月18日 17:33:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7474
```java
private static String domainip = "10.1.44.193";
	private static String username = "administrator";
	private static String password = "1q2w3e4r5t!@#";
	private static String remoteurl = "smb://10.1.44.193/data";
```
```java
NtlmPasswordAuthentication auth = new NtlmPasswordAuthentication(domainip, username, password);  //先登录验证
		SmbFile fp = new SmbFile(remoteurl+"//"+dir,auth);
```
访问方式如上即可。
正常访问示例：
[点击打开链接](http://blog.csdn.net/z69183787/article/details/14161163)
