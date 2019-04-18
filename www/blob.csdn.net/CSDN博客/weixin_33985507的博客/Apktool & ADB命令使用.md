# Apktool & ADB命令使用 - weixin_33985507的博客 - CSDN博客
2019年02月13日 14:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## Apktool & ADB命令使用
@(ApkTool&ADB)[apktool, adb]
#### 1.反编译
在cmd中执行反编译命令：apktool d apk的名字，生成文件夹ztgame
```
apktool d ztgame.apk
```
#### 2.重新编译并签名生成APK
- 在cmd中执行apk编译命令：apktool b 文件名 -o 生成pak名字.apk(例如：ztgame_debug.apk)，生成apk为ztgame_debug.apk
```
apktool b ztgame -o ztgame_debug.apk
```
- 新建.sh脚本(例如：signer.sh)
```
#!/bin/sh
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore $3 -signedjar $2 $1 $4
```
- 在git中执行命令(记得拷贝keystore文件到对应文件夹)
```
./signer.sh ztgame_debug.apk ztgame_sign.apk wd.keystore com.ztgame.wd keystore密码  alias密码
./signer.sh   未签名apk的名字   想要生成的签名apk的名字   签名文件kestore   别名alias keystore密码  alias密码
```
> 
1.会出现一个等待输入命令的光标，输入keystore的密码
2.这里ztgame_debug.apk为上面编译生成的apk，ztgame_sign.apk是根据脚本签名之后生成的最终签名的apk
3.com.ztgame.wd是别名
#### 3.直接查看APK的MD5值
```
keytool -printcert -jarfile xxx.apk
//APK后缀改成zip，然后解压缩，在META-INF找到xxx.RSA文件
keytool -printcert -file  解压后的RSA
```
#### 4.直接查看签名文件的MD5和别名(需要密码)
```
keytool -list -v -keystore  签名文件
```
#### 5.忘记别名和密码时，通过签名文件获取别名和别名密码
- 下载**AndroidKeystoreBrute_v1.05.jar**到指定目录
- 新建一个txt文本，里面按行写上你可能记得的密码
- 拷贝签名文件到该目录
- CMD中执行命令(一般使用第三个命令较好)
```
java -jar AndroidKeystoreBrute_v1.05.jar -m 1 -k <...keystore> -start AAAAAA
java -jar AndroidKeystoreBrute_v1.05.jar -m 2 -k "C:\\mykeystore.keystore" -d "wordlist.txt" 
java -jar AndroidKeystoreBrute_v1.05.jar -m 3 -k "C:\\mykeystore.keystore" -d "wordlist.txt"
```
- **AndroidKeystoreBrute_v1.05.jar下载地址**
链接：[https://pan.baidu.com/s/1kUXyhAv](https://pan.baidu.com/s/1kUXyhAv) 密码：zjo3
