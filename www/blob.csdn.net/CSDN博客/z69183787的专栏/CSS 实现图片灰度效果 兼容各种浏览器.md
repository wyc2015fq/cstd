# CSS 实现图片灰度效果 兼容各种浏览器 - z69183787的专栏 - CSDN博客
2014年03月03日 07:13:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3486
CSS实现图片灰度效果就是通过CSS样式让彩色图片呈现为灰色，相当于把一张图像的颜色模式调整为灰度，CSS可以通过以下几种方法来实现灰度效果。
方式1. IE滤镜
img { filter: progid:DXImageTransform.Microsoft.BasicImage(grayscale=1); } //标准写法
img { filter:Gray; } //简写
IE系列浏览器都支持IE滤镜的这一属性，非IE浏览器不支持。 
方式2. CSS3滤镜 
img { -webkit-filter: grayscale(100%); }
CSS3滤镜目前还不属于标准属性，现在暂时只有Google Chrome浏览器支持，其他浏览器均不支持 
CSS实现灰度效果可以让网页更加容易实现一些特效，还记得纪念5.12大地震国内很多网页在一夜之间全部变成灰色吗？给网站加上下面的样式就可以轻松实现： 
1html { filter:Gray; -webkit-filter: grayscale(100%); }
有人肯定要说了CSS实现灰度效果虽好，但不能做到兼容全部浏览器呀，怎么办呢？CSS做不到的事情，大家肯定会想到用JS来实现，下面给大家推荐一款JS灰度效果插件：
[grayscale.js](http://james.padolsey.com/demos/grayscale/)
使用方法：
//引入插件
<script type="text/javascript" src="grayscale.js"></script>
//调用插件
<script type="text/javascript">
    window.onload = function(){
        var el = document.getElementById( 'body' );
        grayscale( el );
    };
</script>
兼容所有浏览器，执行后会在元素上加入很多行内样式，如果你对兼容性要求不高的话建议使用CSS实现灰度效果。
