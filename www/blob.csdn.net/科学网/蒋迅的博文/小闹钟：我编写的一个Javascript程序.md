# 科学网—小闹钟：我编写的一个Javascript程序 - 蒋迅的博文




# 小闹钟：我编写的一个Javascript程序                           

已有 5230 次阅读2012-9-13 07:44|个人分类:[科技](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&classid=136794&view=me)|系统分类:[科普集锦](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=420554&catid=7)|关键词:程序,,闹钟,,JavaScript|[程序](misc.php?mod=tag&id=15652), [闹钟](misc.php?mod=tag&id=29079), [Javascript](misc.php?mod=tag&id=33170)



作者：蒋迅

我写过不少 JavaScript 
程序，最得意的应该说是一个闹钟程序。当时我写好后交到一个网站上发表，很快上了首页的推荐栏，一个德国杂志专门介绍了我的这个程序。后来我看到一些做的
更漂亮的闹钟 JavaScript 程序，我有时就虚荣地想，都是从我的那个程序衍生出来的。

![](http://image.sciencenet.cn/album/201201/27/161151j1r01ow08c83ho0v.jpg)
德国一家杂志的介绍
这个程序是利用计算机的系统闹钟和正弦、余弦函数。在每一秒钟，决定指针的位置。这是十多年前写的，当时在不同的浏览器里的JavaScript版本也不兼容，所以当时要花很大功夫使得它可以在不同的浏览器上运行。最近看到王庆老师写的“[Mathematica小程序──模拟时钟](http://blog.sciencenet.cn/blog-88791-611719.html)”，让我又想起这个程序。现在的JavaScript版本已经有了一个国际统一标准，但是否能运行我那时写的程序呢？为了写这篇博文，我又试了一下。我发现稍做修改后，仍然可以在火狐8和IE8下运行，虽然表现并不尽相同。

![](http://image.sciencenet.cn/album/201209/12/154029am2755n7x61p4up9.png)

这些年，JavaScript的应用出现上升趋势，一些程序包做的非常漂亮。但到底是[应该发展还是消灭JavaScript的争论](http://developers.slashdot.org/story/11/09/23/2058252/The-Great-.-Debate-Improve-It-Or-Kill-It)一直就没有停止过。可以说，[javascript已经开始下降](http://internet.solidot.org/internet/12/07/23/0225247.shtml?tid=5)。脸书CEO马克·扎克伯格就表示：[押在HTML5上是Facebook最大的错误](http://techcrunch.com/2012/09/11/mark-zuckerberg-our-biggest-mistake-with-mobile-was-betting-too-much-on-html5/)。

我估计自己是不会回到JavaScript去了。

本来不打算写出程序。因为这个程序是1999年写的，所以已经非常过时。下面应部分同学的要求，把原程序附在下面（原来的网站已经不存在）。现在大家用的JavaScript应该更简洁。这里只是提供参考。这个程序分三个部分：Style sheet, JavaScript 和 
HTML。注意，在HTML码中，我用的Layer不是HTML标准中的。建议不要继续使用。我不能保证它在任何浏览器都能运行，也不保证运行效果。


<style TYPE="text/css">

.a {color: red;   font-weight: 400; font-size: 20pt;}

.b {color: blue;  font-weight: 400; font-size: 20pt;}

.c {color: green; font-weight: 400; font-size: 20pt;}

.d {color: black; font-weight: 400; font-size: 20pt;}

.e {color: black; font-weight: 400; font-size: 5pt;}

</style>


<script>

doc = "document.all";

sty = ".style";

htm = "";

xpos = "event.x";

ypos = "event.y";


function Point(x,y) {

   this.x = x;

   this.y = y;

}


var pts = new Point ();

var PI  = 3.14159265;

var RADIUS = 50;

pts[0] = new Point(0,0);

for (i=1;i<=12;i++) {

   pts[i] = new Point(-RADIUS*Math.sin((i+6)*PI/6),RADIUS*Math.cos((i+6)*PI/6));

}


var mu = 20;


function layerSetup() {

    Hr0_Lyr = eval(doc + '["LayerHr0"]' + sty);

    Hr0_Lyr.left = (available_width)/2;

    Hr0_Lyr.top = (available_height-145)/2 - mu;

    var i = 1;

    Hr1_Lyr = eval(doc + '["LayerHr1"]' + sty);

    Hr1_Lyr.left = (available_width)/2 + pts[i].x;

    Hr1_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr2_Lyr = eval(doc + '["LayerHr2"]' + sty);

    Hr2_Lyr.left = (available_width)/2 + pts[i].x;

    Hr2_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr3_Lyr = eval(doc + '["LayerHr3"]' + sty);

    Hr3_Lyr.left = (available_width)/2 + pts[i].x;

    Hr3_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr4_Lyr = eval(doc + '["LayerHr4"]' + sty);

    Hr4_Lyr.left = (available_width)/2 + pts[i].x;

    Hr4_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr5_Lyr = eval(doc + '["LayerHr5"]' + sty);

    Hr5_Lyr.left = (available_width)/2 + pts[i].x;

    Hr5_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr6_Lyr = eval(doc + '["LayerHr6"]' + sty);

    Hr6_Lyr.left = (available_width)/2 + pts[i].x;

    Hr6_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr7_Lyr = eval(doc + '["LayerHr7"]' + sty);

    Hr7_Lyr.left = (available_width)/2 + pts[i].x;

    Hr7_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr8_Lyr = eval(doc + '["LayerHr8"]' + sty);

    Hr8_Lyr.left = (available_width)/2 + pts[i].x;

    Hr8_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr9_Lyr = eval(doc + '["LayerHr9"]' + sty);

    Hr9_Lyr.left = (available_width)/2 + pts[i].x;

    Hr9_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr10_Lyr = eval(doc + '["LayerHr10"]' + sty);

    Hr10_Lyr.left = (available_width)/2 + pts[i].x;

    Hr10_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr11_Lyr = eval(doc + '["LayerHr11"]' + sty);

    Hr11_Lyr.left = (available_width)/2 + pts[i].x;

    Hr11_Lyr.top = (available_height-145)/2 + pts[i].y;

    i++;

    Hr12_Lyr = eval(doc + '["LayerHr12"]' + sty);

    Hr12_Lyr.left = (available_width)/2 + pts[i].x;

    Hr12_Lyr.top = (available_height-145)/2 + pts[i].y;


    var now=new Date();

    var hrs= now.getHours();

    hrs = (hrs>12) ? parseInt(hrs-12) : parseInt(hrs);

    var mins=now.getMinutes();

    mins = parseInt(mins)-30;

    var secs=now.getSeconds();

    secs = parseInt(secs)-30;


    HrCurr_Lyr = eval(doc + '["LayerCurrHr"]' + sty);

    HrCurr_Lyr.left = (available_width)/2 + 0.5*pts[hrs].x;

    HrCurr_Lyr.top = (available_height-145)/2 + 0.5*pts[hrs].y - mu;

    MnCurr_Lyr = eval(doc + '["LayerCurrMn"]' + sty);

    MnCurr_Lyr.left = (available_width)/2 - 0.6*RADIUS*Math.sin(mins*PI/30);

    MnCurr_Lyr.top = (available_height-145)/2 + 0.6*RADIUS*Math.cos(mins*PI/30) - mu;

    ScCurr_Lyr = eval(doc + '["LayerCurrSc"]' + sty);

    ScCurr_Lyr.left = (available_width)/2 - 0.7*RADIUS*Math.sin(secs*PI/30);

    ScCurr_Lyr.top = (available_height-145)/2 + 0.7*RADIUS*Math.cos(secs*PI/30) - mu;


    visibilitySetup();

}


function visibilitySetup() {

     Hr0_Lyr.visibility = "visible";

     Hr1_Lyr.visibility = "visible";

     Hr2_Lyr.visibility = "visible";

     Hr3_Lyr.visibility = "visible";

     Hr4_Lyr.visibility = "visible";

     Hr5_Lyr.visibility = "visible";

     Hr6_Lyr.visibility = "visible";

     Hr7_Lyr.visibility = "visible";

     Hr8_Lyr.visibility = "visible";

     Hr9_Lyr.visibility = "visible";

     Hr10_Lyr.visibility = "visible";

     Hr11_Lyr.visibility = "visible";

     Hr12_Lyr.visibility = "visible";

     HrCurr_Lyr.visibility = "visible";

     MnCurr_Lyr.visibility = "visible";

     ScCurr_Lyr.visibility = "visible";


  updateTime ();

}


function updateTime () {

     var now=new Date();

     var hrs= now.getHours();

     hrs = (hrs>12) ? parseInt(hrs-12) : parseInt(hrs);

     var mins=now.getMinutes();

     mins = parseInt(mins)-30;

     var secs=now.getSeconds();

     secs = parseInt(secs)-30;


     HrCurr_Lyr = eval(doc + '["LayerCurrHr"]' + sty);

     HrCurr_Lyr.left = (available_width)/2 + 0.5*pts[hrs].x;

     HrCurr_Lyr.top = (available_height-145)/2 + 0.5*pts[hrs].y - mu;

     MnCurr_Lyr = eval(doc + '["LayerCurrMn"]' + sty);

     MnCurr_Lyr.left = (available_width)/2 - 0.6*RADIUS*Math.sin(mins*PI/30);

     MnCurr_Lyr.top = (available_height-145)/2 + 0.6*RADIUS*Math.cos(mins*PI/30) - mu;

     ScCurr_Lyr = eval(doc + '["LayerCurrSc"]' + sty);

     ScCurr_Lyr.left = (available_width)/2 - 0.7*RADIUS*Math.sin(secs*PI/30);

     ScCurr_Lyr.top = (available_height-145)/2 + 0.7*RADIUS*Math.cos(secs*PI/30) -mu;


     setTimeout("updateTime()", 1000);

}


function clock() {

     var now=new Date();

     var hrs= now.getHours();

     var mins=now.getMinutes();

     var secs=now.getSeconds();

     var disp=((hrs>12) ? (hrs-12) : hrs) + ":";

     disp+=((mins<10) ? "0" + mins : mins) + ":" + ((secs<10) ? "0" + secs : secs);

     disp+=((hrs>12) ? " PM" : " AM");

        document.forms[0].elements[0].value=disp;

        setTimeout("clock()", 1000);

}

</script>


下面就是定义几个layers,包括表针和12个数字。这部分怎么也贴不上来。下面是图片显示的原程序：

![](http://image.sciencenet.cn/album/201209/14/103241kowc37h66zt0wk03.jpg)

相关阅读：- 
[](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&id=487212)用JavaScript作图表一例- 
[](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&id=531567)杨辉三角展开的JavaScript程序 



转载本文请联系原作者获取授权，同时请注明本文来自蒋迅科学网博客。
链接地址：[](http://blog.sciencenet.cn/blog-420554-612207.html)http://blog.sciencenet.cn/blog-420554-612207.html 

上一篇：[](blog-420554-610324.html)[转载]谁给数学老师陈平福扣了一顶颠覆国家的大帽？
下一篇：[](blog-420554-612928.html)这个小费够数学的




