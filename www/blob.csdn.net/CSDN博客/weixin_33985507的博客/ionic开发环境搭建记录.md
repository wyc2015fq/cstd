# ionic开发环境搭建记录 - weixin_33985507的博客 - CSDN博客
2016年09月06日 15:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
**资料下载地址：**
1、node.js：[官网下载](https://nodejs.org/en/download)；[网盘下载（Windows-64bit， 密码: yz2i）](https://pan.baidu.com/s/1jXRN3tOP3avl-ggfkzRREA)；
2、androidSDK：[官网下载](http://dl.google.com/android/installer_r24.3.4-windows.exe)；[网盘下载（密码：3ppy）](http://pan.baidu.com/s/1sly7FlV)；
3、JDK官：[官网下载](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)；
4、其它相关资料：[ionic添加某模块插件命令](https://www.npmjs.com/package/i18n)。
**大致步骤（注意安装顺序）：**安装node.js - 安装jdk - 安装android(adk) - 命令安装ionic - 创建项目 - 编译项目apk，同时IOS前两部流程相同
1、下载并安装node.js，并保证最新的稳定版本，打开命令窗口通过 node -v验证是否安装成功；
![2962550-516359b013c14556.png](https://upload-images.jianshu.io/upload_images/2962550-516359b013c14556.png)
验证node.js安装是否成功
2、安装cordova和ionic：npm install -g cordova ionic（在命令窗口执行）；
![2962550-4c4dad86fd1fc14c.png](https://upload-images.jianshu.io/upload_images/2962550-4c4dad86fd1fc14c.png)
验证ionic和cordova是否安装成功
若之前安装过ionci以及cordova，应当及时更新最新版本（npm update -g ionic/npm update -g cordova）。
3、安装javaJDK；
android开发环境较为繁琐，以下是安卓的后续搭建步骤：
1)安装androidJDK；（注意：设置环境变量全局访问android）
*我的电脑 - 右键属性 - 高级系统设置 - 环境变量*
*新建变量名               变量值*
*ANDROID_HOME     D:\ionic\androidsdk(你sdk的路径)*
*然后在path里添加(注意前面的分号)*
*;%ANDROID_HOME%\platform-tools;%ANDROID_HOME%\tools*
*执行android -h看到命令即安装设置成功*
2)执行androidsdk目录下的SDK Manager.exe即可打开工具包（若打不开，请用管理员权限运行！）；
3)配置AndroidSDK，国内无法访问google服务器，所以列表是空的需要配置镜像，选择Tools - Options 打开设置界面：
![2962550-01af4ed8d19f4ec1.png](https://upload-images.jianshu.io/upload_images/2962550-01af4ed8d19f4ec1.png)
打开设置界面
![2962550-1c3072cc0b2fa758.png](https://upload-images.jianshu.io/upload_images/2962550-1c3072cc0b2fa758.png)
填入mirrors.neusoft.edu.cn 端口80并勾选 “Forcehttps://...sources to be fetched using[http://...](/)单击Close关闭
4)依次选择Packages - Reload（或者重新打开）**这里千万注意只需要勾选3个Android SDK Platform-tools、****Android SDK Build-tools、****API23下的****SDK Platform**(编译的时候会提示你需要的api版本，目前是API24，pass：项目集成Crosswalk需要勾选Extras下的Android Support Repository和Google Repository否则会报错);
![2962550-f2f8cd37afc08ee8.png](https://upload-images.jianshu.io/upload_images/2962550-f2f8cd37afc08ee8.png)
选择必要的功能模块（弹出窗里选中Accept，然后点击下一步）
至此，ionic开发环境搭建完毕。
**常用命令：**
npm install -g cnpm --registry=https://registry.npm.taobao.org（npm镜像源指向淘宝）
cnpm install -g cordova ionic（安装cordova ionic）
cnpm update -g cordova ionic（更新cordova ionic）
ionic -help（查看帮助）
ionic -v（查看版本）
ionic start myApp blank（空项目）
ionic start myApp tabs（带导航条）
ionic start myApp sidemenu（带侧滑菜单）
ionic platform add android（添加android平台）
ionic platform remove android（移除android平台）
ionic build android（编译项目apk）
ionic emulate android（运行项目apk 手机连接在手机运行 模拟器连接在模拟器运行）
ionic run android（相当于build + emulate）
ionic serve（开启服务调试）
