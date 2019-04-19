# 得到Android keystore签名的命令方法 - =朝晖= - 博客园
# [得到Android keystore签名的命令方法](https://www.cnblogs.com/dhcn/p/7125049.html)
- keytool -list -v -keystore keyfile.jks   
上面这个命令是用来得到一些签名字符串，这些字符串用来在各种平台上填资料的
再来个手动签名的命令:
- jarsigner -verbose -keystore ks2.keystore -signedjar GasBuddy_signed_00.apk GasBuddy002.apk alias_name  
