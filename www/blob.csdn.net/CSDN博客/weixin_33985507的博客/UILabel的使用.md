# UILabel的使用 - weixin_33985507的博客 - CSDN博客
2016年12月21日 16:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### UILabel背景透明字体不透明的效果
**通过storyboard设置**
在属性栏里设置当前控件的`backgroundColor–other–Opacity`的百分比来控制背景透明，字体不透明。
**通过代码设置**
```
hudLabel.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.4];
```
