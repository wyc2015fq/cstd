
# 基于jQuery标题有打字效果的焦点图 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月20日 13:05:08[Jlins](https://me.csdn.net/dyllove98)阅读数：1475标签：[javascript																](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)[jquery																](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)[jquery插件																](https://so.csdn.net/so/search/s.do?q=jquery插件&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)个人分类：[jquery																](https://blog.csdn.net/dyllove98/article/category/1349854)
[
																								](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)


给大家分享一款基于jQuery标题有打字效果的焦点图。之前为大家分享了好多jquery的焦点图。这款焦点图适用浏览器：IE8、360、FireFox、Chrome、Safari、Opera、傲游、搜狗、世界之窗。
![](http://pic.w2bc.com/upload/201501/17/201501171849436305.jpg)
[在线预览](http://www.w2bc.com/demo/2015-01-17-jquery-focus/index.html)[源码下载](http://www.w2bc.com/article/17457)
实现的代码。
html代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!--代码 开始--><divid="header"><divclass="wrap"><divid="slide-holder"><divid="slide-runner"><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-1"src="images/a1.jpg"class="slide"alt=""/></a><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-2"src="images/a2.jpg"class="slide"alt=""/></a><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-3"src="images/a3.jpg"class="slide"alt=""/></a><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-4"src="images/a4.jpg"class="slide"alt=""/></a><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-5"src="images/a5.jpg"class="slide"alt=""/></a><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-6"src="images/a6.jpg"class="slide"alt=""/></a><ahref="http://www.w2bc.com/"target="_blank"><imgid="slide-img-7"src="images/a4.jpg"class="slide"alt=""/></a><divid="slide-controls"><pid="slide-client"class="text"><strong></strong><span></span></p><pid="slide-desc"class="text"></p><pid="slide-nav"></p></div></div></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
js代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
if(!window.slider) {varslider ={};
                }
                slider.data=[
    {"id": "slide-img-1",//与slide-runner中的img标签id对应"client": "标题1","desc": "这里修改描述 这里修改描述 这里修改描述"//这里修改描述},
    {"id": "slide-img-2","client": "标题2","desc": "add your description here"},
    {"id": "slide-img-3","client": "标题3","desc": "add your description here"},
    {"id": "slide-img-4","client": "标题4","desc": "add your description here"},
    {"id": "slide-img-5","client": "标题5","desc": "add your description here"},
    {"id": "slide-img-6","client": "标题6","desc": "add your description here"},
    {"id": "slide-img-7","client": "标题7","desc": "add your description here"}
    ];![复制代码](http://common.cnblogs.com/images/copycode.gif)


**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


