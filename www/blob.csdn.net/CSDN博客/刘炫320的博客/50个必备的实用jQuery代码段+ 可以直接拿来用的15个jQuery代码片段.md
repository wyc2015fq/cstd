# 50个必备的实用jQuery代码段+ 可以直接拿来用的15个jQuery代码片段 - 刘炫320的博客 - CSDN博客
2017年06月18日 20:26:49[刘炫320](https://me.csdn.net/qq_35082030)阅读数：216标签：[jquery																[javascript																[绑定事件																[过滤器																[加载图片](https://so.csdn.net/so/search/s.do?q=加载图片&t=blog)](https://so.csdn.net/so/search/s.do?q=过滤器&t=blog)](https://so.csdn.net/so/search/s.do?q=绑定事件&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)
个人分类：[前端](https://blog.csdn.net/qq_35082030/article/category/6846971)
本文会给你们展示50个jquery代码片段，这些代码能够给你的javascript项目提供帮助。其中的一些代码段是从jQuery1.4.2才开始支持的做法，另一些则是真正有用的函数或方法，他们能够帮助你又快又好地把事情完成。如果你发现你任何可以做得更好的地方的话，欢迎把你的版本粘贴在评论中!
- 如何修改jQuery默认编码（例如默认UTF-8改成改GB2312）：
```
$.ajaxSetup({
ajaxSettings:{ contentType:"application/x-www-form-urlencoded;chartset=GB2312"} 
});
```
- 解决jQuery, prototype共存，$全局变量冲突问题：
```xml
<script src="prototype.js"></script>
<script src="http://blogbeta.blueidea.com/jquery.js"></script>
<script type="text/javascript">
      jQuery.noConflict();
</script>
```
注意：一定要先引入prototype.js 再引入jquery.js，先后顺序不可错。
- jQuery 判断元素上是否绑定了事件
```php
//jQuery event封装支持判断元素上是否绑定了事件，此方法只适用于jQuery绑定的事件
var $events</span> = $("#foo").data("events");
if( $events</span> && <span class="hljs-variable">$events["click"] ){
　　//your code}
```
- 如何使用jQuery来切换样式表
`//找出你希望切换的媒体类型（media-type），然后把href设置成新的样式表。$('link[media='screen']').attr('href', 'alternative.css');`- 如何限制选择范围（基于优化目的）：
```
//尽可能使用标签名来作为类名的前缀，
//这样jQuery就不需要花费更多的时间来搜索
//你想要的元素。还要记住的一点是，
//针对于你的页面上的元素的操作越具体化，
//就越能降低执行和搜索的时间。var in_stock = $('#shopping_cart_items input.is_in_stock');
<ul id="shopping_cart_items">
　　<li><input type="radio" value="Item-X" name="item" class="is_in_stock" /> Item X</li>
　　<li><input type="radio" value="Item-Y" name="item" class="3-5_days" /> Item Y</li>
　　<li><input type="radio" value="Item-Z" name="item" class="unknown" /> Item Z</li>
</ul>
```
- 如何正确地使用toggleClass：
```bash
//切换（toggle）类允许你根据某个类的
//是否存在来添加或是删除该类。
//这种情况下有些开发者使用：a.hasClass('blueButton') ? a.removeClass('blueButton') : a.addClass('blueButton');
//toggleClass允许你使用下面的语句来很容易地做到这一点a.toggleClass('blueButton');
```
- 如何设置IE特有的功能：
```
if ($.browser.msie) {
// Internet Explorer就是个虐待狂}
8. 如何使用jQuery来代替一个元素：
$('#thatdiv').replaceWith('fnuh');
```
- 如何验证某个元素是否为空：
```python
// 方法一if (! $('#keks').html()) {
//什么都没有找到;}
// 方法二if ($('#keks').is(":empty")) {
//什么都没有找到;}
```
- 如何从一个未排序的集合中找出某个元素的索引号
```java
$("ul > li").click(function () {
　　var index = $(this).prevAll().length; //prevAll([expr]): 查找当前元素之前所有的同辈元素});
```
- 如何把函数绑定到事件上：
```
//方法一$('#foo').click(function(event) { 
　　alert('User clicked on "foo."'); 
}); 
//方法二, 支持动态传参$('#foo').bind('click', {test1:"abc", test2:"123"}, function(event) { 
　　alert('User clicked on "foo."' + event.data.test1 + event.data.test2 ); 
});
```
- 如何追加或是添加html到元素中：
`$('#lal').append('sometext'); `- 在创建元素时，如何使用对象字面量（literal）来定义属性
```java
var e = $("", { href: "#", class: "a-class another-class", title: "..." });
```
- 如何使用多个属性来进行过滤
```
//在使用许多相类似的有着不同类型的input元素时， 
//这种基于精确度的方法很有用 var elements = $('#someid input[type=sometype][value=somevalue]').get();
```
- 如何使用jQuery来预加载图像：
```java
jQuery.preloadImages = function() { 
　　for(var i = 0; i < arguments.length; i++) { 
　　　　$("<img />").attr('src', arguments[i]); 
　　}
}; 
//用法 $.preloadImages('image1.gif', '/path/to/image2.png', 'some/image3.jpg');
```
- 如何为任何与选择器相匹配的元素设置事件处理程序：
```java
$('button.someClass').live('click', someFunction); 
//注意，在jQuery 1.4.2中，delegate和undelegate选项 
//被引入代替live，因为它们提供了更好的上下文支持 
//例如，就table来说，以前你会用$("table").each(function(){ 
　　$("td", this).live("hover", function(){ 
　　　　$(this).toggleClass("hover"); 
　　}); 
}); 
//现在用 $("table").delegate("td", "hover", function(){ 
　　$(this).toggleClass("hover"); 
});
```
- 如何找到一个已经被选中的option元素：
`$('#someElement').find('option:selected'); `- 如何隐藏一个包含了某个值文本的元素：
`$("p.value:contains('thetextvalue')").hide(); `- 如何创建嵌套的过滤器：
```
//允许你减少集合中的匹配元素的过滤器，
//只剩下那些与给定的选择器匹配的部分。在这种情况下，
//查询删除了任何没（:not）有（:has）
//包含class为“selected”（.selected）的子节点。.filter(":not(:has(.selected))")
```
- 如何检测各种浏览器：
```
检测Safari (if( $.browser.safari)),  
检测IE6及之后版本 (if ($.</span>browser.msie && <span class="hljs-variable">$.browser.version > 6 )),  
检测IE6及之前版本 (if ($.</span>browser.msie && <span class="hljs-variable">$.browser.version <= 6 )),  
检测FireFox 2及之后版本 (if ($.</span>browser.mozilla && <span class="hljs-variable">$.browser.version >= '1.8' ))
```
- 任何使用has()来检查某个元素是否包含某个类或是元素：
```
//jQuery 1.4.*包含了对这一has方法的支持。
//该方法找出某个元素是否包含了其他另一个元素类或是其他任何的你正在查找并要在其之上进行操作的东东。$("input").has(".email").addClass("email_icon");
```
- 如何禁用右键单击上下文菜单：
```java
$(document).bind('contextmenu',function(e){ 
　　return false; 
});
```
- 如何定义一个定制的选择器
```java
$.expr[':'].mycustomselector = function(element, index, meta, stack){ 
// element- 一个DOM元素 
// index – 栈中的当前循环索引 
// meta – 有关选择器的元数据 
// stack – 要循环的所有元素的栈 
// 如果包含了当前元素就返回true 
// 如果不包含当前元素就返回false }; 
// 定制选择器的用法： $('.someClasses:test').doSomething();
```
- 如何检查某个元素是否存在
```
if ($('#someDiv').length) { 
//万岁！！！它存在…… }
```
- 如何使用jQuery来检测右键和左键的鼠标单击两种情况：
```java
$("#someelement").live('click', function(e) { 
    if( (!$.browser.msie && e.button == <span class="hljs-number">0</span>) || ($.browser.msie && e.button == 1) ) { 
        alert("Left Mouse Button Clicked"); 
    } else if(e.button == 2) { 
        alert("Right Mouse Button Clicked"); 
    }
});
```
- 如何替换串中的词
```java
var el = $('#id'); 
el.html(el.html().replace(/word/ig, ''));
```
- 如何在一段时间之后自动隐藏或关闭元素（支持1.4版本）：
```
//这是1.3.2中我们使用setTimeout来实现的方式 setTimeout(function() { 
$('.mydiv').hide('blind', {}, 500) 
}, 5000); 
//而这是在1.4中可以使用delay()这一功能来实现的方式（这很像是休眠） $(".mydiv").delay(5000).hide('blind', {}, 500);
```
- 如何把已创建的元素动态地添加到DOM中：
```java
var newDiv = $('<div></div>'); 
newDiv.attr('id','myNewDiv').appendTo('body');
```
- 如何限制“Text-Area”域中的字符的个数：
```
jQuery.fn.maxLength = function(max){ 
    return this.each(function(){
        var type = this.tagName.toLowerCase(); 
        var inputType = this.type? this.type.toLowerCase() : null; 
        if(type == "input" && inputType == "text" || inputType == "password"){ 
            //Apply the standard maxLength             this.maxLength = max; 
        } else if(type == "textarea"){
            this.onkeypress = function(e){ 
                var ob = e || event; 
                var keyCode = ob.keyCode; 
                var hasSelection = document.selection? document.selection.createRange().text.length > 0 : this.selectionStart != this.selectionEnd; 
                return !(this.value.length >= max && (keyCode > 50 || keyCode == 32 || keyCode == 0 || keyCode == 13) && !ob.ctrlKey && !ob.altKey && !hasSelection); 
            }; 
            this.onkeyup = function(){ 
                if(this.value.length > max){ 
                    this.value = this.value.substring(0,max); 
                } 
            };
        }
    });
};
//用法 $('#mytextarea').maxLength(500);
```
- 如何jQuery注册和禁用jQuery全局事件
```
//jQuery注册ajax全局事件ajaxStart，ajaxStop：$(document).ajaxStart(function(){
    $("#background,#progressBar").show();
}).ajaxStop(function(){
    $("#background,#progressBar").hide();
});
//ajax请求禁用全局事件：$.ajax() 有个参数global (默认: true) 是否触发全局 AJAX 事件.设置为 false 将不会触发全局 AJAX 事件，如 ajaxStart 或 ajaxStop 可用于控制不同的 Ajax 事件。
```
- 如何在jQuery中克隆一个元素：
```java
var cloned = $('#somediv').clone();
```
- 在jQuery中如何测试某个元素是否可见
```
if($(element).is(':visible')) { 
　　//该元素是可见的 }
```
- 如何把一个元素放在屏幕的中心位置：
```java
jQuery.fn.center = function () { 
　　return this.each(function(){
　　　　$(this).css({
　　　　　　position:'absolute',
　　　　　　top, ( $(window).height() - <span class="hljs-keyword">this</span>.height() ) / <span class="hljs-number">2</span> + $(window).scrollTop() + 'px', 
　　　　　　left, ( $(window).width() - <span class="hljs-keyword">this</span>.width() ) / <span class="hljs-number">2</span> + $(window).scrollLeft() + 'px'
　　　　});
　　});
}
//这样来使用上面的函数：  $(element).center();
```
- 如何把有着某个特定名称的所有元素的值都放到一个数组中：
```java
var arrInputValues = new Array(); 
$("input[name='xxx']").each(function(){ 
　　arrInputValues.push($(this).val());
});
```
- 如何从元素中除去HTML
```java
(function($) { 
$.fn.stripHtml = function() { 
　　var regexp = /<("[^"]*"|'[^']*'|[^'">])*>/gi; 
　　this.each(function() { 
　　　　$(<span class="hljs-keyword">this</span>).html( $(this).html().replace(regexp,'') ); 
　　});
　　return $(this); 
} 
})(jQuery); 
//用法： $('p').stripHtml();
```
- 如何使用closest来取得父元素：
`$('#searchBox').closest('div'); `- 如何使用Firebug和Firefox来记录jQuery事件日志：
```
// 允许链式日志记录jQuery.log = jQuery.fn.log = function (msg) { 
　　if (console){ 
　　　　console.log("%s: %o", msg, this); 
　　}
　　return this; 
};
// 用法： $('#someDiv').hide().log('div hidden').addClass('someClass');
```
- 如何强制在弹出窗口中打开链接：
```java
$('a.popup').live('click', function(){ 
　　var newwindow = window.open($(this).attr('href'),'','height=200,width=150'); 
　　if (window.focus) { 
　　　　newwindow.focus(); 
　　} 
　　return false;
});
```
- 如何强制在新的选项卡中打开链接：
```java
$('a.newTab').live('click', function(){ 
　　var newwindow=window.open(this.href); 
　　$(this).target = "_blank"; 
　　return false; 
});
```
- 在jQuery中如何使用.siblings()来选择同辈元素
```
// 不这样做 $('#nav li').click(function(){ 
　　$('#nav li').removeClass('active'); 
　　$(this).addClass('active'); 
});
//替代做法是 $('#nav li').click(function(){ 
　　$(this).addClass('active').siblings().removeClass('active'); 
});
```
- 如何切换页面上的所有复选框：
```
var tog = false; 
// 或者为true，如果它们在加载时为被选中状态的话 $('a').click(function() { 
　　$("input[type=checkbox]").attr("checked",!tog); 
　　tog = !tog;
});
```
- 如何基于一些输入文本来过滤一个元素列表：
```java
//如果元素的值和输入的文本相匹配的话,该元素将被返回 $('.someClass').filter(function() { 
　　return $(<span class="hljs-keyword">this</span>).attr(<span class="hljs-string">'value'</span>) == $('input#someId').val(); 
})
```
- 如何获得鼠标垫光标位置x和y
```java
$(document).ready(function() { 
　　$(document).mousemove(function(e){ 
　　　　$(’#XY’).html(”X Axis : ” + e.pageX + ” | Y Axis ” + e.pageY); 
　　});
});
```
- 如何扩展String对象的方法
```java
$.extend(String.prototype, {
        isPositiveInteger:function(){
            return (new RegExp(/^[1-9]\d*$/).test(this));
        },
        isInteger:function(){
            return (new RegExp(/^\d+$/).test(this));
        },
        isNumber: function(value, element) {
            return (new RegExp(/^-?(?:\d+|\d{1,3}(?:,\d{3})+)(?:\.\d+)?$/).test(this));
        },
        trim:function(){
            return this.replace(/(^\s*)|(\s*$)|\r|\n/g, "");
        },
        trans:function() {
            return this.replace(/</g, '<').replace(/>/g,'>').replace(/"/g, '"');
        },
        replaceAll:function(os, ns) {
            return this.replace(new RegExp(os,"gm"),ns);
        },
        skipChar:function(ch) {
            if (!this || this.length===0) {return '';}
            if (this.charAt(0)===ch) {return this.substring(1).skipChar(ch);}
            return this;
        },
        isValidPwd:function() {
            return (new RegExp(/^([_]|[a-zA-Z0-9]){6,32}$/).test(this)); 
        },
        isValidMail:function(){
            return(new RegExp(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/).test(this.trim()));
        },
        isSpaces:function() {
            for(var i=0; i<this.length; i+=1) {
                var ch = this.charAt(i);
                if (ch!=' '&& ch!="\n" && ch!="\t" && ch!="\r") {return false;}
            }
            return true;
        },
        isPhone:function() {
            return (new RegExp(/(^([0-9]{3,4}[-])?\d{3,8}(-\d{1,6})?$)|(^\([0-9]{3,4}\)\d{3,8}(\(\d{1,6}\))?$)|(^\d{3,8}$)/).test(this));
        },
        isUrl:function(){
            return (new RegExp(/^[a-zA-z]+:\/\/([a-zA-Z0-9\-\.]+)([-\w .\/?%&=:]*)$/).test(this));
        },
        isExternalUrl:function(){
            return this.isUrl() && this.indexOf("://"+document.domain) == -1;
        }
    });
```
- 如何规范化写jQuery插件：
```
(function($){
    $.fn.extend({
        pluginOne: function(){
            return this.each(function(){
                // my code            });
        },
        pluginTwo: function(){
            return this.each(function(){
                // my code            });
        }
    });
})(jQuery);
```
- 如何检查图像是否已经被完全加载进来
```java
$('#theImage').attr('src', 'image.jpg').load(function() { 
　　alert('This Image Has Been Loaded'); 
});
```
- 如何使用jQuery来为事件指定命名空间：
```
//事件可以这样绑定命名空间 $('input').bind('blur.validation', function(e){ 
　　// ... }); 
//data方法也接受命名空间 $('input').data('validation.isValid', true);
```
- 如何检查cookie是否启用
```java
var dt = new Date(); 
dt.setSeconds(dt.getSeconds() + 60); 
document.cookie = "cookietest=1; expires=" + dt.toGMTString(); 
var cookiesEnabled = document.cookie.indexOf("cookietest=") != -1; 
if(!cookiesEnabled) { 
　　//没有启用cookie }
```
- 如何让cookie过期：
```
var date = new Date(); 
date.setTime(date.getTime() + (x * 60 * 1000)); 
$.cookie('example', 'foo', { expires: date });
```
- 如何使用一个可点击的链接来替换页面中任何的URL
```java
$.fn.replaceUrl = function() { 
　　var regexp = /((ftp|http|https):\/\/(\w+:{0,1}\w*@)?(\S+)(:[0-9]+)?(\/|\/([\w#!:.?+=&%@!\-\/]))?)/gi; 
　　return this.each(function() { 
　　　　$(this).html( 
　　　　　　$(<span class="hljs-keyword">this</span>).html().replace(regexp,<span class="hljs-string">'<a href="$1">$1</a>')
　　　　); 
　　});
} 
//用法　 
$('p').replaceUrl();
```
1.预加载图片
```java
function($) {
  var cache = [];
  // Arguments are image paths relative to the current page.
  $.preLoadImages = function() {
    var args_len = arguments.length;
    for (var i = args_len; i--;) {
      var cacheImage = document.createElement('img');
      cacheImage.src = arguments[i];
      cache.push(cacheImage);
    }
  }
jQuery.preLoadImages("image1.gif", "/path/to/image2.png");
```
- 让页面中的每个元素都适合在移动设备上展示
```java
var scr = document.createElement('script');
scr.setAttribute('src', 'https://ajax.googleapis.com/ajax/libs/jquery/1.5.2/jquery.min.js');
document.body.appendChild(scr);
scr.onload = function(){
    $('div').attr('class', '').attr('id', '').css({
        'margin' : 0,
        'padding' : 0,
        'width': '100%',
        'clear':'both'
    });
}
```
;
3.图像等比例缩放
```java
$(window).bind("load", function() {
    // IMAGE RESIZE
    $('#product_cat_list img').each(function() {
        var maxWidth = 120;
        var maxHeight = 120;
        var ratio = 0;
        var width = $(this).width();
        var height = $(this).height();
        if(width > maxWidth){
            ratio = maxWidth / width;
            $(this).css("width", maxWidth);
            $(this).css("height", height * ratio);
            height = height * ratio;
        }
        var width = $(this).width();
        var height = $(this).height();
        if(height > maxHeight){
            ratio = maxHeight / height;
            $(this).css("height", maxHeight);
            $(this).css("width", width * ratio);
            width = width * ratio;
        }
    });
    //$("#contentpage img").show();
    // IMAGE RESIZE
});
```
4.返回页面顶部
```java
// Back To Top
$(document).ready(function(){
  $('.top').click(function() { 
     $(document).scrollTo(0,500); 
  });
});
//Create a link defined with the class .top
<a href="#" class="top">Back To Top</a>
```
5.使用jQuery打造手风琴式的折叠效果
```php
var accordion = {
     init: function(){
           var $container</span> = $('#accordion');
           $container.find('li:not(:first) .details').hide();
           $container.find('li:first').addClass('active');
           $container.on('click','li a',function(e){
                  e.preventDefault();
                  var $this</span> = $(this).parents('li');
                  if($this.hasClass('active')){
                         if($('.details').is(':visible')) {
                                $this.find('.details').slideUp();
                         } else {
                                $this.find('.details').slideDown();
                         }
                  } else {
                         $container.find('li.active .details').slideUp();
                         $container.find('li').removeClass('active');
                         $this.addClass('active');
                         $this.find('.details').slideDown();
                  }
           });
     }
};
```
6.通过预加载图片廊中的上一幅下一幅图片来模仿Facebook的图片展示方式
```java
var nextimage = "/images/some-image.jpg";
$(document).ready(function(){
     window.setTimeout(function(){
         var img = $("").attr("src", nextimage).load(function(){
         //all done
         });
     }, 100);
});
```
7.使用jQuery和Ajax自动填充选择框
```java
$(function(){
    $("select#ctlJob").change(function(){
        $.getJSON("/select.php", {
            id: $(this).val(),
            ajax: 'true'
        }, function(j){
            var options = '';
            for (var i = 0; i < j.length; i++) {
                options += '' + j[i].optionDisplay + '';
            }
            $("select#ctlPerson").html(options);
        });
    });
});
```
8.自动替换丢失的图片
```java
// Safe Snippet
$("img").error(function () {
    $(this).unbind("error").attr("src", "missing_image.gif");
});
// Persistent Snipper
$("img").error(function () {
    $(this).attr("src", "missing_image.gif");
});
```
9.在鼠标悬停时显示淡入/淡出特效
```java
$(document).ready(function(){
    $(".thumbs img").fadeTo("slow", 0.6);// This sets the opacity of the thumbs to fade down to 60% when the page loads
    $(".thumbs img").hover(function(){
        $(this).fadeTo("slow", 1.0);// This should set the opacity to 100% on hover
    },function(){
        $(this).fadeTo("slow", 0.6);// This should set the opacity back to 60% on mouseout
    });
});
```
10.清空表单数据
```java
function clearForm(form) {
  // iterate over all of the inputs for the form
  // element that was passed in
  $(':input', form).each(function() {
    var type = this.type;
    var tag = this.tagName.toLowerCase();// normalize case
    // it's ok to reset the value attr of text inputs,
    // password inputs, and textareas
    if (type == 'text' || type == 'password' || tag == 'textarea')
      this.value = "";
    // checkboxes and radios need to have their checked state cleared
    // but should *not* have their 'value' changed
    else if (type == 'checkbox' || type == 'radio')
      this.checked = false;
    // select elements need to have their 'selectedIndex' property set to -1
    // (this works for both single and multiple select elements)
    else if (tag == 'select')
      this.selectedIndex = -1;
  });
};
```
11.预防对表单进行多次提交
```java
$(document).ready(function() {
  $('form').submit(function() {
    if(typeof jQuery.data(this, "disabledOnSubmit") == 'undefined') {
      jQuery.data(this, "disabledOnSubmit", { submited: true });
      $('input[type=submit], input[type=button]', this).each(function() {
        $(this).attr("disabled", "disabled");
      });
      return true;
    }
    else
    {
      return false;
    }
  });
});
```
12.动态添加表单元素
```java
//change event on password1 field to prompt new input
$('#password1').change(function() {
        //dynamically create new input and insert after password1
        $("#password1").append("");
});
```
13.让整个Div可点击
```java
//blah blah blah. link
//The following lines of jQuery will make the entire div clickable: 
$(".myBox").click(function(){
    window.location = $(this).find("a").attr("href");
    return false;
});
```
14.平衡高度或Div元素
```java
var maxHeight = 0;
$("div").each(function(){
   if ($(<span class="hljs-keyword">this</span>).height() > maxHeight) { maxHeight = $(this).height(); }
});
$("div").height(maxHeight);
```
- 在窗口滚动时自动加载内容
```java
var loading = false;
$(window).scroll(function(){
    if((($(window).scrollTop()+$(window).height())+250)>=$(document).height()){
        if(loading == false){
            loading = true;
            $('#loadingbar').css("display","block");
            $.get(<span class="hljs-string">"load.php?start="</span>+$('#loaded_max').val(), function(loaded){
                $('body').append(loaded);
                $(<span class="hljs-string">'#loaded_max'</span>).val(<span class="hljs-built_in">parseInt</span>($('#loaded_max').val())+50);
                $('#loadingbar').css("display","none");
                loading = false;
            });
        }
    }
});
$(document).ready(function() {
    $('#loaded_max').val(50);
});
```
[原文链接](http://blog.csdn.net/u014083580/article/details/73395554)
