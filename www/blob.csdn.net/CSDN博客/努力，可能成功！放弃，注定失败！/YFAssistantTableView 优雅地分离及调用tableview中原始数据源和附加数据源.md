# YFAssistantTableView:优雅地分离及调用tableview中原始数据源和附加数据源 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:47:19[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：46
你是否遇到过这样的需求,在tableview中显示一列数据,点击某一个cell时，在此cell下显示相应的附加信息。如下图：
![](https://upload-images.jianshu.io/upload_images/2984839-7e14bc865d161b85.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/306/format/webp)
你是不是觉得需求很容易实现，只要使用tableview的insertRowsAtIndexPaths:withRowAnimation:插入一个附加cell就可以了（为表述清楚，我们姑且称被点击的cell为原始cell，展示的附加信息为附加cell）。如果你是这么做的，那么后遗症来了：展开的附加cell影响了后面cell的indexPath，当我们对后面的cell配置、点击、增加、删除、系统回调等等操作时，都需要考虑附加cell对indexPath的影响。例如我们已经展开了row == 0的原始cell的附加cell，配置row == 0的附加cell实际是对tableview的row == 1的配置；配置原始数据的row == 1时，实际是配置tableview的row == 2的cell。每次indexPath相关操作都要转换一遍(特别是当展开的附件cell很多时)，非常麻烦，且数据源杂乱、代码可读性差。
本文介绍了一种方法消除附加cell对后面的indexPath产生的影响；并且使delegate和dataSourse回调只配置原始cell，将附加cell的配置分离出来。
1.首先定义两种数据类型：
> 
typedef NSIndexPath ActualIndexPath;
实际indexPath：考虑附加cell影响的indexPath，暨系统调用传入的indexPath。
> 
typedef NSIndexPath LogicIndexPath;
逻辑indexPath：不考虑附加cell影响的indexPath，暨我们希望在代码中使用的indexPath。
参照上面的例子解释:展开row == 0的原始cell后，row == 0的原始cell的附加cell的实际indexPath.row为 1，逻辑indexPath.row为0；row == 1的原始cell实际indexPath.row为2，逻辑indexPath.row 为1。
2.为tableview增加了一个assistantDelegate，负责附加cell的配置。
> 
@interfaceYFAssistantTableView :UITableView
@property(nonatomic,weak)id<YFAssistantDelegate> assistantDelegate;
YFAssistantDelegate中声明了配置附加cell的方法（为了减少过多的protocol声明，在不影响代码可读性前提下，将附加cell的delegate和dataSource统一放到assistantDelegate里）。理论上YFAssistantDelegate里面的方法应该是UITableViewDelegate和UITableViewDataSource的并集，但很多方法并不会使用所以不再声明。
> 
@protocol YFAssistantDelegate <NSObject>
//截取了部分YFAssistantDelegate的方法
@required
- (UITableViewCell*)YFAssistantTableView:(YFAssistantTableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath;
- (BOOL)YFAssistantTableView:(YFAssistantTableView*)tableView shouldSpreadAssistantAtIndexPath:(NSIndexPath*)indexPath;- (CGFloat)YFAssistantTableView:(YFAssistantTableView*)tableView heightForRowAtIndexPath:(NSIndexPath*)indexPath;  @end
3.将ActualIndexPath转化为LogicIndexPath
> 
- (LogicIndexPath*)actualIndexPath2Logic:(ActualIndexPath*)indexPath{
NSIntegersection = indexPath.section;
__block NSInteger row = indexPath.row;
//self.assistantsIndexPaths存储了所有已展开的附加cell的indexPath
[self.assistantsIndexPaths enumerateObjectsUsingBlock:^(ActualIndexPath *obj,NSUInteger idx,BOOL *stop) {
if(obj.section== section && obj.row<=indexPath.row) {
row--;
}
}];
return[NSIndex PathindexPathForRow:row inSection:section];
}
3.将对附加cell的方法调用从delegate（dataSource）转到assistantDelegate。
这里用Method Swizzling钩住delegate（dataSource）的每一个方法，在hook方法里首先将ActualIndexPath转为LogicIndexPath，然后判断传进来的indexPath指向的cell的类型，如果指向原始cell那么调用delegate（dataSource）方法并传入LogicIndexPath；如果指向附加cell那么调用assistantDelegate方法并传入LogicIndexPath。(以下以tableView:cellForRowAtIndexPath:为例，其他方法一样）
> 
Method method1 = class_getInstanceMethod([delegate class], @selector(tableView:cellForRowAtIndexPath:));
const char *type = method_getTypeEncoding(method1);
IMP imp = class_getMethodImplementation([self class], @selector(hookTableView:cellForRowAtIndexPath:);
class_addMethod([delegate class], @selector(hookTableView:cellForRowAtIndexPath:), imp, type);
Method method2 = class_getInstanceMethod([delegate class], @selector(hookTableView:cellForRowAtIndexPath:));
method_exchangeImplementations(method1, method2);
> 
- (UITableViewCell*)hookTableView:(YFAssistantTableView*)tableView cellForRowAtIndexPath:(ActualIndexPath*)indexPath{
LogicIndexPath ***logicIndexPath** = 实际indexPath转为逻辑indexPath(indexPath);
if(indexPath指向附加cell) {
return [tableView.assistantDelegate YFAssistantTableView:tableView cellForRowAtIndexPath:**logicIndexPath**];
}
//indexPath指向原始cell
return [self hookTableView:tableView cellForRowAtIndexPath:**logicIndexPath**];
}
至此，YFAssistantTableView的基本思路和主要方法已经介绍完毕。在实际使用中，我们只需要关心哪个indexPath的附加cell需要展开、收回，而不用关心indexPath的变化;并将原始cell和附加cell的分离解耦，调用配置数据清晰顺畅、代码可读性强。附上demo
![](https://upload-images.jianshu.io/upload_images/2984839-81092265e31c7fa7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/2984839-0c6a7697ecf62ffd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/2984839-ac7c965286be1793.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
源码：[https://github.com/haltNoShut/YFAssistantTableView](https://link.jianshu.com?t=https://github.com/haltNoShut/YFAssistantTableView)
作者：欧巴撒浪嘿呦
链接：https://www.jianshu.com/p/e5e75c135120
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
