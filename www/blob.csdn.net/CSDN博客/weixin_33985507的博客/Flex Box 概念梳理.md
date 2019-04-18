# Flex Box 概念梳理 - weixin_33985507的博客 - CSDN博客
2018年11月08日 18:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
## Flex Box
Flexible Box 模型，通常被称为 flexbox，是一种一维的布局模型。
#### 之所以说 flexbox 是一种一维的布局，是因为一个 flexbox 一次只能处理一个维度上的元素布局，一行或者一列。作为对比的是另外一个二维布局CSS Grid Layout，可以同时处理行和列上的布局。
采用 Flex 布局的元素，称为 Flex 容器（flex container），容器里的子元素称为项目（item）。
## 1.容器
- 
#### 概念
![13661853-02708ad7f5d43895.png!web](https://upload-images.jianshu.io/upload_images/13661853-02708ad7f5d43895.png!web)
从容器的角度来看，有两个不变的轴，主轴由 flex-direction 定义，另一根轴垂直于它。默认定义是：
​ 1) **从左往右的主轴** （main axis），主轴开始位置称为 `main start` ，结束位置称为 `main end`
​ 2) **从上到下的交叉轴** （cross axis），交叉轴开始位置称为 `cross start` ，结束位置称为 `cross end`
- 
#### 相关属性
- flex-direction：定义了主轴方向。决定容器内项目排列方向，默认从左到右 `row` 。 `column` 表示容器内项目沿着主轴排列， `reverse` 表示沿着交叉轴排列。
```
flex-direction：row | row-reverse | column | column-reverse
```
特别注意：flex-direction设置后会引起项目排列方向改变。
![13661853-eee70352382fc51e.png!web](https://upload-images.jianshu.io/upload_images/13661853-eee70352382fc51e.png!web)
- flex-wrap：决定容器主轴方向上项目是否换行，默认不换行 `nowrap` 。 `reverse` 表示反向。
```
flex-wrap: nowrap | wrap | wrap-reverse;
```
![13661853-b9c6cf4f26952f00.png!web](https://upload-images.jianshu.io/upload_images/13661853-b9c6cf4f26952f00.png!web)
- flex-flow：是 `flex-direction` 和 `flex-wrap` 属性的简写。默认为： `row nowrap` 。
```
flex-flow: <flex-direction> || <flex-wrap>;
```
- justify-content：决定容器内项目 **在主轴上** 对齐方式。默认左对齐 `flex-start` 。
```
justify-content: flex-start | flex-end | center | space-between | space-around;
    /**
    * flex-start（默认值）：左对齐。项目堆放在容器主轴开始位置(main start)；
    * flex-end：右对齐。项目堆放在容器主轴结束位置(main end)；
    * center： 居中。项目沿着主轴居中排列；
    * space-between：两端对齐，项目之间的间隔都相等。
    * space-around：每个项目两侧的间隔相等。所以，项目之间的间隔比项目与边框的间隔大一倍。
    **/
```
![13661853-b18403bcca139158.png!web](https://upload-images.jianshu.io/upload_images/13661853-b18403bcca139158.png!web)
- align-items：决定容器内项目 **在交叉轴上** 对齐方式。默认为 `stretch` 。
```
align-items: flex-start | flex-end | center | baseline | stretch;
    /**
    * flex-start：交叉轴的起点对齐。
    * flex-end：交叉轴的终点对齐。
    * center：交叉轴的中点对齐。
    * baseline: 项目的第一行文字的基线对齐。
    * stretch（默认值）：如果项目未设置高度或设为auto，将占满整个容器的高度。
    **/
```
![13661853-26be81410a8bb831.png!web](https://upload-images.jianshu.io/upload_images/13661853-26be81410a8bb831.png!web)
![13661853-b38a891ed93e8c4d.png!web](https://upload-images.jianshu.io/upload_images/13661853-b38a891ed93e8c4d.png!web)
![13661853-8431a815b737d526.png!web](https://upload-images.jianshu.io/upload_images/13661853-8431a815b737d526.png!web)
- 
align-content：决定容器内项目 **在交叉轴上** 内容与剩余空间对齐方式。默认为 `stretch` 。
特别注意：该属性只对设置了`flex-wrap:wrap`的容器生效，并且容器里的元素构成了多行。
```
align-content: flex-start | flex-end | center | space-between | space-around | stretch;
    /**
    * flex-start：与交叉轴的起点对齐。
    * flex-end：与交叉轴的终点对齐。
    * center：与交叉轴的中点对齐。
    * space-between：与交叉轴两端对齐，轴线之间的间隔平均分布。
    * space-around：每根轴线两侧的间隔都相等。所以，轴线之间的间隔比轴线与边框的间隔大一倍。
    * stretch（默认值）：轴线占满整个交叉轴。
    **/
```
## 2.项目
- 
#### 概念
项目默认沿主轴排列。单个项目占据的主轴空间叫做 `main size` ，占据的交叉轴空间叫做 `cross size` 。
- 
#### 相关属性
- order：定义项目在容器 **主轴方向上的** 排列顺序，数值越小排列越靠前。默认为 0 。
```
order: <integer>;
```
![13661853-ca34c9d6bac57088.png!web](https://upload-images.jianshu.io/upload_images/13661853-ca34c9d6bac57088.png!web)
- flex-grow：定义项目的放大比例，默认为 0 ，即便容器存在剩余空间，也不放大。不允许负值。
```
flex-grow: <number>; /* default 0 */
    /**
    * 如果所有项目的flex-grow属性都为1，则它们将等分剩余空间（如果有的话）。
    * 如果一个项目的flex-grow属性为2，其他项目都为1，则前者占据的剩余空间将比其他项多一倍。
    **/
```
![13661853-ce0382264ed337aa.png!web](https://upload-images.jianshu.io/upload_images/13661853-ce0382264ed337aa.png!web)
- flex-shrink：定义项目在容器空间不足时的缩小比例，默认为1，即容器空间不足时该项目要缩小，并且项目内容会换行。不允许负值。
```
flex-shrink: <number>; /* default 1 */
    /**
    * 如果所有项目的flex-shrink属性都为1，当空间不足时，都将等比例缩小。
    * 如果一个项目的flex-shrink属性为0，其他项目都为1，则空间不足时，前者不缩小。
    **/
```
![13661853-99fb14e8c76e9669.png!web](https://upload-images.jianshu.io/upload_images/13661853-99fb14e8c76e9669.png!web)
![13661853-f268fc3324b43d25.png!web](https://upload-images.jianshu.io/upload_images/13661853-f268fc3324b43d25.png!web)
- flex-basis：可以理解成设置项目默认大小。定义项目分配多余空间之前， **在主轴上** 占据的空间。默认为auto，即项目内容大小。
```
flex-basis: <length> | auto; /* default auto */
```
如果容器内所有项目大小总和超过容器，不设置 wrap 的情况下会溢出容器：
![13661853-ba02f3439aca3c3b.png!web](https://upload-images.jianshu.io/upload_images/13661853-ba02f3439aca3c3b.png!web)
- flex：是 `flex-grow` , `flex-shrink` 和 `flex-basis` 的简写，默认值为 `0 1 auto` 。后两个属性可选。
```
flex: none | [ <'flex-grow'> <'flex-shrink'>? || <'flex-basis'> ]
```
建议优先使用这个属性，而不是单独写三个分离的属性，因为浏览器会推算相关值。
- align-self：用于设置单个项目 **在交叉轴上** 对齐方式，可覆盖容器的 `align-items` 属性，默认为 `auto` 。如果值为 `auto` ，则计算值为父元素的 'align-items ' 值，否则为指定值。
```
align-self: auto | flex-start | flex-end | center | baseline | stretch;
```
![13661853-b28fb77ba6e16b4b.png!web](https://upload-images.jianshu.io/upload_images/13661853-b28fb77ba6e16b4b.png!web)
![13661853-74325c0db4958e7a.png!web](https://upload-images.jianshu.io/upload_images/13661853-74325c0db4958e7a.png!web)
![13661853-f22d122894077b62.png!web](https://upload-images.jianshu.io/upload_images/13661853-f22d122894077b62.png!web)
