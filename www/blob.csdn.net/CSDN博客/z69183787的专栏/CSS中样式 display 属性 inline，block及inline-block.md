# CSS中样式 display 属性 inline，block及inline-block - z69183787的专栏 - CSDN博客
2012年10月22日 09:32:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12643
div 元素默认display属性为block 为块状元素，无论div中文字字数，总以块状显示，即占满浏览器宽度。
`<div style="background-color:red;">测试测试</div>`
![](https://img-my.csdn.net/uploads/201210/22/1350870475_3052.jpg)
`<div style="display:inline;background-color:red;">sss</div>`
![](https://img-my.csdn.net/uploads/201210/22/1350870507_9123.jpg)
而span元素默认display属性为inline，只占据span中所占字数的宽度。
`<span style="background-color:red;">测试测试<span>`
![](https://img-my.csdn.net/uploads/201210/22/1350870551_6825.jpg)
`<span style="background-color:red;display:block;">测试测试<span>`![](https://img-my.csdn.net/uploads/201210/22/1350870475_3052.jpg)
inline-block
```
<div style="display:inline-block;background-color:red;width:20px;">sfffs</div>
<p>
<span style="display:inline-block;background-color:red;width:20px;">sfffs</span>
```
![](https://img-my.csdn.net/uploads/201210/22/1350870586_2500.jpg)
inline-block 与 inline 的区别
默认情况下 inline-block 与inline 效果一致
但当对div设置width或height时，由于width和height属性只对块状元素即block类型起作用，就能看出inline-block与inline 的区别了。
```
<div style="display:inline-block;background-color:red;width:200px;">sfffs</div>
<p>
<span style="display:inline;background-color:red;width:200px;">sfffs</span>
```
![](https://img-my.csdn.net/uploads/201210/22/1350912803_4248.jpg)
