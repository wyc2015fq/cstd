# iOS开发之有间距的UITableViewCell - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月22日 14:36:07[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2363
**UITableView是最常用的一个iOS控件，现要做一个如下图的UITableView，其中白色部分就是cell，可是默认的UITableView中cell之间是没有间隔的，那么办呢？网上有2种做法，我这里顺带提一下吧**
![](http://upload-images.jianshu.io/upload_images/735757-a53adf6607cdfc04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果图.png
**1、方式一**
通过设置cell的contentView来间接实现，在cell的contentView的顶部或者底部留下一定的间距，这样就会有cell间就有间距的效果。但是这种方式在cell有点击效果的时候，会很明显的看出有分层，因为这时候cell是被点击的，contentView都会有系统点击的阴影效果。这种方式在cell左滑删除，置顶等操作的时候，左滑出的视图会高出一部分（左滑显示出的高度=（cell的高度-留下的间距高度）+ 留下的间距高度），很显然这种方式有致命缺陷。**
**2、方式二**
通过分组的方式间接的实现，每组的Header可以当做是cell之间的间距，每组中只有一个cell，代码如下：
```
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView 
{ return 10;}
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
 { return 10;}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
 { return 1;}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
 { return 100;}
```
但是呢，这还是会出现一个问题，因为系统默认分组的时候每组的Header会停留在tableview的顶部，这要怎么处理呢？网上也有一种解决办法
```cpp
```cpp
- (void)scrollViewDidScroll:(UIScrollView *)scrollView {    
if (scrollView == self.tableView)    {        
CGFloat sectionHeaderHeight = 10;    
if (scrollView.contentOffset.y <= sectionHeaderHeight && scrollView.contentOffset.y >= 0) {            
        scrollView.contentInset = UIEdgeInsetsMake(-scrollView.contentOffset.y, 0, 0, 0);        
       } else if (scrollView.contentOffset.y >= sectionHeaderHeight) {
        scrollView.contentInset = UIEdgeInsetsMake(-sectionHeaderHeight, 0, 0, 0);        
       }   
   }
}
```
```
但是这种方式是通过scroll偏移量来监听和改变tableview的contentInset，因为只要UITableView在滚动就一直在运行，不是很好。
**优雅的方式**
其实优雅的也是最简单的方法是直接让UITableView中的cell收缩一点，这样UITableView的底色就是分割线的颜色了，如上图就是橘色。这种方式只需要重写cell的setFrame方法即可
```cpp
```cpp
-(void)setFrame:(CGRect)frame
{
    frame.origin.x = 10;//这里间距为10，可以根据自己的情况调整
    frame.size.width -= 2 * frame.origin.x;
    frame.size.height -= 2 * frame.origin.x;
    [super setFrame:frame];   
}
```
```
如果此时想要实现圆角也很简单，直接加上
```
self.layer.masksToBounds = YES;
self.layer.cornerRadius = 8.0;
```
此时效果图：
![](http://upload-images.jianshu.io/upload_images/735757-60557b7a73935dd1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
圆角矩形cell.png
文／YungFan（简书作者）
原文链接：http://www.jianshu.com/p/01f61359b30d
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
