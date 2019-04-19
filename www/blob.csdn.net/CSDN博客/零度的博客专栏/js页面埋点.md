# js页面埋点 - 零度的博客专栏 - CSDN博客
2017年02月28日 15:07:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：3848
页面埋点的作用，其实就是用于流量分析。而流量的意思，包含了很多：页面浏览数(PV)、独立访问者数量(UV)、IP、页面停留时间、页面操作时间、页面访问次数、按钮点击次数、文件下载次数等。而流量分析又有什么用处：
1、提高网站的转化率
         根据页面埋点可得到一些重要信息，它告诉你用户对网站的反应，以及如何提高网站流量、改进网站性能，了解用户访问网站的行为，为更好地满足用户需求提供支持。 
2、反应用户黏度
         即使网站吸引了很多用户访问，但是通过流量分析发现，用户停留的时间非常短，重复访问用户不多，用户平均浏览的页面也少，这样的网站用户黏度不够，有流量但是没有忠实的用户，一旦有其它可替代网站，用户随时可能流失。这样的网站，如果不采取有效的运营措施，很难有长期发展的后劲。
3、为网站内容管理和网站的产品策划提供方向
         通过流量分析，可以挖掘出整个网站哪个频道最有人气，频道之间的流量比例是多少，每个页面的流量是多少，哪个页面最受欢迎，每个页面中具体的哪个栏目点击 量最高，这样通过对频道、栏目、页面的具体流量分析和对比，可以挖掘出用户的需求，发现用户最关心什么内容，这对评估网站频道、内容、页面的价值有重要的 参考作用，也对网站内容下一步的优化有直接的参考意义。
