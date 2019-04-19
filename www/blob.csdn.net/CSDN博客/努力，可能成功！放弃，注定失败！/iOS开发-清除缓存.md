# iOS开发 - 清除缓存 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月01日 09:30:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4186
- 
### Bg
1）由于项目需求需要，需要在设置页面其中一个cell里面展示缓存大小，并提供清除缓存功能，发现之前自己一直使用的那个计算并清除缓存的方法并不完善，而且一直都有想要完善一下的想法，由此写下这篇文章，提供给有需要帮助的朋友参考
2）代码的话就只提供一个简版的工具类了，使用很简单，也简单说明下
- 
先看下简单的效果图
![](http://upload-images.jianshu.io/upload_images/313229-2f110190644587f4.gif?imageMogr2/auto-orient/strip)
cache.gif
- 
### 简单介绍
现在这个功能已经被抽成一个简单的工具类，既然是工具类，那么就先说明下如何使用
- 
##### 获取文件夹大小
1）既然清除缓存就需要先计算缓存，工具类里面提供了一个计算缓存大小的类方法，你只需要提供一个需要计算的文件夹路径
```php
```php
/**
 *  @author li bo, 16/05/29
 *
 *  获取path路径文件夹的大小
 *
 *  @param path 要获取大小的文件夹全路径
 *
 *  @return 返回path路径文件夹的大小
 */
+ (NSString *)getCacheSizeWithFilePath:(NSString *)path;
```
```
2）一般显示缓存大小都是在cell上的label里面，这个label可以是使用系统的可以自己添加的，这里提供一个简单版的使用
```
在tableView的cellForRowAtIndexPath这个方法里面
    if (indexPath.row==0)  //如果是清除缓存对应的这个cell
   {
       NSString *fileSize = [LBClearCacheTool getCacheSizeWithFilePath:filePath];
        if ([fileSize integerValue] == 0)//如果没有缓存，那么不需要显示缓存大小
       {
       cell.textLabel.text = @"清除缓存";
       }else //缓存大小不为0，显示缓存大小
       {
       cell.textLabel.text = [NSString stringWithFormat:@"清除缓存(%@)",fileSize];
       }
    }
```
3）获取文件夹大小部分代码实现
```
//调试
#ifdef DEBUG
    //如果文件夹不存在或者不是一个文件夹那么就抛出一个异常
    //抛出异常会导致程序闪退，所以只在调试阶段抛出，发布阶段不要再抛了,不然极度影响用户体验
    BOOL isDirectory = NO;
    BOOL isExist = [fileManager fileExistsAtPath:path isDirectory:&isDirectory];
    if (!isExist || !isDirectory)
    {
        NSException *exception = [NSException exceptionWithName:@"fileError" reason:@"please check your filePath!" userInfo:nil];
        [exception raise];
    }
    NSLog(@"debug");
//发布
#else
    NSLog(@"post");
#endif
//以上判断目的是忽略不需要计算的文件
        if (!isExist || isDirectory || [filePath containsString:@".DS"]) continue;
            //NSLog(@"%@",filePath);
            //指定路径，获取这个路径的属性
            //attributesOfItemAtPath:需要传文件夹路径
            //但是attributesOfItemAtPath 只可以获得文件属性，不可以获得文件夹属性，这个也就是需要for-in遍历文件夹里面每一个文件的原因
            NSDictionary *dict=   [fileManager attributesOfItemAtPath:filePath error:nil];
            NSInteger size=[dict[@"NSFileSize"] integerValue];
            totleSize+=size;
     }
```
##### 清除文件夹缓存
1）既然要清除，工具类里面也提供了一个清除文件夹缓存的类方法，需要辛苦你操作的依然是提供你需要清除缓存的文件夹全路径
```php
```php
/**
 *  @author li bo, 16/05/29
 *
 *  清除path路径文件夹的缓存
 *
 *  @param path  要清除缓存的文件夹全路径
 *
 *  @return 是否清除成功
 */
+ (BOOL)clearCacheWithFilePath:(NSString *)path;
```
```
2）一般清除缓存都是在我们点击了对应这一行cell的时候才会去执行的，这里就在cell被点击的方法里面执行对应的清除操作
```
在tableView的didSelectRowAtIndexPath方法中执行
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
   if (indexPath.row == 0) {//如果是需要清除缓存对应的cell
       UIAlertController *alert=[UIAlertController alertControllerWithTitle:@"确定清除缓存吗?" message:nil preferredStyle:UIAlertControllerStyleActionSheet];
       //创建一个取消和一个确定按钮
       UIAlertAction *actionCancle=[UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
       //因为需要点击确定按钮后改变文字的值，所以需要在确定按钮这个block里面进行相应的操作
       UIAlertAction *actionOk=[UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
           //清楚缓存
          BOOL isSuccess = [LBClearCacheTool clearCacheWithFilePath:filePath];
           if (isSuccess)  //如果清除成功
          { 
               //刷新指定行，让cell更新缓存大小这个状态
               [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
               //来一个指示器，提示清除成功，这样用户体验比较好
               [SVProgressHUD setDefaultStyle:SVProgressHUDStyleDark];
               [SVProgressHUD showSuccessWithStatus:@"清除成功"];
               //弹框提示1秒后，把这个弹框消失，默认的消失时间有点长
               dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                   [SVProgressHUD dismiss];
               });
           }
      }];
       //将取消和确定按钮添加进弹框控制器
       [alert addAction:actionCancle];
       [alert addAction:actionOk];
        //显示弹框控制器
       [self presentViewController:alert animated:YES completion:nil];
   }
}
```
3）清除缓存功能部分代码实现
```
/拿到path路径的下一级目录的子文件夹
    NSArray *subpathArray = [fileManager contentsOfDirectoryAtPath:path error:nil];
 //删除子文件夹
        [fileManager removeItemAtPath:filePath error:&error];
        if (error) {
            message = [NSString stringWithFormat:@"%@这个路径的文件夹删除失败了，请检查后重新再试",filePath];
            return NO;
        }else {
            message = @"成功了";
        }
```
##### OK，文章暂时写到这里啦，上班时间还要专心工作滴，具体的获取缓存大小以及清除缓存功能的实现，包括这个工具类，由于文字解释比较详细，如果感兴趣的朋友，大家可以直接下载代码查看，另外大家没事可以多参考SD_Webimage源码，里面很多好东西的哦，它里面其实就有比较好的计算以及清除缓存的功能的。
##### 另外参考欢迎交流，感谢支持。
##### 可以[点击这里进行下载](https://github.com/LBCoderLee/ClearFileCache)
文／小小小阿博er（简书作者）
原文链接：http://www.jianshu.com/p/d03ab9160ef4
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
