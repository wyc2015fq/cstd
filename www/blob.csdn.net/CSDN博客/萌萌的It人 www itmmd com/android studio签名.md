
# android studio签名 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月20日 17:24:40[Jlins](https://me.csdn.net/dyllove98)阅读数：6940


androidstudio会给默认的buildType为debug的包签名，所以可以直接运行安装apk到手机上，如果我们自己增加了buildType（例如user,eng）那么直接运行就会说没有签名，安装不了。

签名步骤：
在你需要保存签名文件的目录下（譬如要签名test项目，那么一般就进入test目录下的build-apk里）
运行：
keytool -genkey -aliasuser_key.keystore -keyalg RSA -validity20000-keystoreuserkey.keystore
(其中user_key为你自定义的文件名，20000为有效期）
运行完后可能出现“程序keytool”已经包含在下列软件包中：gcj-4.6-jre-headless,……
那么先运行sudo apt-get install gcj-……

Enter key store password: android  (密码,不少于6位）
Enter key password for <user_key.keystore>:android (密码，我是随便写的）
Common Name:android
(我后面全部输入android)

完成后就会在build-apk(你自定义的目录）下生成了userkey.keystore
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

