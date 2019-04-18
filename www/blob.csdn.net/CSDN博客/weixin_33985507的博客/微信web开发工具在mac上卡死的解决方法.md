# 微信web开发工具在mac上卡死的解决方法 - weixin_33985507的博客 - CSDN博客
2017年04月06日 09:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
下载了mac版本的微信web开发工具，但是打开后经常会出现界面没响应，只有到活动监视器里强制杀掉重新打开，重新安装也没有效果。
其实这主要是因为旧的缓存导致的。
我们可以尝试以下步骤。
1、在应用程序里，找到“微信web开发者工具”，右键，移除到废纸篓
2、删除一下几个配置和缓存文件：
```
～/Library/Application Support/微信web开发者工具 
~/Library/Preferences/com.tencent.wechat.devtools.plist
 ~/Library/Caches/微信web开发者工具
```
3、重新安装微信web开发者工具。
