# clearfix清除浮动闭合容器之:after与:before - z69183787的专栏 - CSDN博客
2014年07月14日 12:31:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2651
首先对伪类:before与:after做下注脚
:before 伪元素在元素之前添加内容。
:after 伪元素在元素之后添加内容。
这2个伪元素允许创作人员在元素内容的 最前面/最后面 插入生成内容。默认地，这个伪元素是行内元素，不过可以使用属性 display 改变这一点。**该二伪类不被IE6/7所支持。**
好了，进入正题
在平常我们coding时候经常会发生子元素浮动而引起父级元素不能完全撑开的状况，
可能很多朋友给父级元素用overflow:hidden简单粗暴的解决，但这种方式有弊端，一是在IE6下不能完美解决，二是假如在该模块有定位的层就有定位层显示不完整的可能。
也有可能有些朋友是在父级元素里最后添加一个<div style=”clear:both”></div>来解决这个问题，但这样我们的HTML代码里就多了些冗余的代码且从语义化角度来看这也是不合理的。
下面创建一个.clearfix类来完美解决这个问题
上代码：
`<``pre``>.clearfix:after {``content`
```
:
```
`"."``;``display`
```
:
```
`block``;``height`
```
:
```
`0``;``clear`
```
:
```
`both``;``visibility`
```
:
```
`hidden``;}`
`* html .clearfix{zoom:``1`
```
;}
```
`/* IE6 */`
`*+html .clearfix{zoom:``1`
```
;}
```
`/* IE7 */`
快给你的不能完全闭合的父级元素加上.clearfix类试试吧
