# rpm信息查看 - weixin_33985507的博客 - CSDN博客
2017年02月21日 16:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
rpm -qpi xxx.rpm #查看rpm包相关信息，一般有版本，build日期，功能描述，大小，公司等等
rpm -qpl xxx.rpm  #查看rpm包含安装的目录和文件
rpm -qpc xxx.rpm  #查看rpm包的配置文件，没有就不显示
rpm -qpR xxx.rpm  #查看依赖关系
