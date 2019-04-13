
# WordPress 添加投稿功能 - 阳光岛主 - CSDN博客

2015年11月29日 13:21:05[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5330


WordPress网站开放投稿功能，接受读者的投稿。但WordPress本身并不提供投稿功能，只拥有强大的扩展能力，我们可以自己添加这个投稿功能。
实现用户投稿，有两种方法：
一种是**开放后台注册功能**，普通用户注册进去默认设置为投稿者，登陆进去即可添加文章（默认为草稿）；
另一种是在**前台提供投稿表单**，用户填写相应的表格，例如米扑博客：[http://blog.mimvp.com](http://blog.mimvp.com/)
![add-posting-functionality-to-wordpress-02](http://cdn-blog.mimvp.com/wp-content/uploads/2015/12/add-posting-functionality-to-wordpress-02.png)
前一种方法实现起来比较简单，基本不需要配置太多，只是有些博主可能会觉得别扭，不愿让他人看到自己的博客后台；而后一种方法对投稿者来说方便了很多，博主也不用担心自己博客的后台隐私，只是该方法实现起来比较麻烦，需要配置的东西很多。本文只将介绍后一种方法，希望对你有所帮助，当然也只是复制粘贴代码就可以了。
#### 一、添加投稿表单
1、首先在当前主题目录（/wp-content/themes/your_theme/pages/）下新建一个php文件，命名为tougao.php，并将page.php中的所有代码复制到tougao.php中（tougao.php名称可能在your_theme/pages/tougao.php下，也可能在your_theme/template-tougao.php）；
**tougao.php 代码**
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
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|100
|101
|102
|103
|104
|105
|106
|107
|108
|109
|110
|111
|112
|113
|114
|115
|116
|117
|118
|119
|120
|121
|122
|123
|124
|125
|126
|127
|128
|129
|130
|131
|132
|133
|134
|135
|136
|137
|138
|139
|140
|141
|142
|143
|144
|145
|146
|147
|148
|149
|150
|151
|152
|153
|154
|155
|156
|157
|158
|159
|160
|161
|162
|163
|164
|165
|166
|167
|168
|169
|170
|171
|172
|173
|174
|175
|176
|177
|178
|179
|180
|181
|182
|183
|184
|185
|186
|187
|188
|189
|190
|191
|192
|193
|194
|195
|196
|197
|<?php
|/**
|* template name: 投稿
|* website demo : http://blog.mimvp.com
|* description: template for mimvp.com
|* author : yanggang
|* date   : 2015-11-20
|*/
|if|( isset(|$_POST|[|'tougao_form'|]) &&|$_POST|[|'tougao_form'|] ==|'mimvp_auto_post'|){
|if|( isset(|$_COOKIE|[|"tougao"|]) && ( time() -|$_COOKIE|[|"tougao"|] ) < 120 ){
|wp_die(|'您投稿也太勤快了吧，先歇会儿！'|);
|}
|print_r(|$_POST|);
|//表单变量初始化
|$authorname|= isset(|$_POST|[|'tougao_authorname'|] ) ?|$_POST|[|'tougao_authorname'|] :|''|;
|$email|= isset(|$_POST|[|'tougao_authoremail'|] ) ?|$_POST|[|'tougao_authoremail'|] :|''|;
|$blog|= isset(|$_POST|[|'tougao_authorblog'|] ) ?|$_POST|[|'tougao_authorblog'|] :|''|;
|$title|= isset(|$_POST|[|'tougao_title'|] ) ?|$_POST|[|'tougao_title'|] :|''|;
|$tags|= isset(|$_POST|[|'tougao_tags'|] ) ?|$_POST|[|'tougao_tags'|] :|''|;
|$category|= isset(|$_POST|[|'tougao_cat'|] ) ?|$_POST|[|'tougao_cat'|] :|'4766'|;
|$postdate|= isset(|$_POST|[|'tougao_date'|] ) ?|$_POST|[|'tougao_date'|] :|date|(|"Y-m-d H:i:s"|,|strtotime|(|"+8 hour"|));
|$content|= isset(|$_POST|[|'tougao_content'|] ) ?|$_POST|[|'tougao_content'|] :|''|;
|$category_list|=|explode|(|","|,|$category|);
|//    $category_list = array("4924","1798","783","771");
|/*
|echo $postdate;
|//表单项数据验证
|if ( empty($authorname) || strlen($authorname) > 20 ){
|wp_die('昵称必须填写，且不得超过20个长度');
|}
|if ( empty($email) || strlen($email) > 60 || !preg_match("/^([a-z0-9\+_\-]+)(\.[a-z0-9\+_\-]+)*@([a-z0-9\-]+\.)+[a-z]{2,6}$/ix", $email)){
|wp_die('邮箱必须填写，且不得超过60个长度，必须符合email格式');
|}
|if ( empty($title) || strlen($title) > 100 ){
|wp_die('文章标题必须填写，且不得超过100个长度');
|}
|if ( empty($content) || strlen($content) < 100){
|wp_die('内容必须填写，且不得少于100个长度');
|}
|*/
|$tougao|=|array|(|'post_title'|=>|$title|,
|'post_content'|=>|$content|,
|'post_status'|=>|'publish'|,
|'tags_input'|=>|$tags|,
|'post_date'|=>|$postdate|,
|// 'post_category' => array($category));
|'post_category'|=>|$category_list|);
|// 将文章插入数据库
|$status|= wp_insert_post(|$tougao|);
|if|(|$status|!= 0){
|global|$wpdb|;
|$myposts|=|$wpdb|->get_results(|"SELECT ID FROM $wpdb->posts WHERE post_status = 'publish' AND post_type = 'post' ORDER BY post_date DESC"|);
|add_post_meta(|$myposts|[0]->ID,|'tcp_postauthor'|,|$authorname|);|//插入投稿人昵称的自定义域
|if|( !|empty|(|$blog|))
|add_post_meta(|$myposts|[0]->ID,|'tcp_posturl'|,|$blog|);|//插入投稿人网址的自定义域
|setcookie(|"tougao"|, time(), time()+180);
|wp_die(|'投稿成功！'|,|'投稿成功！'|);
|}|else|{
|wp_die(|'投稿失败！'|,|'投稿失败！'|);
|}
|}
|get_header();
|?>
|<?php get_sidebar(); ?>
|<body>
|<script type=|"text/javascript"|src=|"<?php bloginfo('template_url'); ?>/kindeditor/kindeditor-min.js"|></script>
|<script type=|"text/javascript"|src=|"<?php bloginfo('template_url'); ?>/kindeditor/lang/zh_CN.js"|></script>
|<script type=|"text/javascript"|>
|$(document).ready(|function|(){
|var|editor;
|KindEditor.ready(|function|(K) {
|editor = K.create(|'textarea[name="tougao_content"]'|, {
|allowFileManager : false,
|allowImageUpload : false
|});
|});
|});
|</script>
|<?php|if|(have_posts()) : ?>
|<?php|while|(have_posts()) : the_post(); ?>
|<?php|include|(|'top.php'|); ?>
|<div id=|"wrapper"|class|=|"clearfix"|>
|<div id=|"navigation"|>
|<div|class|=|"navigation"|>
|<h2><a title=|"<?php the_title(); ?>"|><?php the_title(); ?></a></h2>
|</div>
|<?php|include|(|"includes/notice.php"|); ?>
|</div>
|<div|class|=|"tougao divmargin"|>
|<div|class|=|"entry"|>
|<form method=|"post"|action=|"<?php echo $_SERVER["|REQUEST_URI|"]; $current_user = wp_get_current_user(); ?>"|>
|<div id=|"basicinfo"|>
|<table style=|"width: 700px; line-height: 30px; font-size: 16px;"|>
|<tr>
|<td style=|"width: 75px; text-align: right; padding-right: 10px;"|>昵称:</td>
|<td>
|<input type=|"text"|style=|"width: 200px;"|name=|"tougao_authorname"|value=|"<?php if ( 0 != $current_user->ID ) echo $current_user->user_login; ?>"|/>
|<small>*</small>
|</td>
|</tr>
|<tr>
|<td style=|"width: 75px; text-align: right; padding-right: 10px;"|>E-Mail:</td>
|<td>
|<input type=|"text"|style=|"width: 200px;"|name=|"tougao_authoremail"|value=|"<?php if ( 0 != $current_user->ID ) echo $current_user->user_email; ?>"|/>
|<small>*</small>
|</td>
|</tr>
|<tr>
|<td style=|"width: 75px; text-align: right; padding-right: 10px;"|>您的网站:</td>
|<td>
|<input type=|"text"|style=|"width: 200px;"|name=|"tougao_authorblog"|value=|"<?php if ( 0 != $current_user->ID ) echo $current_user->user_url; ?>"|/>
|</td>
|</tr>
|<tr>
|<td style=|"width: 75px; text-align: right; padding-right: 10px;"|>文章标题:</td>
|<td>
|<input type=|"text"|style=|"width: 200px;"|name=|"tougao_title"|value=|""|/>
|<small>*</small>
|</td>
|</tr>

|<tr>
|<td style=|"width: 75px; text-align: right; padding-right: 10px;"|>关键字:</td>
|<td>
|<input type=|"text"|style=|"width: 200px;"|value=|""|name=|"tougao_tags"|/>
|<small>*</small>
|</td>
|</tr>
|<tr>
|<td style=|"width: 75px; text-align: right; padding-right: 10px;"|>文章分类:</td>
|<td>
|<?php|//wp_dropdown_categories('show_count=1&hierarchical=1'); ?>
|<?php wp_dropdown_categories(|'title_li=0&hierarchical=1'|); ?>
|<small>*</small>
|</td>
|</tr>
|</div>
|<tr>
|<td colspan=|"2"|>
|文章内容:(必须)
|</td>
|</tr>
|<tr>
|<td colspan=|"2"|>
|<div|class|=|"post-area"|>
|<textarea rows=|"15"|cols=|"60"|id=|"tougao_content"|name=|"tougao_content"|></textarea>
|</div>
|</td>
|</tr>
|<tr>
|<td>发布密码:</td>
|<td>
|<input type=|"password"|style=|"width: 200px;"|value=|"123456"|name=|"tougao_form"|/>
|<small>*</small>
|</td>
|</tr>

|<tr>
|<td colspan=|"2"|>
|<input id=|"submit"|name=|"submit"|type=|"submit"|value=|"提交"|/>
|&nbsp;&nbsp;&nbsp;
|<input id=|"reset"|name=|"submit"|type=|"reset"|value=|"重填"|/>
|</td>
|</tr>
|</table>

|</form>
|</div>
|</div>
|</div>
|<?php|endwhile|;|else|: ?>
|<?php|endif|; ?>
|<?php get_footer(); ?>
|

2、tougao.php开头注释/* 与 */ ，需要添加如下内容：
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
|<?php
|/**
|* template name: 投稿
|* website demo : http://blog.mimvp.com
|* description: template for mimvp.com
|* author : yanggang
|* date   : 2015-11-20
|*/
|……
|

上面注释里，必须要有 template name 模板名称，供发布页面时选用
3、最后以UTF-8编码保存tougao.php，否则中文可能会乱码。
4、然后进入WordPress管理后台 – 页面 – 创建页面，标题为投稿，内容填上投稿说明等，右下侧选择模板投稿（template name 模板名称为“投稿”）。
![add-posting-functionality-to-wordpress-03](http://cdn-blog.mimvp.com/wp-content/uploads/2015/12/add-posting-functionality-to-wordpress-03.png)
**此页面即自定义的前台注册页面，将该页面的链接放到网站任何位置，供用户点击注册即可，示例为米扑博客：****[http://blog.mimvp.com](http://blog.mimvp.com/)**
![add-posting-functionality-to-wordpress-02](http://cdn-blog.mimvp.com/wp-content/uploads/2015/12/add-posting-functionality-to-wordpress-02.png)
好了，投稿的基本功能已经添加完毕，至于表单样式不好看，表单缺少你想要的项目等问题，你就自己添加css、表单项吧。最后，也欢迎给[米扑博客](http://blog.mimvp.com/)投稿，当然本站也支持第一种注册投稿功能，秉持开放自由的态度。

#### 代码补充说明
1、如果你想让投稿的文章立即发布，而不需要审核再编辑，那么请将以上代码66行改成：
‘post_content’ => $post_content, ‘post_status’ => ‘publish’,
2、如果你想让用户在投稿的同时，将投稿者注册成你本站的投稿者，并将文章的作者归到这个投稿者的名下，你可以参考上面的投稿代码tougao.php
3、如果你的博客文章都有自定义栏目，并且想在用户投稿的同时添加自定义栏目，参考上面的投稿代码tougao.php
4、如果你觉得本文提供的文章编辑框太过单调，需要一个富文本编辑，你可以看看这篇文章（包含图片上传功能）：[WordPress投稿功能添加富文本编辑器](http://www.ludou.org/tougao-text-editor.html)
5、如果你使用了一些富文本编辑器，文章提交后内容中的代码都被转义了，参考上面的投稿代码tougao.php
6、如果你需要投稿的文章发布后通知投稿者，可以看看这篇文章（前提投稿的文章默认是草稿状态，而不是直接发布）：[WordPress投稿功能添加邮件提醒功能](http://www.ludou.org/wordpress-tougao-mail.html)
7、如果你想给投稿页面增加验证码功能，可以[点此下载](http://www.ludou.org/tool/file/captcha-php)验证码文件，解压后将captcha目录放到当前主题目录下，然后在代码一中，将35行的：
<br clear=”all”>
改成：
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
|<div style=|"text-align: left; padding-top: 10px;"|>
|<label|for|=|"CAPTCHA"|>验证码:
|<input id=|"CAPTCHA"|style=|"width:110px;*float:left;"|class|=|"input"|type=|"text"|tabindex=|"24"|size=|"10"|value=|""|name=|"captcha_code"|/> 看不清？<a href=|"javascript:void(0)"|onclick=|"document.getElementById('captcha_img').src='<?php bloginfo('template_url'); ?>/captcha/captcha.php?'+Math.random();document.getElementById('CAPTCHA').focus();return false;"|>点击更换</a>
|</label>
|</div>
|<div style=|"text-align: left; padding-top: 10px;"|>
|<label>
|<img id=|"captcha_img"|src=|"<?php bloginfo('template_url'); ?>/captcha/captcha.php"|/>
|</label>
|</div>
|<br clear=|"all"|>
|

将代码二中的：
if( isset($_POST[‘tougao_form’]) && $_POST[‘tougao_form’] == ‘send’) {
改成：
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
|if|(!isset(|$_SESSION|)) {
|session_start();
|session_regenerate_id(TRUE);
|}
|if|( isset(|$_POST|[|'tougao_form'|]) &&|$_POST|[|'tougao_form'|] ==|'send'|) {
|if|(|empty|(|$_POST|[|'captcha_code'|])
||||empty|(|$_SESSION|[|'ludou_lcr_secretword'|])
||| (trim(|strtolower|(|$_POST|[|'captcha_code'|])) !=|$_SESSION|[|'ludou_lcr_secretword'|])
|) {
|wp_die(|'验证码不正确！<a href="'|.|$current_url|.|'">点此返回</a>'|);
|}
|

大功造成！
**禁止WordPress自动加br标签的方法**
每当WordPress添加html代码时候WordPress都会进行格式检查自动在后面加入br标签或p标签导致代码出现错误，打开WordPress程序中的wp-includes/formatting.php，查找
function wpautop($pee, $br = true)
将他改为：
function wpautop($pee, $br =false)
就可以禁止wordpress自动加br或p标签了。
![add-posting-functionality-to-wordpress-04](http://cdn-blog.mimvp.com/wp-content/uploads/2015/12/add-posting-functionality-to-wordpress-04.png)
修改后，不换行的效果，请见米扑代理：
[米扑代理2.0发布，集成支付功能](http://blog.mimvp.com/2015/05/m-agent-2-0-released-bashing-integrated-payment-function/)


