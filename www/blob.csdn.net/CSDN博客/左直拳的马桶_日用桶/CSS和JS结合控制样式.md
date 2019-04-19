# CSS和JS结合控制样式 - 左直拳的马桶_日用桶 - CSDN博客
2015年02月03日 11:02:59[左直拳](https://me.csdn.net/leftfist)阅读数：1587
CSS控制样式，毋庸置疑。但有时，仅靠CSS却很难控制好，比如说，页面在多种条件下进行不同的呈现；或者运行过程中，会发生变化，比如说，左侧的菜单栏收缩了，那么右侧的宽度就变大了。
就拿这个元素的width、height来说，页面变了，预先设定的值就不合适；甚至乎，从一开始，你就不知道该预先设定什么值，因为前端的浏览器各种各样，类似height:100%这样的好像不起作用。CSS可能支持一些表达式，但我没怎么用过，印象中有说法说会有浏览器兼容问题，或者是性能问题。
这时应该换一种思路，用JS，jQuery来控制。
例子如下：
```java
<script type="text/javascript">
    $(function(){//这是一个在页面加载完成后执行的函数
        $("#div1").css("height",($(document).height() - 133) +  "px");
        $('.bottomTabs').css('position',"relative");
    });
</script>
```
