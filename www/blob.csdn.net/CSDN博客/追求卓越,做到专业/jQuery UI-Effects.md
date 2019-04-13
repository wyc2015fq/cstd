
# jQuery UI - Effects - 追求卓越,做到专业 - CSDN博客


2012年10月29日 14:25:37[Waldenz](https://me.csdn.net/enter89)阅读数：731个人分类：[jqGrid																](https://blog.csdn.net/enter89/article/category/1309802)


JQuery UI特有的动画效果库，允许元素在改变样式或外观或状态时，使用动画效果。
官方示例地址：http://jqueryui.com/demos/effect/
·effect( effect, [options], [speed], [callback] )
支持的样式有：
'blind'//从下至上收起来，直到隐藏
'bounce'//上下晃动元素
'clip'//上下同时收起来，直到隐藏
'drop'//向左边移动并升高透明度，直到隐藏
'explode'//将元素拆分成9宫，向外扩展并升高表明度，直到隐藏
'fold'//向上收起，再向左收起，直到隐藏
'highlight'//高亮某个元素
'puff'//扩大元素宽高度并升高透明度，直到隐藏
'pulsate'//闪烁元素
'scale'//从右下向左上收起，直到隐藏
'shake'//左右晃动元素
'size'//将元素缩至最小
'slide'//从左往右滑动元素，直到全部显示
'transfer'//缩小并迁移元素至触发事件的HTML元素
·addClass(class, [duration] )
扩展了JQuery的addClass方法，当往元素增加一个css样式的时候，允许实现动画效果，包括文本缩进、字母间隔、宽度、高度、内外补丁、字体大小。
·removeClass( [class], [duration] )
扩展了JQuery的removeClass方法，当往元素移除一个css样式的时候，允许实现动画效果，包括文本缩进、字母间隔、宽度、高度、内外补丁、字体大小。
·animate( style, [duration] )
扩展了JQuery的animate方法，当元素执行动画时，允许使用颜色进行过渡，支持的样式有：
backgroundColor、borderBottomColor、borderLeftColor、borderRightColor、borderTopColor、color、outlineColor
支持的值类型：hex(\#FF0000)、rgb(rgb(255,255,255))、names("black")
示例：
$(".block").toggle(function() {
$(this).animate({ backgroundColor:"black"}, 1000);
},function() {
$(this).animate({ backgroundColor:"\#68BFEF"}, 500);
});
·hide( effect, [options], [speed], [callback] )
扩展了JQuery的hide方法，当隐藏一个元素时，允许执行动画效果。
·show( effect, [options], [speed], [callback] )
扩展了JQuery的show方法，当显示一个元素时，允许执行动画效果。
·switchClass( remove, add, [duration] )
扩展了JQuery的switchClass方法，当切换调用两个函数时，允许执行动画效果。
·toggle( effect, [options], [speed], [callback] )
扩展了JQuery的toggle方法，在显示和隐藏元素时，允许执行动画效果。
·toggleClass(class, [duration] )
扩展了JQuery的toggleClass方法，当启用和禁用样式时，允许执行动画效果。

