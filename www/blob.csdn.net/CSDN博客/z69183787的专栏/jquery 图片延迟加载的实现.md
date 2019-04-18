# jquery 图片延迟加载的实现 - z69183787的专栏 - CSDN博客
2014年07月30日 20:28:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1258
最近c2c电子商务已经进入优化阶段 ，我负责前台大多数的功能的实现 ，其实商品列表页，由于会显示很多的商品 ，要是一次性都显示出来，会造成服务器很大的压力，此时我们可以考虑当用户滑动滚动条的时候 ，图片显示出现在屏幕范围之内的时候在加载进来，这样就可以减少服务器一次性过多请求带来的压力了
此时我们用到jquery.js相信大家都知道的
还有一个延迟加载的jquery插件  jquery.scrollLoading.js
下面像大家介绍一下它的用法 ，很简单
下面是jquery.scrollLoading.js的源码：
**[javascript]**[view
 plain](http://blog.csdn.net/ajun_studio/article/details/6728358#)[copy](http://blog.csdn.net/ajun_studio/article/details/6728358#)[print](http://blog.csdn.net/ajun_studio/article/details/6728358#)[?](http://blog.csdn.net/ajun_studio/article/details/6728358#)
- /*
-  * jquery.scrollLoading.js
-  * by zhangxinxu  http://www.zhangxinxu.com
-  * 2010-11-19 v1.0
- */
- (function ($) {  
-     $.fn.scrollLoading = function (options) {  
- var defaults = {  
-             attr: "data-url"
-         };  
- var params = $.extend({}, defaults, options || {});  
-         params.cache = [];  
-         $(this).each(function () {  
- var node = this.nodeName.toLowerCase(), url = $(this).attr(params["attr"]);  
- if (!url) { return; }  
- //重组
- var data = {  
-                 obj: $(this),  
-                 tag: node,  
-                 url: url  
-             };  
-             params.cache.push(data);  
-         });  
- 
- //动态显示数据
- var loading = function () {  
- var st = $(window).scrollTop(), sth = st + $(window).height();  
-             $.each(params.cache, function (i, data) {  
- var o = data.obj, tag = data.tag, url = data.url;  
- if (o) {  
-                     post = o.position().top;  
- if (post < 10)  
-                         post = o.offset().top;  
-                     posb = post + o.height();  
- 
- if ((post > st && post < sth) || (posb > st && posb < sth)) {  
- //在浏览器窗口内
- 
- if (tag === "img") {  
- //图片，改变src
-                             o.attr("src", url);  
-                         } elseif (tag == "iframe") {  
-                             o.attr("src", url);  
-                         } elseif (tag == "span") {                            
-                             eval(url);  
-                         }  
- else {  
-                             o.load(url);  
-                         }  
-                         data.obj = null;  
-                     }  
-                 }  
-             });  
- returnfalse;  
-         };  
- 
- //事件触发
- //加载完毕即执行
-         loading();  
- //滚动执行
-         $(window).bind("scroll", loading);  
-     };  
- })(jQuery);  
然后再你页面引用
**[javascript]**[view
 plain](http://blog.csdn.net/ajun_studio/article/details/6728358#)[copy](http://blog.csdn.net/ajun_studio/article/details/6728358#)[print](http://blog.csdn.net/ajun_studio/article/details/6728358#)[?](http://blog.csdn.net/ajun_studio/article/details/6728358#)
- <script type="text/javascript" src="jquery.js"></script>  
- 
- <script type="text/javascript" src="jquery.scrollLoading.js"></script>  
在加载图片的地方可以这样
**[html]**[view
 plain](http://blog.csdn.net/ajun_studio/article/details/6728358#)[copy](http://blog.csdn.net/ajun_studio/article/details/6728358#)[print](http://blog.csdn.net/ajun_studio/article/details/6728358#)[?](http://blog.csdn.net/ajun_studio/article/details/6728358#)
- <imgdata-url="http://xxx.com/898.jpg.167_167.jpg"class="lm"align="absmiddle"width="167"height="167"/>
然后再加一段js就可以了
**[javascript]**[view
 plain](http://blog.csdn.net/ajun_studio/article/details/6728358#)[copy](http://blog.csdn.net/ajun_studio/article/details/6728358#)[print](http://blog.csdn.net/ajun_studio/article/details/6728358#)[?](http://blog.csdn.net/ajun_studio/article/details/6728358#)
- $(function () {  
-     $(".lm").scrollLoading();  
- });  
这样就搞定 屏幕中出现这个图片的时候，才会进行加载，是不是很方便，
同事你也可以一次返回一个列表 ，比如你想一行几个图片的在屏幕的时候 ，这样你可以ajax放回一个list
**[html]**[view
 plain](http://blog.csdn.net/ajun_studio/article/details/6728358#)[copy](http://blog.csdn.net/ajun_studio/article/details/6728358#)[print](http://blog.csdn.net/ajun_studio/article/details/6728358#)[?](http://blog.csdn.net/ajun_studio/article/details/6728358#)
- <divclass="searchdiv"data-url="http://xxx.com/xx/imgList.action">
- <imgsrc="http://xxxx.com/ui-anim_basic_16x16.gif"
- alt=""/></div>
- js：  
-    $(function () {  
-                         $(".searchdiv").scrollLoading();  
-                     });  
上面代码解释：
页面加载完成前DIV显示loading图标，当该DIV的一部分在浏览器的可视范围内时，开始使用ajax加载data-url属性对应的HTML内容,正确返回后填充到该DIV里
赶快试试吧 很简单的。
现在淘宝或者京东这样的电子商务网站 ，都是这样类似实现的。
