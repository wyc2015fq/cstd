# 解决SwfUpload在IE10上不出现上传按钮 - z69183787的专栏 - CSDN博客
2015年06月30日 11:32:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1239
       在系统测试过程中，发现使用了SwfUpload实现的无刷新上传功能，在IE10上竟然无法使用了，难道SwfUpload不支持吗?还是需要换一种实现方式呢？最后通过了解SwfUplad.JS文件发现，我们是可以修改的，让其支持IE10,具体解决方案如下：
        打开SwfUpload.js，在js文件中找到// Private: getFlashHTML generates the object tag needed to embed the flash in to the document”这行和“// Private: getFlashVars builds the parameter string that will be passed”和这行，然后把中间的用如下代码替换就OK了
**[javascript]**[view
 plain](http://blog.csdn.net/huangxiangec/article/details/9167971#)[copy](http://blog.csdn.net/huangxiangec/article/details/9167971#)[print](http://blog.csdn.net/huangxiangec/article/details/9167971#)[?](http://blog.csdn.net/huangxiangec/article/details/9167971#)
- SWFUpload.prototype.getFlashHTML = function (flashVersion) {  
- // Flash Satay object syntax: http://www.alistapart.com/articles/flashsatay
- var classid = "";  
- var Sys = {};  
- var ua = navigator.userAgent.toLowerCase();  
- if (window.ActiveXObject) {  
-     Sys.ie = ua.match(/msie ([\d.]+)/)[1];  
- if (Sys.ie && Sys.ie.substring(0, 1) == "9" || Sys.ie.substring(0, 2) == "10") {  
-         classid = ' classid = "clsid:d27cdb6e-ae6d-11cf-96b8-444553540000"';  
-     }  
- }  
- return ['<object', classid, ' id="', this.movieName, '" type="application/x-shockwave-flash" data="', (this.support.imageResize ? this.settings.flash_url : this.settings.flash9_url), '" width="', this.settings.button_width, '" height="', this.settings.button_height, '" class="swfupload">',  
- '<param name="wmode" value="', this.settings.button_window_mode, '" />',  
- '<param name="movie" value="', (this.support.imageResize ? this.settings.flash_url : this.settings.flash9_url), '" />',  
- '<param name="quality" value="high" />',  
- '<param name="allowScriptAccess" value="always" />',  
- '<param name="flashvars" value="' + this.getFlashVars() + '" />',  
- '</object>'].join("");  
- };  
         保存，运行就可以看到效果了，完美！
