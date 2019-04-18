# 纯css 实现 三角形、梯形等 效果 - z69183787的专栏 - CSDN博客
2019年01月10日 20:05:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：65
个人分类：[前端-Html5																[前端-动画效果																[前端-Css](https://blog.csdn.net/z69183787/article/category/2175041)](https://blog.csdn.net/z69183787/article/category/6683002)](https://blog.csdn.net/z69183787/article/category/5816199)
[http://www.cnblogs.com/linsinan/p/6928734.html](http://www.cnblogs.com/linsinan/p/6928734.html)
[https://www.cnblogs.com/wcf52web/p/5608272.html](https://www.cnblogs.com/wcf52web/p/5608272.html)
　今天一个刚开始学习html 的小白问我一个问题，css 可以实现正方形，长方形，和圆型（border-radius），怎么能做出个三角形、梯形等等形状呢？于是我便开启了装逼模式， 给他讲解了一下我的思路，在我的帮助下， 他终于用css 做出了自己的三角形、梯形。我表示很欣慰， 于是，为了帮助更多像我这个朋友一样基础的小白，我决定献丑，把我的思路，和做法写成一篇博文，分享给大家。
　　闲话少叙，开始！
　　1，首先，我创建了一个 class 名为 ‘box’ 的div，我们暂且亲切的称它为‘小盒子’
　　html：
|12|`<``div``class='box'>``</``div``>`|
 　　css：
|12345678|`<!--小盒子的样式-->``       ``.box``       ``{``           ``width:20px;``           ``height:20px;``           ``background-color:blue;``           ``}``  `|
 　　我们看一下效果， 其实就是一个普通的，背景颜色为蓝色的div。
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601115917430-1352603417.png)
　　2，然后，我们给‘小盒子’添加四条边框样式，边框设置的宽一点， 四条边框的颜色给不一样的值， 方便我们查看，
|1234567891011|`.box``        ``{``            ``width:50px;``            ``height:50px;``            ``background-color:blue;``            ``            ``border-top:50px solid red;``            ``border-right:50px solid yellow;``            ``border-bottom:50px solid green;``            ``border-left:50px solid pink;``            ``}`|
 　　看看效果， 已经是一个比较花哨的存在了。
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601123750149-1423356599.png)
　　3，然后，我们把‘小盒子’的宽高设置为0px，去掉蓝色的背景色， 当然就只剩下四周的边框了，脑补一下， 然后我们上代码，看卡效果。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box
        {
            width:0px;
            height:0px;
            border-top:50px solid red;
            border-right:50px solid yellow;
            border-bottom:50px solid green;
            border-left:50px solid pink;
            }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
　　来，上效果，look一下！童鞋们， 我么似乎已经看到了4个颜色不同的三角形了，接下来要做的步骤，就很清晰了。
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601124322508-1250099356.png)
　　4，假设我们想要一个向上的三角形，只要把‘小盒子’上、右、左 方向边框的颜色设置为透明， 就只剩下下边框，也就是向上的小三角了， 来上代码，look一下！
![复制代码](http://common.cnblogs.com/images/copycode.gif)
        .box
        {
            width:0px;
            height:0px;
            border-top:50px solid rgba(0,0,0,0);
            border-right:50px solid  rgba(0,0,0,0);
            border-bottom:50px solid green;
            border-left:50px solid  rgba(0,0,0,0);
            }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
　　看效果！绿色的向上的小三角就出现了， \(^o^)/
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601125323883-81072990.png)
-------------------------------第一阶段-总结-------------------------------------
　　通过上面的操作，我们已经成功的用css 做出了小三角形，是不是很简单。
　　上面我说过了，也可以做出梯形，该怎么做呢？ 下面我们一起学习一下。↓
------------------------------------css做梯形---------------------------------------------
　　我们先理理思路，大家想一想，梯形是怎么构成的，小学的数学课本都讲过的，就拿等腰梯形来说， 是由两个直角三角形，和一个长方形或正方形组成的，看草图↓
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601130211164-1542663129.png)
　　这意味着，我们要用三个‘小盒子’来拼接成一个梯形，这么一说大家的思路是不是豁然开朗了，
　　不多说，来，继续分析，
　　1，首先，做三个‘小盒子’  1号，和3号小盒子都做成小三角形，2号小盒子做成一个正方形。
　　html：
    <div class='box'>
    </div>
    <div class='box2'>
    </div>
    <div class='box3'>
    </div>
　　css：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
        .box,.box3
        {
            width:0px;
            height:0px;
            border-top:50px solid rgba(0,0,0,0);
            border-right:50px solid  rgba(0,0,0,0);
            border-bottom:50px solid green;
            border-left:50px solid  rgba(0,0,0,0);
            margin-bottom:10px;
            }
            .box2
        {
            width:50px;
            height:50px;
            background-color:green;
            }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
　　先看看效果↓
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601131318258-329968470.png)
　　2，有人要问了， 形状是出来了， 但是怎么拼起来呢，这里就要用到css中的定位指示 和 css3 里的transfrom 了。不了解的话，w3c里都有详细的解释，我这里只管实现我们要的效果。
　　3，先把所有的‘小盒子’排成一排，使块状元素排成一排的方法 ：可以给‘小盒子’加 浮动 float，也可以直接设置‘小盒子’的display：inline-block；由于我的习惯是只要加浮动， 就要给父元素清浮动，比较麻烦，我就直接设置‘小盒子’的display：inline-block 了。
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601132259524-1060865574.png)
　　4,最后设置一下 1号，和3号小盒子 的位移 就ok了， 看代码↓
　　　　　　 .box{
                transform: translate(54px,10px);
            }
            .box3{
                transform: translate(-54px,10px);
            }
