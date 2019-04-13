
# jquery学习笔记-jQuery实现单击和鼠标感应事件 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月21日 13:07:14[Jlins](https://me.csdn.net/dyllove98)阅读数：1261


对于单击事件而言，jQuery同样提供了动态交替的toggle()方法，这个方法接受两个参数，两个参数均为监听函数，在click事件中交替使用。
例子：点击事件的动态交互。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("\#ddd").toggle(function(oEvent) {
                        $(oEvent.target).css("opacity","0.5");
                    },function(oEvent) {
                        $(oEvent.target).css("opacity","1.0");
                    }
                );
            });</script><divid="ddd">11</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
2.实现鼠标感应
在css中可以利用:hover伪类实现样式的改版，实现单独的css样式，在jQuery引入后，几乎所有的元素都可以用hover()来感应鼠标。并且可以制作更复杂的效果。其本质是mouseover和mouseout事件的合并
hover(over,out)方法可接受两个参数，均为函数。第一个是鼠标移动到元素上面触发，第二个是鼠标移出元素时触发。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("\#ddd").hover(function(oEvent) {//第一个函数相当于mouseover事件监听$(oEvent.target).css("opacity","0.5");
                    },function(oEvent) {//第二个函数相当于mouseover事件监听$(oEvent.target).css("opacity","1.0");
                    }
                );
            });</script><divid="ddd">11</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
与第一个例子相比，只是把toggle()换成了hover().

---
受[wenzi0_0](http://www.cnblogs.com/xumengxuan/)指导，写几个关于toggle()的小例子
1.常规的应用
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("\#ddd").click(function(){
                $("\#eee").toggle();    
                });
            });</script><divid="ddd">11</div><divid="eee">122</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
2.css属性
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function(){
                $("\#eee").toggle(function() {
                        $("\#ddd").css("background-color","green");
                    },function() {
                        $("\#ddd").css("background-color","red");
                    },function() {
                        $("\#ddd").css("background-color","yellow");
                    }
                );
            });</script><divid="ddd">11</div><divid="eee">122</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)

写博客不容易，欢迎大家给评论以给鼓励，分享是快乐！欢迎大家拍砖和点赞。（JavaScript、ajax、jQuery系列文章不断更新，关注我即可随时关注更新）
**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)




