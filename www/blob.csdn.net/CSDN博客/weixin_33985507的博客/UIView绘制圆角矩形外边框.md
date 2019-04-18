# UIView绘制圆角矩形外边框 - weixin_33985507的博客 - CSDN博客
2017年08月09日 13:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
内外cornerRadius可以控制
baseLayer是我们原始的图层，现在想截取圆角矩形出来，中间挖空成一个圆角矩形。
CAShapeLayer* cropLayer = [[CAShapeLayer alloc] init];
[baseLayer addSublayer:cropLayer];
// 创建一个绘制路径
CGMutablePathRef mPath =CGPathCreateMutable();
// 空心矩形的rect（即要挖空的内部区域）
CGRect cropRect = CGRectMake(10, 10, 60, 40);
```
// 绘制rect
CGPathAddRoundedRect(mPath, nil, baseLayer.bounds, 5, 5);
CGPathAddRoundedRect(mPath, nil, cropRect, 5, 5);
// 设置填充规则(重点)
[cropLayer setFillRule:kCAFillRuleEvenOdd];
// 关联绘制的path
[cropLayer setPath:mPath];
// 设置填充的颜色
[cropLayer setFillColor:[[UIColor greenColor] CGColor]];
```
