# iOS  9.3 以下系统，AppStore 下载应用崩溃处理 - weixin_33985507的博客 - CSDN博客
2018年02月07日 09:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
最近有用户反馈自己的手机系统是9.1，9.2的，下载应用后，无规律的闪退。排查了半天无果后，求助度娘。说是9.3系统以下，对于一些图片资源的编码格式不支持。给出解决方案如下：
    1、将应用打出 .ipa的包
    2、将 .ipa的包的尾缀格式改为 .zip
    3、将 .zip解压，生成一个 Payload 的文件夹
    4、终端 cd 到 Payload 目录下
    5、输入：find . -name 'Assets.car'    //寻找Assets.car 目录
        eg：./TestProject.app/Assets.car
    6、输入： sudo xcrun --sdk iphoneos assetutil --info ./TestProject.app/Assets.car > Assets.json  //通过 assetutil 命令将assets里的图片资源信息，全部汇总到 Assets.json 这个文件里。
ps：网上给出的路径格式一点都不直白
7、通过文本编辑打开  Assets.json ，查找 “P3” 或者 “DisplayGamut": “P3”
“Encoding” : “ARGB-16″。9.3 以下的系统不支持 16位图片格式
就能找到9.3以下，不支持的图片资源。重新切图替换一下就OK了。
