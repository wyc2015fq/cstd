# 用微软makecert.exe生成一个自签名的证书----Wince相关 - xqhrs232的专栏 - CSDN博客
2014年10月18日 15:59:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1146
原文地址::[http://blog.csdn.net/popozhu/article/details/5793923](http://blog.csdn.net/popozhu/article/details/5793923)
相关网贴
1、
# [证书创建工具 和 参数详解 (Makecert.exe)](http://blog.csdn.net/ydongsky/article/details/7887652)----[http://blog.csdn.net/ydongsky/article/details/7887652](http://blog.csdn.net/ydongsky/article/details/7887652)
# 2、
[Windows下如何生成数字证书](http://www.cnblogs.com/xiaofoyuan/archive/2012/12/07/2807211.html)----[http://www.cnblogs.com/xiaofoyuan/archive/2012/12/07/2807211.html](http://www.cnblogs.com/xiaofoyuan/archive/2012/12/07/2807211.html)
3、wince6.0 A8平台加密，签名可执行文件后还是无法运行----[http://bbs.csdn.net/topics/390509632?page=1#post-398364152](http://bbs.csdn.net/topics/390509632?page=1#post-398364152)
4、sysgen_certmod fails----[https://social.msdn.microsoft.com/Forums/zh-CN/c47400f8-6e62-4d54-ab38-2a7efe6b12ff/sysgencertmod-fails?forum=winembplatdev](https://social.msdn.microsoft.com/Forums/zh-CN/c47400f8-6e62-4d54-ab38-2a7efe6b12ff/sysgencertmod-fails?forum=winembplatdev)

RT
makecert.exe不用去找，安装VS2008后，在开始菜单相应的路径找到该命令提示符：Microsoft Visual Studio 2008/Visual Studio Tools/Visual Studio 2008 命令提示
打开后，输入makecert，就可以开始了
参数为：makecert -r -pe -n "cn=MyCA" -$ commercial -a sha1 -b 08/05/2010 -e 01/01/2012 -cy authority -ss my -sr currentuser
其中各部分的意义：
-r： 自签名
-pe： 将所生成的私钥标记为可导出。这样可将私钥包括在证书中。
-n "cn=MyCA"： 证书的subject name，.net自带类库中有X509Store类，可以在store中根据证书subject name，来找到改证书
store参考：[X509Store 类](http://msdn.microsoft.com/zh-cn/library/system.security.cryptography.x509certificates.x509store%28VS.80%29.aspx)
-$ commercial：指明证书商业使用。。。
-a：指定签名算法。必须是 md5（默认值）或 sha1。
-b 08/05/2010：证书有效期的开始时间，默认为证书的创建日期。格式为：mm/dd/yyyy
-e 01/01/2012：指定有效期的结束时间。默认为 12/31/2039 11:59:59 GMT。格式同上
-ss my：证书产生到my个人store区
-sr currentuser：保持到计算机当前个人用户区，其他用户登录系统后则看不到该证书。。
查看该生成的证书并导出：
可以在MMC的证书管理单元中对证书存储区进行管理。Windows没有给我们准备好直接的管理证书的入口。自己在MMC中添加，步骤如下： 
1. 开始→运行→MMC，打开一个空的MMC控制台。
2. 在控制台菜单，文件→添加/删除管理单元→添加按钮→选”证书”→添加→选”我的用户账户”→关闭→确定
3. 在控制台菜单，文件→添加/删除管理单元→添加按钮→选”证书”→添加→选”计算机账户”→关闭→确定
然后导出为PFX格式的证书，PKCS#12规范的证书，包含了公钥和私钥，导出时需要提供一个私钥的保护密码，在导出时设置即可
参考：[证书创建工具 (Makecert.exe)](http://msdn.microsoft.com/zh-cn/library/bfsktky3%28VS.80%29.aspx)
//===========================================================================================================
备注：：
1》makecert.exe，cert2spc.exe ，pvk2pfx.exe这3个工具软件的路径为----D:\Program Files\Microsoft Visual Studio 8\Common7\Tools\Bin
2》signbin.exe,signfile.exe,signimage.exe这3个工具软件的路径为----D:\WINCE600\PUBLIC\COMMON\OAK\BIN\I386
# wince6.0 A8平台加密，签名可执行文件后还是无法运行
