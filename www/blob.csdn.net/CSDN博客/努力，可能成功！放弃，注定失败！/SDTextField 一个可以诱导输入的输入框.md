# SDTextField:一个可以诱导输入的输入框 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月12日 21:33:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：490标签：[SDTextField																[iOS学习资料																[iOS常用方法																[iOS 资源大全](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=SDTextField&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/1396375-8771b71fb1587e0e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 前言
因为前两天老大让翻找以前的工程,无意间发现这个了,就想搞一搞.以前我做诱导输入的时候一般是跳转控制器,在另外的一个控制器中searchBar和tableView配合实现诱导输入功能,然后通过代理或者通知中心进行传值.其实这种还是很常见的,因为页面的话也比较美观,相比于SDTextField.制作SDTextField的主要是老大要求,我也没办法,如果有需要的童鞋请拿走.具体的效果如下所示.
![](http://upload-images.jianshu.io/upload_images/1396375-095d33c5cb06b7dd.gif?imageMogr2/auto-orient/strip)
#### SDTextField的快速集成
SDTextField使用起来也是比较简单.我们只需要简简单单的两三步就能快速创建SDTextField对象.首先把SDTextFieldDemo中SDTextField.h和SDTextField.m文件拖到你的工程中.
![](http://upload-images.jianshu.io/upload_images/1396375-c8368cf6270b3885.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
然后使用**+(instancetype)initWithFrame:(CGRect)frame;**创建即可.创建完成之后,我们还需要配置诱导输入查询库数组,然后添加即可.整体代码如下.
```
self.textField = [SDTextField initWithFrame:CGRectMake(75, 100, 250, 35)];
    self.textField.dataArray = [NSMutableArray arrayWithArray:@[@"a",@"ab",@"A",@"c",@"admin"]];
    [self.view addSubview:self.textField];
```
唯一值得注意的,就是高度问题.诱导输入列表的的高度将会是textfield的三倍,如果你需要让列表高度更高或者更低,请自行修改**heightMultiple**,这个属性将会影响两者的高度比例.用法如下所示.
`    self.textField.heightMultiple = 5;`
#### 结束
SDTextField的相关的就到这了,最后附上Demo的传送门.
#### --->[SDTextFieldDemo的传送门](https://github.com/wang82426107/SDTextField)
