# 使用UITextField自定义UISearchBar - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月29日 20:28:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1845
# 使用UITextField自定义UISearchBar
字数267
阅读51评论1喜欢1
> 
有的时候，使用iOS自带的控件不符合变态产品狗的需求时，我们只好自定义控件来满足他们。在此处，我们通过创建一个GUSearchBar类，继承自UITextField类来自定义搜索框。
![](http://upload-images.jianshu.io/upload_images/2013105-1edbd522c10f91ca.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 创建GUSearchBar
> 
代码如下：GUSearchBar.h
```
#import <UIKit/UIKit.h>
@interface GUSearchBar : UITextField
+(instancetype) searchBar;
@end
```
> 
GUSearchBar.m
```
#import "GUSearchBar.h"
@implementation GUSearchBar
-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.background = [UIImage resizedImage:@"searchbar_textfield_background"];
        self.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
        UIImageView *leftView = [[UIImageView alloc] init];
        leftView.image = [UIImage imageWithName:@"searchbar_textfield_search_icon"];
        leftView.width = leftView.image.size.width + 10;
        leftView.height = leftView.image.size.height;
        leftView.contentMode = UIViewContentModeCenter;
        self.leftView = leftView;
        self.leftViewMode = UITextFieldViewModeAlways;
        self.clearButtonMode = UITextFieldViewModeAlways;
        self.placeholder = @"搜索";
    }
    return self;
}
+(instancetype) searchBar
{
    return [[self alloc] init];
}
@end
```
> 
注意：上述有类似leftView.width，leftView.height这样的语法是使用了自定义UIView的扩展分类，详情请点击[UIView扩展](http://reclusegu.github.io/2016/05/25/UIView-Extend-Property/)。
## 使用
```cpp
```cpp
GUSearchBar *searchBar = [[GUSearchBar alloc] init];
searchBar.width = 350;
searchBar.height = 30;
self.navigationItem.titleView = searchBar;
```
```
## 使用效果
![](http://upload-images.jianshu.io/upload_images/2013105-38a9400e80110f0f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
GUSearchBar
> 
来自RecluseGu的博客：[http://reclusegu.github.io/2016/06/29/Customizing-UISearchBar-with-UITextField/](http://reclusegu.github.io/2016/06/29/Customizing-UISearchBar-with-UITextField/)
