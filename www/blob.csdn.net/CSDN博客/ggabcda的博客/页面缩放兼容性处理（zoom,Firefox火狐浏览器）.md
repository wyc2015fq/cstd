# 页面缩放兼容性处理（zoom,Firefox火狐浏览器） - ggabcda的博客 - CSDN博客





2017年08月29日 21:29:32[ggabcda](https://me.csdn.net/ggabcda)阅读数：140








**1、页面缩放用到的技术点**

**（1）zoom**

```
zoom：normal | <number> | <percentage>
默认值：normal
适用于：所有元素
继承性：有
```

可以使用用浮点数和百分比来定义缩放比例。

zoom的浏览器兼容性：http://caniuse.com/#search=zoom

![](http://images2017.cnblogs.com/blog/757824/201708/757824-20170828163150640-436917803.png)



zoom的兼容性：**firefox 全系列不支持**。

**（2）transform**

```
transform:scale(1.1,1.1);
```

scale(x,y) 定义
**2D 缩放转换**。

兼容性：http://caniuse.com/#search=transform

![](http://images2017.cnblogs.com/blog/757824/201708/757824-20170828164115999-645373681.png)

transform 属于CSS3属于，其兼容性：**IE6-8不支持**。



**2、页面缩放示例代码**

```
<!DOCTYPE html>
<html>

    <head>
        <meta charset="utf-8" />
        <title>页面缩放兼容性处理（zoom,Firefox火狐浏览器）</title>
        <style>
            * {
                margin: 0;
                padding: 0;
                font-size: 14px;
                font-family: "microsoft yahei";
                box-sizing: border-box;
            }
            
            p {
                text-indent: 2em;
                line-height: 25px;
            }
            
            .btn {
                display: inline-block;
                padding: 6px 12px;
                margin-bottom: 0;
                font-size: 14px;
                font-weight: 400;
                line-height: 1.42857143;
                text-align: center;
                white-space: nowrap;
                vertical-align: middle;
                -ms-touch-action: manipulation;
                touch-action: manipulation;
                cursor: pointer;
                -webkit-user-select: none;
                -moz-user-select: none;
                -ms-user-select: none;
                user-select: none;
                background-image: none;
                border: 1px solid transparent;
                border-radius: 4px;
            }
        </style>
    </head>

    <body>
        <button type="button" id="pageBig" class="btn">页面放大</button>
        <button type="button" id="pageSmall" class="btn">页面缩小</button>
        <p>
            前端对于网站来说，通常是指，网站的前台部分包括网站的表现层和结构层。因此前端技术一般分为前端设计和前端开发，前端设计一般可以理解为网站的视觉设计，前端开发则是网站的前台代码实现，包括基本的HTML和CSS以及JavaScript/ajax，现在最新的高级版本HTML5、CSS3，以及SVG等。
        </p>
        <script src="https://cdn.bootcss.com/jquery/1.12.4/jquery.min.js"></script>

        <script type="text/javascript">
            $(function() {
                //初始化默认缩放级别
                var zoomNum = 1;
                var $body = $('body');
                //页面放大函数
                function PageBig() {
                    zoomNum += 0.1;
                    //兼容firefox浏览器代码
                    //需要transform-origin:center top设置，否则页面顶部看不到了
                    $body.css({
                        '-moz-transform': 'scale(' + zoomNum + ')',
                        'transform-origin': 'center top'
                    });
                    $body.css('zoom', zoomNum)
                }
                //页面缩小函数
                function PageSmall() {
                    zoomNum -= 0.1;
                    //兼容firefox浏览器代码
                    //需要transform-origin:center top设置，否则页面顶部看不到了
                    $body.css({
                        '-moz-transform': 'scale(' + zoomNum + ')',
                        'transform-origin': 'center top'
                    });
                    $body.css('zoom', zoomNum);
                }
                $('#pageBig').click(function() {
                    PageBig();
                });
                $('#pageSmall').click(function() {
                    PageSmall();
                });
            })
        </script>
    </body>

</html>
```

效果：

![](http://images2017.cnblogs.com/blog/757824/201708/757824-20170828165252015-1353366501.png)



注意：
transform转换的基准位置属性为**transform-origi**n，transform-origin属性默认值为上下左右中间位置，即：
```
transform-origin:50% 50% 0
```

为**防止页面顶部看不到**，可以对transform-origin进行重新设置：

```
transform-origin: center top;
```





