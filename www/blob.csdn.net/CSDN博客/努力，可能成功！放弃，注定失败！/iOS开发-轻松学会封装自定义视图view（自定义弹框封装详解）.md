# iOS开发-轻松学会封装自定义视图view（自定义弹框封装详解） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月04日 14:35:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：282
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)

原文地址:[点击打开链接](http://www.jianshu.com/p/de2ecfd770c2)
我们在做开发时经常会自定义一些view，并且如果如果该view比较复杂或者经常被调用到，我们就可以将该view单独放到一个类中，方便调用或者修改。笔者前段时间封装了一个自定义的弹出框，功能简单，借此跟大家交流一下封装自定义的视图view的方法与心得，仅仅是为了分享学习，并不建议直接在项目中使用，如果有错误或者不足之处，欢迎大家前来批评指正。
先上效果图：
http://www.jianshu.com/p/de2ecfd770c2![](http://upload-images.jianshu.io/upload_images/1946077-571516e59f028720.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
058C982B-D7A0-4F82-875D-71559F13A562.png
在ViewController.m中 导入"SZKAlterView.h"头文件
调用代码：
```
SZKAlterView *lll=[SZKAlterView alterViewWithTitle:@"简书号:iOS_凯" content:@"感谢各位朋友的关注与鼓励" cancel:@"取消" sure:@"确定" cancelBtClcik:^{
        //取消按钮点击事件
        NSLog(@"取消");
    } sureBtClcik:^{
        //确定按钮点击事件
        NSLog(@"确定");
    }];
    [self.view addSubview:lll];
```
需要注意的是：
SZKAlterView直接封装成了类方法，并且可以手动填写标题，内容，取消按钮的内容，确定按钮的内容，还有一个需要注意的就是笔者将取消按钮和确定按钮的点击事件利用block传递出来了，具体实现方法，下面讲到。
实现方法：
SZKAlterView.h 中
```
#import <UIKit/UIKit.h>
//取消按钮点击事件
typedef void(^cancelBlock)();
//确定按钮点击事件
typedef void(^sureBlock)();
@interface SZKAlterView : UIView
@property(nonatomic,copy)cancelBlock cancel_block;
@property(nonatomic,copy)sureBlock sure_block;
/**
 *  简书号：iOS_凯  http://www.jianshu.com/users/86b0ddc92021/latest_articles
 *
 *  @param title       标题
 *  @param content     内容
 *  @param cancel      取消按钮内容
 *  @param sure        确定按钮内容
 *  @param cancelBlock 取消按钮点击事件
 *  @param sureBlock   确定按钮点击事件
 *
 *  @return SZKAlterView
 */
+(instancetype)alterViewWithTitle:(NSString *)title
                          content:(NSString *)content
                           cancel:(NSString *)cancel
                             sure:(NSString *)sure
                    cancelBtClcik:(cancelBlock)cancelBlock
                      sureBtClcik:(sureBlock)sureBlock;
@end
```
SZKAlterView.m中
```
-(instancetype)initWithFrame:(CGRect)frame
{
    self=[super initWithFrame:frame];
    if (self) {
        //标题
        _titleLb=[[UILabel alloc]initWithFrame:CGRectMake(0, 0, self.bounds.size.width, 50)];
        _titleLb.textAlignment=NSTextAlignmentCenter;
        _titleLb.textColor=[UIColor blackColor];
        [self addSubview:_titleLb];
        //内容
        _contentLb=[[UILabel alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(_titleLb.frame), self.bounds.size.width, 50)];
        _contentLb.textAlignment=NSTextAlignmentCenter;
        _contentLb.textColor=[UIColor redColor];
        [self addSubview:_contentLb];
        //取消按钮
        _cancelBt=[[UIButton alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(_contentLb.frame), self.bounds.size.width/2, 50)];
        _cancelBt.layer.borderColor=[UIColor grayColor].CGColor;
        _cancelBt.layer.borderWidth=0.5;
        [_cancelBt setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [_cancelBt addTarget:self action:@selector(cancelBtClick) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:_cancelBt];
        //确定按钮
        _sureBt=[[UIButton alloc]initWithFrame:CGRectMake(self.bounds.size.width/2, CGRectGetMaxY(_contentLb.frame), self.bounds.size.width/2, 50)];
        _sureBt.layer.borderColor=[UIColor grayColor].CGColor;
        _sureBt.layer.borderWidth=0.5;
        [_sureBt setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [_sureBt addTarget:self action:@selector(sureBtClick) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:_sureBt];
    }
    return self;
}
#pragma mark----实现类方法
+(instancetype)alterViewWithTitle:(NSString *)title
                          content:(NSString *)content
                           cancel:(NSString *)cancel
                             sure:(NSString *)sure
                    cancelBtClcik:(cancelBlock)cancelBlock
                      sureBtClcik:(sureBlock)sureBlock;
{
    SZKAlterView *alterView=[[SZKAlterView alloc]initWithFrame:CGRectMake(0, 0, 250, 150)];
    alterView.backgroundColor=[UIColor whiteColor];
    alterView.center=CGPointMake(WIDTH/2, HEIGHT/2);
    alterView.layer.cornerRadius=5;
    alterView.layer.masksToBounds=YES;
    alterView.title=title;
    alterView.content=content;
    alterView.cancel=cancel;
    alterView.sure=sure;
    alterView.cancel_block=cancelBlock;
    alterView.sure_block=sureBlock;
    return alterView;
}
#pragma mark--给属性重新赋值
-(void)setTitle:(NSString *)title
{
    _titleLb.text=title;
}
-(void)setContent:(NSString *)content
{
    _contentLb.text=content;
}
-(void)setSure:(NSString *)sure
{
    [_sureBt setTitle:sure forState:UIControlStateNormal];
}
-(void)setCancel:(NSString *)cancel
{
    [_cancelBt setTitle:cancel forState:UIControlStateNormal];
}
#pragma mark----取消按钮点击事件
-(void)cancelBtClick
{
    [self removeFromSuperview];
    self.cancel_block();
}
#pragma mark----确定按钮点击事件
-(void)sureBtClick
{
    [self removeFromSuperview];
    self.sure_block();
}
```
需要注意的是：
因为笔者将该view封装成了类方法，所以会调用这个类时会先执行`+(instancetype)alterViewWithTitle:(NSString *)title...`方法，紧接着执行`-(instancetype)initWithFrame:(CGRect)frame`方法，然后会继续执行`alterView.title=title;`属性赋值的方法，但是这时界面展示不出来内容的，需要在set方法中重新给相关内容赋值才会展示出来，最后俩个函数就是利用block将点击事件传递到外部类中。
大致的实现方法写到这里，如果有不明白的可以联系笔者，笔者有问必答，如果想要下载完整demo请点击：
SZKAlterView链接：[https://github.com/18811314750/SZKAlterView](https://github.com/18811314750/SZKAlterView)
如果觉得笔者写的不错，可以star下哟。
如果各位观众老爷还没有看够，还有一篇关于无限轮播图的封装
iOS开发-两句代码快速实现无限轮播图（基于ScrollView封装）
[http://www.jianshu.com/p/d240bd977689](http://www.jianshu.com/p/d240bd977689)
笔者的其他文章：
iOS在线音乐播放SZKAVPlayer（基于AVPlayer的封装）
[http://www.jianshu.com/p/4e0ac2898de0](http://www.jianshu.com/p/4e0ac2898de0)
iOS开发-一句代码调用实现网络的监测功能（基于AFNetworkReachabilityManager的封装）
[http://www.jianshu.com/p/b901ad0c1d81](http://www.jianshu.com/p/b901ad0c1d81)
如果有不足或者错误的地方还望各位读者批评指正，可以评论留言，笔者收到后第一时间回复。
QQ/微信：790057066 。
简书号：iOS_凯：[http://www.jianshu.com/users/86b0ddc92021/latest_articles](http://www.jianshu.com/users/86b0ddc92021/latest_articles)
GitHub个人主页：[https://github.com/18811314750](https://github.com/18811314750)
欢迎各位前来查看，star,感谢各位的阅读。
作者：iOS_凯
链接：http://www.jianshu.com/p/de2ecfd770c2
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
