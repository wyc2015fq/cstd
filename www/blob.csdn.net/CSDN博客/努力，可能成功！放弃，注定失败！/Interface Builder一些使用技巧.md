# Interface Builder一些使用技巧 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月02日 13:49:07[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：303标签：[ios																[iOS开发																[Interface Builder](https://so.csdn.net/so/search/s.do?q=Interface Builder&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)

当年轻的开发者朋友们写了一大堆的代码时，他们会觉得很高兴，并且喜欢夸耀自己写的项目有成千上万行代码。
随着经验越来越丰富，我们意识到代码是昂贵的，因为要花很多时间去维护、调试和寻找原因。
这也是为什么我们应该使用工具来帮助我们避免写出样板代码。Interface Builder就是这些工具中的一个。
下面说一些我喜欢的小提示和技巧。
**Storyboards还是Xib**
很显然，苹果是极力推荐我们使用storyboard的，因为下列的一些特性只在storyboard中才有：
- Prototype cells
- Segues
- Container View Controllers
很多人抱怨在团队合作的时候使用storyboard很困难，因为在合并代码的时候会有冲突。以前这个问题是会经常出现，但是现在storyboard的格式更好、更容易理解了。
即使这样，我也不喜欢干合并代码的活。不过幸运的是，当我们使用多个storyboard合理地划分项目时，就可以避免很多潜在的冲突。
我比较喜欢给每一个场景创建一个storyboard，例如最新的那个app:
- 入职 — 新建账户以及初始的信息界面
- 新建流 — 当用户可以新建内容时
- 活动 — 查看提到自己、自己或其他用户的活动的相关界面
iOS 9给我们介绍了Storyboard References，使我们能够更容易地使用多个storyboard来分割应用。
如果需要部署在老版本的iOS target上呢？
**玩转Storyboard, Xib或类引用**
如果你运气不够好，未能使用iOS9作为最低的部署目标，仍旧可以用你自己的方法实现。虽然需要花费点功夫，但这些努力都是值得的。多年以来，我一直在使用一个非常简单的扩展去拥有我自己的Storyboard, xib 和 类引用。
扩展的主要内容：
- 在storyboard中新建一个空的UIViewController
- 在UIViewController中用IBInspectable扩展来支持自定义的replacementIdentifier图式
storyboard.StoryboardName.ControllerIdentifier
storyboard.StoryboardName -> loads the initial vc
class.name
xib.name
**@interface****UIViewController**(**KZReplacement**)
**@property**(nonatomic,copy)**IBInspectable****NSString***replacementIdentifier;
*//! 如果设了replacementIdentifier，就新建一个controller，否则返回self*
- (**UIViewController*******)kz_replacementController;
**@end**
在Interface Builder里是这样的：

这个方法有一个简单的实现，下面的是一个最“复杂”的：
(**UIViewController*******)kz_replacementControllerWithIdentifier:(**NSString*******)identifier;
{
**NSDictionary***replacementSchemes = @{
@"class" :
[**NSValue** valueWithPointer:**@selector**(kz_createControllerWithClassIdentifier:)],
@"storyboard" :
[**NSValue** valueWithPointer:**@selector**(kz_loadControllerWithStoryboardIdentifier:)],
@"xib" :
[**NSValue** valueWithPointer:**@selector**(kz_loadControllerWithXibIdentifier:)]
};
__block **UIViewController***replacement = **nil**;
[replacementSchemes enumerateKeysAndObjectsUsingBlock:^(**NSString***scheme,**NSValue***selectorValue,**BOOL***stop){
const**struct**_NSRange
range = [identifier rangeOfString:scheme];
**if**(range.location ==
0)
{
*stop =
**YES**;
**UIViewController** *(*objc_msgSendTyped)(**id**,**SEL**,**NSString*******) =
(**UIViewController** *(*****)(**id**,**SEL**,**NSString*******))objc_msgSend;
replacement =
objc_msgSendTyped(**self**,selectorValue.pointerValue,[identifier substringFromIndex:range.length +
1]);
}
}];
**return**replacement;
}
现在你只需在容器内的每一个VC执行这个方法。举个例子，如果你的app使用了TabBar，下面的方法对你每一个UITabBarController的子类都会起作用：
- (**void**)awakeFromNib;
{
[**super**awakeFromNib];
[**super** setViewControllers:[**self**
 processViewControllersByReplacingMocks:self.viewControllers] animated:NO];
}
- (**void**)setViewControllers:(**NSArray*******)viewControllers animated:(**BOOL**)animated;
{
[**super** setViewControllers:[**self**
 processViewControllersByReplacingMocks:viewControllers] animated:animated];
}
- (**void**)setViewControllers:(**NSArray*******)viewControllers;
{
[**super** setViewControllers:[**self**
 processViewControllersByReplacingMocks:viewControllers]];
}
- (**NSArray*******)processViewControllersByReplacingMocks:(**NSArray*******)viewControllersAndMocks;
{
**NSMutableArray***array = [**NSMutableArray**new];
[viewControllersAndMocks enumerateObjectsUsingBlock:^(**UIViewController***viewController,**NSUInteger**idx,**BOOL***stop){
**UIViewController***replacement = [viewController
kz_replacementController];
[array addObject:replacement?: viewController];
}];
**return**[array copy];
}
**使用行为(Behaviours)**
如果要给你们推荐一种模式的话，毫无疑问我会推荐“组合”(composition)，这是个非常棒的模式，并且能自然而然地带来其他好的方面。
行为是一种组合，只能基于Interface Builder配置。
之前我在objc.io的架构指南中写过。
**运行时属性和@IBInspectable**
多年前我们就可以直接在IB内设置一些自定义属性了，但是还有很多人是不知道的。他们在Interface Builder中新建了一个UIView，然后再用代码来设置View的一些属性。
Xcode 6的运行时属性升级到支持@IBInspectable了，将老且脆弱的运行时属性：
修改成了一个可编辑的属性，我们再也不会拼写错误了：

