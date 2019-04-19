# iOS10--消息通知的基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月20日 17:57:24[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：15556
官方将通知单独放在了UserNotifications.framework，使用时需要导入框架。
UserNotifications.framework主要类文件：
> 
UNCalendarNotificationTrigger
UNLocationNotificationTrigger
UNMutableNotificationContent
UNNotification
UNNotificationAction
UNNotificationAttachment
UNNotificationCategory
UNNotificationContent
UNNotificationRequest
UNNotificationResponse
UNNotificationServiceExtension
UNNotificationSettings
UNNotificationSound
UNNotificationTrigger
UNPushNotificationTrigger
UNTextInputNotificationAction
UNTextInputNotificationResponse
UNTimeIntervalNotificationTrigger
UNUserNotificationCenter
#### UNUserNotificationCenter的应用：
- **请求用户授权：**
```
UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];    
// 请求授权    
/*     
UNAuthorizationOptionBadge   = (1 << 0),     
UNAuthorizationOptionSound   = (1 << 1),     
UNAuthorizationOptionAlert   = (1 << 2),     
UNAuthorizationOptionCarPlay = (1 << 3),     
*/    
[center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert + UNAuthorizationOptionSound + UNAuthorizationOptionBadge)                          completionHandler:^(BOOL granted, NSError * _Nullable error) {                             
}];
```
`补充：`获取授权设置信息
```
// 获取通知授权和设置    
[center getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings * _Nonnull settings) {        
/*         
UNAuthorizationStatusNotDetermined : 没有做出选择         
UNAuthorizationStatusDenied : 用户未授权         
UNAuthorizationStatusAuthorized ：用户已授权         
*/        
if (settings.authorizationStatus == UNAuthorizationStatusNotDetermined) 
{                       
      NSLog(@"未选择");                  
}else if (settings.authorizationStatus == UNAuthorizationStatusDenied){                        
      NSLog(@"未授权");                   
}else if (settings.authorizationStatus == UNAuthorizationStatusAuthorized){                      
      NSLog(@"已授权");        
}       
}];
```
- 
**创建本地通知：**
```
// 创建一个本地通知
UNMutableNotificationContent *content_1 = [[UNMutableNotificationContent alloc] init];    
// 主标题    
content_1.title = [NSString localizedUserNotificationStringForKey:@"title" arguments:nil];    
// 副标题    
content_1.subtitle = [NSString localizedUserNotificationStringForKey:@"subtitle" arguments:nil];    
content_1.badge = [NSNumber numberWithInteger:1];    
content_1.body = [NSString localizedUserNotificationStringForKey:@"title_message_for_yan" arguments:nil];    
content_1.sound = [UNNotificationSound defaultSound];    
// 设置触发时间    
UNTimeIntervalNotificationTrigger *trigger_1 = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:10 repeats:NO];    
// 创建一个发送请求
UNNotificationRequest *request_1 = [UNNotificationRequest requestWithIdentifier:@"my_notification" content:content_1 trigger:trigger_1];
```
`补充：`
- UserNotifications提供了三种触发器：
```
UNTimeIntervalNotificationTrigger ：一定时间后触发
UNCalendarNotificationTrigger ： 在某月某日某时触发
UNLocationNotificationTrigger ： 在用户进入或是离开某个区域时触发
```
- @“my_notification”请求的标识符可以用来管理通知：
```
- 移除还未展示的通知
[center removePendingNotificationRequestsWithIdentifiers: @[@“my_notification”
]];
[center removeAllPendingNotificationRequests]; //  - (void)cancelAllLocalNotifications；
- 移除已经展示过的通知
[center removeDeliveredNotificationsWithIdentifiers:@[@“my_notification”
]];
[center removeAllDeliveredNotifications];
- 获取未展示的通知    
[center getPendingNotificationRequestsWithCompletionHandler:^(NSArray<UNNotificationRequest *> * _Nonnull requests) {
   NSLog(@"%@",requests);    
}];    
- 获取展示过的通知    
[center getDeliveredNotificationsWithCompletionHandler:^(NSArray<UNNotification *> * _Nonnull notifications) { 
  NSLog(@"%@",notifications);
}];
```
- 远程通知的格式：
`{ "aps":{ "alert":{ "title":"I am title", "subtitle":"I am subtitle", "body":"I am body" }, "sound":"default", "badge":1 } }`
具体请参考[官方文档](https://developer.apple.com/library/content/documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/TheNotificationPayload.html)
- 
**将通知请求添加到通知中心（UNUserNotificationCenter）：**
```
[center addNotificationRequest:request_1 withCompletionHandler:^(NSError * _Nullable error) {       
  }];
```
![](http://upload-images.jianshu.io/upload_images/1109379-a6749807020281cf.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
接收通知
- 
**处理通知：**
设置UNUserNotificationCenterDelegate：
`注意：`UNUserNotificationCenter 的 delegate 必须在
application:willFinishLaunchingWithOptions: or
application:didFinishLaunchingWithOptions: 方法中实现；
`center.delegate = self;`- 
应用内展示通知:
```
```
- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler{
// 如果不想显示某个通知，可以直接用空 options 调用 completionHandler: // completionHandler([])
completionHandler(UNNotificationPresentationOptionBadge + UNNotificationPresentationOptionSound);   
}
```
```
- 在用户与你推送的通知进行交互时被调用：
```
```
- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler{    
completionHandler();       
NSLog(@"userInfo--%@",response.notification.request.content.userInfo);
}
```
```
#### **UNNotificationCategory的应用：**
- **创建一个 category：**
```
/*     
UNNotificationActionOptionAuthenticationRequired = (1 << 0),    
UNNotificationActionOptionDestructive = (1 << 1), 取消     
UNNotificationActionOptionForeground = (1 << 2), 启动程序     
*/   
UNTextInputNotificationAction *textAction = [UNTextInputNotificationAction actionWithIdentifier:@"my_text" title:@"text_action" options:UNNotificationActionOptionForeground textInputButtonTitle:@"输入" textInputPlaceholder:@"默认文字"];    
UNNotificationAction *action = [UNNotificationAction actionWithIdentifier:@"my_action" title:@"action" options:UNNotificationActionOptionDestructive];    
UNNotificationAction *action_1 = [UNNotificationAction actionWithIdentifier:@"my_action_1" title:@"action_1" options:UNNotificationActionOptionAuthenticationRequired];    
/*     
UNNotificationCategoryOptionNone = (0),     
UNNotificationCategoryOptionCustomDismissAction = (1 << 0),     
UNNotificationCategoryOptionAllowInCarPlay = (2 << 0),     
*/    
UNNotificationCategory *category = [UNNotificationCategory categoryWithIdentifier:@"my_category" actions:@[textAction,action,action_1] intentIdentifiers:@[] options:UNNotificationCategoryOptionCustomDismissAction];    
NSSet *setting = [NSSet setWithObjects:category, nil];    
[center setNotificationCategories:setting];
```
- **在创建 UNNotificationContent 时把 categoryIdentifier 设置为需要的 category id 即可：**`content.categoryIdentifier = @"my_category";`
> 
远程推送也可以使用 category，只需要在 payload 中添加 category 字段，并指定预先定义的 category id 就可以了
- **处理category的通知：**
```
- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler{    
  completionHandler();       
  NSLog(@"userInfo--%@",response.notification.request.content.userInfo);       
// 获取通知中心的所有的Categories    
[center getNotificationCategoriesWithCompletionHandler:^(NSSet<UNNotificationCategory *> * _Nonnull categories) { 
            for (UNNotificationCategory *category in categories) { 
         if ([category.identifier isEqualToString:@"my_category"] && [response.notification.request.content.categoryIdentifier isEqualToString:@"my_category"]) { 
                for (UNNotificationAction *textAction in category.actions) {
                         if ([textAction.identifier isEqualToString:@"my_text"]) { 
                                       UNTextInputNotificationAction *text = (UNTextInputNotificationAction *)textAction;
                                       UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提示" message:text.textInputButtonTitle preferredStyle:UIAlertControllerStyleAlert];
                                      [alert addAction:[UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleCancel handler:nil]]; 
                                      [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:alert animated:YES completion:nil];
                          }
              } 
         }
      }
}];
}
```
![](http://upload-images.jianshu.io/upload_images/1109379-0eb2e4255284da49.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
长按 3D touch 效果图
![](http://upload-images.jianshu.io/upload_images/1109379-98145e34312e78ef.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
进入应用
##### iOS 10 中被标为弃用的 API
UILocalNotification
UIMutableUserNotificationAction
UIMutableUserNotificationCategory
UIUserNotificationAction
UIUserNotificationCategory
UIUserNotificationSettings
handleActionWithIdentifier:forLocalNotification:
handleActionWithIdentifier:forRemoteNotification:
didReceiveLocalNotification:withCompletion:
didReceiveRemoteNotification:withCompletion:
![](http://upload-images.jianshu.io/upload_images/1109379-9f0f959f28f31a29.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文／芝麻绿豆（简书作者）
原文链接：http://www.jianshu.com/p/98c740600dfa
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