　　再看看效果↓
![](https://images2015.cnblogs.com/blog/537989/201706/537989-20170601133152539-1140274038.png)
　　介绍到这里， 我们已经用css 把梯形做出来了， 还有其他的图形， 比如平行四边形等，类似的东西，举一反三，都可以通过这种方法做出来，在这里我就不一一介绍了。最后，祝大家学习愉快。
# [乔安娜~春凤](https://www.cnblogs.com/wcf52web/)
## [CSS 实现三角形、梯形、等腰梯形](https://www.cnblogs.com/wcf52web/p/5608272.html)
三角形
 .graph:before{ content: ""; display: block; height: 0; width: 0px; border-width: 0px 30px 45px 145px; border-style: none solid solid; border-color: transparent transparent red; }
![](https://images2015.cnblogs.com/blog/884904/201606/884904-20160622185500328-141994207.png)
等腰三角形（只用修改border-width属性值就可以了）
.graph:before{ content:""; display:block; width:0; height:0; border-width:0px 300px 100px; border-style:none solid solid; border-color:transparent transparent red; }
![](https://images2015.cnblogs.com/blog/884904/201606/884904-20160622185656219-760464664.png)
直角三角形
.graph:before{ content:""; display:block; width:0; height:0; border-width:0px 0px 200px 200px; border-style:none solid solid; border-color:transparent transparent red; }
![](https://images2015.cnblogs.com/blog/884904/201606/884904-20160622190131719-2018616029.png)
梯形
.graph2:before{border-color:transparent transparent red;border-style:none solid solid;border-width:0 20px 60px 145px;content:"";display:block;height:0;left:-10px;width:200px;}
![](https://images2015.cnblogs.com/blog/884904/201606/884904-20160622192332781-1255963033.png)
等腰梯形
.graph2:before{border-color:transparent transparent red;border-style:none solid solid;border-width:0 45px 45px;content:"";display:block;height:0;left:-10px;width:200px;}
![](https://images2015.cnblogs.com/blog/884904/201606/884904-20160622192128703-745266284.png)
 等腰梯形旋转
![复制代码](https://common.cnblogs.com/images/copycode.gif)
.promotion-contain2 {
    content:"";
    display:block;
    height:0;
    border-width:0px 15px 15px;
    border-style:none solid solid;
    border-color:transparent transparent red;
    position:absolute;
    -moz-transform:rotate(-45deg);/*FF浏览器*/
    -webkit-transform:rotate(-45deg);/*chrome浏览器*/
    -o-transform:rotate(-45deg);/*oprea浏览器*/
    -ms-transform:rotate(-45deg);/*IE浏览器*/
    left:-10px;
    top:17px;
    width:40px;
}
