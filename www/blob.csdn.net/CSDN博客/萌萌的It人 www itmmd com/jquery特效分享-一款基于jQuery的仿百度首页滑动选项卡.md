
# jquery特效分享-一款基于jQuery的仿百度首页滑动选项卡 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月21日 13:09:12[Jlins](https://me.csdn.net/dyllove98)阅读数：2453


今天给大家分享一款基于jQuery的仿百度首页滑动选项卡。这款选项卡适用浏览器：IE8、360、FireFox、Chrome、Safari、Opera、傲游、搜狗、世界之窗。效果图如下：
![](http://pic.w2bc.com/upload/201501/19/201501192259213579.jpg)

实现的代码。
html代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<divclass="main-page"><divclass="left"><divclass="nav-back"></div><divclass="nav"><divclass="on">导航</div><div>新闻</div><div>世界杯</div><div>音乐</div><div>彩票</div></div></div><divclass="right"><divclass="content-back"></div><divclass="content"><div>站长素材1</div><div>站长素材2</div><div>站长素材3</div><div>站长素材4</div><div>站长素材5</div></div></div><divclass="clear"></div></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
css代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
body{background:url(images/65.jpg) no-repeat fixed center center;
        }.clear{clear:both;
        }.main-page{margin:200px auto 0 auto;width:700px;height:300px;
        }.main-page .left, .main-page .right{float:left;
        }.main-page .nav-back{width:60px;height:300px;background:\#000;opacity:.3;filter:alpha(opacity=30);
        }.main-page .nav{position:relative;margin-top:-300px;width:60px;text-align:center;font-size:14px;font-family:"微软雅黑";color:\#fff;
        }.main-page .nav div{height:32px;line-height:28px;
        }.main-page .nav div.on{background:\#0094ea;
        }.main-page .right{width:620px;height:300px;margin-left:20px;
        }.main-page .content-back{width:620px;height:300px;background:\#fff;opacity:.3;
        }.main-page .content{position:relative;width:600px;height:280px;margin-top:-300px;padding:10px;overflow:hidden;
        }.main-page .content div{width:600px;height:280px;margin-bottom:10px;background:\#fff;
        }![复制代码](http://common.cnblogs.com/images/copycode.gif)
js代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
$(".main-page .nav div").mouseenter(function() {var$this= $(this);varindex = $this.index();
        }).mouseleave(function() {var$this= $(this);varindex = $this.index();
        }).click(function() {var$this= $(this);varindex = $this.index();varl = -(index * 290);
            $(".main-page .nav div").removeClass("on");
            $(".main-page .nav div").eq(index).addClass("on");
            $(".main-page .content div:eq(0)").stop().animate({ "margin-top": l }, 500);
        });![复制代码](http://common.cnblogs.com/images/copycode.gif)


**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


