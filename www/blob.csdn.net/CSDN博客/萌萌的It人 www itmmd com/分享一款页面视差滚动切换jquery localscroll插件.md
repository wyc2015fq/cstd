
# 分享一款页面视差滚动切换jquery.localscroll插件 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月19日 11:36:38[Jlins](https://me.csdn.net/dyllove98)阅读数：2026


今天给大家分享一款页面视差滚动切换jquery.localscroll插件。 滚动鼠标液动条看下页面的切换效果。该插件适用浏览器：IE8、360、FireFox、Chrome、Safari、Opera、傲游、搜狗、世界之窗。效果图如下:
![](http://pic.w2bc.com/upload/201501/17/201501170934281471.jpg)
[在线预览](http://www.w2bc.com/demo/2015-01-17-jquery-page/index.html)[源码下载](http://www.w2bc.com/article/17400)
实现的代码。
html代码：

```python
<div class="pageWidth" id="box_root">
        <!-- 头部导航开始 -->
        <div id="nav">
            <a href="index.html" title="返回引导页" class="logo_a">站长素材</a>
            <ul id="menu">
                <li class="" id="mn_home"><a href="index.html#home">网站首页</a> </li>
                <li id="mn_about" class="selected"><a href="index.html#about">站长素材</a> </li>
                <li id="mn_news"><a href="index.html#news">新闻资讯</a> </li>
                <li id="mn_products" class=""><a href="index.html#products">核心项目</a> </li>
                <li id="mn_chanye" class=""><a href="index.html#sector">产业板块</a> </li>
                <li id="mn_about" class=""><a href="index.html#famous">名家寄语</a> </li>
                <li class="last" id="mn_contact"><a href="index.html#contact">联系我们</a> </li>
            </ul>
        </div>
        <div class="top_rim">
            <a href="index.html#home">回到顶部</a>
        </div>
        <!-- 头部导航结束 -->
        <div id="box_main">
            <div id="home" style="background-position: 50% -110px;">
                <div class="sprite1" style="background-position: 50% 550px;">
                </div>
                <div class="sprite" style="background-position: 50% 0px;">
                </div>
                <div class="screen">
                </div>
            </div>
            <div id="about" style="background-position: 50% -110px;">
                <div class="contents">
                    <div class="about_rim">
                        <div class="columnSpace" id="elem-FrontSpecifies_show01-1367889271850" name="说明页">
                            <!-- 装饰器样式开始 -->
                            <div class="border_05">
                                <div class="border_05-topr">
                                    <div class="border_05-topl">
                                    </div>
                                </div>
                                <div class="border_05-mid">
                                    <div class="borderContent">
                                        <div id="FrontSpecifies_show01-1367889271850" class="FrontSpecifies_show01-d3_c1">
                                            <div class="comptitle_05">
                                                <em><span class="extendLink"><a href="index.html#about_cont" target="_self">更多详情 +</a></span></em></div>
                                            <div class="describe htmledit">
                                                <div class="about_txt">
                                                </div>
                                            </div>
                                        </div>
                                        <div class="clearBoth">
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <!-- 装饰器样式结束 -->
                        </div>
                    </div>
                </div>
                <div class="sprite" style="background-position: 50% 0px;">
                </div>
                <div class="screen" style="background-position: 50% 550px;">
                </div>
            </div>
            <div id="about_cont" style="background-position: 50% 0px;">
                <div class="contents">
                    <div class="about_rim">
                        <p>
                            关于我们</p>
                    </div>
                </div>
                <div class="sprite" style="background-position: 50% 0px;">
                </div>
                <div class="screen" style="background-position: 50% 550px;">
                </div>
                <div class="screen1" style="background-position: 50% 110px;">
                </div>
            </div>
            <div id="news" style="background-position: 50% -110px;">
                <div class="contents">
                    <div id="box_news">
                        <p>
                            新闻资讯</p>
                    </div>
                </div>
            </div>
            <div id="products" style="background-position: 50% -110px;">
                <div class="pro_list">
                    <p>
                        核心项目</p>
                </div>
                <div class="sprite" style="background-position: 50% 0px;">
                </div>
                <div class="screen" style="background-position: 50% 220px;">
                </div>
            </div>
            <div id="sector" style="background-position: 50% -110px;">
                <div class="contents">
                    <div class="sector_img">
                        <p>
                            产业模块</p>
                    </div>
                </div>
                <div class="screen" style="background-position: 50% 550px;">
                </div>
                <div class="sprite" style="background-position: 50% 0px;">
                </div>
            </div>
            <div id="famous" style="background-position: 50% -90px;">
                <div class="contents">
                    <div class="famous_rim">
                        <p>
                            名家寄语</p>
                    </div>
                </div>
                <div class="sprite" style="background-position: 50% 450px;">
                </div>
            </div>
            <div id="contact">
                <div class="contents">
                    <div id="box_contact">
                        <div class="columnSpace" id="elem-FrontSpecifies_show01-1368432548167" name="说明页">
                            <div id="FrontSpecifies_show01-1368432548167" class="FrontSpecifies_show01-d3_c1">
                                <div class="describe htmledit">
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="sprite" style="background-position: 50% 0px;">
                </div>
                <div class="screen">
                </div>
            </div>
        </div>
    </div>
```
**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


