# 混合开发经验之谈-3TH - DEVELOPER - CSDN博客





2017年12月29日 15:34:01[学术袁](https://me.csdn.net/u012827205)阅读数：108
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![这里写图片描述](https://img-blog.csdn.net/20171229144538844?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 创建一个 CORDOVA-APP

这篇博客目的主要是记录一下我的学历过程，并能让我加深对 [Cordova](http://cordova.axuer.com/docs/zh-cn/latest/guide/cli/index.html) 的理解！下面就让我根据Cordova的官网在基于window系统的环境下，创建一个android app。当然这些是在我的配置基础上进行的。
- 基于我的ANDROID STUDIO 配置环境（因为什么 JDK 各种环境我已经配置过了）
- 安装Cordova CLI 
- [到官网下载Node.js并安装](https://nodejs.org/en/download/)  ，然后双击、下一步、下一步的这种傻瓜点击就好。
- 对于安装过的 Node.js 字符界面，输入命令 `npm install -g cordova` 安装 Cordova 。但是会有问题！！  如：   ![这里写图片描述](https://img-blog.csdn.net/20171229145939627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

看到这里必定会很好奇，因为官网就是这么给示例的： 
![这里写图片描述](https://img-blog.csdn.net/20171229150107681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据上面的提示，我们就需要去windows 自带的字符界面，使用基本的shell命令操作。通过 “cmd”进入：并执行官网上的指示过程，如： 

        - 安装 Cordova 命令： `npm install -g cordova` 这个过程比较久，需等待。 

        - 创建android 的app 命令： `cordova create hello com.example.hello HelloWorld`

        - 切换到项目文件目录hello 命令： `cd hello`

        - 在项目工程中添加Android平台的app 命令： `cordova platform add android --save` 然后进入到创建的目录可以看到已经创建的 Android平台项目。 

        - 在对项目及进行编译(  `cordova build android`    )、运行( `cordova run android` )，可以查看一下有哪一台安卓机已经连接并可以进行安装 命令：`cordova platform ls`  然后可以根据自己对应的id进行指定目标安装。如果只连一台安卓机，当然也可以直接进行安装 命令：`cordova run android` 。 不过，在编译时候需要很久，因为这个编译环境或许会去重新下载gradle等。
我这次只是生成了一个Android 然后传到码云，[有兴趣可以翻看她的结构](https://gitee.com/yuan1530702811/CordovaHybrid)






