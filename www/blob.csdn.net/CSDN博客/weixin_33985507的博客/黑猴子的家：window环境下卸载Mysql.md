# 黑猴子的家：window环境下卸载Mysql - weixin_33985507的博客 - CSDN博客
2019年02月25日 16:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
#### 1、控制面板->程序->程序和功能->右键卸载
![9193428-e5e53efe89025e2a.png](https://upload-images.jianshu.io/upload_images/9193428-e5e53efe89025e2a.png)
![9193428-3a3a91b973a452d0.png](https://upload-images.jianshu.io/upload_images/9193428-3a3a91b973a452d0.png)
#### 2、显示隐藏文件夹
![9193428-6162317e42e7c651.png](https://upload-images.jianshu.io/upload_images/9193428-6162317e42e7c651.png)
#### 3、删除遗留文件
根据自己的安装目录来
```
C:\ProgramData\MySQL
```
#### 4、点背，终极卸载
**（1）使用360 卸载**
**（2）删除注册表**
```
window+R
regedit
```
![9193428-5ed33f62c2e74d17.png](https://upload-images.jianshu.io/upload_images/9193428-5ed33f62c2e74d17.png)
![9193428-98441bc0b158ec15.png](https://upload-images.jianshu.io/upload_images/9193428-98441bc0b158ec15.png)
```
A.HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Eventlog\Application\MySQL 目录
B.HKEY_LOCAL_MACHINE\SYSTEM\ControlSet002\Services\Eventlog\Application\MySQL 目录
C.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Eventlog\Application\MySQL 目录
D.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControl001\Services\MYSQL 目录
E.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControl002\Services\MYSQL　目录
F.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\MYSQL　目录
```
**（3）删除C:\Documents and Settings\All Users\Application Data\MySQL 目录 （隐藏的目录）**
**（4）删除安装路径文件，重新安装新版MYSQL**
