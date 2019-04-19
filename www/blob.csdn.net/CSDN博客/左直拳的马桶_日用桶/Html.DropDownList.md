# Html.DropDownList - 左直拳的马桶_日用桶 - CSDN博客
2015年08月25日 19:26:15[左直拳](https://me.csdn.net/leftfist)阅读数：1285
现在又在搞ASP.NET MVC。此前搞了2次，但没有怎么深入，好多东西都是似懂非懂。当然现在也还是这种状态，不过总算是在原来的基础上前进一点点。
好记性不如烂笔头，点滴记录在博唠嗝。
今天记录的是下拉框，@Html.DropDownList
前端，view这里：
`@Html.DropDownList("下拉框的名字",ViewBag.selectListObject as SelectList)`
控制器：
```cpp
this.ViewBag.selectListObject = new SelectList(list<...>对象
                , "对应value的属性"
                , "对应text的属性"
                , 默认值);
```
这两个方法都有许多重载的版本，这里只记录自己觉得最常用的