埋点脚本：
借助新浪IP地址库，显示本地城市名称：
**[js]**[view plain](http://blog.csdn.net/z69183787/article/details/53406245#)[copy](http://blog.csdn.net/z69183787/article/details/53406245#)[print](http://blog.csdn.net/z69183787/article/details/53406245#)[?](http://blog.csdn.net/z69183787/article/details/53406245#)
- <script src="http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=js" type="text/ecmascript"></script>    
- remote_ip_info.country  
- remote_ip_info.province  
- remote_ip_info.city  
![](http://static.blog.csdn.net/images/save_snippets.png)
<script src="http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=js" type="text/ecmascript"></script>  
remote_ip_info.country
remote_ip_info.province
remote_ip_info.city
获取客户端IP：
**[js]**[view plain](http://blog.csdn.net/z69183787/article/details/53406245#)[copy](http://blog.csdn.net/z69183787/article/details/53406245#)[print](http://blog.csdn.net/z69183787/article/details/53406245#)[?](http://blog.csdn.net/z69183787/article/details/53406245#)
- <script type="text/javascript" src="http://www.coding123.net/getip.ashx?js=1"></script>  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
<script type="text/javascript" src="http://www.coding123.net/getip.ashx?js=1"></script>
获取用户的访问开始时间、访问结束时间、以及用户与网站的交互时间（当用户在网页无操作时间超过30秒，理解为离开网页），其中无操作包括：鼠标点击、移动、滑轮无操作，键盘无按键操作。
**[js]**[view plain](http://blog.csdn.net/z69183787/article/details/53406245#)[copy](http://blog.csdn.net/z69183787/article/details/53406245#)[print](http://blog.csdn.net/z69183787/article/details/53406245#)[?](http://blog.csdn.net/z69183787/article/details/53406245#)
- var start = new Date();  
- var strStart = start.getFullYear()+"-"+(start.getMonth()+1)+"-"+start.getDate()+" "+  
-                 start.getHours()+":"+start.getMinutes()+":"+start.getSeconds();  
- var len = 0;  
- var end;  
- var status = "in";  
- var second = 30;  
- function revive(){  
- if(status == "out"){  
-         start = new Date();  
-         status = "in";  
-     }  
-     second = 30;  
- }  
- window.setInterval(function(){  
-     second -= 1;  
- if(0 == second){  
-         end = new Date();  
-         len += (end.getTime() - start.getTime())/1000;  
-         status = "out";  
-     }  
- },1000);  
- $(‘body‘).click(function(){  
-     revive();  
- });  
- $(‘body‘).mousedown(function(){  
-     revive();  
- });  
- $(‘body‘).mouseup(function(){  
-     revive();  
- });  
- $(‘body‘).mousemove(function(){  
-     revive();  
- });  
- //(Firefox)
- $(‘body‘).bind(‘DOMMouseScroll‘, function() {  
-     revive();  
- });  
- //(IE,Google)
- $(‘body‘).bind(‘mousewheel‘,function(){  
-     revive();  
- });  
- $(‘body‘).keydown(function(e){  
-     revive();  
- });   
- $(‘body‘).keyup(function(e){  
-     revive();  
- });   
- $(‘body‘).keypress(function(e){  
-     revive();  
- });   
- window.onbeforeunload = function(){  
-    end = new Date();  
- var strEnd = end.getFullYear()+"-"+(end.getMonth()+1)+"-"+end.getDate()+" "+  
-                 end.getHours()+":"+end.getMinutes()+":"+end.getSeconds();  
-    len += (end.getTime() - start.getTime())/1000;   
- var img = new Image();  
-    img.src = contextPath + "behavior?stayTime=" + len + "&strStart" + strStart + "&lastDate=" + strEnd;  
- };  
![](http://static.blog.csdn.net/images/save_snippets.png)
var start = new Date();
var strStart = start.getFullYear()+"-"+(start.getMonth()+1)+"-"+start.getDate()+" "+
                start.getHours()+":"+start.getMinutes()+":"+start.getSeconds();
var len = 0;
var end;
var status = "in";
var second = 30;
function revive(){
    if(status == "out"){
        start = new Date();
        status = "in";
    }
    second = 30;
}
window.setInterval(function(){
    second -= 1;
    if(0 == second){
        end = new Date();
        len += (end.getTime() - start.getTime())/1000;
        status = "out";
    }
},1000);
$(‘body‘).click(function(){
    revive();
});
$(‘body‘).mousedown(function(){
    revive();
});
$(‘body‘).mouseup(function(){
    revive();
});
$(‘body‘).mousemove(function(){
    revive();
});
//(Firefox)
$(‘body‘).bind(‘DOMMouseScroll‘, function() {
    revive();
});
//(IE,Google)
$(‘body‘).bind(‘mousewheel‘,function(){
    revive();
});
$(‘body‘).keydown(function(e){
    revive();
}); 
$(‘body‘).keyup(function(e){
    revive();
}); 
$(‘body‘).keypress(function(e){
    revive();
}); 
window.onbeforeunload = function(){
   end = new Date();
   var strEnd = end.getFullYear()+"-"+(end.getMonth()+1)+"-"+end.getDate()+" "+
                end.getHours()+":"+end.getMinutes()+":"+end.getSeconds();
   len += (end.getTime() - start.getTime())/1000; 
   var img = new Image();
   img.src = contextPath + "behavior?stayTime=" + len + "&strStart" + strStart + "&lastDate=" + strEnd;
};
获取点击按钮：
**[js]**[view plain](http://blog.csdn.net/z69183787/article/details/53406245#)[copy](http://blog.csdn.net/z69183787/article/details/53406245#)[print](http://blog.csdn.net/z69183787/article/details/53406245#)[?](http://blog.csdn.net/z69183787/article/details/53406245#)
- <a onclick="return getid(this.id)"> 按钮</a>  
- function getid(id) {  
- var img = new Image();  
-     img.src = contextPath + "button?id=" + id;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
<a onclick="return getid(this.id)"> 按钮</a>
function getid(id) {
    var img = new Image();
    img.src = contextPath + "button?id=" + id;
}
获取搜索来源：
判断从哪个搜索工具找到你的网站，document.referrer(上级URL)，这个可以获取到你上级页面，如果看到上级URL里面包含baidu，就可以粗略的判断是从百度访问的页面。
流量统计系统一般包括以下情况分析：
1、按在线情况分析
　　在线情况分析分别记录在线用户的活动信息，包括：来访时间、访客地域、来路页面、当前停留页面等，这些功能对企业实时掌握自身网站流量有很大的帮助。
2、按时段分析
　　时段分析提供网站任意时间内的流量变化情况.或者某一段时间到某一段时间的流量变化，比如小时段分布，日访问量分布，对于企业了解用户浏览网页的的时间段有一个很好的分析。
3、按来源分析
         来源分析提供来路域名带来的来访次数、IP、独立访客、新访客、新访客浏览次数、站内总浏览次数等数据。这个数据可以直接让企业了解推广成效的来路，从而分析出那些网站投放的广告效果更明显。
　　以上这些网站分析技巧，为网络营销人员从不同方面分析网站运营的效果提供了方便，不过，这些基本统计信息本身所能反映的问题并不全面，有些隐性问题可能并非反映出来。对一些问题的深度分析，则需要考虑更多的因素，也可能涉及到更多的专业分析手段。
