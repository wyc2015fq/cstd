# iOS开发 给Label加下划线、中划线 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月01日 09:25:58[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1852
#### 引言:
> 
当我们在做一些购物类APP或者是商品展示类的APP时，我们经常会遇到某种商品打折优惠或者抢购的场景。并且为了吸引客户，商家一般把打折商品的现价和原价同时显示出来，进行对比，此时就出现了为展示原价的label添加中划线或者是下划线的需求。
我们参考一下驴妈妈的首页：
![](http://upload-images.jianshu.io/upload_images/1975627-b290ef9c1a340919.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
驴妈妈首页－中划线
下面就直接上代码了：
##### 添加中划线:
```
UILabel * strikeLabel = [[UILabel alloc] initWithFrame:(CGRectMake(10, 10, 50, 30))];
  NSString *textStr = [NSString stringWithFormat:@"%@元", primeCost];
  //中划线
  NSDictionary *attribtDic = @{NSStrikethroughStyleAttributeName: [NSNumber numberWithInteger:NSUnderlineStyleSingle]};
  NSMutableAttributedString *attribtStr = [[NSMutableAttributedString alloc]initWithString:textStr attributes:attribtDic];
  // 赋值
  strikeLabel.attributedText = attribtStr;
  [self.view addSubview:strikeLabel];
```
##### 添加下划线:
```
UILabel *underlineLabel = [[UILabel alloc] initWithFrame:(CGRectMake(10, 10, 50, 30))];
  NSString *textStr = [NSString stringWithFormat:@"%@元", primeCost];
  // 下划线
  NSDictionary *attribtDic = @{NSUnderlineStyleAttributeName: [NSNumber numberWithInteger:NSUnderlineStyleSingle]};
  NSMutableAttributedString *attribtStr = [[NSMutableAttributedString alloc]initWithString:textStr attributes:attribtDic];
  //赋值
  underlineLabel.attributedText = attribtStr;
  [self.view addSubview:underlineLabel];
```
效果如图所示：
![](http://upload-images.jianshu.io/upload_images/1975627-7dfcab97f92a7872.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果图
文／Li_Cheng（简书作者）
原文链接：http://www.jianshu.com/p/fc1500a2b936
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
