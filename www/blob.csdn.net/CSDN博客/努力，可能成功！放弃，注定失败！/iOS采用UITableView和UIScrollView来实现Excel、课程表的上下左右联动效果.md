# iOS采用UITableView和UIScrollView来实现Excel、课程表的上下左右联动效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:04:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：36标签：[iOS版采用的的UITableView和UIScrollVi](https://so.csdn.net/so/search/s.do?q=iOS版采用的的UITableView和UIScrollVi&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
前言
在项目中遇到了许多报表，虽然目前只是一个简单的展示，可以用tableView单元格显示，满足目前的需求，但是我仔细考虑了，能不能实现类似Excel表那样展示，既可以左右滑动，又可以上下滑动，当时找了许多资料，也找了许多国外网站的资料，却找不到这样类似的开源项目，不过后面在code4app发现了UITableViewLinkageDemo，但是仔细研究代码后，发现其内部复用性是个问题同时不能支持上下拉刷新等，后面在github这个demo的作者提了一些我的想法，最终我决定自己去实现一个类似Excel的控件，因此结合UITableViewLinkageDemo的思想，去摸索，最终实现了这样的控件--YWExcel: [https://github.com/flyOfYW/YWExcel](https://github.com/flyOfYW/YWExcel)
效果图
![](https://upload-images.jianshu.io/upload_images/3030124-8e3edbd810124be5.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/410/format/webp)
ezgif-4-a09e4eaf14.gif
一 整体描述图
![](https://upload-images.jianshu.io/upload_images/3030124-24e03180b43919f6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/411/format/webp)
QQ20180817-125308.png
1、红色部分为：UITableView
2 蓝色部分：UITableViewCell
3 黄色部分：UISrollView
4 类目那一行：目前设置2个模式
1）作为独立tableView之上的view
2）作为tableView的组头View
二 设计联动的思路
在 -(void)scrollViewDidScroll:(UIScrollView *)scrollView
监听偏移量，去改变其他scrollView的偏移量，那么采用什么方式去改变其他的偏移量呢，三种方式：block、delegate，NSNotificationCenter
最初的时候，我使用了block，但是发现并没实现我想要的那种效果，后来我就重新梳理下思路，scrollView添加在cell上，但是cell是多个，想一个cell上的scrollView改变偏移量，同时能改变其他cell上的scrollView的偏移量的这样场景下（一对多的关系），就使用NSNotificationCenter的方法去实现联动
注意点：使用NSNotificationCenter的第一个版本，效果确实出来，但是遇到一个bug，因为当时使用的通知名称是写”死“ 如：
```
[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(scrollMove:) name:@"cellNotification" object:nil];
```
这样就会造成一个界面有多个表格（YWExcelView的对象）的情况下，就会造成通知混乱。
因此，现在这个版本，使用当前对象的指针地址作为通知中的name
```
_NotificationID = [NSString stringWithFormat:@"%p",self];    
 [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(scrollMove:) name:_NotificationID object:nil];
```
这样就可以确保每个YWExcelView的对象的通知中心都是独立的，互不影响
三 使用方法
跟tableView的用法差不多
```
typedef NS_ENUM(NSInteger, YWExcelViewStyle) {
    YWExcelViewStyleDefalut = 0,//整体表格滑动，上下、左右均可滑动（除第一列不能左右滑动以及头部View不能上下滑动外）
    YWExcelViewStylePlain,//整体表格滑动，上下、左右均可滑动（除第一行不能上下滑动以及头部View不能上下滑动外）
    YWExcelViewStyleheadPlain,//整体表格(包括头部View)滑动，上下、左右均可滑动（除第一列不能左右滑动外）
    YWExcelViewStyleheadScrollView,//整体表格(包括头部View)滑动，上下、左右均可滑动
};
@protocol YWExcelViewDataSource<NSObject>
@required
//多少行
- (NSInteger)excelView:(YWExcelView *)excelView numberOfRowsInSection:(NSInteger)section;
//多少列
- (NSInteger)itemOfRow:(YWExcelView *)excelView;
@optional
- (void)excelView:(YWExcelView *)excelView label:(UILabel *)label textAtIndexPath:(YWIndexPath *)indexPath;
- (void)excelView:(YWExcelView *)excelView headView:(UILabel *)label textAtIndexPath:(YWIndexPath *)indexPath;
//分组
- (NSInteger)numberOfSectionsInExcelView:(YWExcelView *)excelView;
@end
@protocol YWExcelViewDelegate <NSObject>
@optional
//自定义每列的宽度/默认每列的宽度为80
- (NSArray *)widthForItemOnExcelView:(YWExcelView *)excelView;
@end
```
demo:[https://github.com/flyOfYW/YWExcel](https://github.com/flyOfYW/YWExcel)
作者：微威
链接：https://www.jianshu.com/p/14d3515983bc
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
