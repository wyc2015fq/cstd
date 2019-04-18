# HTML5 Canvas：绘制图片 - weixin_33985507的博客 - CSDN博客
2018年09月18日 11:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
#### HTML示例：
```
<canvas id="canvas" style="width:400;height:300"></canvas>
```
#### JS示例：
```
var context = document.getElementById("canvas").getContext("2d");
var img = new Image;
// 获取图片地址
img.src = "/background.jpg";
// 开启跨域支持
img.crossOrigin = "*";
// 开始绘制的横坐标
var x = 0;
// 开始绘制的纵坐标
var y = 0;
// 绘制的宽度
var width = 400;
// 绘制的高度
var height = 300;
// 图片加载完成后执行
img.onload = function() {
    context.beginPath();
    // 绘制图片
    context.drawImage(img, x, y, width, height);
    context.closePath();
}
```
> 
备注：
- 如果变量 width、height 和 绘制图片的 宽度、高度 一样，那么绘制的图片是原图
- 如果变量 width / height 和 绘制图片的 宽度 / 高度 一样，但是 width 和 宽度不相等，那绘制的图片是原图的 width / 宽度 倍
- 如果变量 width / height 和 绘制图片的 宽度 / 高度 不一样，那么绘制的图片将会变形
