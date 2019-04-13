
# jQuery中extend和fn.extend - 追求卓越,做到专业 - CSDN博客


2012年10月16日 10:13:12[Waldenz](https://me.csdn.net/enter89)阅读数：922


jQuery为开发插件提供了两种方法，分别是：
jQuery.extend(object);    为扩展jQuery类本身,为jQuery类添加新的静态方法。
jQuery.fn.extend(object);  给jQuery对象添加方法。
jQuery.extend(object);　为jQuery类添加添加类方法，可以理解为添加静态方法。如：
jQuery.extend({
min: function(a, b) { return a < b ? a : b; },
max: function(a, b) { return a > b ? a : b; }
});
便为jQuery添加两个“静态方法”，之后便可以在引入 jQuery的地方，使用方法了：
jQuery.min(2,3); //  2
jQuery.max(4,5); // 5
jQuery.fn.extend(object); 对jQuery.prototype进得扩展，就是为jQuery类添加“成员函数”。
jQuery类的实例可以使用这个“成员函数”。
比如我们要开发一个插件，做一个特殊的编辑框，当它被点击时，便alert 当前编辑框里的内容。可以这么做：

```python
(function($) {
    $.fn.extend({
        alertWhileClick: function() {
            $(this).click(function() {
                alert($(this).val());
            });
        }
    });
})(jQuery)
$("#input1").alertWhileClick();
```


