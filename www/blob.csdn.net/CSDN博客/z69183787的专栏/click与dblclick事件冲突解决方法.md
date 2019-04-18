# click与dblclick事件冲突解决方法 - z69183787的专栏 - CSDN博客
2014年05月05日 10:31:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11246
一个DOM元素，如：div，既绑定了 click 事件，又绑定了 dblclick 事件，这两个事件分别要做独立的事情。事件处理上没有冲突，都可以各自完成各自的操作。双击的时候虽然是执行了 dblclick 事件，但是在这之前，也执行了 click 事件，那么，如何来禁止或者说屏蔽这次多余的 click 事件呢？本文将提供给您一个比较好的解决办法。
情况分析：
　　首先，来了解一下点击事件发生的先后顺序：
单击：mousedown, mouseup, click
双击：mousedown, mouseup, click, mousedown, mouseup, click, dblclick
　　由此看来，dblclick 事件发生之前，实际上发生了两次 click 事件。其中，第一次的 click 是会被屏蔽掉，但是第二次则不会，所以就出现在双击的时候，也触发 click 事件。
解决办法：
　　知道了原因，接下来自然就是想办法把这次 click 给屏蔽掉，但是由于各浏览器均未提供直接去停止事件的方法，所以值得改变思路。
　　由于我们只需要屏蔽一次 click 事件即可，由此联想到，可以利用 setTimeout() 方法来延时完成 click 事件的处理，在需要停止 click 的时候利用 clearTimeout() 方法停止这一事件的处理。这样，就可以比较容易的写出如下的 javascript 代码：
```
`var``timer = ``null``;`
`$(``'button'``).live(``'click'`
```
,
```
`function``(event){`
`    ``timer && clearTimeout(timer);`
`    ``timer = setTimeout(``function``(){`
`        ``console.log(event.type);`
`    ``},300);`
`}).live(``'dblclick'`
```
,
```
`function``(event){`
`    ``timer && clearTimeout(timer);`
`    ``console.log(event.type);`
`});`
```
