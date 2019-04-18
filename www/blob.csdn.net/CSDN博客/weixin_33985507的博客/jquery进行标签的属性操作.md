# jquery进行标签的属性操作 - weixin_33985507的博客 - CSDN博客
2018年03月14日 23:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
1、html() 取出或设置html内容
```
// 取出html内容
var $htm = $('#div1').html();
// 设置html内容
$('#div1').html('<span>添加文字</span>');
```
2、prop() 取出或设置某个属性的值
```
// 取出图片的地址
var $src = $('#img1').prop('src');
// 设置图片的地址和alt属性
$('#img1').prop({src: "test.jpg", alt: "Test Image" });
```
