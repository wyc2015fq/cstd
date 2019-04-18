# Skywind Inside » ASCII Art Algorithm
## ASCII Art Algorithm
September 20th, 2007[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
因为希望将图片转换成字符以后可以方便的帖到论坛或者BBS上，所以画时间写了这个算法。现有很多算法都是将一个点匹配成一个字符，这样转换工作只是简单的将点亮度查表后换成ASCII字符而已，但是其实这样做的效果并不十分好，首先80×25的字符屏幕就只能表示80×25个点，无法充分发挥单个字符的字形特点，而且图片很多精度和细节都丢失了。比如下面这个连接：
http://www.sebastian-r.de/asciiart/exe.php?image=http%3A%2F%2Fwww.sebastian-r.de%2Fasciiart%2Fpng%2Fcatherine.png&resolution=3&mode=1&color=%23000000&font-size=11&line-height=9&letter-spacing=0&fixed_char=W&new_window=on
所以我的算法主要是匹配周围一部分点到ASCII字符，这样斜线能够顺利匹配成“/”，其他形状的东西也能够顺利按照字形特点进行匹配，因此同样80×25个点，但是后者所能够表达的像素点更多，细节度更加丰富：
![](../wp-content/uploads/2011/04/ascii_art_2.jpg)
![](../wp-content/uploads/2011/04/ascii_art_1.gif)
上面的效果是作了误差扩散的，其实转换成ASCII字符时不做也可以，只是说希望转换前的局部/整体亮度等于转换后亮度，能量守恒一些而已。其实如果用同 一种颜色的64个常见字符表达精度和细节度很高，对比度不高的图片还是比较困难的，可以进一步优化的方法也有几种，其一是对照片作拉普拉斯变换，将噪声过 滤掉再取出轮廓，这样转换出来的就是仅仅包含轮廓的对比度很高的图片了。或者将图片频谱中能量不高的，比较弱的频率去掉，留下能量高的频率，这样图片看起 来更干净一些，只是后面这两种方法就无法保证实时渲染了。
