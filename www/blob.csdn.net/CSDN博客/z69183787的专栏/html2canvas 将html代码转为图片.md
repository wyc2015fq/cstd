# html2canvas 将html代码转为图片 - z69183787的专栏 - CSDN博客
2017年08月02日 17:46:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4700

转换代码到图片使用html2canvas，这是一个非常著名的从浏览器网页截图的开源库，使用很方便，功能也很强大。
使用 html2canvas
        html2canvas 的使用非常简单，简单到只需要传入一个 DOM 元素，然后通过回调拿到 canvas：
![](https://img-blog.csdn.net/20161107223952229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在实际使用的时候，有两个注意点：
        1.html2canvas 通过解析元素实际的样式来生成 canvas 图片内容，因此它对元素实际的布局和视觉显示有要求。如果要完整截图，最好将元素从文档流中独立出来（例如 position:absolute）
        2.默认生成的 canvas 图片在 retina 设备上显示很模糊，处理成 2 倍图能解决这个问题：
**[javascript]**[view
 plain](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[copy](http://blog.csdn.net/jiangzunshao/article/details/53040908#)
- var w = $("#code").width();  
- var h = $("#code").height();//要将 canvas 的宽高设置成容器宽高的 2 倍
- var canvas = document.createElement("canvas");  
-     canvas.width = w * 2;  
-     canvas.height = h * 2;  
-     canvas.style.width = w + "px";  
-     canvas.style.height = h + "px";  
- var context = canvas.getContext("2d");//然后将画布缩放，将图像放大两倍画到画布上
-     context.scale(2,2);  
-     html2canvas(document.querySelector("#code"), {          canvas: canvas,          onrendered: function(canvas) {                ...          }    });  
使用 html2canvas的实际案例
1.html代码结构
**[html]**[view
 plain](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[copy](http://blog.csdn.net/jiangzunshao/article/details/53040908#)
- <sectionclass="share_popup"id="html2canvas">
- <p>html2canvas 的使用非常简单，简单到只需要传入一个 DOM 元素，然后通过回调拿到 canvas</p>
- <p><imgsrc="1.jpg"></p>
- <p>html2canvas 的使用非常简单，简单到只需要传入一个 DOM 元素，然后通过回调拿到 canvas</p>
- </section>
2.[js](http://lib.csdn.net/base/javascript)代码结构
**[javascript]**[view
 plain](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[copy](http://blog.csdn.net/jiangzunshao/article/details/53040908#)
- var str = $('#html2canvas');  
- //console.log(str);
- html2canvas([str.get(0)], {  
-     onrendered: function (canvas) {  
- var image = canvas.toDataURL("image/png");  
- var pHtml = "<img src="+image+" />";  
-         $('#html2canvas').html(pHtml);  
-     }  
- });  
[](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[](http://blog.csdn.net/jiangzunshao/article/details/53040908#)[](http://blog.csdn.net/jiangzunshao/article/details/53040908#)
