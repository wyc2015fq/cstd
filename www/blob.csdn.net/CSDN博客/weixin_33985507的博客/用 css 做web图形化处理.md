# 用 css 做web图形化处理 - weixin_33985507的博客 - CSDN博客
2018年10月11日 13:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：26
![5072851-91ebe62c52bc08ad.jpg](https://upload-images.jianshu.io/upload_images/5072851-91ebe62c52bc08ad.jpg)
图片来自 CHRIS COYIER 
在前端开发中，你或多或少都会接触到css图形。之前我整理过 [伪元素&伪类](https://github.com/lvzhenbang/pseudo) 的内容，为的就是更加熟悉了解它们，以便更好的使用它们。同时也也使用它一做了一些动画。 [css3-animate](https://github.com/lvzhenbang/css3-animate)。
这里说的用css做图形，其实是使用一个html元素，结合它的伪元素 `::before & ::after` （不需要其他额外的非伪元素的html元素），然后定义样式来生成所需的图形。
这里不是说不可以使用其它的html元素，只是这样的好处是在html方便引入，而不需要每次引入都需要添加大量的html片段（夸张说法）。
### 主要方法
可以使用的样式属性如下：
```
* border
* border-radius
* transform
* box-shadow
```
为了兼容性，使用pseudo-elements(::before, ::after) 和上面样式属性中的一种或几种随机组合。
当然，在前端我们经常使用的时svg和font-icon。
还用一种是css3的shape-outside实现文本环绕的效果。
这种单元素的css图形还是比较实用的。
### 图形分类
- 几何图形。如：三角形，正方形，矩形，平行四边形，五边形，六边形等
- 星形
- 卡通图形。如：吃豆人等
- 图标。 如：心形，箭头，锁，放大镜等。
具体的图形列表和代码可参考[github](https://github.com/lvzhenbang/pseudo/blob/master/shape.md)
