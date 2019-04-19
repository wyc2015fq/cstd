# HTML5中自定义的data-*属性使用和js/jquery的data()方法操作 - 零度的博客专栏 - CSDN博客
2015年08月26日 17:14:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：3624
可能大家在使用[jquery](http://www.mb5u.com/jquery/) mobile时，经常会看到[data](http://www.mb5u.com/biancheng/net/net_96008.html)-role、data-theme等的使用，比如：通过如下[代码](http://www.mb5u.com/changyongdaima/)即可实现页眉的效果：
代码如下:
<div data-role="header">
<h1>我是标题</h1>
</div>
通过手机浏览，效果如下：
![](http://www.mb5u.com/uploads/allimg/130422/1F11311U-0.png)![](http://www.mb5u.com/uploads/allimg/130422/1F1135520-1.png)
为什么写一个data-role="header"就能实现底部为黑色、文字居中显示的效果呢？
本文提供一种最简单的实现办法，让大家对这些用法有个直观的了解。
我们写一个html页面，自定义一个data-chb="header"的属性，希望具备这个属性的div区域背景颜色为黑色，文字为白色，居中显示；不具备data-chb自定义属性的div按照默认方式显示，html代码如下：
 代码如下:
<body>
<div data-chb="header">
<h1>我是使用了data-chb自定义属性的div</h1>
</div>
<br/>
<div>
我没有使用data-chb自定义属性，该怎么展现就怎么展现；
</div>
</body>
要想实现"背景颜色为黑色，文字为白色，居中显示"的显示效果，我们定义如下的[css](http://www.mb5u.com/divcssjiaocheng/cssjiaocheng/)：
代码如下:
<style>
.ui_header {
background-color: black;
text-align: center;
color:white;
border:1px solid #000;
}
</style>
然后我们通过如下js方法实现在页面加载时，动态添加css定义，改变具备data-chb属性的div的显示样式：
代码如下:
<script type="text/javascript">
window.onload=function(){
var elems = document.getElementsByTagName("div");
if(elems!=null&&elems.length>0){
var length = elems.length;
//遍历所有DIV控件
for(var i=0;i<length;i++){
var elem = elems[i];
//获取该控件的自定义属性
var customAttr = elem.dataset.chb;
//也可以通过如下方式获得自定义属性
//var customAttr = elem.dataset["chb"];
//如果是我们预先定义好的header值，表示需要处理
if(customAttr=="header"){
//添加样式
elem.setAttribute("class","ui_header");
}
}
}
}
</script>
最后页面显示效果如下：
![](http://www.mb5u.com/uploads/allimg/130422/1F11345V-2.png)
人们总喜欢往HTML标签上添加自定义属性来存储和操作数据。但这样做的问题是，你不知道将来会不会有其它脚本把你的自定义属性给重置掉，此外，你这样做也会导致html语法上不符合Html规范，以及一些其它副作用。这就是为什么在[HTML5](http://www.mb5u.com/HTML5/)规范里增加了一个自定义data属性，你可以拿它做很多有用的事情。
你可以去读一下HTML5的详细规范，但这个自定义data属性的用法非常的简单，就是你可以往HTML标签上添加任意以 "data-"开头的属性，这些属性页面上是不显示的，它不会影响到你的页面布局和风格，但它却是可读可写的。
**下面的一个代码片段是一个有效的HTML5标记**： 
代码如下:
<div id="awesome" 
data-myid="3e4ae6c4e">Some awesome data</div> 
可是，怎么来读取这些数据呢？你当然可以遍历页面元素来读取你想要的属性，但jquery已经内置了方法来操作这些属性。使用[jQuery](http://www.mb5u.com/jquery/)的.data()方法来访问这些"data-*" 属性。其中一个方法就是 .data(obj)，这个方法是在 jQuery1.4.3版本后出现的，它能返回相应的data属性。
举个例子，你可以用下面的写法读取 data-myid属性值： 
代码如下:
var myid= jQuery("#awesome").data('myid'); 
console.log(myid);你还可以在"data-*" 属性里使用json语法，例如，如果你写出下面的html： 
代码如下:
<div id="awesome-json" data-awesome='{"game":"on"}'></div> 
你可以通过js直接访问这个数据，通过json的key值，你能得到相应的value： 
代码如下:
var gameStatus= jQuery("#awesome-json").data('awesome').game; 
console.log(gameStatus);
你也可以通过.data(key,value)方法直接给"data-*" 属性赋值。一个重要的你要注意的事情是，这些"data-*" 属性应该和它所在的元素有一定的关联，不要把它当成存放任意东西的存储工具。
尽管"data-*" 是HTML5才出现的属性，但jquery是通用的，所以，在非HTML5的页面或浏览器里，你仍然可以使用.data(obj)方法来操作"data-*" 数据。
