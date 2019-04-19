# ios阴影圆角共存 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:05:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：518
先上图
![](https://upload-images.jianshu.io/upload_images/4102666-d98c59cec0458ccb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Simulator Screen Shot - iPhone X - 2018-08-10 at 11.23.45.png
阴影和圆角同时存在
无法共存的主要原因是属性masksToBounds=YES（视图的图层上的子图层,如果超出父图层的部分就截取掉），设置为YES可以有圆角但是没有阴影，设置为NO有阴影没有圆角，既然圆角我能给你切走那阴影老夫也能给你切走，所以出现如此尴尬之场景。
## 解决：
CALayer可以完成视图上的所有UI操作，那我们把CALayer当成一个视图加到view上，所以分为两步实现
1：将需要操作的self.bg_view直接设置为圆角
```
self.bg_view.layer.cornerRadius = 6;
self.bg_view.layer.masksToBounds = YES;
```
2：创建CALayer并添加到self.bg_view的父视图上（放在self.bg_view的下层）
```
CALayer *subLayer=[CALayer layer];
    CGRect fixframe = self.bg_view.frame;
    subLayer.frame= fixframe;
    subLayer.cornerRadius=6;
    subLayer.backgroundColor=[[UIColor blackColor] colorWithAlphaComponent:0.8].CGColor;
    subLayer.masksToBounds=NO;
    subLayer.shadowColor = [UIColor blackColor].CGColor;//shadowColor阴影颜色
    subLayer.shadowOffset = CGSizeMake(3,2);//shadowOffset阴影偏移,x向右偏移3，y向下偏移2，默认(0, -3),这个跟shadowRadius配合使用
    subLayer.shadowOpacity = 0.8;//阴影透明度，默认0
    subLayer.shadowRadius = 4;//阴影半径，默认3
    [self.layer insertSublayer:subLayer below:self.bg_view.layer];
```
作者：路有点颠簸
链接：https://www.jianshu.com/p/48b1a601febf
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
