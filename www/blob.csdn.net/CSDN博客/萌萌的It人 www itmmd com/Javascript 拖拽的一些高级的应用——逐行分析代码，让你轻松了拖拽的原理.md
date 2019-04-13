
# Javascript 拖拽的一些高级的应用——逐行分析代码，让你轻松了拖拽的原理 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月16日 13:19:36[Jlins](https://me.csdn.net/dyllove98)阅读数：1108


我们看看之前的拖拽在周围有东西的时候会出现什么问题？ 在高级浏览器中不会有啥问题，我们放到IE7下面测试一下，问题就出来了。如图
![image](http://images.cnitblog.com/blog/587107/201501/161053040897721.png)![image](http://images.cnitblog.com/blog/587107/201501/161053064484109.png)
我们可以很清楚的看到，文字都已经被选中了。那这个用户体验很不好，用起来也不方便。顺便提一下，我们之前加了一个return false;帮我们解决了很多问题，如果去掉这个的话,chrome也会出现一样的问题。那么也就是说这个return false;可以解决chrome ff IE9+ 这些浏览器的问题。
实际上在我们开发中，页面上会有许多的元素组成，不可能就一个div，在你拖动的时候其他地方是不会被选中的，比如说百度地图，大家可以玩玩。
那我们如何做到这样子的一个拖拽呢？能够解决IE7的问题呢？
解决方案：
> 我们可以用一个小技巧来解决，这个技巧只有在IE6-8支持，实际上就能解决我们的问题，因为其他的浏览器用return false; 就够了。下面看看是什么技巧
就是事件捕获！！ 简单说明下 附上代码
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<title></title><scripttype="text/javascript">window.onload=function(){varoBtn=document.getElementById("btn");
                oBtn.onclick=function(){
                    alert(1);
                };//网页上所有地方的上的事件都集中到一个按钮身上 IE 专用oBtn.setCapture();//点击哪里都是弹a}</script></head><body><inputtype="button"id="btn"value="按钮"/></body>![复制代码](http://common.cnblogs.com/images/copycode.gif)
实际上就是页面上所有地方上的事件都集中到一点，点击页面任何位置都会弹出a，也就是setCapture()作用。
把所有事件都集中到一个按钮来处理！！ 这个就只有IE兼容！！
这样，让我来看看如何修改之前的代码。。。。
我们首先把所有的document改回div，记不记得我们之前讲过因为鼠标拖动的快点就容易拖出div，所以把事件都加在document上。
而现在就不必这样做了，给我们之前的div加上一个setCapture()看看效果。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<body>IE 7 中的文字会被选中 ，<br/>如果不加return false chrome ff 也会有这样的问题 asdsadad<br/><divid="div1">asdsadad asdsadad asdsadad</div>asdsadadasdsadadasdsadad</body>![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<style type="text/css">
            \#div1{width:200px;height:200px;background:red;position:absolute;
            }</style>![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script type="text/javascript">//拖拽空div 低版本的火狐有bugwindow.onload =function() {varoDiv = document.getElementById("div1");vardisX = 0;vardisY = 0;
        oDiv.onmousedown =function(ev) {varoEvent = ev || event;
          disX = oEvent.clientX - oDiv.offsetLeft;
          disY = oEvent.clientY - oDiv.offsetTop;
          oDiv.onmousemove =function(ev) {varoEvent = ev || event;varoDivLeft = oEvent.clientX - disX;varoDivTop = oEvent.clientY - disY;
            oDiv.style.left = oDivLeft + 'px';
            oDiv.style.top = oDivTop + 'px';
          };
          oDiv.onmouseup =function() {
            oDiv.onmousemove =null;
            oDiv.onmouseup =null;
          };
          oDiv.setCapture();returnfalse;//阻止默认事件,解决火狐的bug};
      };
    </script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
这个时候实际上我们拖动在快也不会出现鼠标拖出Div的问题了。 实际上加了setCapture()以后，整个网页上所有的事件都会聚集在这一个div上面。
其实现在，这个文字就不会被选中了。为什么呢？ 因为现在网页上的文字，图片的所有事件都在div上了，他们已经得不到事件了！所以自然他们就不会被选中了。
当然现在又有个问题？？？？你会发现当你要试图去选中那些文字的时候，就选不中了。![image](http://images.cnitblog.com/blog/587107/201501/161053081208653.png)
如何是好，事件都集中在div上了。。。!!!!!
所以呢，其实这个setCapture()就像一把锁，现在都锁住了，事件都在div上面，现在解锁就可以了 那相对应的就有 releaseCapture();
releaseCapture(); 就是释放捕获。 实际上在鼠标抬起的时候加上就可以了。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
window.onload =function() {varoDiv = document.getElementById("div1");vardisX = 0;vardisY = 0;
        oDiv.onmousedown =function(ev) {varoEvent = ev || event;
          disX = oEvent.clientX - oDiv.offsetLeft;
          disY = oEvent.clientY - oDiv.offsetTop;
          oDiv.onmousemove =function(ev) {varoEvent = ev || event;varoDivLeft = oEvent.clientX - disX;varoDivTop = oEvent.clientY - disY;
            oDiv.style.left = oDivLeft + 'px';
            oDiv.style.top = oDivTop + 'px';
          };
          oDiv.onmouseup =function() {
            oDiv.onmousemove =null;
            oDiv.onmouseup =null;
            oDiv.releaseCapture();
          };
          oDiv.setCapture();returnfalse;//阻止默认事件,解决火狐的bug};
      };![复制代码](http://common.cnblogs.com/images/copycode.gif)

现在就能解决文字选中的问题了。 最后我们坐下兼容，实际上来说这个setCapture() 是不兼容的，放在其他浏览器就错了。
那么很简单， 我们只要合并这次与上一次的代码就可以了，兼容嘛 做个if判断就好。最后附上整理好的代码
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script type="text/javascript">
            window.onload =function() {varoDiv = document.getElementById("div1");vardisX = 0;vardisY = 0;
                oDiv.onmousedown =function(ev) {varoEvent = ev || event;
                    disX = oEvent.clientX - oDiv.offsetLeft;
                    disY = oEvent.clientY - oDiv.offsetTop;if(oDiv.setCapture) {
                        oDiv.onmousemove = mouseMove;
                        oDiv.onmouseup = mouseUp;
                        oDiv.setCapture();//IE 7 下文字就不会被选中 其实就是文字或图片得不到事件}else{
                        document.onmousemove = mouseMove;
                        document.onmouseup = mouseUp;
                    }functionmouseMove(ev) {varoEvent = ev || event;varoDivLeft = oEvent.clientX - disX;varoDivTop = oEvent.clientY - disY;
                        oDiv.style.left = oDivLeft + 'px';
                        oDiv.style.top = oDivTop + 'px';
                    }functionmouseUp(ev) {this.onmousemove =null;this.onmouseup =null;if(oDiv.releaseCapture) {
                            oDiv.releaseCapture();//释放捕获}
                    }returnfalse;//阻止默认事件,解决火狐的bug};
            };
        </script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
好了，都搞定了 O(∩_∩)O哈哈~
**其他精彩文章**
## [jQuery教程(19)-jquery ajax操作之序列化表单](http://www.itmmd.com/201501/497.html)
## [jQuery教程(18)-ajax操作之执行POST请求](http://www.itmmd.com/201501/496.html)
## [jQuery教程(20)-jquery ajax + php 操作之为Ajax请求提供不同...](http://www.itmmd.com/201501/499.html)
## [jQuery教程(21)-jquery ajax 回调函数](http://www.itmmd.com/201501/500.html)
## [jQuery教程(22)-ajax操作之错误处理](http://www.itmmd.com/201501/503.html)

## [jQuery教程(24)-ajax操作之Ajax和事件](http://www.itmmd.com/201501/504.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


