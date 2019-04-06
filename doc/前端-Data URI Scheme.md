# 前端-Data URI Scheme

 

了解Data URI scheme，首要要掌握一些URI、URL的基本知识，很多做移动端上开发的同学对这两个基本概念掌握的不够，本文首先会对这两个基本概念做一些简单的介绍。

## 基本概念

> 《HTTP权威指南》对URI和URL的定义：

```
URI（Uniform Resource Identifier）:统一资源标识符,服务器资源名被称为统一资源标识符。
URL（Uniform Resource Locator）:统一资源定位符，描述了一台特定服务器上某资源的特定位置。
URN（Uniform Resource Name）:统一资源名称
```

> URL组成：

```
协议://主机名[:端口]/ 路径/[:参数] [?查询]#Fragment

protocol :// hostname[:port] / path / [:parameters][?query]#fragment
```

示例：[http://www.jianshu.com/writer#/notebooks/3670815/notes/3395927/preview](https://www.jianshu.com/writer#/notebooks/3670815/notes/3395927/preview)

> URI,URL,URN三者关系：

```
URL,URN是URI的子集
```



![img](https://upload-images.jianshu.io/upload_images/1790518-377e59a4b6394bfc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/180)

## 什么是Data URI Scheme

data URI scheme 允许我们使用内联（inline-code）的方式在网页中包含数据，目的是将一些小的数据，直接嵌入到网页中，从而不用再从外部文件载入。常用于将图片嵌入网页。

> 传统的图片HTML是这样用的,

```
img src="images/image.png"/
```

> Data URI的图片内嵌式是这样用的,

```
img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABcAAAAWCAYAAAArdgcFAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAACXBIWXMAAAsTAAALEwEAmpwYAAABSUlEQVQ4Ee1VPUsDQRB9s3dBRCRNQvCjkDQW/hJ/gb9AOwWDoIXFgYXYXGP+hYW1/gIr7WzsE8VGxZBcwt04dznCJWw2twtWutXMmzdvlrd7t4SZVbvi1ZihZuCFqT9A/B7Qd5FI44RpPcQpE1oC1IsEq5jRBeGyc0zXaV8mvhbyOREurIRMZMZhp0VthYB9mXBi4jrUztIev1bNbKg6CMxvIWwg5GWVDOHNZ7lX6l/wrG+Fzbh/ca1bf9QWBhKtHznobgtjKF92ZBL3TUWpRbK7LhiVCY8kY3xK/iTiexNcEywSXxKBrfHvrdBN2JRsp4BoQ3dbtHLT4K+JqxWw8vr4YDaf+vR+SmXRWw99lT9N96VaypIYtwgoyWyJRtiX3T+X7TXxROcxGeAo5chlyNcBVxrb2JVpTUHsz4IQi/DL6wPucENxqvoDx69PXP8OKn4AAAAASUVORK5CYII="/
```

> Data URI的直接通过url传递方式，

```
data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABcAAAAWCAYAAAArdgcFAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAACXBIWXMAAAsTAAALEwEAmpwYAAABSUlEQVQ4Ee1VPUsDQRB9s3dBRCRNQvCjkDQW/hJ/gb9AOwWDoIXFgYXYXGP+hYW1/gIr7WzsE8VGxZBcwt04dznCJWw2twtWutXMmzdvlrd7t4SZVbvi1ZihZuCFqT9A/B7Qd5FI44RpPcQpE1oC1IsEq5jRBeGyc0zXaV8mvhbyOREurIRMZMZhp0VthYB9mXBi4jrUztIev1bNbKg6CMxvIWwg5GWVDOHNZ7lX6l/wrG+Fzbh/ca1bf9QWBhKtHznobgtjKF92ZBL3TUWpRbK7LhiVCY8kY3xK/iTiexNcEywSXxKBrfHvrdBN2JRsp4BoQ3dbtHLT4K+JqxWw8vr4YDaf+vR+SmXRWw99lT9N96VaypIYtwgoyWyJRtiX3T+X7TXxROcxGeAo5chlyNcBVxrb2JVpTUHsz4IQi/DL6wPucENxqvoDx69PXP8OKn4AAAAASUVORK5CYII=
```

在浏览器地址栏中输入以上代码，可直接显示一个蓝色图片。

![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABcAAAAWCAYAAAArdgcFAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAACXBIWXMAAAsTAAALEwEAmpwYAAABSUlEQVQ4Ee1VPUsDQRB9s3dBRCRNQvCjkDQW/hJ/gb9AOwWDoIXFgYXYXGP+hYW1/gIr7WzsE8VGxZBcwt04dznCJWw2twtWutXMmzdvlrd7t4SZVbvi1ZihZuCFqT9A/B7Qd5FI44RpPcQpE1oC1IsEq5jRBeGyc0zXaV8mvhbyOREurIRMZMZhp0VthYB9mXBi4jrUztIev1bNbKg6CMxvIWwg5GWVDOHNZ7lX6l/wrG+Fzbh/ca1bf9QWBhKtHznobgtjKF92ZBL3TUWpRbK7LhiVCY8kY3xK/iTiexNcEywSXxKBrfHvrdBN2JRsp4BoQ3dbtHLT4K+JqxWw8vr4YDaf+vR+SmXRWw99lT9N96VaypIYtwgoyWyJRtiX3T+X7TXxROcxGeAo5chlyNcBVxrb2JVpTUHsz4IQi/DL6wPucENxqvoDx69PXP8OKn4AAAAASUVORK5CYII=)



> Data URI的格式规范,

```
data:[<mime type>][;charset=<charset>][;<encoding>],<encoded data>
1.  data ：协议名称；
2.  [<mime type>] ：可选项，数据类型（image/png、text/plain等）
3.  [;charset=<charset>] ：可选项，源文本的字符集编码方式
4.  [;<encoding>] ：数据编码方式（默认US-ASCII，BASE64两种）
5.  ,<encoded data> ：编码后的数据
```

目前，Data URI scheme支持的类型有：

```
data:,                            文本数据
data:text/plain,                    文本数据
data:text/html,                  HTML代码
data:text/html;base64,            base64编码的HTML代码
data:text/css,                    CSS代码
data:text/css;base64,              base64编码的CSS代码
data:text/javascript,              Javascript代码
data:text/javascript;base64,        base64编码的Javascript代码
data:image/gif;base64,            base64编码的gif图片数据
data:image/png;base64,            base64编码的png图片数据
data:image/jpeg;base64,          base64编码的jpeg图片数据
data:image/x-icon;base64,          base64编码的icon图片数据
```

## Data URI Scheme优缺点

> 优点：

```
1. 减少资源请求链接数。
2. 当访问外部资源很麻烦或受限时，可以很好的利用Data URI Scheme
```

> 缺点：

```
1. Data URL形式的图片不会被浏览器缓存，这意味着每次访问这样页面时都被下载一次，
   但可通过在css文件的background-image样式规则使用Data URI Scheme，使其随css文件一同被浏览器缓存起来）。
2. Base64编码的数据体积通常是原数据的体积4/3，
   也就是Data URL形式的图片会比二进制格式的图片体积大1/3。
3. 移动端性能比较低。
```

关于移动端的性能可以参考 Peter McLachlan在13年写的一片文章：[http://www.mobify.com/blog/data-uris-are-slow-on-mobile/](https://link.jianshu.com?t=http://www.mobify.com/blog/data-uris-are-slow-on-mobile/)



![img](https://upload-images.jianshu.io/upload_images/1790518-03ce4f648645583e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

image_0.png

数据比较老，后面如有时间我会对目前主流机型及系统进行一次全面的测试，并整理另一篇相关文章

### Data URI Scheme适用场景：

```
1. 当访问外部资源很麻烦或受限时。
2. 当图片是在服务器端用程序动态生成，每个访问用户显示的都不同时。
3. 当图片的体积太小，占用一个HTTP会话不是很值得时。
```

绝大多数的现代浏览器都支持data URI，关于各浏览器URL长度，网上给的很多数据，经测试都不是很准确，Safari经验证可支持80,000以上字节。

## Thanks

[http://www.mobify.com/blog/data-uris-are-slow-on-mobile/](https://link.jianshu.com?t=http://www.mobify.com/blog/data-uris-are-slow-on-mobile/)
 [http://aiyouu.net/data-uris-explained/](https://link.jianshu.com?t=http://aiyouu.net/data-uris-explained/)
 ...