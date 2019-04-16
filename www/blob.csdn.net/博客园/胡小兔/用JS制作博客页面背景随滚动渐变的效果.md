# 用JS制作博客页面背景随滚动渐变的效果 - 胡小兔 - 博客园







# [用JS制作博客页面背景随滚动渐变的效果](https://www.cnblogs.com/RabbitHu/p/change_background_color.html)





今天颓了一会，用JavaScript给我的博客园博客写了一个页面背景随滚动而渐变的效果，做完之后自我感觉良好……

下面就以我的博客园博客为例，介绍一下如何制作这个效果！

## 准备
- [x] 申请博客园的JS权限（如果你也想把它用在自己的博客园上）
- [x] 可能需要一些JS的基础知识（你可以打开w3school，然后就当自己准备好了）
- [x] 可能需要一些CSS基础知识（你需要知道颜色是怎样用16进制表示的）

——当然，你也可以复制下面的代码然后走人……

**注意：**如果你想要用在自己的博客园上，请注意博客园的不同模板对应的元素`class`也可能是不同的。

## 代码

首先，在“页首HTML代码”中新建一个div，放在最下面一层。这个div用来放渐变的背景颜色。
`<div id="backGround" style="position: fixed; width:100%; height: 100%; top: 0; left: 0; background-color: #bff687; z-index: -1"></div>`
然后用js监听`window.onscroll`事件，记录一个cnt变量，每监听到一次，就给cnt加上1，然后用cnt生成当前颜色。

至于颜色的生成方式你可以自己发明一个。我用了三个相位不同的sin函数生成R、G、B三个颜色的值。

```
function getTitleValue(start, x){
        var ret = titleMax - 1 - rangeValue + Math.floor(rangeValue * Math.sin(start + x));
        return ret;
    }
function getBackgroundColor(){
    var red = getBackgroundValue(0, cnt * 2 * PI / 256).toString(16);
    var green = getBackgroundValue(2 * PI / 3, cnt * 2 * PI / 256).toString(16);
    var blue = getBackgroundValue(4 * PI / 3, cnt * 2 * PI / 256).toString(16);
    return "#" + red + green + blue;
}
```

这里讲一下如何用js修改某个元素的颜色：

```
function changeColor(){
    var backGround = document.getElementById("backGround");
    var titles = document.getElementsByClassName("postTitle2");
    var newTitleColor = getTilteColor();
    backGround.style.backgroundColor = getBackgroundColor();
    for(var i = 0; i < titles.length; i++)
        titles[i].style.color = newTitleColor;
    cnt = (cnt + 1) % 256;
}
```

先`getElementById`，然后调用生成颜色的函数，生成了一个**字符串**，形如`#aabbcc`，代表颜色。然后用JS修改CSS。

完整代码：

```
<div id="backGround" style="position: fixed; width:100%; height: 100%; top: 0; left: 0; background-color: #bff687; z-index: -1"></div>
        
<script>
    var cnt = 0, PI = Math.acos(-1);
    var rangeValue = 64, backgroundMax = 256, titleMax = 192;
    function getBackgroundValue(start, x){
        var ret = backgroundMax - 1 - rangeValue + Math.floor(rangeValue * Math.sin(start + x));
        return ret;
    }
    function getTitleValue(start, x){
        var ret = titleMax - 1 - rangeValue + Math.floor(rangeValue * Math.sin(start + x));
        return ret;
    }
    function getBackgroundColor(){
        var red = getBackgroundValue(0, cnt * 2 * PI / 256).toString(16);
        var green = getBackgroundValue(2 * PI / 3, cnt * 2 * PI / 256).toString(16);
        var blue = getBackgroundValue(4 * PI / 3, cnt * 2 * PI / 256).toString(16);
        return "#" + red + green + blue;
    }
    function getTilteColor(){
        var red = getTitleValue(0, cnt * 2 * PI / 256).toString(16);
        var green = getTitleValue(2 * PI / 3, cnt * 2 * PI / 256).toString(16);
        var blue = getTitleValue(4 * PI / 3, cnt * 2 * PI / 256).toString(16);
        return "#" + red + green + blue;
    }
    window.onload = window.onscroll = function changeColor(){
        var backGround = document.getElementById("backGround");
        var titles = document.getElementsByClassName("postTitle2");
        var newTitleColor = getTilteColor();
        backGround.style.backgroundColor = getBackgroundColor();
        for(var i = 0; i < titles.length; i++)
            titles[i].style.color = newTitleColor;
        cnt = (cnt + 1) % 256;
    }
</script>
```












