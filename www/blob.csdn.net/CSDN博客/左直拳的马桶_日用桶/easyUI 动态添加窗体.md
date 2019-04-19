# easyUI 动态添加窗体 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月24日 17:40:28[左直拳](https://me.csdn.net/leftfist)阅读数：4804
有一张页面A，在页面开头引用了jquery.easyUI.min.js。
现在想达到这么一种效果，点击页面A的一个按钮，弹出一个easyUI窗体。因为想分模块的原因，这个窗体对应的是另一张页面B。在点击按钮的时候，才用ajax动态加载页面B，加载到页面A指定的容器里（<div id="divSubWin"></div>），然后将这个容器里的内容作为模式窗体弹出：
```java
$('#dlg_Add').dialog('open');//#div_Add存在于页面B；divSubWin则在页面A。现在要将页面B的全部内容加载到页面A的divSubWin里
```
但是，动态加载之后，窗体是出来了，但并没有呈现模式窗体的效果。事实上，浏览器提示有脚本错误。
如果将页面B的代码，放在页面A，作为页面A不可分割的一部分，那么将什么问题也没有。
究其原因是这样的：
*我们之所以在页面中，只要书写相应easyui的class，Easyui就能成功渲染页面，这是因为解析器在默认情况下，parser会在文档装载完成的时候($(document).ready)被调用一次，而且是渲染整个页面。然而当页面装载完后，如果用javascript生成的DOM中包含了Easyui支持控件的class，比如说，用javascript生成了以下代码：*
`<a id="tt" href="#" class="easyui-linkbutton" data-options="iconCls:'icon-search'">easyui</a>` 虽然页面上有这样的DOM了，但是没有被渲染为Easyui的linkbutton插件，原因是Easyui并不会一直监听页面，所以不会主动渲染，这时候就需要手工调用Easyui的parser进行解析了。
手工调用需要注意的是：
解析目标为指定DOM的所有子孙元素，不包含这个DOM自身：
由于我们页面A里面的divSubWin是作为一个容器来装载页面B的内容，所以这样写刚刚好：
```java
$.parser.parse($('#divSubWin'));
```
问题解决！
参考文章：
[http://www.cnblogs.com/y20091212/p/easyui.html](http://www.cnblogs.com/y20091212/p/easyui.html)
