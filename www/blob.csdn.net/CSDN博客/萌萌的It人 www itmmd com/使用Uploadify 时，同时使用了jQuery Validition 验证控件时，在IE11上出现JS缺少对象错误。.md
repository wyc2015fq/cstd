
# 使用Uploadify 时，同时使用了jQuery.Validition 验证控件时，在IE11上出现JS缺少对象错误。 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月19日 15:46:02[Jlins](https://me.csdn.net/dyllove98)阅读数：4431




场景：
使用jQuery.1.8.2
使用 Uploadify 3.2上传控件
使用jQuery.Validition 1.9 验证

使用IE 11 时，当鼠标点击上传按钮时，会出现JS 缺少对象错误。如下图：
![](http://images.cnitblog.com/blog/641607/201501/190936270941519.jpg)
错误定位在jQuery中.
![](http://images.cnitblog.com/blog/641607/201501/190938471258164.jpg)

排查后发现是引用了jQuery Validition 验证控件导致的。
在jQuery Validition控件初始化中，有下面一段代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
$currentSection
                .validateDelegate(":text, [type='password'], [type='file'], select, textarea, " +
                    "[type='number'], [type='search'] ,[type='tel'], [type='url'], " +
                    "[type='email'], [type='datetime'], [type='date'], [type='month'], " +
                    "[type='week'], [type='time'], [type='datetime-local'], " +
                    "[type='range'], [type='color'] ","focusin focusout keyup", delegate)
                .validateDelegate("[type='radio'], [type='checkbox'], select, option", "click", delegate);![复制代码](http://common.cnblogs.com/images/copycode.gif)

你只要将这段代码注释掉，错误就会消失。
查看一下validateDelegate这个方法：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|$.fn.validateDelegate=|function|(delegate,
 type, handler) {
|//
 验证委托
|return|this|.on(type,|function|(event)
 {
|var|target
 = $(event.target);
|if|(target.is(delegate))
 {
|return|handler.apply(target,
 arguments);
|}
|});
|};
|


里面只是对元素绑定一下事件，没有什么特殊代码。
尝试在Html中移除了jQuery.validitioin插件后，在当前页面写入以下代码：
|1
|2
|3
|$(|"form"|).on(|"focusin
 focusout keyup"|,|function|(){
|//
 TODO:
|});
|


这段代码也会导致上面说的问题。
因为是在jQuery上抛出的错误，定位到jQuery中：
|1
|2
|3
|4
|5
|6
|acceptData:|function|(
 elem ) {
|var|noData
 = elem.nodeName && jQuery.noData[ elem.nodeName.toLowerCase() ];
|//
 nodes accept data unless otherwise specified; rejection can be conditional
|return|!noData
 || noData !==|true|&&
 elem.getAttribute(|"classid"|)
 === noData;
|}
|


注重调试到elem.getAttribute这个方法，在其它的元素上都没用问题，但是在获取object元素上，在IE11 这个上，getAttribute方法不存在为null，所以就会报出之前上述代码。
将代码加一个判断即可:
|1
|2
|3
|4
|5
|6
|acceptData:|function|(
 elem ) {
|var|noData
 = elem.nodeName && jQuery.noData[ elem.nodeName.toLowerCase() ];
|//
 nodes accept data unless otherwise specified; rejection can be conditional
|return|!noData
 || noData !==|true|&&
 <span style=|"color:
 \#ff0000;"|>elem.getAttribute
 !=|null|</span>
 && elem.getAttribute(|"classid"|)
 === noData;
|}
|


我尝试使用jQuery 1.9和1.12都依然存在这个问题。所以只将1.8.2的代码更改了，其它还有一处碰到object这个元素都会有这个问题。
更改后的jquery 1.8.2 下载:
[jquery-1.8.2-FIX_Object.getAttribute_is_Null Download](http://files.cnblogs.com/xakoy/jquery-1.8.2-FIX_Object.getAttribute_is_Null.rar)

**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章



