# jquery获取文档高度和窗口高度的例子 - weixin_33985507的博客 - CSDN博客
2014年02月17日 10:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

[jquery](http://www.111cn.net/list-139/)获取文档高度和窗口高度，$(document).height()、$(window).height()
$(document).height()：整个网页的文档高度
$(window).height()：浏览器可视窗口的高度
$(window).scrollTop()：浏览器可视窗口顶端距离网页顶端的高度（垂直偏移）
$(document.body).height();//浏览器当前窗口文档body的高度
$(document.body).outerHeight(true);//浏览器当前窗口文档body的总高度 包括border padding margin
$(window).width(); //浏览器当前窗口可视区域宽度
$(document).width();//浏览器当前窗口文档对象宽度
$(document.body).width();//浏览器当前窗口文档body的高度
$(document.body).outerWidth(true);//浏览器当前窗口文档body的总宽度 包括border padding margin
用一句话理解就是：当网页滚动条拉到最低端时，$(document).height() == $(window).height() + $(window).scrollTop()。
当网页高度不足浏览器窗口时$(document).height()返回的是$(window).height()。
不建议使用$("html").height()、$("body").height()这样的高度。
原因：
$("body").height()：body可能会有边框，获取的高度会比$(document).height()小；
$("html").height()：在不同的浏览器上获取的高度的意义会有差异，说白了就是浏览器不兼容。
$(window).height()值有问题，返回的不是浏览器窗口的高度？
原因：网页没有加上<!DOCTYPE>声明。
懒人建站整理js获取页面高度和窗口高度
实际应用：设置内容区域合适的高度
|代码如下|复制代码|
|----|----|
|//设置内容区域合适高度    var docH = $(document).height(),        winH = $(window).height(),        headerH = $(".header").outerHeight();        footerH = $(".footer").outerHeight();    if(docH<=winH+4){        $("div.container").height(winH-headerH-footerH-50);    }| |
注：winH+4 因为IE8下只有4像素偏差
例
|代码如下|复制代码|
|----|----|
|// 获取页面的高度、宽度function getPageSize() {    var xScroll, yScroll;    if (window.innerHeight && window.scrollMaxY) {        xScroll = window.innerWidth + window.scrollMaxX;        yScroll = window.innerHeight + window.scrollMaxY;    } else {        if (document.body.scrollHeight > document.body.offsetHeight) { // all but Explorer Mac               xScroll = document.body.scrollWidth;            yScroll = document.body.scrollHeight;        } else { // Explorer Mac...would also work in Explorer 6 Strict, Mozilla and Safari               xScroll = document.body.offsetWidth;            yScroll = document.body.offsetHeight;        }    }    var windowWidth, windowHeight;//www.111cn.net    if (self.innerHeight) { // all except Explorer           if (document.documentElement.clientWidth) {            windowWidth = document.documentElement.clientWidth;        } else {            windowWidth = self.innerWidth;        }        windowHeight = self.innerHeight;    } else {        if (document.documentElement && document.documentElement.clientHeight) { // Explorer 6 Strict Mode               windowWidth = document.documentElement.clientWidth;            windowHeight = document.documentElement.clientHeight;        } else {            if (document.body) { // other Explorers                   windowWidth = document.body.clientWidth;                windowHeight = document.body.clientHeight;            }        }    }          // for small pages with total height less then height of the viewport       if (yScroll < windowHeight) {        pageHeight = windowHeight;    } else {        pageHeight = yScroll;    }       // for small pages with total width less then width of the viewport       if (xScroll < windowWidth) {        pageWidth = xScroll;    } else {        pageWidth = windowWidth;    }    arrayPageSize = new Array(pageWidth, pageHeight, windowWidth, windowHeight);    return arrayPageSize;}// 滚动条document.body.scrollTop;$(document).scrollTop();| |
你可能感兴趣的文章
- [js/jquery获取浏览器窗口的可视区域高度和宽度,滚动条高度](http://www.111cn.net/wy/js-ajax/44709.htm)
- [获取浏览器窗口高度宽度大小js/jquery代码](http://www.111cn.net/wy/js-ajax/42075.htm)
