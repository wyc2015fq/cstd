# javascript更新元素加载延迟及图片src - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2017年11月03日 17:31:08[dutong0321](https://me.csdn.net/dutong0321)阅读数：264
# 问题描述
今天使用javascript改变图片的src，发现了俩个问题： 
1.改变图片的src后，width和height还是原width和height。 
2.javascript更新元素后，浏览器需要时间加载。 
首先上代码：
```xml
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>javascipt更新图片</title>
        <script type="text/javascript" src="js/jquery.min.js"></script>
        <script>
            function normal_change() {
                $("#img_have_height").attr("src", "img/open.jpg");
                $("#img_no_have").attr("src", "img/open.jpg");
                var img_have_height_height = $("#img_have_height").height();
                var img_no_have = $("#img_no_have").height();
                alert(img_have_height_height + " " + img_no_have);
            }
            function real_change() {
                var height_before = document.getElementById("img_have_height").naturalHeight;
                $("#img_have_height").attr("src", "img/open.jpg");
                var height_after = document.getElementById("img_have_height").naturalHeight;
                alert(height_before + " " + height_after);
            }
            function relay_go() {
                $("#img_have_height").attr("src", "img/open.jpg");
                setTimeout('$("#img_have_height").css("height", document.getElementById("img_have_height").naturalHeight);', 100);
            }
        </script>
    </head>
    <body>
        <img id="img_have_height" src="img/imut.jpg" style="height: 134px;" />
        <img id="img_no_have" src="img/imut.jpg" />
        <br />
        <button onclick="normal_change()">普通改变</button>
        <button onclick="real_change()">图片实际大小改变</button>
        <button onclick="relay_go()">更新后执行</button>
    </body>
</html>
```
**开始界面：这里是我母校的俩个校徽，第一个有height属性，第二个没有。**
![](https://img-blog.csdn.net/20171103170300259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**点击“普通改变“：可以看到alert已经打印出来，但是图片还没有改变，而且显示两个图片的高度还是134px。**
![](https://img-blog.csdn.net/20171103170434648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**点击“确定”：确定后，发现有height属性高度和以前保持一致，而没有的则是原图片的宽高。**
![](https://img-blog.csdn.net/20171103170919922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### javascript改变图片的src或者input的value等更新元素的操作需要一定加载时间
#### javascript改变图片的src后，会继承改变之前图片设置的width和height属性
关于第二点，W3C school中有提到 [http://www.w3school.com.cn/jsref/prop_img_src.asp](http://www.w3school.com.cn/jsref/prop_img_src.asp)
对于对付第二点来讲，在html5中有了新的方法 img.naturalHeight和img.naturalWidth 是获取图片源的实际高度和宽度。 
**不刷新页面后，点击“图片实际大小改变”：134是改变前图片源的实际大小，没问题，为什么改变后的图片源的实际大小是0？这是因为改变后的图片还没有加载成功，就开始输出了提示信息。**
![](https://img-blog.csdn.net/20171103171949345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
关于判断图片完全载入，这里有个不错的博客 [http://www.cnblogs.com/snandy/p/3704938.html](http://www.cnblogs.com/snandy/p/3704938.html)
我采用更加直接暴力的方法，而且通杀所有元素的载入问题，我加了一个延时，不过这样也有BUG，如果网速等原因造成特别慢的载入，我的方法就不好使了。 
**点击“更新后执行”**
![](https://img-blog.csdn.net/20171103172941084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 2017-12-6 重要更新
在测试环境中一直是没问题的，但是今天想了一想觉得还是不可以采用上面那种暴力的办法，万一图片太大或者是网速太慢都是问题。所以，在这里郑重道歉，我建议采用onload方法。 
即：在img属性中采用onload方法，然后在该方法中对图片元素完全加载后的操作，修改后的代码：
```xml
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>javascipt更新图片</title>
        <script type="text/javascript" src="../js/jquery.min.js"></script>
        <script>
            function img_change() {
                $("#img_no_load").attr("src", "../img/open.jpg");
                $("#img_have_load").attr("src", "../img/open.jpg");
            }
            function img_onload() {
                $("#img_have_load").attr("height", 300px);
            }
        </script>
    </head>
    <body>
        <img id="img_no_load" src="../img/imut.jpg" />
        <img id="img_have_load" onload="img_load()" src="../img/imut.jpg" />
        <br />
        <button onclick="img_change()">执行</button>
    </body>
</html>
```
可以看到，onload在改变图片源后，onload会再次执行。所以，我们可以把对图片尺寸的读写都放到这里面。
