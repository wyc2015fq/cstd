# cordova自定义插件 - weixin_33985507的博客 - CSDN博客
2016年11月18日 13:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
**.准备工作**
1. jdk安装，配置jdk环境变量
2. sdk安装，配置adb环境变量
3. node.js环境搭建
4. 命令行 ：npm install -g cordova (时间较长，需要耐心等候)
**.自定义插件啦**
1. plugman安装
   命令行：npm install -g plugman
2. 创建插件（在E:\plugin路径下创建）
  命令行：plugman create --name 参数1 --plugin_id 参数2 --plugin_version 参数3 
  语法：
![3663585-09c06ffec42efbc0.png](https://upload-images.jianshu.io/upload_images/3663585-09c06ffec42efbc0.png)
  例子：plugman create --name HelloPlugin --plugin_id helloPlugin --plugin_version 0.1.1
   创建成功后 文件目录如下所示：
![3663585-ab2743f558a107c7.png](https://upload-images.jianshu.io/upload_images/3663585-ab2743f558a107c7.png)
3.为插件添加平台，这个插件是要给IOS用呢，还是Android用呢,还是俩者都用 我们看看语法，这里添加Android为例子。
命令行：plugman platform add -platform_name android
  添加平台后，文件目录结构如下图所示：
![3663585-5d52ff198aa393a2.png](https://upload-images.jianshu.io/upload_images/3663585-5d52ff198aa393a2.png)
