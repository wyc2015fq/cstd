# Nodejs 安装 grunt ，Webstorm grunt 配置 - z69183787的专栏 - CSDN博客
2015年07月21日 17:20:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4968
遇到安装 grunt 卡死:
npm
 config set registry="http://r.cnpmjs.org/"
npm
 config set registry="https://registry.npmjs.org"
设置下载镜像
首先要安装有gruntjs
命令行中输入:
npm install -g grunt-cli
最后会显示安装到哪里了：
![webstorm中配置gruntjs拓展工具](http://s2.sinaimg.cn/mw690/4c8b519dgx6C9DDIOrLf1&690)
然后进入webstorm -> file -> settings -> external tools，如下图新建：
![webstorm中配置gruntjs拓展工具](http://s1.sinaimg.cn/mw690/4c8b519dgx6C9DRSPfi80&690)
install为项目配置文件，只需运行一次：
![webstorm中配置gruntjs拓展工具](http://s1.sinaimg.cn/mw690/4c8b519dgx6C9E3auic00&690)
grunt开启本地web服务：
![webstorm中配置gruntjs拓展工具](http://s1.sinaimg.cn/mw690/4c8b519dgx6C9E8ytDqb0&690)
watch开启听服务，主要是js和css的编译合并压缩等服务：
![webstorm中配置gruntjs拓展工具](http://s16.sinaimg.cn/mw690/4c8b519dgx6C9En2nVBef&690)
草草记录下，其实里面有很多值得研究的地方。
顺便吐槽下win8的输入法有bug啊，在chrome下经常看不见！捉急！
在webstorm中也有bug。。。
