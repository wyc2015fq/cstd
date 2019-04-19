# 手把手教你修改iOS版QQ的运动步数 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月29日 22:26:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：8594
现在很多软件都加上了运动模块，比如QQ和微信，而且还有排行榜，可以和好友比较谁的运动步数多，任何东西只要添加了比较功能，就变得不一样了。今天教大家用代码去修改QQ上的运动步数，修改完效果是这样的：
![](http://upload-images.jianshu.io/upload_images/1276620-7fd16d3b64de6a05.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-06-21 下午5.02.56.png
###### 本帖子是抱着学习的心态来看待这个功能的，如果不喜欢请略过。
##### 1准备工作
你需要一个iOS程序员所需要的设备：一个mac系统的电脑、一个Xcode、一个开发者账号（用开发者账号这是因为要使用HealthKIt框架,就必须开启授权）、还有一个苹果手机.
##### 2新建项目
既然你都有开发者账号了，这我就只介绍重要的东西了，像怎么新建项目就不说了。
1.添加HealthKit.famework库。
2.开启授权，将Healthkit开关打开.如图：
![](http://upload-images.jianshu.io/upload_images/1276620-d6b282d30ab3ee00.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
786E95C6-E1E0-4272-BFBF-D19FBF4CE9AB.png
打开开关后会自动下载一个文件是后缀是：.entitlement的文件，如上图文件目录。
##### 3 上代码
1引入头文件
`#import <HealthKit/HealthKit.h>`
2初始化 HKHealthStore
HKHealthStore —— 关键类（使用HealthKit框架必须创建该类）
` self.healthStore = [[HKHealthStore alloc] init];`
3发出具体的请求许可,这里我们只需要写入步数就可以。
创建步数类型
```
HKQuantityType *stepCountType = [HKObjectType quantityTypeForIdentifier:HKQuantityTypeIdentifierStepCount];
 NSSet *writeDataTypes = [NSSet setWithObjects:stepCountType,  nil];
```
发出具体的请求许可
```
[self.healthStore requestAuthorizationToShareTypes:writeDataTypes readTypes:readDataTypes completion:^(BOOL success, NSError *error) {
            if (!success) {
              //失败了
                return;
            }
        }];
```
程序运行到这就回弹出健康的那个提示界面了，选择允许选项。
4 设置步数并且保存
```
//数据看类型为步数.
    HKQuantityType *quantityTypeIdentifier = [HKObjectType quantityTypeForIdentifier:HKQuantityTypeIdentifierStepCount];
    //表示步数的数据单位的数量
    HKQuantity *quantity = [HKQuantity quantityWithUnit:[HKUnit countUnit] doubleValue:100];
    //数量样本.
    HKQuantitySample *temperatureSample = [HKQuantitySample quantitySampleWithType:quantityTypeIdentifier quantity:quantity startDate:[NSDate date] endDate:[NSDate date] metadata:nil];
    //保存
    [self.healthStore saveObject:temperatureSample withCompletion:^(BOOL success, NSError *error) {
        if (success) {
            //保存成功
        }else {
            //保存失败
        }
    }];
```
那个100就是增加的步数了。
然后打开手机里的健康应用，你会发现步数增加了。再打开QQ里的运动，步数也增加了。理论上来说只要软件的步数是从健康应用里获取的就都可以改变，不只是QQ哦！！！！！
[github下载地址](https://github.com/idage/idageChangeQqSport)
赶快去试试吧
感谢观看，欢迎评论和批评
文／idage（简书作者）
原文链接：http://www.jianshu.com/p/3a3a800d25c4
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
