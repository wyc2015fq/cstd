
# javascript浏览器窗口之间传递数据 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月15日 13:36:15[Jlins](https://me.csdn.net/dyllove98)阅读数：1347



# 摘要：
在项目开发中我们经常会遇到弹窗，有的是通过div模拟弹窗效果，有的是通过iframe，也有通过window自带的open函数打开一个新的窗口。今天给大家分享的是最后一种通过window.open()函数打开页面进行数据交互。首先看下效果图：
![](http://images.cnitblog.com/blog/582503/201501/151310597457361.png)
# 原理：
父窗口给子窗口传递数据是通过url的参数传递过去，子窗口给父窗口传递数据是通过父窗口的全局函数传递。
# 代码：
index.html
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><htmllang="en"><head><metacharset="UTF-8"><title>Document</title></head><body><divid="content"></div><buttonid="test">按钮</button><script>vartest=document.getElementById('test');
        test.onclick=function() {
            window.open('./window.html?param1=name&param2=password','_blank','width=960,height=650,menubar=no,toolbar=no,location=no,directories=no,status=no,scrollbars=yes,resizable=yes');
        };
        window.getContent=function(tx) {
            document.getElementById('content').innerText=tx;
        }</script></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

window.html
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><htmllang="en"><head><metacharset="UTF-8"><title>Document</title></head><body><divid="content"></div><selectname=""id="city"><optionvalue="shanghai">上海</option><optionvalue="hangzhou">杭州</option></select><script>varparams=location.href.substring(location.href.lastIndexOf('?')+1).split('&');
        document.getElementById('content').innerText=params;varcity=document.getElementById('city');
        city.onchange=function() {
            window.opener.getContent(city.value);
        }</script></body></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
**注意：**要有服务环境
**其他精彩文章**
## [android学习笔记（41）android选项菜单和子菜单（SubMenu )](http://www.itmmd.com/201501/469.html)
## [android学习笔记（40）Notification的功能与用法](http://www.itmmd.com/201501/464.html)
## [android学习笔记（42）android使用监听器来监听菜单事件](http://www.itmmd.com/201501/475.html)
## [android学习笔记（43）android创建单选菜单和复选菜单](http://www.itmmd.com/201501/476.html)
## android学习笔记（44）android设置与菜单项关联的Activity

## [android学习笔记（45）android上下文菜单](http://www.itmmd.com/201501/485.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章

