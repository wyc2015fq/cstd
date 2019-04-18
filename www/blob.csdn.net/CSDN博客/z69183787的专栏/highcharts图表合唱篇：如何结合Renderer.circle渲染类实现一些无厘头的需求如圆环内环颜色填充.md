# highcharts图表合唱篇：如何结合Renderer.circle渲染类实现一些无厘头的需求如圆环内环颜色填充 - z69183787的专栏 - CSDN博客
2014年12月14日 19:36:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5491
今天与群里的大伙弄了一个很有意思的东西。这个有意思的东西来源于一个想借用饼状图**pie**实现一个环形图，且环形图内环需要设置一个颜色，另外显示最小块的百分比在内环内。至于如何在饼状图的基础上弄成环形图前面已经写了一篇相关性的文章，请参考：[http://www.stepday.com/topic/?739](http://www.stepday.com/topic/?739)。接着我们就来聊聊重点地方吧。
**1、如何将最小块的百分比显示在内环中**
1）、我们如何才能够让每一块区域都显示数据值呢？我们通常是设置plotOptions内的dataLabels的enabled为true，代码见下：
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`1.``plotOptions: {`
`2.``pie: {`
`3.``dataLabels: {`
`4.``enabled:``true``//显示数据值                 `
`5.``}`
`6.``}`
`7.``}`
2）、但是我们需要作出相应的控制，只需要所在百分比最小的才显示数据值，所以我们可以通过配置formatter格式化方法来处理：
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`01.``plotOptions: {`
`02.``pie: {`
`03.``dataLabels: {`
`04.``enabled:``true`
```
,
```
`//显示数据值                 `
`05.``formatter:``function``(){`
`06.``if``(``this`
```
.point.name ==
```
`"Firefox"``)`
`07.``return``""``;`
`08.``else`
`09.``return``this``.percentage.toFixed(2)+``"%"`
```
;
```
`//百分比保留两位小数`
`10.``}`
`11.``}`
`12.``}`
`13.``}`
3）、数据值默认都是显示在外围，所以我们需要通过控制其distance距离值参数来加以调整：
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`1.``plotOptions: {`
`2.``pie: {`
`3.``dataLabels: {`
`4.`
```
distance:-170,
```
`//数据值的距离值`
4）、我们还可以设置数据值的颜色值和字体大小等属性：
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`01.``plotOptions: {`
`02.``pie: {`
`03.``borderColor:``null`
```
,
```
`//块状边框设置为空`
`04.``dataLabels: {`
`05.`
```
distance:-170,
```
`06.``formatter:``function``(){`
`07.``if``(``this`
```
.point.name ==
```
`"Firefox"``)`
`08.``return``""``;`
`09.``else`
`10.``return``this``.percentage.toFixed(2)+``"%"``;`
`11.``},`
`12.``style:{`
`13.``fontSize:``"50px"`
`14.``}`
`15.``},`
`16.``innerSize:``'75%'``,`
`17.``colors: [`
`18.``'#8bbc21'`
```
,
```
`19.``'#0d233a'`
`20.``]`
`21.``}`
`22.``}`
这样一来数据值就显示在了内环中了的。
**2、如何设置内环的颜色**
我们设置了innerSize属性值后，饼状图的内径就变大了，内径空出来的那一块就变得和整个图形区为一体。所以我们无法通过某个属性值来设置内径处的颜色。怎么办呢？下面是一些朋友提供的解决办法：
1）、**@倘若先生**提供的思路：弄一个背景图，通过设置图形区域的margin属性值加以调整，以此达到与背景图的设计融为一体，从视觉上面欺骗用户内环有颜色。
2）、最后由**@微.浅笑**大仙提供了一个采用Renderer类来画一个圆，通过控制其圆的相对位置来稳合环形图。
上面的两种方法都是可行的，但是最终都要解决掉一个很实际的问题，那就是不同浏览器在展现图表的时候环的大小会变动，一旦变动那么用于卡位的Renderer话的circle圆位置就会出现错位现象。
![由于尚未设置一些关键位置属性导致不同浏览器下错位问题](http://www.stepday.com/kindeditor/attached/image/20131213/20131213172987188718.jpg)
最后通过观察是因为木有指定饼状图size大小值以及chart的高宽值问题导致的，因为distance以及circle的x和y坐标都是针对图表的左上角来说的。所以设置这些属性是很有必要的。
**21）、设置chart的高宽值**
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`1.``chart: {`
`2.`
```
type:
```
`'pie'``,`
`3.``height:400,`
`4.``width:400`
`5.``},`
**22）、设置pie的size的直径值**
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`1.``plotOptions: {`
`2.``pie: {`
`3.``size:300,``//图表直径大小`
这样一来我们就可以完美将环形图和圆融合在一起实现内环颜色的渲染效果。效果图如下所示：
![环形图和Renderer圆形完美融合效果展示图](http://www.stepday.com/kindeditor/attached/image/20131213/2013121317370614614.jpg)
**最后贴上完美的示例代码：**
[view source](http://www.stepday.com/topic/?744#viewSource)[print](http://www.stepday.com/topic/?744#printSource)[?](http://www.stepday.com/topic/?744#about)
`01.``$(``function``() {`
`02.``$(``'#container'``).highcharts({`
`03.``chart: {`
`04.`
```
type:
```
`'pie'``,`
`05.``height:400,`
`06.``width:400`
`07.`
```
},
```
`08.``title:{`
`09.``text:``"圆环示例图"`
`10.``},`
`11.``plotOptions: {`
`12.``pie: {`
`13.``size:300,``//图表直径大小`
`14.``borderColor:``null``,`
`15.``dataLabels: {`
`16.`
```
distance:-170,
```
`17.``formatter:``function``(){`
`18.``if``(``this`
```
.point.name ==
```
`"Firefox"``)`
`19.``return``""``;`
`20.``else`
`21.``return``this``.percentage.toFixed(2)+``"%"``;`
`22.``},`
`23.``style:{`
`24.``fontSize:``"50px"`
`25.``}`
`26.``},`
`27.``innerSize:``'75%'``,`
`28.``colors: [`
`29.``'#8bbc21'`
```
,
```
`30.``'#0d233a'`
`31.``]`
`32.``}`
`33.``},`
`34.``tooltip:{`
`35.``enabled:``false`
`36.``},`
`37.``credits:{`
`38.``text:``"www.stepday.com"``,`
`39.``href:``"[http://www.stepday.com](http://www.stepday.com)"``,`
`40.``style:{`
`41.``color:``"#ffffff"`
`42.``}`
`43.``},`
`44.``series: [{`
`45.``data: [`
`46.``[``'Firefox'``,   44.2],`
`47.``[``'IE7'``,       6.6]`
`48.``]`
`49.``}]`
`50.``},``function``(chart){`
`51.``//在图表内的指定位置画一个圆`
`52.``//第一个参数表示圆心距离图表左侧边缘的距离值`
`53.``//第二个参数表示圆心距离图表上侧边缘的距离值`
`54.``//第三个参数表示圆半径大小`
`55.``chart.renderer.circle(200, 212, 115).attr({`
`56.`
```
fill:
```
`'#1aadce'``,`
`57.`
```
stroke:
```
`'black'``,`
`58.``'stroke-width'``: 0`
`59.``}).add();`
`60.``});`
`61.``});`
