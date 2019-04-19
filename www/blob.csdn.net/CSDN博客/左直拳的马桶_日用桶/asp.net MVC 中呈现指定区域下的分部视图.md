# asp.net MVC 中呈现指定区域下的分部视图 - 左直拳的马桶_日用桶 - CSDN博客
2015年10月08日 11:55:26[左直拳](https://me.csdn.net/leftfist)阅读数：4618
Html.RenderAction() 可以呈现分部视图。
asp.net MVC就是有这种好处，可以将多个子视图无缝合成到一个视图上再输出，那么开发的时候，能够进行模块化开发。看上去同属一个页面上的东西，实际上分布于不同的物理视图文件，逻辑清晰得很。
Html.RenderAction() 可以呈现分部视图，当然Html.RenderPartial()也可以。二者有区别。Html.RenderAction() 可以对应有控制器，Html.RenderPartial()直接指向视图。
所以某目前比较多的是用Html.RenderAction() ，呵呵。
但是@{Html.RenderAction(“action名字”, “controller名字”);}只能指向当前区域下的action，如果要用到其他区域呢？
可以酱紫： 
@{Html.RenderAction(“action名字”, “controller名字”, new { area = “Area名字” });}
