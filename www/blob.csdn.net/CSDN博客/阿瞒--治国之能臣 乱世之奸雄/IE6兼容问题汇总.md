# IE6兼容问题汇总 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2013年03月30日 13:03:26[baofenny](https://me.csdn.net/jiaminbao)阅读数：1443
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
1、终极方法：条件注释
<!--[if lte IE 6]> 这段文字仅显示在 IE6及IE6以下版本。 <![endif]-->
<!--[if gte IE 6]> 这段文字仅显示在 IE6及IE6以上版本。 <![endif]-->
<!--[if gt IE 6]> 这段文字仅显示在 IE6以上版本（不包含IE6）。 <![endif]-->
<!--[if IE 5.5]> 这段文字仅显示在 IE5.5。 <![endif]-->
<!--在 IE6及IE6以下版本中加载css-->
<!--[if lte IE 6]> <link type="text/css" rel="stylesheet" href="css/ie6.css" mce_href="css/ie6.css" /><![endif]-->
缺点是在IE浏览器下可能会增加额外的HTTP请求数。
2、CSS选择器区分
IE6不支持子选择器；先针对IE6使用常规申明CSS选择器，然后再用子选择器针对IE7+及其他浏览器。
/* IE6 专用 */
.content {color:red;}
/* 其他浏览器 */
div>p .content {color:blue;} -->
3、PNG半透明图片的问题
虽然可以通过JS等方式解决，但依然存在载入速度等问题，所以，这个在设计上能避免还是尽量避免为好。以达到网站最大优化。
4、IE6下的圆角
IE6不支持CSS3的圆角属性，性价比最高的解决方法就是用图片圆角来替代，或者放弃IE6的圆角。
5、IE6背景闪烁
如果你给链接、按钮用CSS sprites作为背景，你可能会发现在IE6下会有背景图闪烁的现象。造成这个的原因是由于IE6没有将背景图缓存，每次触发hover的时候都会重新加载，可以用JavaScript设置IE6缓存这些图片：
document.execCommand("BackgroundImageCache",false,true);
6、最小高度
IE6 不支持min-height属性，但它却认为height就是最小高度。解决方法：使用ie6不支持但其余浏览器支持的属性!important。
#container {min-height:200px; height:auto !important; height:200px;}
7、最大高度
//直接使用ID来改变元素的最大高度
var container = document.getElementById('container');
container.style.height = (container.scrollHeight > 199) ? "200px" : "auto";
//写成函数来运行
function setMaxHeight(elementId, height){
var container = document.getElementById(elementId);
container.style.height = (container.scrollHeight > (height - 1)) ? height + "px" : "auto";
}
//函数示例
setMaxHeight('container1', 200);
setMaxHeight('container2', 500);
8、100% 高度
在IE6下，如果要给元素定义100%高度，必须要明确定义它的父级元素的高度，如果你需要给元素定义满屏的高度，就得先给html和body定义height:100%;。
9、最小宽度
同max-height和max-width一样，IE6也不支持min-width。
//直接使用ID来改变元素的最小宽度
var container = document.getElementById('container');
container.style.width = (container.clientWidth < width) ? "500px" : "auto";
//写成函数来运行
function setMinWidth(elementId, width){
var container = document.getElementById(elementId);
container.style.width = (container.clientWidth < width) ? width + "px" : "auto";
}
//函数示例
setMinWidth('container1', 200);
setMinWidth('container2', 500);
10、最大宽度
//直接使用ID来改变元素的最大宽度
var container = document.getElementById(elementId);
container.style.width = (container.clientWidth > (width - 1)) ? width + "px" : "auto";
//写成函数来运行
function setMaxWidth(elementId, width){
var container = document.getElementById(elementId);
container.style.width = (container.clientWidth > (width - 1)) ? width + "px" : "auto";
}
//函数示例
setMaxWidth('container1', 200);
setMaxWidth('container2', 500);
11、双边距Bug
当元素浮动时，IE6会错误的把浮动方向的margin值双倍计算。个人觉得较好解决方法是避免float和margin同时使用。
12、清除浮动
如果你想用div(或其他容器)包裹一个浮动的元素，你会发现必须给div(容器)定义明确的height、width、overflow之中一个属性（除了auto值）才能将浮动元素严实地包裹。
#container {border:1px solid #333; overflow:auto; height:100%;}
#floated1 {float:left; height:300px; width:200px; background:#00F;}
#floated2 {float:right; height:400px; width:200px; background:#F0F;}
更多：[http://www.twinsenliang.net/skill/20090413.html](http://www.twinsenliang.net/skill/20090413.html)
13、浮动层错位
当内容超出外包容器定义的宽度时，在IE6中容器会忽视定义的width值，宽度会错误地随内容宽度增长而增长。
浮动层错位问题在IE6下没有真正让人满意的解决方法，虽然可以使用overflow:hidden;或overflow:scroll;来修正， 但hidden容易导致其他一些问题，scroll会破坏设计；JavaScript也没法很好地解决这个问题。所以建议是一定要在布局上避免这个问题发 生，使用一个固定的布局或者控制好内容的宽度（给内层加width）。
14、躲猫猫bug
在IE6和IE7下，躲猫猫bug是一个非常恼人的问题。一个撑破了容器的浮动元素，如果在他之后有不浮动的内容，并且有一些定义了:hover的链接，当鼠标移到那些链接上时，在IE6下就会触发躲猫猫。
解决方法很简单：
1.在（那个未浮动的）内容之后添加一个<span style="clear: both;"> </span>
2.触发包含了这些链接的容器的hasLayout，一个简单的方法就是给其定义height:1%;
15、绝对定位元素的1像素间距bug
IE6下的这个错误是由于进位处理误差造成（IE7已修复），当绝对定位元素的父元素高或宽为奇数时，bottom和right会产生错误。唯一的解决办法就是给父元素定义明确的高宽值，但对于液态布局没有完美的解决方法。
16、3像素间距bug
在IE6中，当文本(或无浮动元素)跟在一个浮动的元素之后，文本和这个浮动元素之间会多出3像素的间隔。
给浮动层添加 display:inline 和 -3px 负值margin
给中间的内容层定义 margin-right 以纠正-3px
17、IE下z-index的bug
在IE浏览器中，定位元素的z-index层级是相对于各自的父级容器，所以会导致z-index出现错误的表现。解决方法是给其父级元素定义z-index，有些情况下还需要定义position:relative。
18、Overflow Bug
在IE6/7中，overflow无法正确的隐藏有相对定位position:relative;的子元素。解决方法就是给外包容器.wrap加上position:relative;。
19、横向列表宽度bug
如果你使用float:left;把<li>横向摆列，并且<li>内包含的<a>（或其他）触发了 hasLayout，在IE6下就会有错误的表现。解决方法很简单，只需要给<a>定义同样的float:left;即可。
20、列表阶梯bug
列表阶梯bug通常会在给<li>的子元素<a>使用float:left;时触发，我们本意是要做一个横向的列表(通常 是导航栏)，但IE却可能呈现出垂直的或者阶梯状。解决办法就是给<li>定义float:left;而非子元素<a>，或者 给<li>定义display:inline;也可以解决。
21、垂直列表间隙bug
当我们使用<li> 包含一个块级子元素时，IE6(IE7也有可能)会错误地给每条列表元素（<li>）之间添加空隙。
解决方法：把<a>flaot并且清除float来解决这个问题；另外一个办法就是触发<a>的hasLayout（如定 义高宽、使用zoom:1;）；也可以给<li> 定义display:inline;来解决此问题；另外还有一个极有趣的方法，给包含的文本末尾添加一个空格。
22、IE6中的:hover
在IE6中，除了(需要有href属性)才能触发:hover行为，这妨碍了我们实现许多鼠标触碰效果，但还是有一些法子是可以解决它的。最好是不要用:hover来实现重要的功能，仅仅只用它来强化效果。
23、IE6调整窗口大小的 Bug
当把body居中放置，改变IE浏览器大小的时候，任何在body里面的相对定位元素都会固定不动了。解决办法：给body定义position:relative;就行了。
24、文本重复Bug
在IE6中，一些隐藏的元素（如注释、display:none;的元素）被包含在一个浮动元素里，就有可能引发文本重复bug。解决办法：给浮动元素添加display:inline;。