可支持的属性有下列这些类型：
- Int
- CGFloat
- Double
- String
- Bool
- CGPoint
- CGSize
- CGRect
- UIColor
- UIImage
**扩展UIKit**
你还可以扩展现有的类型，比如UIKit中的类：
extension **UIView**{
@**IBInspectable**var kz_borderColor: UIColor?{
get{
**if**let colorRef =
layer.borderColor{
**return****UIColor**(**CGColor**: colorRef)
}
**return****nil**
}
set{
layer.borderColor =
newValue?.**CGColor**
}
}
}
给UIView增加了一个漂亮的边界颜色编辑器

**消除副作用**
给NSLayoutConstraint增加一个标示符会咋样呢？iOS7增加了一个identifier字段作调试用，不过我在另外的一些地方使用了自定义的identifier。
我们的应用在不同的iOS设备的显示略有一些差异，不幸的是Size Classes未能满足这个特别的设计。
为了处理这个需求，我创建了一种简单的DSL语法来描述每一种设备的值：
**KZ_SPEC**(**Create**,(@{
@"create.size.captionTextWidth": kz_spec(260,250,234,220),
@"create.size.segmentCollectionView.height": kz_spec(116,116,116,116).offsetBy(16)
}))
并且扩展了NSLayoutConstraint，使我们能够为实际的界面绑定规格。这样就避免了在view中写样板代码了：
**@interface****NSLayoutConstraint**(**KZDesignSpec**)
**@property**(nonatomic,copy)**IBInspectable****NSString***
kz_specName;
**@end**
**@implementation****NSLayoutConstraint**(**KZDesignSpe**)
static**void**const*kDesignSpecKey = &kDesignSpecKey;
- (**NSString*******)kz_specName;
{
**return**objc_getAssociatedObject(**self**,kDesignSpecKey);
}
- (**void**)setKz_specName:(**NSString*******)kz_specName;
{
objc_setAssociatedObject(**self**,kDesignSpecKey,kz_specName,**OBJC_ASSOCIATION_COPY_NONATOMIC**);
**self**.constant =
kz_floatForSpec(kz_specName);
**self**.identifier =
kz_specName;
}
**@end**
**快捷键和隐藏选项**
确认你知道自己的快捷键以及Interface Builder的一些隐藏选项。
我还记得在我使用了2年的IB后看见一个人使用Media Tab拉出一张图片，并创建了一个合适尺寸的UIImageView.而这时的我还是手动的……
快捷键有很多，这里给出了一些不错的。
当你在设置AutoLayout的时候，记得在拉约束的同时按Option键，或者使用编辑菜单。
**总结**
Interface Builder离完美还有很长的路，但它也远不是一些开发者认为的一个糟糕的工具。
很多应用不能说是“用Interface Builder来实现太复杂”了，这是很多人在我问他们为什么不用IB的时候的回答。
Foldify和Storest都大量地使用IB.
实际上，很多情况下使用IB足矣，特别是加上我之前给出的扩展，或者是你自己实现的扩展。
其他情况下使用代码。代码或IB，不是一个只能二选一的问题。
