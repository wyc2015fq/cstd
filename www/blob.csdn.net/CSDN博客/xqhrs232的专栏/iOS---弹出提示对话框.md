# iOS---弹出提示对话框 - xqhrs232的专栏 - CSDN博客
2019年02月26日 16:31:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：45
原文地址::[https://www.cnblogs.com/anakin/p/7908335.html](https://www.cnblogs.com/anakin/p/7908335.html)
相关文章
1、[ios 自定义弹出对话框效果](https://www.cnblogs.com/zhangsongbai/p/3441568.html)----[https://www.cnblogs.com/zhangsongbai/p/3441568.html](https://www.cnblogs.com/zhangsongbai/p/3441568.html)
2、IOS弹出对话框让用户等待----[https://blog.csdn.net/lius1984/article/details/8030255?utm_source=blogxgwz2](https://blog.csdn.net/lius1984/article/details/8030255?utm_source=blogxgwz2)
**一、就一个选项的对话框**
### 代码块
|12345678910|`#pragma mark - 封装弹出对话框方法``// 提示错误信息``- (``void``)showError:(``NSString``*)errorMsg {``    ``// 1.弹框提醒``    ``// 初始化对话框``    ``UIAlertController *alert = [UIAlertController alertControllerWithTitle:@``"提示"``message:errorMsg preferredStyle:UIAlertControllerStyleAlert];``    ``[alert addAction:[UIAlertAction actionWithTitle:@``"确定"``style:UIAlertActionStyleDefault handler:``nil``]];``    ``// 弹出对话框``    ``[``self``presentViewController:alert animated:``true``completion:``nil``];``}`|
需要调用弹出对话框方法的地方使用的代码如下：
### 代码块
```
// 弹出“请检查用户名和密码是否为空！”对话框
[self showError:@"请检查用户名和密码是否为空！"];
```
效果如图所示： 
![这里写图片描述](https://img-blog.csdn.net/20160920163037071)
**二、如果是要做两个选项的对话框**
先在.h文件中定义如下：
```
@property (strong, nonatomic) UIAlertAction *okAction;
@property (strong, nonatomic) UIAlertAction *cancelAction;
```
然后在.m文件中写入如下代码：
#pragma mark - 注销：弹出对话框
- (void) logout {
    // 初始化对话框
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提示" message:@"确认注销吗？" preferredStyle:UIAlertControllerStyleAlert];
    // 确定注销
    _okAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction *_Nonnull action) {
        // 1.清除用户名、密码的存储
        // 2.跳转到登录界面
        [self performSegueWithIdentifier:@"Logout" sender:nil];
    }];
    _cancelAction =[UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
    [alert addAction:_okAction];
    [alert addAction:_cancelAction];
    // 弹出对话框
    [self presentViewController:alert animated:true completion:nil];
}
需要调用弹出对话框方法的地方使用的代码如下：
### 代码块
```
// 弹出“确认注销吗？”对话框
[self logout];
```
效果如图所示： 
![这里写图片描述](https://img-blog.csdn.net/20160920163114021)
