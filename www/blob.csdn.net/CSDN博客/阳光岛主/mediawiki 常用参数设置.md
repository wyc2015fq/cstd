
# mediawiki 常用参数设置 - 阳光岛主 - CSDN博客

2015年01月18日 17:30:49[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4505


在编写wiki文档的过程中，遇到了很多自定义的需求，通过学习和实践，先总结一些mediawiki 的常用参数设置。
**本文原文转自我的米扑博客**：[mediawiki 常用参数设置](http://blog.mimvp.com/2015/01/common-parameters-for-mediawiki-setting/)
**1 替换logo，外链打开，去掉链接图标**
**1.1 替换logo**
mediawiki 中文的logo路径为mediawiki/skins/common/images/wiki.png，替换为自己的即可。
**示例：**http://wiki.mimvp.com/skins/common/images/wiki.png
**1.2 外链在新窗口打开**（mediawiki 默认在本窗口打开）
修改
vim mediawiki/LocalSettings.php
在最下面添加一行：
$wgExternalLinkTarget = ‘_blank';
**1.3 去掉链接的图标**
mediawiki 链接默认都有一个”箭头”的图标，很不美观，删除方法如下：
修改
vim mediawiki/skins/vector/components/externalLinks.less
注释掉：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|1|@import|"mediawiki.mixins.less"|;
|2|// External links
|3|\#content {
|4|.external {
|5|background-position|:|center|right|;
|6|background-repeat|:|no-repeat|;
|7|/*.background-image-svg('images/external-link-ltr-icon.svg', 'images/external-link-ltr-icon.png');*/
|10|padding-right|:|13px|;
|11|}
|12|}
|

**注释掉如下图：**
![mediawiki-setting_0](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_0.png)
注释掉后网页显示效果（外链图标已不见了，赞~）
![mediawiki-setting_00](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_00.png)
**2 强制显示目录**
mediawiki 默认需要多余4条目录标题时才显示，但也可以在少于3条目录标题时强制显示目录。
强制显示方法，是在页面开口强制添加：
<div style=””>__TOC__</div>
**示例：**
![mediawiki-setting_01](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_01.png)
**3 字体颜色设置**
**示例：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|<div>__TOC__</div>
|== 米扑公司 ==
|<b>米扑科技</b>： <font color=|"red"|>MIMVP</font> - <font color=|"blue"|><font color=|"red"|><b>M</b></font>ost <font color=|"red"|><b>V</b></font>aluable <font color=|"red"|><b>P</b></font>ackage of <font color=|"red"|><b>M</b></font>obile <font color=|"red"|><b>I</b></font>nternet</font>
|<b>价值观</b>：让世界每一个人平等的共享移动互联网的最大价值。
|<b>米扑人</b>：精益求精，追求极致，为用户创造价值。
|<b>qq群</b>：|463103470
|<b>微信</b>： ithomer
|

**显示效果：**
![mediawiki-setting_02](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_02.png)
**4 表格格式设置**
**示例：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|=== 对外产品 ===
|{| border=|"1"|cellpadding=|"5"|cellspacing=|"0"|class=|"wikitable"
||-
|! width=|"80"|style=|"color:\#0000ff;"|align=|"center"|| 产品
|! width=|"170"|style=|"color:\#0000ff;"|| 网址
|! width=|"280"|style=|"color:\#0000ff;"|| 详细描述
|! width=|"160"|style=|"color:\#0000ff;"|| 备注
|! width=|"80"|style=|"color:\#0000ff;"|align=|"center"|| 上线日期
||-
|| style=|"color: \#006400; font-weight:bold;"|align=|"center"|| 米扑论坛
|| [http://forum.mimvp.com http://forum.mimvp.com]
|| 收集用户反馈 <br>不断打磨产品，与用户交朋友，创造价值
|| 倾听用户对产品的反馈建议
|| align=|"center"|||2013|-10|-01
||-
|| style=|"color: \#006400; font-weight:bold;"|align=|"center"|| 米扑博客
|| [http://blog.mimvp.com http://blog.mimvp.com]
|| 专注技术研究 <br>积累Python，PHP，MySQL，Redis等
|| 为创业储备技术
|| align=|"center"|||2013|-11|-10
||-
|| style=|"color: \#006400; font-weight:bold;"|align=|"center"|| 米扑代理
|| [http://proxy.mimvp.com http://proxy.mimvp.com]
|| 搜集检测代理 <br>为自己抓取网页服务；同时对外售卖，保障租用服务器的费用
|| 创业所需的基础服务
|| align=|"center"|||2014|-11|-22
||-
|| style=|"color: \#006400; font-weight:bold;"|align=|"center"|| 米扑域名
|| [http://domain.mimvp.com http://domain.mimvp.com]
|| 免费服务 <br>获取流量，让用户转发推广米扑的系列产品
|| 创业过程中的爱好
|| align=|"center"|style=|"color:\#ff0000;"|| 待定
||-
|| style=|"color: \#006400; font-weight:bold;"|align=|"center"|| 米扑排名
|| [http://apprank.mimvp.com http://apprank.mimvp.com]
|| 提供App排名 <br>包含Android, iOS, WP, Amazon
|| 创业的核心产品
|| align=|"center"|style=|"color:\#ff0000;"|| 待定
||}
|

**显示效果：**
![mediawiki-setting_03](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_03.png)
**5 修改css样式**
修改 table.wikitable 表格的背景色
vim skins/common/shared.css
注释并修改：
520         /*background-color: \#f9f9f9;*/
521         background-color: \#F5FAE2;
注释掉 table.wikitable > * > tr > td 的padding
531         /*padding: 0.2em;*/
修改后的效果：
![mediawiki-setting_04](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_04.png)
**修改左侧，右侧布局的间隔**
vim ./skins/vector/screen-hd.less
注释掉：
6 /*      margin-left: 11em;*/
修改后如下：
![mediawiki-setting_05](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_05.png)
**网页显示效果：**
![mediawiki-setting_06](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_06.png)
**继续修改 \#left-navigation**
vim ./skins/vector/screen-hd.less
注释掉：
23 /*      margin-left: 11em;*/
修改后如下：
![mediawiki-setting_07](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_07.png)
**网页显示效果：**
![mediawiki-setting_08](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_08.png)
**6 修改logo居中显示**
vim ./skins/vector/screen-hd.less
注释掉：
10 /*      left: @menu-main-logo-left;*/
修改后如下：
![mediawiki-setting_09](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_09.png)
**修改左侧导航栏字体大小，行距**
vim ./skins/vector/components/navigation.less
注释掉并修改：
116                                 li {
117                                         /*line-height: 1.125em;*/
118                                         line-height: 1.2em;
119                                         padding: 0;
120                                         padding-bottom: 0.5em;
121                                         margin: 0;
122                                         /*font-size: @menu-main-body-font-size;*/
123                                         font-size: 0.875em;
124                                         word-wrap: break-word;
修改后左侧栏显示效果：
![mediawiki-setting_10](http://cdn-blog.mimvp.com/wp-content/uploads/2015/01/mediawiki-setting_10.png)
**原文**：[mediawiki 常用参数设置](http://blog.mimvp.com/2015/01/common-parameters-for-mediawiki-setting/)


