
# WordPress显示评论者IP归属地、浏览器、终端设备、电信运营商 - 阳光岛主 - CSDN博客

2017年10月21日 17:48:07[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1328


在网上查资料闲逛，偶然间看到了张戈博客的评论框有点意思，于是就收走拿到了我的米扑博客。
本文为[米扑博客](https://blog.mimvp.com/)原创：[总结分享 WordPress显示评论者IP归属地、浏览器、终端设备、电信运营商](https://blog.mimvp.com/article/21606.html)

WordPress显示评论者IP归属地、浏览器、终端设备、电信运营商，如下图：
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-01.png)
米扑博客评论演示效果：[https://blog.mimvp.com/donate/\#comments](https://blog.mimvp.com/donate/#comments)

恩，本来搞定了不打算写博客的，但是想了想原文步骤繁琐，有些细节还没有，都是自己一点一点捣鼓出来的，很多朋友可能用得到，所以还是总结分享写出来
张戈博客，WordPress显示访客UA信息，共分了两篇博客：
[WordPress显示访客UA信息：Show UserAgent纯代码轻度汉化版](https://zhangge.net/4960.html)
[分享WordPress显示评论者IP的归属地及运营商信息的2种方案](https://zhangge.net/5060.html)
本文，将合并两篇博客的内容，主要合并了两个脚本，并对国旗、浏览器、终端设备等图标进行了美化

## 插件实现
**1）Show UserAgent 插件**（推荐）
官网：[https://wordpress.org/plugins/show-useragent/](https://wordpress.org/plugins/show-useragent/)
下载：[show-useragent.1.0.9.zip](https://blog.mimvp.com/wp-content/uploads/2016/09/show-useragent.1.0.9.zip)
效果图如下：
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-02.png)
**2）WP-UserAgent 插件**（适合老外）
官网：[https://wordpress.org/plugins/wp-useragent/](https://wordpress.org/plugins/wp-useragent/)
下载：[wp-useragent.zip](https://blog.mimvp.com/wp-content/uploads/2016/09/wp-useragent.zip)
效果图如下：
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-03.png)

**代码实现**
到这里，请注意，全部是重点，按步骤做就好了
**1.  下载zip包**
米扑博客整理了图标和文字两种显示方式，命名为加强版zip包
**Show UserAgent 加强版：**[show-useragent-pro.zip](https://blog.mimvp.com/wp-content/uploads/2016/09/show-useragent-pro.zip)
**2. 上传到主题根目录**
把[show-useragent-pro.zip](https://blog.mimvp.com/wp-content/uploads/2016/09/show-useragent-pro.zip)上传到主题的根目录，注意不是WordPress根目录
例如，米扑博客使用的主题是 officefolders ，则zip包上传至 wp-content/themes/officefolders/ 目录下
上传后，解压zip包：
unzip  show-useragent-pro.zip
解压后的文件列表如下：
\# tree show-useragent -L 1
show-useragent
├── browsers
├── flags
├── ip2c
├── ip2c-qqwry.dat
├── ip2c-text.php
└── show-useragent.php
**3. 添加代码到 functions.php**
vim functions.php
在 php 作用域内，添加两行代码：
|1
|2
|include|(|"show-useragent/show-useragent.php"|);|// 显示访客信息
|include|(|"show-useragent/ip2c-text.php"|);|// IP归属地和运营商查询功能
|

**4. 添加代码到 xxx-comment.php 模板**
这里，我们需要在评论者用户后面，添加显示IP归属地、浏览器图标等信息，因此需要找到评论者的位置，查找方法如下：
1）打开有评论的网页，按 F12（Mac用户组合键 option + command + i ）进入开发者模式，定位到评论者，例如：阳光岛主
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-04.png)
2）搜索定位的关键词
在WordPress根目录下，搜索定位到的关键词，如上图中的 "comment-author vcard" ，找到对应的评论模板文件
一般模板文件在 wp-include/ 目录下，所以搜索命令为：
\#grep "comment-author vcard" wp-includes/ -r | grep -vE ".svn|.git"
wp-includes/class-walker-comment.php:           <div class="comment-author vcard">
wp-includes/class-walker-comment.php:                                   <div class="comment-author vcard">
3）修改评论模板文件
打开步骤2）找到的评论模板文件
vim wp-includes/class-walker-comment.php
再次搜索关键词 "comment-author vcard"，定位到在文件中的具体位置，添加代码如下：
|1
|2
|3
|4
|<span id=|"comment_ua_info"|class|=|"comment_ua_info"|style=|"white-space: nowrap;overflow: hidden;display:none;"|>
|<?php|echo|'&nbsp;&nbsp;&nbsp;'|; CID_print_comment_flag();|echo|' '|; CID_print_comment_browser(); ?>
|<?php|echo|"<span id='ua-info-text' style='font-size:14px;font-weight:normal;color:\#aaa;'>"|;|echo|convertip(get_comment_author_ip());|echo|"</span>"|; ?>
|</span>
|

添加后的效果：
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-05.png)
简单解释：
<span id="comment_ua_info" ...  >包含IP归属地、浏览器，以及文本信息等，目的是为后面鼠标悬浮显示时使用，js实现显示和隐藏效果
CID_print_comment_flag()显示IP归属地的国旗
CID_print_comment_browser()显示浏览器和用户设备
convertip(get_comment_author_ip())显示IP归属地的文本信息
至此，显示评论者信息已经实现了
如果想看效果，先把id="comment_ua_info"中的样式display:none;去掉，就可以看到效果了
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-06.png)
**5. 添加代码到 footer.php**
初看步骤4的效果还不错，但是看上去信息会很多，颜色等有点乱，于是想追求极致，还需要继续努力
为了实现鼠标悬浮在评论框才显示的效果，我们需要结合js实现，添加js代码到主题下的footer.php中
vim footer.php
添加js代码
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
|<!-- comment ua-info -->
|<script>
|jQuery(|'.comment-body'|).hover(
|function|(){
|jQuery(|this|).find(|'span.comment_ua_info'|).show();
|},
|function|(){
|jQuery(|this|).find(|'span.comment_ua_info'|).hide();
|});
|jQuery(|'.comment-body'|).click(
|function|(){
|jQuery(|this|).find(|'span.comment_ua_info'|).show();
|});
|</script>
|

说明：
1）jQuery('.comment-body').hover  悬浮事件，主要用于PC电脑端，但是手机端没有悬浮事件
2）jQuery('.comment-body').click  点击事件，主要用于手机端，点击可以显示，但需要注意click事件里不要加hide，否则在电脑端与hover相冲突
3）span.comment_ua_info 标签，是在步骤4中特别说明的，记得要把id="comment_ua_info"中的样式设置为display:none;默认不显示ua-info信息
至此，评论者IP归属地等信息默认是隐藏了，只有当鼠标悬浮在评论框时才会显示出来
演示效果，请见米扑博客：[https://blog.mimvp.com/donate/\#comments](https://blog.mimvp.com/donate/#comments)
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-01.png)
**6. 按钮显示评论所有IP**
上面的功能，在一些博客里都已实现，我的米扑博客只是把人家的效果，用代码实现分享出来
最后，奉献一点小创新，个人感觉还是比较实用的，就是添加一个按钮，能够一键显示所有IP
恩，你说的对，步骤4早已经实现了，折腾出步骤5的隐藏IP，现在步骤6又整出一键显示所有IP，闲的蛋疼，哈
恩，蛋疼也好，折腾也罢，也要搞出来，微创新嘛，直接上代码和步骤啦
**1） 修改评论文件 comments.php**
进入主题目录，修改评论文件 comments.php
vim comments.php
添加一行用于显示的按钮（按钮放哪儿，由你自己决定了）
<span id="comment_show_ip" class="comment_show_ip" style="color:blue; font-size:14px;">显示评论IP</span>
**2） 添加js代码到 footer.php**
进入主题目录，修改页脚文件 footer.php
vim footer.php
添加js代码如下：
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
|<script>
|jQuery(|".comment_show_ip"|).click(|function|(){
|if|(jQuery(|".comment_show_ip"|).text() ==|"显示评论IP"|) {
|jQuery(|".comment_ua_info"|).show();
|jQuery(|".comment_show_ip"|).html(|"隐藏评论IP"|);
|}
|else|{
|jQuery(|".comment_ua_info"|).hide();
|jQuery(|".comment_show_ip"|).html(|"显示评论IP"|);
|}
|});
|</script>
|

**3）检阅成果**
a）点击“显示评论IP”，则显示全部评论IP，并把按钮置为“隐藏评论IP”，如下图
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-07.png)
b）点击“隐藏评论IP”，则隐藏全部评论IP，并把按钮置为“显示评论IP”，如下图
![](https://blog.mimvp.com/wp-content/uploads/2016/09/mimvp-bo-ke-zong-jie-fen-xiang-wordpress-xian-shi-ping-lun-zhe-ip-gui-shu-di-liu-lan-qi-zhong-duan-she-bei-dian-xin-yun-ying-shang-08.png)
好了，折腾完了，到底蛋闲不闲疼不疼，看看米扑博客的效果再说吧 <img draggable="false" class="emoji" alt="

