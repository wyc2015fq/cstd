# mediawiki常用设置 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月31日 14:10:08[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：245
修改mediawiki的logo图标
在LocalSettings.php文件中添加或修改以下代码：
//$wgLogo             = "$wgStylePath/common/images/wiki.png"; 新版已变成resources/assets/wiki.png
$wgLogo             = "$wgStylePath/common/images/logo.png";
并在相应位置放在对应图片即可。
/skins/common/images
添加favicon.ico
直接上传到根目录下即可、
开放注册
$wgGroupPermissions['*']['createaccount']=**true**;
在新窗口中打开外部链接
"LocalSettings.php"里加 上
 $wgExternalLinkTarget = '_blank';
去掉mediawiki右下脚的Powered by MediaWiki图标
打开LocalSettings.php
unset ($wgFooterIcons['poweredby']['mediawiki']);
背景颜色改为护眼色
搜索框中输入MediaWiki:Common.css
复制下面所有内容即可。
/* 此处的 CSS 将应用于所有的皮肤 */
body {background-color: rgb(204,232,207); font-family:Consolas}
* {background-color: rgb(204,232,207); font-family:Consolas}
.mw-code{background-color: rgb(204,232,207); font-family:Consolas}
.mw-content-ltr pre{background-color: rgb(204,232,207); font-family:Consolas}
.firstHeading{background-color: rgb(204,232,207); font-family:Consolas}
.catlinks{background-color: rgb(204,232,207); font-family:Consolas}
修改站点通知，公告Sitenotice
在搜索框中输入MediaWiki:Sitenotice即可编辑
如插入链接：
[http://www.elesos.com/ <font color="red"><b><big>欢迎访问艺搜</big></b></font>]
可以使用<!-- -->进行注释
