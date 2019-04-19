# YYText 库学习总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月29日 09:26:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：148标签：[YYText](https://so.csdn.net/so/search/s.do?q=YYText &t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
[YYText](https://link.jianshu.com?t=https://github.com/ibireme/YYText) 是一个强大的富文本库.在iOS开发中经常会用到富文本。我们常用到的效果如下图所示：
![](https://upload-images.jianshu.io/upload_images/760391-3205c6711b7550cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/988/format/webp)
yytext.png
下面我们来看看各个功能的实现：
先创建一个可变属性字符串：
```
NSMutableAttributedString *text = [[NSMutableAttributedString alloc] initWithString:DaoXiang];
    [text yy_setFont:[UIFont systemFontOfSize:20] range:text.yy_rangeOfAll];//字体
    text.yy_lineSpacing = 20;//行间距
```
`DaoXiang`是一个宏定义字符串，
![](https://upload-images.jianshu.io/upload_images/760391-44ddd3b8f9ad32a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/785/format/webp)
DaoXiang.png
字体、颜色、文字间距
实现：
```
NSRange range0 = [[text string] rangeOfString:@"对这个世界如果你有太多的抱怨" options:NSCaseInsensitiveSearch];
//字体
[text yy_setFont:[UIFont systemFontOfSize:25] range:range0];
//文字颜色
[text yy_setColor:[UIColor purpleColor] range:range0];
//文字间距
[text yy_setKern:@(2) range:range0];
```
效果：
![](https://upload-images.jianshu.io/upload_images/760391-1761b31e45753d0b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/431/format/webp)
字体、颜色、文字间距.png
文字描边(空心字)
实现代码
```
NSRange range1 = [[text string] rangeOfString:@"跌倒了 就不敢继续往前走" options:NSCaseInsensitiveSearch];
//文字描边（空心字）默认黑色，必须设置width
[text yy_setStrokeColor:[UIColor orangeColor] range:range1];
[text yy_setStrokeWidth:@(2) range:range1];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-1a24997bdb39bbec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/315/format/webp)
文字描边
删除样式、下划线
实现代码
```
NSRange range2 = [[text string] rangeOfString:@"为什麼 人要这麼的脆弱 堕落" options:NSCaseInsensitiveSearch];
        
YYTextDecoration *decoration = [YYTextDecoration decorationWithStyle:YYTextLineStyleSingle
                                                                       width:@(1)
                                                                       color:[UIColor blueColor]];
//删除样式
[text yy_setTextStrikethrough:decoration range:range2];
//下划线
[text yy_setTextUnderline:decoration range:range2];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-f1c3b9e13a3477ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/349/format/webp)
删除样式、下划线
边框
实现代码
```
NSRange range3 = [[text string] rangeOfString:@"请你打开电视看看 多少人" options:NSCaseInsensitiveSearch];
    
//边框
YYTextBorder *border = [YYTextBorder new];
border.strokeColor = [UIColor colorWithRed:1.000 green:0.029 blue:0.651 alpha:1.000];
border.strokeWidth = 3;
border.lineStyle = YYTextLineStylePatternCircleDot;
border.cornerRadius = 3;
border.insets = UIEdgeInsetsMake(0, -2, 0, -2);
    
[text yy_setTextBorder:border range:range3];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-3616bd6f69aaddf8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/321/format/webp)
边框
阴影
实现代码
```
NSRange range4 = [[text string] rangeOfString:@"为生命在努力勇敢的走下去" options:NSCaseInsensitiveSearch];
//阴影
NSShadow *shadow = [[NSShadow alloc] init];
[shadow setShadowColor:[UIColor redColor]];
[shadow setShadowBlurRadius:1.0];
[shadow setShadowOffset:CGSizeMake(2, 2)];
[text yy_setShadow:shadow range:range4];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-8b837ece14553080.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/340/format/webp)
阴影
文本内阴影
实现代码
```
NSRange range5 = [[text string] rangeOfString:@"我们是不是该知足" options:NSCaseInsensitiveSearch];
//文本内阴影
YYTextShadow *shadow = [YYTextShadow new];
shadow.color = [UIColor redColor];
shadow.offset = CGSizeMake(0, 2);
shadow.radius = 1;
[text yy_setTextInnerShadow:shadow range:range5];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-8a321f098ec3024b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/228/format/webp)
文本内阴影
多重阴影
实现代码
```
//多重阴影
NSRange range6 = [[text string] rangeOfString:@"珍惜一切就算没有拥有" options:NSCaseInsensitiveSearch];
YYTextShadow *shadow = [YYTextShadow new];
shadow.color = [UIColor redColor];
shadow.offset = CGSizeMake(0, -1);
shadow.radius = 1.5;
YYTextShadow *subShadow = [YYTextShadow new];
subShadow.color = [UIColor greenColor];
subShadow.offset = CGSizeMake(0, 1);
subShadow.radius = 1.5;
shadow.subShadow = subShadow;
[text yy_setTextShadow:shadow range:range6];
YYTextShadow *shadow1 = [YYTextShadow new];
shadow1.color = [UIColor orangeColor];
shadow1.offset = CGSizeMake(0, 2);
shadow1.radius = 1;
[text yy_setTextInnerShadow:shadow range:range6];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-6cac359d7348be26.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/277/format/webp)
多重阴影
简单文本高亮
实现代码
```
//文本高亮简单版
NSRange range8 = [[text string] rangeOfString:@"随著稻香河流继续奔跑" options:NSCaseInsensitiveSearch];
[text yy_setTextHighlightRange:range8
                         color:[UIColor colorWithRed:0.093 green:0.492 blue:1.000 alpha:1.000]
               backgroundColor:[UIColor colorWithWhite:0.000 alpha:0.220]
                     tapAction:^(UIView *containerView, NSAttributedString *text, NSRange range, CGRect rect){
                         [AppUtility showMessage:[NSString stringWithFormat:@"Tap: %@",[text.string substringWithRange:range]]];
                     }];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-a8db4f2acb381654.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/285/format/webp)
简单文本高亮
//文本高亮pro
实现代码
```
//文本高亮pro
UIColor *colorNormal = [UIColor colorWithRed:0.093 green:0.492 blue:1.000 alpha:1.000];
UIColor *colorHighlight = [UIColor purpleColor];
NSRange range9 = [[text string] rangeOfString:@"微微笑 小时候的梦我知道" options:NSCaseInsensitiveSearch];
YYTextDecoration *decorationNomal = [YYTextDecoration decorationWithStyle:YYTextLineStyleSingle
                                                                    width:@(1)
                                                                    color:colorNormal];
YYTextDecoration *decorationHighlight = [YYTextDecoration decorationWithStyle:YYTextLineStyleSingle
                                                                        width:@(1)
                                                                        color:colorHighlight];
//未点击时颜色
[text yy_setColor:colorNormal range:range9];
//未点击时下划线
[text yy_setTextUnderline:decorationNomal range:range9];
//点击后的状态
YYTextHighlight *highlight = [YYTextHighlight new];
[highlight setColor:colorHighlight];
[highlight setUnderline:decorationHighlight];
highlight.tapAction = ^(UIView *containerView, NSAttributedString *text, NSRange range, CGRect rect) {
    [AppUtility showMessage:[NSString stringWithFormat:@"Tap: %@",[text.string substringWithRange:range]]];
};
[text yy_setTextHighlight:highlight range:range9];
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-5b999bf92efec983.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/316/format/webp)
文本高亮pro
@,#,email,link
实现代码
```
// 高亮状态的背景
YYTextBorder *highlightBorder = [YYTextBorder new];
highlightBorder.insets = UIEdgeInsetsMake(-2, 0, -2, 0);
highlightBorder.cornerRadius = 3;
highlightBorder.fillColor = [UIColor greenColor];
//@用户名称
NSArray *resultAt= [[Utility regexAt] matchesInString:text.string options:kNilOptions range:text.yy_rangeOfAll];
for (NSTextCheckingResult *at in resultAt)
{
    if (at.range.location == NSNotFound && at.range.length <= 1)
    {
        continue;
    }
    
    if ([text yy_attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil)
    {
        [text yy_setColor:[UIColor blueColor] range:at.range];
        // 高亮状态
        YYTextHighlight *highlight = [YYTextHighlight new];
        [highlight setBackgroundBorder:highlightBorder];
        // 数据信息，用于稍后用户点击
        NSString *atName = [text.string substringWithRange:NSMakeRange(at.range.location + 1, at.range.length - 1)];
        highlight.userInfo = @{@"linkValue" : atName,@"linkType":@(LinkTypeAt)};
        [text yy_setTextHighlight:highlight range:at.range];
    }
}
//#话题#
NSArray *resultTopic = [[Utility regexTopic] matchesInString:text.string options:kNilOptions range:text.yy_rangeOfAll];
for (NSTextCheckingResult *at in resultTopic)
{
    if (at.range.location == NSNotFound && at.range.length <= 1)
    {
        continue;
    }
    
    if ([text yy_attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil)
    {
        [text yy_setColor:[UIColor blueColor] range:at.range];
        // 高亮状态
        YYTextHighlight *highlight = [YYTextHighlight new];
        [highlight setBackgroundBorder:highlightBorder];
        // 数据信息，用于稍后用户点击
        highlight.userInfo = @{@"linkValue" : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length)],@"linkType":@(LinkTypeTopic)};
        [text yy_setTextHighlight:highlight range:at.range];
    }
}
//email
NSArray *resultEmail = [[Utility regexEmail] matchesInString:text.string options:kNilOptions range:text.yy_rangeOfAll];
for (NSTextCheckingResult *at in resultEmail)
{
    if (at.range.location == NSNotFound && at.range.length <= 1)
    {
        continue;
    }
    
    if ([text yy_attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil)
    {
        [text yy_setColor:[UIColor blueColor] range:at.range];
        // 高亮状态
        YYTextHighlight *highlight = [YYTextHighlight new];
        [highlight setBackgroundBorder:highlightBorder];
        // 数据信息，用于稍后用户点击
        highlight.userInfo = @{@"linkValue" : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length)],@"linkType":@(LinkTypeEmail)};
        [text yy_setTextHighlight:highlight range:at.range];
    }
}
//link
NSArray *resultLink = [[Utility regexUrl] matchesInString:text.string options:kNilOptions range:text.yy_rangeOfAll];
for (NSTextCheckingResult *at in resultLink)
{
    if (at.range.location == NSNotFound && at.range.length <= 1)
    {
        continue;
    }
    
    if ([text yy_attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil)
    {
        [text yy_setColor:[UIColor blueColor] range:at.range];
        // 高亮状态
        YYTextHighlight *highlight = [YYTextHighlight new];
        [highlight setBackgroundBorder:highlightBorder];
        // 数据信息，用于稍后用户点击
        highlight.userInfo = @{@"linkValue" : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length)],@"linkType":@(LinkTypeURL)};
        [text yy_setTextHighlight:highlight range:at.range];
    }
}
```
效果
![](https://upload-images.jianshu.io/upload_images/760391-601c5353b21aa048.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/452/format/webp)
@,#,email,link
以话题为例，简单说明一下：首先用正则来查出所有话题，然后再将话题部分设置高亮状态以及数据信息，这些信息在用户点击时会用到。后面会说到如何处理点击，别急。
添加gif动画
实现代码
```
YYImage *image = [YYImage imageNamed:@"zuqiu"];
image.preloadAllAnimatedImageFrames = YES;
YYAnimatedImageView *imageView = [[YYAnimatedImageView alloc] initWithImage:image];
imageView.autoPlayAnimatedImage = NO;
[imageView startAnimating];
NSMutableAttributedString *attachText = [NSMutableAttributedString yy_attachmentStringWithContent:imageView contentMode:UIViewContentModeCenter attachmentSize:imageView.size alignToFont:[UIFont systemFontOfSize:16] alignment:YYTextVerticalAlignmentBottom];
[text appendAttributedString:attachText];
```
添加普通图片直接使用UIImage和UIImageView就可以了。
布局
实现代码
```
CGSize size = CGSizeMake(SCREEN_WIDTH, CGFLOAT_MAX);
YYTextLayout *layout = [YYTextLayout layoutWithContainerSize:size text:text];
    
// 获取文本显示位置和大小
//layout.textBoundingRect; // get bounding rect
//layout.textBoundingSize; // get bounding size
```
可以由`YYTextLayout`获取文本的`bonding rect`和`size`。
YYLabel添加
实现代码
```
YYLabel *label = [YYLabel new];
label.highlightTapAction = ^(UIView *containerView, NSAttributedString *text, NSRange range, CGRect rect) {
    if ([self.clickDelegate respondsToSelector:@selector(label:tapHighlight:inRange:)])
    {
        YYTextHighlight *highlight = [text yy_attribute:YYTextHighlightAttributeName atIndex:range.location];
        [self.clickDelegate label:(YYLabel *)containerView tapHighlight:highlight inRange:range];
    }
};
label.frame = CGRectMake(0, 0, SCREEN_WIDTH, layout.textBoundingSize.height);
label.textAlignment = NSTextAlignmentCenter;
label.textVerticalAlignment = YYTextVerticalAlignmentCenter;
label.numberOfLines = 0;
label.backgroundColor = RGBCOLOR(246, 246, 246);
label.textLayout = layout;
[self addSubview:label];
```
这里有个属性`highlightTapAction`就是用来处理点击高亮文字事件的，在这里，我定义了一个delegate：
```
@protocol YYHiglightTextClickDelegate <NSObject>
- (void)label:(YYLabel *)label
 tapHighlight:(YYTextHighlight *)highlight
      inRange:(NSRange)textRange;
@end
```
只要实现这个delegate就能方便的处理点击各种高亮文字的事件。`YYTextHighlight `里面包含了一个userInfo，包含了很多需要处理的信息，通过它，能够很容易的处理点击事件，我这里在UIViewController中做了一个实现：
```
#pragma mark - YYHiglightTextClickDelegate
- (void)label:(YYLabel *)label
 tapHighlight:(YYTextHighlight *)highlight
      inRange:(NSRange)textRange
{
    NSDictionary *info = highlight.userInfo;
    LinkType linkType = [info[@"linkType"] integerValue];
    NSString *linkValue = info[@"linkValue"];
    switch (linkType) {
        case LinkTypeAt:
        {
            [AppUtility showMessage:[NSString stringWithFormat:@"选中at：%@",linkValue]];
        }
            break;
        case LinkTypeTopic:
        {
            [AppUtility showMessage:[NSString stringWithFormat:@"选中话题：%@",linkValue]];
        }
            break;
        case LinkTypeEmail:
        {
            [AppUtility showMessage:[NSString stringWithFormat:@"选中email：%@",linkValue]];
        }
            break;
        case LinkTypeURL:
        {
            [AppUtility showMessage:[NSString stringWithFormat:@"选中url：%@",linkValue]];
        }
            break;
        case LinkTypePhoneNum:
        {
            [AppUtility showMessage:[NSString stringWithFormat:@"选中phone：%@",linkValue]];
        }
            break;
        default:
            break;
    }
}
```
我在userInfo中传入了两对键值：
####表情
---
实现代码
```
NSMutableDictionary *mapper = [NSMutableDictionary new];
mapper[@":smile:"] = [self imageWithName:@"002"];
mapper[@":cool:"] = [self imageWithName:@"013"];
mapper[@":biggrin:"] = [self imageWithName:@"047"];
mapper[@":arrow:"] = [self imageWithName:@"007"];
mapper[@":confused:"] = [self imageWithName:@"041"];
mapper[@":cry:"] = [self imageWithName:@"010"];
mapper[@":wink:"] = [self imageWithName:@"085"];
mapper[@":zuqiu:"] = [self imageWithName:@"zuqiu"];
YYTextSimpleEmoticonParser *parser = [YYTextSimpleEmoticonParser new];
parser.emoticonMapper = mapper;
YYTextLinePositionSimpleModifier *mod = [YYTextLinePositionSimpleModifier new];
mod.fixedLineHeight = 22;
```
`YYLabel`已经实现了一个简单的表情解析器`YYTextSimpleEmoticonParser `,你只需要设置一下映射器`emoticonMapper`就好.
然后把解析器和modifier传给`YYLabel`.
最后将文本传给`attributedText`
```
label.textParser = parser;
label.linePositionModifier = mod;
label.attributedText = text;
```
`linePositionModifier`是在文本发生变化时才需要的属性，一般YYTextView用的多，比如修改一个文本之后，整个文本发生了变化，就需要这个属性值。
表情资源来自YYText，可以下载源码获得。
另外我写了个小[demo](https://github.com/flowyears/AttributeTextDemo)，包含YYLabel，YYTextView以及TTTAttributedLabel的简单实用。
[TTTAttributedLabel简单使用](http://www.jianshu.com/p/b457a49fac3d)简单的描述了TTTAttributedLabel的使用，并且在最后将YYLabel，YYTextView以及TTTAttributedLabel三者的功能做了一个简单的比较。
在学习的过程中发现有几个属性无效，一个是斜体，一个是文字背景色，如果有知道怎么实现的小伙伴请告诉我一下，谢谢。
以上只是一个简单的使用。这里有一篇更深层次的剖析：[YYKit之YYText](http://www.cnblogs.com/lujianwenance/p/5716804.html).
end.
```
作者：0o冻僵的企鹅o0
链接：https://www.jianshu.com/p/60aee32ade55
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
