# 【静态页面架构】CSS之盒子模型 - weixin_33985507的博客 - CSDN博客
2018年08月28日 14:22:56[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## CSS架构
### 盒子模型；
以内容区(显示文本和图像)
内边距(内容区至边距的距离)
边距(内容区的边界)
外边距(元素的边框之间的距离)
# 1.边距；
## border属性；
简写属性用来设置边距的上(top)右(right)下(bottom)左(left)。宽度，颜色和样式
```
<style>
        div{
            width: 200px;
            height: 200px;
            background-color: blue;
            border-right: 5px double darkgray;
            border-bottom: 10px dashed red;
            border-left: 15px dotted lime;
        }
    </style>
</head>
<body>
<div></div>
</body>
```
## 盒子的阴影；
以box-shadow属性设置盒子阴影效果
offset-x表示阴影的水平方向偏移量；
负值为左偏移正值为右偏移
offset-y表示阴影的垂直方向偏移量；
负值为左偏移正值为右偏移
```
<style>
        div{
            width:200px;
            height: 200px;
            background-color: blue;
            box-shadow: 3px 3px 3px black;
            /*设置盒子阴影*/
        }
    </style>
</head>
<body>
<div></div>
</body>
```
## 边框圆角；
以border-radius定义来设置边距的圆角；直角，圆角还是椭圆角
```
<style>
        div {
            width: 100px;
            height: 100px;
        }
        #qq {
            background-color:  red;
            border-radius: 25px;
        }
        #ww {
            background-color:  blue;
            border-radius: 50px;
        }
        #ee {
            background-color: lime;
            border-radius: 50px 25px;
        }
        img {
            border-radius: 50px;
        }
    </style>
</head>
<body>
<div id="qq"></div>
<div id="ww"></div>
<div id="ee"></div>
<img src="imgs/26.jpg" width="100px">
</body>
```
## 边框图像；
以border-image定义来设置在元素的边框上的图像
```
<style>
        div {
            width: 100px;
            height: 100px;
            /*background-color: lightcoral;*/
            border: 30px solid transparent;
            /*
                边框的图像 - 类似于背景图像的设置
                * border-image-source - 设置边框图像的路径
                * border-image-width - 设置边框图像的宽度
                * border-image-repeat - 设置边框图像的平铺方式
             */
            border-image: url("https://mdn.mozillademos.org/files/4127/border.png") 30;
        }
    </style>
</head>
<body>
<div>爱新觉罗</div>
</body
```
# 2.内边距；
## paddying属性；
也属于简写属性用来设置盒子的内边距上，右，下，左
```
<style>
            #qq{
                width: 200px;
                height: 200px;
                background-color: blue;
                padding-top: 50px;
                <!---->
                padding-right: 100px;
                padding-bottom: 150px;
                padding-left: 200px;
            }
            #hh{
                width: 200px;
                height: 200px;
                background-color: red;
            }
        </style>
</head>
<body>
<div id="qq">
    <div id="hh"></div>
</div>
</body>
```
# 3.外边距；
上外边距和左外边距；
以margin-left设置元素的水平方向的位置
值为正数，当前元素向右移动
值为负数，当前元素向左移动
  margin-top设置元素垂直方向的位置
值为正数，当前元素向下移动
值为负数，当前元素向上移动
## 上外边距和右外边距
以margin-bottom设置下一个元素的位置
值为正数，下一个兄弟元素向下移动
值为负数，下一个兄弟元素向上移动
  margin-right设置下一个元素的位置
值为正数，下一个兄弟元素向右移动
值为负数，下一个兄弟元素向左移动
```
<style>
        div {
            display: inline-block;
        }
        #d1 {
            width: 200px;
            height: 200px;
            background-color: lightcoral;
            /*
                margin-bottom 下外边距控制块级元素的下一个兄弟元素的位置
                * 如果值为正数的话，下一个兄弟元素向下移动
                * 如果值为负数的话，下一个兄弟元素向上移动
             */
            /*margin-bottom: -100px;*/
            /*
                margin-right 右外边距控制内联元素或行内块级元素的下一个兄弟元素的位置
                * 如果值为正数的话，下一个兄弟元素向右移动
                * 如果值为负数的话，下一个兄弟元素向左移动
             */
            /*margin-right: -5px;*/
        }
        #d2 {
            width: 200px;
            height: 200px;
            background-color: yellowgreen;
            margin-left: -5px;
        }
    </style>
</head>
<body>
<div id="d1"></div>
<div id="d2"></div>
</body>
```
## 外边距重叠；
以设置两个相邻的元素的外边距，第一个设置下外边距第二个设置上外边距
```
<style>
        #qq{
            width: 200px;
            height: 200px;
            background-color: blue;
            margin-bottom: 100px;
            /*id为ww的div元素向下移动100px*/
        }
        #ww{
            width: 200px;
            height: 200px;
            background-color: red;
            margin-top: 200px;
            /*id为ww的div元素向下移动200px*/
        }
    </style>
</head>
<body>
<div id="qq"></div>
<!--<p>爱新觉罗</p>-->
<div id="ww"></div>
</body>
```
## 外边距传递；
以设置两个元素师父子元素，当子元素设置上外边距，该上边距会在父级元素内出现
```
<style>
        #qq{
            width: 200px;
            height: 100px;
            background-color: blue;
            /*设置背景颜色*/
            padding-top: 100px;
            /*为子级元素设置上边距*/
        }
        #ww{
            width:100px;
            height: 100px;
            background-color: red;
            /*设置背景颜色*/
        }
    </style>
</head>
<body>
<div id="qq">
    <!--必须是父级与子级关系的元素-->
    <div id="ww"></div>
</div>
</body>
```
## 内容水平居中；
以margin: 0 auto；父级div设置在元素的水平居中
```
<style>
        p {
            /*该属性只对文本内容水平对齐*/
            text-align: center;
            /*文本水平方向居中*/
        }
        div{
            width: 100px;
            height: 100px;
            background-color: blue;
            margin: 0 auto;
        }
    </style>
</head>
<body>
<p>星际战甲</p>
</body>
```
# 4.盒子模型；
## 块级元素盒子模型；
可设置宽高，设置高度为其中内容的高度
```
<style>
        div{
            width: 200px;
            height: 200px;
            background-color: blue;
            border: 1px solid black;
            /*设置边框的样式*/
            padding: 50px;
            /*设置内边距*/
            margin: 50px;
            /*设置外边距*/
        }
    </style>
</head>
<body>
<div></div>
</body>
```
## 内块级元素盒子模型；
是不可设置宽和高，元素的大小事其中内容撑起
```
<style>
        span{
            width: 200px;
            height: 200px;
            background-color: red;
            border: 1px solid black;
            padding: 30px;
            /*水平方向内边距是有效；设置文本内容的水平方向位置*/
            /*垂直方向内边距是有效；文本内容位置没有变，而内边距会向上或向下扩展*/
            margin: 30px;
            /*垂直方向上或下的外边距是无效的
              而水平方向的外边距是有效的*/
        }
    </style>
</head>
<body>
<span>星际战甲</span>
</body>
```
## 行内块级元素盒子模型；
不自动换行，可设置宽高；
```
<style>
        div{
            width: 100px;
            height: 100px;
            border: 1px solid black;
            padding: 30px;
            /*设置内边距*/
            margin: 30px;
             /*设置外边距*/
        }
        #qq{
            background-color: red;
        }
        #hh{
            background-color: blue;
        }
    </style>
</head>
<body>
<div id="qq"></div>
<div id="hh"></div>
</body>
```
## box-sizing属性；
以box-sizing属性设置盒子模型；
content-box；为默认值为标准盒子模型
设置为内容区；内边距；边框；外边框
border-box；为怪异盒子模型
设置盒子的大小；外边距
```
#qq {
            width: 200px;
            height: 200px;
            /*
                默认值，标准盒子模型
                * 实际的宽度 = width + padding-left + padding-right + border-left + border-right
                * 实际的高度 = height + padding-top + padding-bottom + border-top + border-bottom
             */
            box-sizing: content-box;
            border: 10px solid black;
            <!--设置边框-->
            padding: 50px;
            /*设置内边距*/
            margin: 50px;
            /*设置外边距*/
            background-color: lightcoral;
        }
        #hh {
            width: 200px;
            height: 200px;
            /*
                怪异盒子模型
                * 实际宽度 = width
                * 实际高度 = height
             */
            box-sizing: border-box;
            border: 10px solid black;
            <!--设置边框-->
            padding: 50px;
            /*设置内边距*/
            margin: 50px;
            /*设置外边距*/
            background-color: green;
        }
    </style>
</head>
<body>
<div id="qq"></div>
<div id="hh"></div>
```
