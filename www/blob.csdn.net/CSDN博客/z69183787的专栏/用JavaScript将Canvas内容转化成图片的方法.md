# 用JavaScript将Canvas内容转化成图片的方法 - z69183787的专栏 - CSDN博客
2017年08月02日 17:45:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：843
上周我们花了半天时间开发下一个准备放进Mozilla Marketplace的应用。有一个应用现在非常的火热，那就是Instagram，Facebook花了100万美元收购了它。我们也想有100万美元装到口袋里，我决定开发一个Instagram风格的应用，这篇文章了我将介绍一下如何将一张图片拷贝到canvas里，以及反过来，如何将画布内容保存成图片格式。
## 使用JavaScript将图片拷贝进画布
要想将图片放入画布里，我们使用canvas元素的`drawImage`方法：
// Converts image to canvas; returns new canvas elementfunctionconvertImageToCanvas(image){var canvas = document.createElement("canvas");
	canvas.width = image.width;
	canvas.height = image.height;
	canvas.getContext("2d").drawImage(image,0,0);return canvas;}
这里的`0, 0`参数画布上的坐标点，图片将会拷贝到这个地方。
## 用JavaScript将画布保持成图片格式
如果你的画布上的作品已经完成，你可以用下面简单的方法将canvas数据转换成图片格式：
// Converts canvas to an imagefunctionconvertCanvasToImage(canvas){var image =newImage();
	image.src = canvas.toDataURL("image/png");return image;}
这段代码就能神奇的将canvas转变成PNG格式！
这些在图片和画布之间转换的技术可能比你想象的要简单的多。在以后的文章里，我会写一些将这些图片做不同滤镜处理的技术。
