# Jquery UI dialog 详解 (中文) - z69183787的专栏 - CSDN博客
2013年07月02日 20:25:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：965标签：[JQUERY UI](https://so.csdn.net/so/search/s.do?q=JQUERY UI&t=blog)
个人分类：[Javascript-Jquery插件																[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)](https://blog.csdn.net/z69183787/article/category/2175469)
1 属性
1.11 autoOpen ，这个属性为true的时候dialog被调用的时候自动打开dialog窗口。当属性为false的时候，一开始隐藏窗口，知道.dialog("open")的时候才弹出dialog窗口。默认为：true。
1.12 初始化例：请注意，$('.selector')是dialog 的类名，在本例中.selector=#dialoag,以后不再说明。
$('.selector').dialog({ autoOpen: false });
1.13 初始化后，得到和设置此属性例：
//获得
var autoOpen = $('.selector').dialog('option', 'autoOpen');
//设置
$('.selector').dialog('option', 'autoOpen', false);
1.21 bgiframe 默认为false ，When true, the bgiframe plugin will be used, to fix the issue in IE6 where select boxes show on top of other elements, regardless of zIndex. Requires including the bgiframe plugin. Future versions may not require a separate plugin.
在IE6下，让后面那个灰屏盖住select。
1.22 初始化例：
$('.selector').dialog({ bgiframe: true });
1.23 初始化后，得到和设置：
//获取
var bgiframe = $('.selector').dialog('option', 'bgiframe');
//设置
$('.selector').dialog('option', 'bgiframe', true);
1.31 buttons 显示一个按钮，并写上按钮的文本，设置按钮点击函数。默认为{}，没有按钮。
最上面的例子中已经有buttons属性的用法，请注意。
1.32 初始化例：
$('.selector').dialog({ buttons: { "Ok": function() { $(this).dialog("close"); } } });
1.33 初始化后，得到和设置：
//获取
var buttons = $('.selector').dialog('option', 'buttons');
//设置
$('.selector').dialog('option', 'buttons', { "Ok": function() { $(this).dialog("close"); } });
1.41 closeOnEscape 为true的时候，点击键盘ESC键关闭dialog，默认为true；
1.42 初始化例：
$('.selector').dialog({ closeOnEscape: false });
1.43 初始化后，得到和设置：
//获取
var closeOnEscape = $('.selector').dialog('option', 'closeOnEscape');
//设置
$('.selector').dialog('option', 'closeOnEscape', false);
1.51 dialogClass 类型将被添加到dialog，默认为空
1.52 初始化例：
$('.selector').dialog({ dialogClass: 'alert' });
1.53 初始化后，得到和设置：
//获取
var dialogClass = $('.selector').dialog('option', 'dialogClass');
//设置
$('.selector').dialog('option', 'dialogClass', 'alert');
1.61 draggable、resizable : draggable是否可以使用标题头进行拖动，默认为true，可以拖动;resizable是否可以改变dialog的大小，默认为true，可以改变大小。
1.62 初始化例：
$('.selector').dialog({ draggable: false,resizable:false });
1.63 初始化后，得到和设置：
//获取
var draggable = $('.selector').dialog('option', 'draggable');
//设置
$('.selector').dialog('option', 'draggable', false);
1.71 width、height ，dialog的宽和高，默认为auto，自动。
1.72 初始化例：
$('.selector').dialog({ height: 530,width:200 });
1.73 初始化后，得到和设置：请参考1.63
1.81 maxWidth、maxHeight、minWidth、minHeight ，dialog可改变的最大宽度、最大高度、最小宽度、最小高度。maxWidth、maxHeight的默认为false，为不限。minWidth、minHeight的默认为150。要使用这些属性需要ui.resizable.js 的支持。
1.82 初始化例：
$('.selector').dialog({ maxHeight: 400,maxWidth:600,minHeight:300,minWidth:300 });
1.83 初始化后，得到和设置：请参考1.63
1.91 hide、show ，当dialog关闭和打开时候的效果。默认为null，无效果
1.92 初始化例：最上面的实例中用到，请自己看吧。
1.93 初始化后，得到和设置：请参考1.63
1.101 modal,是否使用模式窗口，模式窗口打开后，页面其他元素将不能点击，直到关闭模式窗口。默认为false不是模式窗口。
1.102 初始化例：$('.selector').dialog({ modal: true });
1.103 初始化后，得到和设置：请参考1.63
1.111 title，dialog的标题文字，默认为空。
1.112 初始化例：见最上面的实例。1.113 初始化后，得到和设置：请参考1.63
1.121 position ，dialog的显示位置：可以是'center', 'left', 'right', 'top', 'bottom',也可以是top和left的偏移量也可以是一个字符串数组例如['right','top']。
1.122 初始化例：$('.selector').dialog({ position: ['top','right'] }); 1.123 初始化后，得到和设置：请参考1.63
1.131 zIndex, dialog的zindex值，默认值为1000.
1.132 初始化例：$('.selector').dialog({ zIndex: 3999 }); 1.133 初始化后，得到和设置：请参考1.63
1.141 stack 默认值为true，当dialog获得焦点是，dialog将在最上面。
1.142 初始化例：$('.selector').dialog({ stack: false }); 1.143 初始化后，得到和设置：请参考1.63
2 事件 
2.11 beforeclose 类型dialogbeforeclose ， 当dialog尝试关闭的时候此事件将被触发，如果返回false，那么关闭将被阻止。
2.12 初始化例：$('.selector').dialog({
   beforeclose: function(event, ui) { ... }
});
2.13 使用类型绑定此事件例：$('.selector').bind('dialogbeforeclose', function(event, ui) {
...
});
2.21 close 类型：dialogclose ，当dialog被关闭后触发此事件。
2.22 初始化例：$('.selector').dialog({
   close: function(event, ui) { ... }
});
2.23 使用类型绑定此事件例：$('.selector').bind('dialogclose', function(event, ui) {
...
});
2.3 open 类型：dialogopen ，当dialog打开时触发。（篇幅有限，该省略的就省略了啊，初始化例和使用类型绑定事件可以向上参考。）
2.4 focus 类型：dialogfocus ，当dialog获得焦点时触发。
2.5 dragStart 类型：dragStart，当dialog拖动开始时触发。
2.6 drag 类型：drag ，当dialog被拖动时触发。
2.7 dragStop 类型：dragStop ，当dialog拖动完成时触发。
2.8 resizeStart 类型：resizeStart ，当dialog开始改变窗体大小时触发。
2.9 resize 类型：resize，当dialog被改变大小时触发。
2.10 resizeStop 类型：resizeStop，当改变完大小时触发。
3 方法
3.1 destroy ，我喜欢这个哦，摧毁地球。。。 例：.dialog( 'destroy' ) 
3.2 disable，dialog不可用，例：.dialog('disable');
3.3 enable,dialog可用，例，如3.2
3.4 close，open，关闭、打开dialog
3.5 option ，设置和获取dialog属性，例如：.dialog( 'option' , optionName , [value] ) ，如果没有value，将是获取。
3.6 isOpen ，如果dialog打开则返回true，例如：.dialog('isOpen')
3.7 moveToTop ,将dialog移到最上层，例如：.dialog( 'moveToTop' )
