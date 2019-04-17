# 【CTF-WEB】WEB小伎俩 _CTF自学纪实 - CD's Coding - CSDN博客





2015年06月04日 14:13:02[糖果天王](https://me.csdn.net/okcd00)阅读数：5073








CTF什么的，以前就略有耳闻，然而较为具体的是在360的GeekGame宣讲会上得知的。

这次试了试，写一些关于这次 【第一次CTF自学纪实】


什么是CTF:



      CTF全称Capture The Flag，即夺旗比赛，衍生自古代军事战争模式，两队人马前往对方基地夺旗，每队人马须在保护好己方旗帜的情况下将对方旗帜带回基地。在如今的计算机领域中，CTF已经成为安(hei)全(ke)竞赛的一种重要比赛形式，参赛选手往往需要组队参加，通过团队之间的相互合作使用逆向、解密、取证分析、渗透利用等技术最终取得flag。
CTF的比赛形式:


      CTF夺旗赛通常有两种形式，解题模式（Jeopardy）和攻防模式（Attack-Defense）。在解题模式的比赛中，主办方会提供一系列不同类型的赛题，比如上线一个有漏洞的服务、提供一段网络流量、给出一个加密后的数据或经过隐写后的文件等，他们将 flag 隐藏在这些赛题中，选手们通过比拼解题来一决高下；在攻防模式比赛中，主办方会事先编写一系列有漏洞的服务，并将它们安装在每个参赛队伍都相同的环境中，参赛队伍一方面需要修补自己服务的漏洞，同时也需要去攻击对手们的服务、拿到对手环境中的 flag 来得分，攻防模式的竞赛往往比解题模式的竞赛更接近真实环境，比赛过程也更加激烈。
CTF的常见题型:


      一般在CTF比赛中，会涉及MISC，PPC，CRYPTO，PWN，REVERSE，WEB，STEGA这几种题目。 

      MISC(miscellaneous)类型的题目比较杂乱，可能要分析数据，可能需要百度一下，还可能需要脑筋急转弯；PPC(Professionally ProgramCoder)会考察一些编程类的题目；是CRYPTO是密码学，考察各种加解密技术，当然这和软件加密解密的REVERSE技术是两码事。PWN在黑客俚语中代表着，攻破，取得权限，多为溢出类题目。STEGA(Steganography)会将flag隐藏到各种有码无码高清不高清的图片和音像制品中；WEB题目不用多说，大家都懂的。。
国内外有哪些著名的CTF比赛:

**国内：**当然是最近炒得火热的[XCTF](http://www.xctf.org.cn/)全国联赛了！
**国外：**
PHDays CTF全称Positive Hack Days CTF，是一个国际性信息安全竞赛，竞赛规则基于CTF，但更贴近网络实战环境。
iCTF国际黑客竞赛是一个面向全球信息安全专业研究学生团队的大型在线黑客竞赛。清华blue-lotus实验室在上届（2011）竞赛中排名第23位（共87个参赛团队）。
DEFCON是世界上最大、最古老的地下黑客大会，和Black Hat的创始人相同，但相比于Black Hat要随意得多。DEFCON以小规模讨论及技术切磋为主，会上最流行的活动是若干人组成一个局域网，然后互打攻防战（CTF），参赛者的目标是进攻对手的网络，但同时保护好自己的地盘。由于想参赛的队伍都得经过会前淘汰赛，因此参赛者都有相当高的实力。




被这些所吸引，作为爱好我也尝试了一下CTF，也就是浅尝辄止的级别吧，待ACM、CISCN、暑期实习之类的压力变得小一些之后，再来继续玩~

作为第一次尝试，于此做一下记录，以便日后查看以及拾起再续。




![](https://img-blog.csdn.net/20150604134748378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


此次使用的是合天平台，环境是云端临时实验机，图为正在申请新实验机。




![](https://img-blog.csdn.net/20150604134726125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


实验机申请完成，这里合天很贴心的做出了任务系统，出现任务，以答题的形式来检测是否真正理解了相关的知识原理。




![](https://img-blog.csdn.net/20150604134825748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


实验机开始后是这样一个简洁的WinXP系统。




![](https://img-blog.csdn.net/20150604135018887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在浏览器中登录提供的CTF的IP地址 10.1.1.23/1/，可以看到第一关的Button




![](https://img-blog.csdn.net/20150604135111561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


由于第一关似乎并没有什么用，于是看看网页源代码，看到一句注释，删除？那就地址栏改成1.php.bak看看要删掉的是啥呗~




![](https://img-blog.csdn.net/20150604135331079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


感觉很像，但是有这么多奇奇怪怪的分号什么情况？再看看源代码




![](https://img-blog.csdn.net/20150604135455974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其实我很好奇这里为啥是红色的……然而我看到了php结尾的东西，第一时间想到的自然是地址栏打一下（不过感觉是不是能通过某种方法把这个button显示出来……嘛，能进行就成，暂时不管了）




![](https://img-blog.csdn.net/20150604135621307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


有个输入框有个第三关……




![](https://img-blog.csdn.net/20150604135717966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


点击button之后问了一句从哪来然后强行返回刚刚的页面




![](https://img-blog.csdn.net/20150604135851816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


提示上说这里可以“插”，那就试试呗，然而尝试插一个弹窗试试看……




![](https://img-blog.csdn.net/20150604140102123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


啊，弹窗了哎




![](https://img-blog.csdn.net/20150604135957453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


试试 window.location='3rd.php' 




![](https://img-blog.csdn.net/20150604140152183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


果然这样子3rd就让进了……




![](https://img-blog.csdn.net/20150604140058652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


眼皮底下……眼皮底下是哪里啦！




![](https://img-blog.csdn.net/20150604140157839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


主要用的都是Chrome，火狐浏览器不熟……不过还是百度到了怎么浏览器自带抓包




![](https://img-blog.csdn.net/20150604140503268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


响应头里有个本不应该存在的东西，NEXT，看来是自定义的，反正给了名字就进去呗（CTF总不至于给个蜜罐来骗咱嘛）




![](https://img-blog.csdn.net/20150604140601596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然而这里有个丧心病狂的按钮，叫做“点我”，然而鼠标只要一移到他附近，按钮就立马消失了




![](https://img-blog.csdn.net/20150604140706134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


审查元素走起，发现这里有个joy函数，就是用来藏元素用的




![](https://img-blog.csdn.net/20150604140752450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


找这个函数在哪里被用到了，onmouseover="joy()" ，咱先把style改成 display:block 把按钮调出来。




![](https://img-blog.csdn.net/20150604140920200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


按钮有了自然就要把这个害人的joy函数删掉咯




![](https://img-blog.csdn.net/20150604141034160?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


看吧 现在就可以点这个按钮了。




![](https://img-blog.csdn.net/20150604141055127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


KEY-GET




中途查了好多资料还问了信息安全班的同学相关的部分知识，看来CTF需要的知识面范围真的好广呢，不过确实好好玩，等以后有时间再学习新的技术吧



