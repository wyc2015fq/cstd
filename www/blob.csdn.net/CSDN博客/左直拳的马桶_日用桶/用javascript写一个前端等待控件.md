# 用javascript写一个前端等待控件 - 左直拳的马桶_日用桶 - CSDN博客
2015年10月26日 19:35:41[左直拳](https://me.csdn.net/leftfist)阅读数：2589
前端等待控件有啥新奇的？什么jquery啦，第三方控件啦，好多好多，信手拈来。
因为项目使用了bootstrap的原因，不想轻易使用第三方，怕不兼容。自己写一个。
技术点包括动态加载CSS，javascript的命名空间，所以记录一下。
这个等待控件主要是：进行某个操作前，显示一个信息提示：“数据加载中，请稍候。。。”,操作成功后，在回调函数中将提示消失。
原理是这个等待控件完全由JS动态加进去，包括CSS，页面中并无预先设定。
那么这个CSS怎么动态加载呢？等待控件中，样式使用了class，如果我们将这些class预先写在样式文件中，那么调用页面除了要引用相关JS文件，还要引用CSS文件；就算在js文件中动态加载此css文件，但想想看，一个如此简单的控件就包含了2个文件，小题大做了点。
我是在JS中动态拼凑、加载CSS。
代码如下：
```java
var FTabPages = function () {
    var tabKeeper = null;
    // e.g.
    // tabKeeper = {
    //     container: ""
    //     , isErase: true
    //     , url: ""
    //     , params: {}
    //     , callback: null
    // };
    var wrap = $(document.body);
    function initTab(tabJson) {
        tabKeeper = tabJson;
    }
    function onTab(tabJson) {//切换页签
        if (tabKeeper != null) {
            var divPrev = $(tabKeeper.container);
            if (tabKeeper.isErase) {
                divPrev.empty();
            }
            divPrev.css("display", "none");
        }
        tabKeeper = tabJson;
        var div = $(tabJson.container);
        div.css("display", "");
        if ($.trim(div.html()).length == 0) {//首次加载或已清空
            loadwaiting();
            getViewRequest(tabJson.url, tabJson.params, function (data) {
                div.empty().html(data);
                docallback(tabJson.callback);
                removeloading();
            }, function (data) {
                alert("数据获取超时或失败！");
                removeloading();
            });
        } else {//非首次加载，隐藏但不清空
            docallback(tabJson.callback);
        }
    }
    function getViewRequest(url, params, onsuccess, onerror) {
        $.ajax({
            type: 'get',
            url: url,
            data: params,
            contentType: "text/html; charset=utf-8",
            timeout:30000,
            success: function (data) {
                if (onsuccess != undefined && onsuccess != null) {
                    onsuccess(data);
                }
            },
            error: function (data) {
                if (onerror != undefined && onerror != null) {
                    onerror(data);
                }
            }
        });
    }
    function docallback(callback) {
        if (typeof callback != 'undefined' && callback instanceof Function) {
            callback();
        }
    }
    function resetTab() {//刷新当前页签
        loadwaiting();
        var div = $(tabKeeper.container);
        getViewRequest(tabKeeper.url, tabKeeper.params, function (data) {
            div.empty().html(data);
            div.css("display", "");
            docallback(tabKeeper.callback);
            removeloading();
        });
    }
    function loadwaiting() {//显示等待信息  
        $("<div class=\"datagrid-mask\"></div>").css({ display: "block", width: wrap.width(), height: wrap.height() }).appendTo(wrap);
        $("<div class=\"datagrid-mask-msg\"></div>").html("数据加载中，请稍候...").appendTo(wrap).css({ display: "block", left: (wrap.width() - $("div.datagrid-mask-msg", wrap).outerWidth()) / 2, top: ($(window).height() - $("div.datagrid-mask-msg", wrap).outerHeight()) / 2 });
    }
    function removeloading() {//隐藏等待信息  
        wrap.find("div.datagrid-mask-msg").remove();
        wrap.find("div.datagrid-mask").remove();
    }
    function initloading() {//设置等待控件样式
        var css = ".datagrid-mask {             ";
        css += "    position: absolute;         ";
        css += "    left: 0;                    ";
        css += "    top: 0;                     ";
        css += "    width: 100%;                ";
        css += "    height: 100%;               ";
        css += "    opacity: 0.3;               ";
        css += "    filter: alpha(opacity=30);  ";
        css += "    display: none;              ";
        css += "}                               ";
        css += ".datagrid-mask-msg {            ";
        css += "    position: absolute;         ";
        css += "    top: 50%;                   ";
        css += "    margin-top: -20px;          ";
        css += "    padding: 10px 5px 10px 10px;";
        css += "    width: auto;                ";
        css += "    height: 16px;               ";
        css += "    border-width: 2px;          ";
        css += "    border-style: solid;        ";
        css += "    display: none;              ";
        css += "}";
        //动态加载CSS
        if (document.all) {
            window.style = css;
            document.createStyleSheet("javascript:style");
        } else {
            var style = document.createElement('style');
            style.type = 'text/css';
            style.innerHTML = css;
            document.getElementsByTagName('HEAD').item(0).appendChild(style);
        }
    }
    initloading();
    return {//这里是供外部调用的方法
        onTab: function (tabJson) {
            onTab(tabJson);
        }
        , resetTab: function () {
            resetTab();
        }
        , init: function (tabJson) {
            initTab(tabJson);
        }
    };
}();
```
外部如何调用呢？就这样：
```java
FTabPages.init({
    container: "#div_BasicInfo"
    , isErase: true
    , url: "http://blog.csdn.net/leftfist"
    , params: {}
    , callback: function () {
         alert("Hello World!");
    }    
});
```
