
# jsPDF – 基于 HTML5 的强大 PDF 生成工具 - 阳光岛主 - CSDN博客

2017年12月24日 13:18:08[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9405


jsPDF 是一个基于 HTML5 的客户端解决方案，用于生成各种用途的 PDF 文档。
使用方法很简单，只要引入 jsPDF 库，然后调用内置的方法就可以了。
米扑科技项目用到了HHTML5生成PDF，原文详见米扑博客：
[jsPDF – 基于 HTML5 的强大 PDF 生成工具](https://blog.mimvp.com/article/16204.html)

**浏览器兼容性：**
IE 10, Firefox 3+, Chrome, Safari 3+, Opera，未来将兼容 IE 10 以下版本，对于 IE10 以下的版本会使用 Downloadify 来实现文件下载功能。
**支持文件格式：**
文本，数字，图形，图片，同时你可以自由的编辑标题或者其它类型元素。
曾经生成PDF都是服务器端代码的专利，在今天的这篇文章中，我们将介绍一个JavaScript类库 - jsPDF，使用它能够帮助你使用前端脚本生成PDF文件，是不是很棒，试试吧！
支持互动的内容制作，例如，你可以输入文字或者数字，然后jsPDF帮助生成最后的PDF内容。
支持现代浏览器，如果是老式浏览器的话，可以很好的使用flash来实现兼容。不过貌似支持Firefox不是很好，如果要查看演示，使用Chrome吧！
官网地址：[http://jspdf.com](http://jspdf.com/)[https://parall.ax/products/jspdf](https://parall.ax/products/jspdf)
下载地址：[https://github.com/MrRio/jsPDF](https://github.com/MrRio/jsPDF)
**jquery生成pdf插件jsPDF**
示例网址：[http://www.jq22.com/jquery-info517](http://www.jq22.com/jquery-info517)
示例演示：[http://www.jq22.com/yanshi517](http://www.jq22.com/yanshi517)
在客户端JavaScript生成PDF文件。
创建您的第一个文档
看examples/basic.html例子。
|1
|2
|3
|var|doc =|new|jsPDF();
|doc.text(20, 20,|'Hello world.'|);
|doc.save(|'Test.pdf'|);
|


### 使用示例
**1、文本**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|var|doc =|new|jsPDF();
|doc.text(20, 20,|'This is the default font.'|);
|doc.setFont(|"courier"|);
|doc.setFontType(|"normal"|);
|doc.text(20, 30,|'This is courier normal.'|);
|doc.setFont(|"times"|);
|doc.setFontType(|"italic"|);
|doc.text(20, 40,|'This is times italic.'|);
|doc.setFont(|"helvetica"|);
|doc.setFontType(|"bold"|);
|doc.text(20, 50,|'This is helvetica bold.'|);
|doc.setFont(|"courier"|);
|doc.setFontType(|"bolditalic"|);
|doc.text(20, 60,|'This is courier bolditalic.'|);
|

**2、图片**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|// You'll need to make your image into a Data URL
|// Use http://dataurl.net/\#dataurlmaker
|var|imgData = 'data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEASABIAAD/4ge....../2Q==|';
|var doc = new jsPDF();
|doc.setFontSize(40);
|doc.text(35, 25, "Octonyan loves jsPDF");
|doc.addImage(imgData, '|JPEG', 15, 40, 180, 180);
|

**3、HTML**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|var|doc =|new|jsPDF();
|// We'll make our own renderer to skip this editor
|var|specialElementHandlers = {
|'|\#editor': function(element, renderer){
|return|true|;
|}
|};
|// All units are in the set measurement for the document
|// This can be changed to "pt" (points), "mm" (Default), "cm", "in"
|doc.fromHTML($('|\#render_me').get(0), 15, 15, {
|'width|': 170,
|'|elementHandlers': specialElementHandlers
|});
|



