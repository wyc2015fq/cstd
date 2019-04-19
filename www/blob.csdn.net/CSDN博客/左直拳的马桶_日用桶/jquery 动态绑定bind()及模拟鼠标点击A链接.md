# jquery 动态绑定bind()及模拟鼠标点击A链接 - 左直拳的马桶_日用桶 - CSDN博客
2016年06月03日 12:03:56[左直拳](https://me.csdn.net/leftfist)阅读数：4314
近来自觉前端有小小进步，幸而记之。
1、两个 css class 紧挨在一起 
则在html元素中，要同时拥有这两个class，才能起作用
```
.block.db{
    background-image:url(/cas/images/hnhy/db.png);
}
```
`<div class="block db"><div class="btn btn_bg" ></div></div>`
2、动态绑定事件 
动态绑定，可以节省代码。设想有一张列表，每行都有一个鼠标移入移出的事件处理，假如事先写好，代码得多出不少。用JS脚本动态绑定，则非常简洁：
```java
$(function(){
        $(".block div.btn").bind("mouseover",function(){
            $(this).removeClass("btn_bg").addClass("btn_bg_on");
        });
        $(".block div.btn").bind("mouseout",function(){
            $(this).removeClass("btn_bg_on").addClass("btn_bg");
        });
    });
```
这里有个问题，就是事件处理中，如何知道哪个元素触发了事件？如果是html中预先写好，当然可以将“this”参数传进去。动态绑定呢？酱紫：$(this)。
另外，这里说的是jquery 的 bind()，而不是标准 javascript的bind()。后者与apply,call一样，改变this指针，唯一不同的是，bind()不是立即执行，而是返回一个函数。详见拙作：[Javascript中的apply、call、bind](http://blog.csdn.net/leftfist/article/details/51136264)
3、代码模拟鼠标点击 a 链接 
假设有
`<a id="a" href=""  target="_blank" style="display:none;">jump</a>`
则下面语句不起作用。是点击了a没错，可以触发其click事件，但并没有跳转：
```java
$("#a").click();
```
原因是，我们在网页中点击链接，其实点击的是链接中的文字或图片什么的，而不是链接本身。因此，应该酱紫写：
```java
$("#a")[0].click();
```
4、两个符合条件的选择器：
```java
$("#firstmenu li:first,#firstmenu li:last").bind('mouseout', function () {
            if ($(this).attr("attr1") == '0') {
                $(this).removeClass("hy_mainmenu_active");
            }
});
```
