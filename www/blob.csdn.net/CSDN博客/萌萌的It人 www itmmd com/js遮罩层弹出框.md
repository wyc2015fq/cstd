
# js遮罩层弹出框 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月15日 13:40:15[Jlins](https://me.csdn.net/dyllove98)阅读数：2591



昨天公司网站需要弹窗提示一些信息,要我在把弹窗的js代码和弹窗窗口html写在一起哪里需要就调用
不说那么多了,直接上代码,感觉肯定会有兼容问题,看到了一定指出呀
```python
<style>
    #H-dialog{display:none;position:absolute;z-index: 9999999;width:400px;height: auto; background-color: #fff;}
    #H-dialog .close{float:right;font-size: 30px;margin-right: 10px;margin-top:5px;cursor:pointer;}
    #H-dialog .title{height: 40px;padding-left:10px; font-size:20px; line-height:40px;}
    #H-dialog #msgCont{height:36px; margin: 30px 0 50px;padding-left: 65px;font-size: 25px;line-height: 36px;vertical-align: middle; background: url(../Images/ui_alert.png) no-repeat 20px 50%;}
</style>
<div id="H-dialog">
    <a class="close" onclick="popupClose(this)">×</a>
    <div class="title">提示</div>
    <div id="msgCont">内容</div>
</div>
<script type="text/javascript">
    //锁定背景屏幕
    function lockScreen() {
        var clientH = document.body.offsetHeight; //body高度
        var clientW = document.body.offsetWidth; //body宽度
        var docH = document.body.scrollHeight; //浏览器高度
        var docW = document.body.scrollWidth; //浏览器宽度
        var bgW = clientW > docW ? clientW : docW; //取有效宽
        var bgH = clientH > docH ? clientH : docH; //取有效高
        var blackBg = document.createElement("div");
        blackBg.id = "blackBg";
        blackBg.style.position = "absolute";
        blackBg.style.zIndex = "99999";
        blackBg.style.top = "0";
        blackBg.style.left = "0";
        blackBg.style.width = bgW+"px";
        blackBg.style.height = bgH+"px";
        blackBg.style.opacity = "0.4";
        blackBg.style.backgroundColor = "#333";
        document.body.appendChild(blackBg);
    }
    //关闭按钮事件
    function popupClose(el) {
        var blackBg = document.getElementById("blackBg");
        blackBg && document.body.removeChild(blackBg);
        el.parentNode.style.display = "none";
    }
    //自动关闭
    function autoClose(id) {
        id = id || "H-dialog";
        var blackBg = document.getElementById("blackBg");
        var objDiv = document.getElementById(id);
        setTimeout(function(){
            blackBg && document.body.removeChild(blackBg);
            objDiv.style.display = "none";
        },2000);
    }
    /**
    *功能 : 弹窗信息
    *参数1 : 提示信息内容
    *参数2 : 提示信息状态默认0 为提示信息,1为成功信息
    *参数3 : 弹窗div的id,默认"H-dialog"
    *参数4 : 弹窗内容的id,默认"msgCont"
    **/
    function showMsg(msg) {
        msg = msg || "请重新操作";
        var status = arguments[1] || 0,
        popupId = arguments[2] || "H-dialog",
        contentId = arguments[3] || "msgCont";       
        lockScreen();
        //屏幕实际高宽
        var pageWidth = window.innerWidth;
        var pageHeight = window.innerHeight;
        if (typeof pageWidth != "number") {
            if (document.compatMode == "CSS1Compat") {
                pageWidth = document.documentElement.clientWidth;
                pageHeight = document.documentElement.clientHeight;
            } else {
                pageWidth = document.body.clientWidth;
                pageHeight = document.body.clientHeight;
            }
        }
        //滚动条高宽
        var scrollLeft = window.document.documentElement.scrollLeft;
        var scrollTop = 0;
        if (typeof window.pageYOffset != 'undefined') {
            scrollTop = window.pageYOffset;
        } else if (typeof window.document.compatMode != 'undefined' &&
            window.document.compatMode != 'BackCompat') {
            scrollTop = window.document.documentElement.scrollTop;
        } else if (typeof window.document.body != 'undefined') {
            scrollTop = window.document.body.scrollTop;
        }
        var div_X = (pageWidth - 400) / 2 + scrollLeft;
        var div_Y = (pageHeight - 200) / 2 + scrollTop;
        var objDiv = document.getElementById(popupId);
        if (status) {
            document.getElementById(contentId).style.background = "url($Root/Assets/Images/ui_success.png) no-repeat 20px 50%";
        }
        document.getElementById(contentId).innerHTML = msg;
        objDiv.style.display = "block";
        objDiv.style.left = div_X + "px";
        objDiv.style.top = div_Y + "px";
        autoClose(popupId);
    }
</script>
```


**其他精彩文章**
## [jQuery教程(19)-jquery ajax操作之序列化表单](http://www.itmmd.com/201501/497.html)
## [jQuery教程(18)-ajax操作之执行POST请求](http://www.itmmd.com/201501/496.html)
## [jQuery教程(17)-ajax操作之向服务器传递数据 执行GET请求](http://www.itmmd.com/201501/493.html)
## [jQuery教程(16)-ajax操作之加载XML文档](http://www.itmmd.com/201501/492.html)
## [jQuery教程(15)-ajax操作之执行脚本](http://www.itmmd.com/201501/491.html)

## [jQuery教程(14)-ajax操作之操作 JavaScript 对象](http://www.itmmd.com/201501/490.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


