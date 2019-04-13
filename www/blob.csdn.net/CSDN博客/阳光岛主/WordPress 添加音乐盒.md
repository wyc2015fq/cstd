
# WordPress 添加音乐盒 - 阳光岛主 - CSDN博客

2018年04月25日 18:56:52[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2228


[米扑博客](https://blog.mimvp.com/article/22260.html)，早在2013年就添加过音乐盒，当时用的豆瓣音乐，后来豆瓣音乐下架了，米扑博客的音乐盒也跟着下架了...
2017.10.24 程序员节，米扑博客重新整理上架了新的音乐盒，聚合了QQ、百度、网易、虾米四大音乐盒，再也不用担心音乐盒下架了![sad](https://blog.mimvp.com/wp-content/plugins/ckeditor-for-wordpress/ckeditor/plugins/smiley/images/smilies-mimvp/mipq_biggrin.gif)

原文请见米扑博客：[WordPress 添加音乐盒](https://blog.mimvp.com/article/22260.html)
米扑音乐盒之赏析：[https://blog.mimvp.com/music](https://blog.mimvp.com/music)

**0. 回顾下架的豆瓣音乐**
<iframe frameborder="0" height="186" name="iframe_canvas" scrolling="no" src="http://douban.fm/partner/baidu/doubanradio" width="420"></iframe>
2013年，很多WordPress博客内嵌了豆瓣音乐盒，做得真的很棒、很精致
后来，可能因为音乐版权，或不是不盈利的项目，被豆瓣下架了
**1. 米扑重新上架音乐盒**
2017.10.24 程序员节，米扑博客在网上重新搜集了许多适合WordPress的音乐盒，最后选定了**WP-Player**
WP-Player 官网：[https://wordpress.org/plugins/wp-player/](https://wordpress.org/plugins/wp-player/)
WP-Player 插件：[wp-player-2.1.6.zip](https://blog.mimvp.com/wp-content/uploads/2017/10/wp-player.zip)
WP-Player 官网示例：[http://webjyh.com/wp-player/](http://webjyh.com/wp-player/)
WP-Player 米扑音乐盒：[https://blog.mimvp.com/music/](https://blog.mimvp.com/music/)（推荐）
**米扑音乐盒效果图：**
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-tian-jia-yin-yue-he-01.png)
**2. WP-Player 插件安装**
方式1）
WordPress 后台 —> 安装插件 —> 搜索 "**WP-Player**"
方式2）
下载上面米扑博客提供的插件[wp-player-2.1.6.zip](https://blog.mimvp.com/wp-content/uploads/2017/10/wp-player.zip)手动上传安装
**3. WP-Player 参数配置**
WordPress 插件的后台配置项很简单，几乎没有什么配置项
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-tian-jia-yin-yue-he-05.png)
WordPress 插件 WP-Player 是一个迷你歌曲播放器，支持多歌曲播放，
**支持使用网易云音乐、虾米音乐、QQ音乐、百度音乐歌曲地址，也支持自定义上传音乐。**
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-tian-jia-yin-yue-he-02.png)
**插件介绍：**
1. WP-Player 支持网易云音乐, 虾米音乐, QQ音乐, 百度音乐平台
2. 如在网易云音乐打开喜欢的歌曲页面，复制歌曲页面的网址如：[http://music.163.com/\#/song?id=191213](http://music.163.com/#/song?id=191213)
3. 并将复制的歌曲网址填写到后面你新建文章下方的表单内，音乐类型将根据网址自动做出选择。
4. 点击获取音乐ID按钮，此时音乐ID出现在表单中。
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-tian-jia-yin-yue-he-03.png)

5. 将短代码填入您的新建文章内容中。
|1
|<!--wp-player start-->|<|div|class|=|"wp-player"|data-wp-player|=|"wp-player"|data-source|=|"netease"|data-autoplay|=|"0"|data-random|=|"0"|data-type|=|"song"|data-id|=|"http://music.163.com/\#/song?id=191213"|data-title|=|""|data-author|=|""|data-address|=|""|data-thumb|=|"https://blog.mimvp.com/wp-content/plugins/wp-player/images/default.png|"|data-lyric|=|"open"|><|div|class|=|"wp-player-box"|><|div|class|=|"wp-player-thumb"|><|img|src|=|"https://blog.mimvp.com/wp-content/plugins/wp-player/assets/images/default.png"|width|=|"90"|height|=|"90"|alt|=|""|><|div|class|=|"wp-player-playing"|><|span|></|span|></|div|></|div|><|div|class|=|"wp-player-panel"|><|div|class|=|"wp-player-title"|>Loading...</|div|><|div|class|=|"wp-player-author"|>Loading...</|div|><|div|class|=|"wp-player-progress"|><|div|class|=|"wp-player-seek-bar"|><|div|class|=|"wp-player-play-bar"|><|span|class|=|"wp-player-play-current"|></|span|></|div|></|div|></|div|><|div|class|=|"wp-player-controls-holder"|><|div|class|=|"wp-player-time"|>00:00</|div|><|div|class|=|"wp-player-controls"|><|a|href|=|"javascript:;"|class|=|"wp-player-previous"|title|=|"上一首"|></|a|><|a|href|=|"javascript:;"|class|=|"wp-player-play"|title|=|"播放"|></|a|><|a|href|=|"javascript:;"|class|=|"wp-player-stop"|title|=|"暂停"|></|a|><|a|href|=|"javascript:;"|class|=|"wp-player-next"|title|=|"下一首"|></|a|></|div|><|div|id|=|"wp-player-lyrics-btn"|class|=|"wp-player-lyrics-btn"|title|=|"歌词"|style|=|"display:block;"|></|div|><|div|class|=|"wp-player-list-btn"|title|=|"歌单"|></|div|></|div|></|div|></|div|><|div|class|=|"wp-player-main"|><|div|class|=|"wp-player-list"|><|ul|></|ul|></|div|><|div|class|=|"wp-player-lyrics"|><|ul|></|ul|></|div|></|div|></|div|>|<!--wp-player end-->
|

6. 短代码中 autoplay 表示是否自动播放；参数”0″表示否；”1″表示是；
7. 短代码中 random 表示是否随机播放；参数”0″表示否；”1″表示是；
8. 支持播放歌单：单音乐页面、专辑页面、艺人页面、精选集页面。
9. 本插件需要您的服务器或主机支持 PHP 5.4+ and Curl, OpenSSL 模块已安装。
10. 本插件仅供个人学习研究使用，请勿作为各种商业用户，音乐版权归各音乐平台所有
**注意：**
1）一篇文章只能插入一个播放器，因为用了MetaBox获取参数，只能一篇文章使用一个。
2）如果要插入多个歌曲，请选择歌曲的专辑，例如：[http://music.163.com/\#/artist?id=13286](http://music.163.com/#/artist?id=13286)
3）如果要插入多个歌曲，可以自己上传多个歌曲，注意要是  http://xxx/xxx.mp3 格式，如下图：
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-tian-jia-yin-yue-he-04.png)
**4. WP-Player 效果展示**
WP-Player 可以自定义修改一些CSS样式，米扑博客的音乐盒是简单修改了原生css样式后的效果
![](https://blog.mimvp.com/wp-content/uploads/2018/04/wordpress-tian-jia-yin-yue-he-01.png)
**添加上一个随机显示正能量的名人名言**
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
|<|div|id|=|"showtxt"|style|=|"font-size: 16px; height: 30px; color: \#ff0000;"|>
|曾经，有个女孩纸摆在我面前，木有好好珍惜
|</|div|>
|<|div|align|=|"center"|class|=|"STYLE8"|style|=|"font-size: 16px; color: \#ff0000;"|>
|<|script|language|=|"JavaScript"|>
|quotes = new Array();
|quotes[0] = '米扑，小而美，简而信';
|quotes[1] = '看看我的眼睛，你会发现你对我而言意味着什么…'
|quotes[2] = '收拾起心情，继续走吧，错过花，你将收获雨，错过他，你才会遇到他…';
|quotes[3] = '有些事不是我不在意，而是我在意了又能怎样。。。';
|quotes[4] = '曾经，有个女孩纸摆在我面前，木有好好珍惜…';
|//var count = 91;
|var count = Math.floor(quotes.length * Math.random());
|function changeme(){
|//var rand1 = Math.floor(quotes.length * Math.random());
|//var quote = quotes[rand1];
|count++;
|if(count == quotes.length) count = 0;
|var quote = quotes[count];
|//alert("quote = " +　quote);
|//document.write( quote );
|document.getElementById('showtxt').innerHTML = quote;
|window.setTimeout(changeme, 1000*10);
|}
|window.onload=function(){
|window.setTimeout(changeme(), 1000*10);
|}
|</|script|>
|</|div|>
|


**5. 音乐盒的其他调研结果**
米扑博客，最后虽然选择了**WP-Player**插件，但下面还是介绍下其它的调研成果，供大家参考
**1）内嵌网页音乐**
a）嵌入百度音乐
<iframe id="mainFrame" src="http://fm.baidu.com" width="100%" height="550px" frameborder="0" scrolling="no" style="width: 100%;"></iframe>
b）嵌入酷狗音乐
<iframe id="mainFrame" src="http://www.kugou.com/fmweb/html/index.html" width="100%" height="550px" frameborder="1" scrolling="yes" style="width: 100%;"></iframe>
效果见：[無作爲 - 百度电台](http://www.wuzuowei.net/music/baidufm.html)
**2）网易云音乐插件**
Netease Music 官网：[https://wordpress.org/plugins/netease-music/​](https://wordpress.org/plugins/netease-music/)
Netease Music 插件：[netease-music.zip](https://blog.mimvp.com/wp-content/uploads/2018/04/netease-music.zip)

米扑音乐盒之赏析：[https://blog.mimvp.com/music](https://blog.mimvp.com/music)


