# linux 设置pip 镜像 Pip Warning：–trusted-host  问题解决方案 - weixin_33985507的博客 - CSDN博客
2015年07月18日 20:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
pip升级到7.0以后，在使用http镜像进行包安装及升级的时候往往会有如下提示：
> 
Collecting beautifulsoup4
The repository located at mirrors.aliyun.com is not a trusted or secure host and is being ignored. If this repository is available via HTTPS it is recommended to use HTTPS instead, otherwise you may silence this warning and allow it anyways with ‘–trusted-host mirrors.aliyun.com’.
Could not find a version that satisfies the requirement beautifulsoup4 (from versions: )
No matching distribution found for beautifulsoup4
解决办法如下：
1.安装时加入–trusted-host mirrors.aliyun.com参数：
` `
2.在pip.conf中加入trusted-host选项，该方法是一劳永逸
` `
