# jQuery Proxy函数 - z69183787的专栏 - CSDN博客
2013年10月24日 14:30:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：903
个人分类：[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)
jQuery.proxy(function,obj)
jquery1.4新增，返回一个新函数，并且这个函数始终保持了特定的作用域。
当有事件处理函数要附加到元素上，但他们的作用域实际是指向另一个对象时，这个方法最有用了。此外，最妙的是，jQuery能够确保即便你绑定的函数是经过jQuery.proxy()处理过的函数，你依然可以传递原先的函数来准确无误地取消绑定。请参考下面的例子。
这个函数还有另一种用法，jQuery.proxy( scope, name 
)。第一个参数是要设定的作用域对象。第二个参数是将要设置作用域的函数名（必须是第一个作用域对象的一个属性）。
看起来有点call/apply的意思是不是？
示例：
<div id="test">Click Here!</div>var obj = {
  name: "John",
  test: function() {
    alert( this.name );
    $("#test").unbind("click", obj.test);
  }
};
 
$("#test").click( jQuery.proxy( obj, "test" ) );
 
// 以下代码跟上面那句是等价的:
// $("#test").click( jQuery.proxy( obj.test, obj ) );
 
// 可以与单独执行下面这句做个比较。
// $("#test").click( obj.test );
前两种输出的是指定的作用域obj，所以弹出john，后一种this.name将只想window.name，弹出undefined
