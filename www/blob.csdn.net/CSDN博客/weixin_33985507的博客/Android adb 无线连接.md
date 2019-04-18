# Android adb 无线连接 - weixin_33985507的博客 - CSDN博客
2018年10月29日 20:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
1.先将测试机用 USB 连接，确保 adb 连接成功，在 Android Studio 的 Terminal 输入命令 `adb tcpip 5555`为手机指定一个端口号，端口号随便取，只要不是被占用的就行。
![3901445-1e481f00ad3cba56.png](https://upload-images.jianshu.io/upload_images/3901445-1e481f00ad3cba56.png)
设置端口并重连成功
2.让手机的 Wi-fi 和计算机位于同一局域网内，输入命令`adb connect 192.168.0.102:5555`  192.168.0.102 换成你手机的 IP，5555 换成第一步指定的端口号。
![3901445-fb987fa73cb10ab0.png](https://upload-images.jianshu.io/upload_images/3901445-fb987fa73cb10ab0.png)
搞定
3.拔掉数据线，切换 LogCat, 发现大功告成。
![3901445-dfeb139dff2f31ef.png](https://upload-images.jianshu.io/upload_images/3901445-dfeb139dff2f31ef.png)
无线连接成功
