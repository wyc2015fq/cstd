# 动态输出html一些效果失效的处理 - 左直拳的马桶_日用桶 - CSDN博客
2015年09月02日 19:22:20[左直拳](https://me.csdn.net/leftfist)阅读数：2222
利用AJAX动态加载页面，实现无刷新加载，有时会出现一些问题。比如说，在一些jquery控件中，是利用在页面加载的时候，对一些带有特殊属性的元素进行处理，比如事件绑定什么的。假如是动态加载，此时页面早已加载完毕，那么动态输出的，带有这些特殊属性的元素，就得不到处理，就不会有特定的效果。
比如：
`<a class="btn blue thickbox" title='添加新角色' href="@Url.Action("Create")?TB_iframe=true&height=350&width=500"><i class="icon-plus icon-white"></i> 新增</a>`
本来点击这个链接，会有一个对话框弹出（参见thickbox），但假如这段代码用ajax动态输出，则没有这个效果。
应对之道，在动态加载完毕后，紧接着执行：
```java
tb_init('a.thickbox', 'tr.thickbox', 'td.thickbox');//解决动态生成无效的问题
//tb_init是jquery.thickbox.js的函数
```
这种通过定义标记的个性化属性，然后由JS加以解释、处理，是一种很妙的思路。以前我在easyUI上见识过，但没有怎么留意。
