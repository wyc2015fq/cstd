# Razor里写函数 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月28日 22:37:15[左直拳](https://me.csdn.net/leftfist)阅读数：4416
asp.net mvc的视图里使用Razor来书写服务器代码，人尽皆知。可以常常见到里面写上for循环语句，输出一大堆东东，牛逼得很。
可是，如果循环语句还不能满足我们的要求，需要定义一个函数来调用，该怎么搞？
网上那些牛逼哄哄的教程文章立即三缄其口一大半。
好的，我现在来给个例子：
```
<ul class="navList clx" id="J_GisSiteNav">
    @functions{
        string outputTags(IList<ViewTag> list,bool isCurrent = false)
        {
            StringBuilder sb = new StringBuilder("");
            ……
            return sb.ToString();
        }
    }
    @if (mylist.Count > 0)
    {
        <span id='spBasicView'>@Html.Raw(outputTags(list, false))</span>
        <span id='spMyView'>@Html.Raw(outputTags(mylist, true))</span>
    }
    else if (list.Count > 0)
    {
        <span id='spBasicView'>@Html.Raw(outputTags(list, true))</span>
    }        
    <li><a class="add" onclick="setTags()" href="javascript:void(0);">......</a></li>
</ul>
```
