# css小结 - 左直拳的马桶_日用桶 - CSDN博客
2017年12月26日 20:12:18[左直拳](https://me.csdn.net/leftfist)阅读数：237标签：[css																[import																[sass																[less](https://so.csdn.net/so/search/s.do?q=less&t=blog)](https://so.csdn.net/so/search/s.do?q=sass&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
CSS的外链有两种方式，一个是耳熟能详的
`<link href="bootstrap.min.css" rel="stylesheet" type="text/css" />`
另一种则是
`@import url(play/timeslider.css);`
第二种方式，可以直接写在样式里，形成一种CSS直接引用别的CSS的效果。当然，在页面中多写一个`<link>` 也可以做到，但我觉得，那毕竟是在页面上，没有CSS自己搞定那么爽。
另外，css中的background-image属性，url是指向该css文件的相对路径的。这样有时就会带来一些好处，结合上面说的@import，有时在写JS插件的时候比较便利：
比如有如图文件结构： 
![这里写图片描述](https://img-blog.csdn.net/20171226195747528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
play.js有一个并列的文件夹`<play>` ，里面有各种图片，以及一个css。play.js是我写的一个插件，所谓插件也者，当然追求尽可能多的独立性。其中图片，CSS都尽量不依赖外部传入的路径。
像以下代码，CSS文件的路径是外部传入的，但里面的图片则不必：
play.js:
```xml
textDiv.innerHTML = '<style>' +
    '@import url(' + _libpath + 'play/timeslider.css);' + //<---------
    '#' + slider_id + ' {' +
    'text-align: center;' +
    'background-color:#fff;' +
    'width:' + ((dates.length + 1) * 80 + 80 * 2) + 'px;' +
    'height:40px;' +
    'margin:auto;' +
    'padding-top:10px;' +
    'opacity: 0.8;' +
    'filter: alpha(opacity=80); /* 针对 IE8 以及更早的版本 */' +
    'border:solid 1px #AAC2C8;' +
    'border-radius: 5px;' +
    '}' +
    '</style>' +
    '<div id="' + slider_id + '"></div>';
```
timeSlider.css
```
.esriTimeSlider .tsPrevButton {
    background-image: url(goBack.gif); /*该图片与本css文件同一目录*/
    background-position: 0px 0px;
}
.esriTimeSlider .tsNextButton {
    background-image: url(goNext.gif);/*该图片与本css文件同一目录*/
    background-position: 0px 0px;
}
```
以上总结，属于技的范畴，跟架构设计比起来，就是雕虫小技。但是，一个项目，既要有架构设计，也要有具体实现。当然，像我这样几十岁还在总结这个，怕是很不妥的。
另外，今天了解到，sass,less等并不是CSS的加强版，而是用于方便书写css，最后需要编译成CSS，浏览器并不能直接执行。就好像coffscript，写起来很简洁，但始终要编译成javascript。
