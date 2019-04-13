
# WordPress管理员评论回复添加标注Admin印章 - 阳光岛主 - CSDN博客

2018年04月29日 09:35:55[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1989


**[米扑博客](https://blog.mimvp.com)**，为了区别管理员与游客或用户的评论或评论回复，显示博主身份是必要的。
本文原文，请见米扑博客：[WordPress管理员评论回复添加标注Admin印章](https://blog.mimvp.com/article/21886.html)

例如：[米扑博客 - 关于](https://blog.mimvp.com/about/)
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-guan-li-yuan-ping-lun-hui-fu-tian-jia-biao-zhu-admin-yin-zhang-00.png)
**WordPress管理员评论回复添加标注Admin印章**
更酷一点，可以给管理员的评论和回复添加Admin个性印章，方法非常简单，
原理是判断用户ID是否为管理员ID，是则输出印章样式。
**截图预览：**
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-guan-li-yuan-ping-lun-hui-fu-tian-jia-biao-zhu-admin-yin-zhang-01.png)
**实现方法：**
在comments.php或functions.php文件中找到代码
<?php comment_text(); ?>
在其下面添加以下代码：
<?php if($comment->user_id == 1){ echo '<span class="admin_author">管理员回复</span>'; } ?>
参考样式，admin.gif为印章图片，博客吧不提供，需要的可以自己制作或网上查找相关素材制作
|1
|2
|3
|4
|5
|6
|7
|8
|.admin_author {
|background:url(images/admin.gif) no-repeat right center; width:60px;
|height:41px;
|position:absolute;
|left:60%;
|bottom:5px;
|text-indent:-9999px;
|}
|

注：需要admin_author的父层添加position:relative;

**WordPress 管理员身份，让管理员评论与众不同**
每一个独立博客都是一个博主的家，身为一家之主，
WordPress博客的管理员肯定要与众不同了，要不然怎能对得起管理员这一华丽丽的身份呢 O(∩_∩)O~
当然让管理员的评论与众不同，还可以防止一些阿猫阿狗的冒充。
还有有的博客是两个管理员管理的，那就更有必要出来澄清一下。
因为每个博客主题的评论模块都不一样，有的是直接用函数，
有的是用 wp_list_comments 的 callback 回调函数来改变博客的评论样式，修改评论回调函数用到的文件是function.php。

首先，要让管理员评论与众不同，就必须先确定出管理员来。
一开始用邮箱来判断管理员身份的，但后来发现，邮箱是可以冒充的，只要有人知道了你的邮箱那么他就可以冒充管理员来使用，所以用邮箱来判断，被否定了，否则谁都能冒充还有什么与众不同。
于是海天用ID来判断，一般来说在wordpress中管理员的ID是1
所以我们只要判断ID，就可以明确管理员的身份，而且这个ID可是不能冒充的。
**ID判断管理员身份：**
|1
|2
|3
|4
|5
|// 设置一个布尔类型的变量用于判断该留言的ID是否为管理员的留言
|$admin_comment|= false;
|if|(|$comment|->user_id == 1){
|$admin_comment|= true;
|}
|

OK，管理员的身份确定了，现在就要来设置管理员评论的与众不同的样式了。
用改变字体样式来区分的，现在换用admin印章感觉比较帅，但这样可能就造成一篇常常的评论下来，都是红章章。
下面把方法简单说一下，何去何从，各位看官自己选择。
**1、改变字体样式**
只需要在回复框的div样式里加上一个判断，是管理员就输出指定样式的 class
1）这里指定管理员评论字体样式为“.admincomment”，但是具体样式，就由各位看官自己添加至style.css文件中。
2）判断是否为管理员，是的话，就添加输出样式。
<div id="comment-<?php comment_ID(); ?>" class="<?php if($admin_comment) echo 'admincomment';?>">
**2、增加印章**
同样是增加一个判断，不过不是输出class样式了，而是输出一个层，一个有印章图片的层。
1）同样是需要指定DIV的层样式的，不过这个不是改变字体，而是层的位置，这个就根据你博客主题自己调整了。
2）判断是否为管理员，是则输出带有图片的层。这个层到底放在哪也就由你自己决定了。
|1
|2
|3
|4
|5
|<?php
|if|(|$admin_comment|) {
|echo|'<div><img src="http://blog.haitianhome.com/wp-content/themes/prowerV3.1/image/admin.gif"></div>'|;
|}
|?>
|

3、印章下载：
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-guan-li-yuan-ping-lun-hui-fu-tian-jia-biao-zhu-admin-yin-zhang-02.gif)
大家想要什么样式来证明你管理员华丽丽的身份，自己选择吧，有什么问题，欢迎大家提问~
最新更新：
用 WP Thread Comment 插件的朋友，可以直接登陆后台插件设置页面，有个 Edit Comment HTML ，这个就是评论框输出的样式，你只需要在适当地方加上代码就可以了。
|1
|2
|3
|4
|5
|<?php
|if|(|$comment|->comment_author_email ==|"123@123.com"|) {
|echo|"<div><img src='http://blog.haitianhome.com/wp-content/themes/prowerV3.1/image/admin.gif'></div>"
|}
|?>
|

**核心重点**
用ID判断只要在相应地方换上“$comment->user_id == 1”  就OK
注意：有的朋友可能开启了缓存插件，造成不会立即显示，清空缓存
**米扑博客做法和效果**
vim wp-includes/class-walker-comment.php
添加代码：
|1
|2
|3
|4
|// 显示博主身份
|if|(|$comment|->user_id == 1) {
|echo|' <span style="font-size: 0.9em; font-weight:normal; color:\#aaa">[博主]</span>'|;
|}
|

效果见本文上面第一张图，或访问[米扑博客 - 关于](https://blog.mimvp.com/about/)



