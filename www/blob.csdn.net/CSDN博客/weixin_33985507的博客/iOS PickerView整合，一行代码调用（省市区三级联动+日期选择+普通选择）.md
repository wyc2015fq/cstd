# iOS PickerView整合，一行代码调用（省市区三级联动+日期选择+普通选择） - weixin_33985507的博客 - CSDN博客
2016年09月05日 22:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
# 2017-8-30更新：
支持CocoaPods安装，
pod 'MOFSPickerManager'即可
# **2018-02-05更新：**
1.去除原来繁琐配置，采用自带XML解析；
2.ToolView更换适配。
## 2018-04-04更新：
1.纯文本选择器自定义解析。
# 一、效果预览图如下：
![1640079-632ca9b9f96b1fda.png](https://upload-images.jianshu.io/upload_images/1640079-632ca9b9f96b1fda.png)
![1640079-265f37b5dd4b208c.png](https://upload-images.jianshu.io/upload_images/1640079-265f37b5dd4b208c.png)
![1640079-b69b52d00a44b37a.png](https://upload-images.jianshu.io/upload_images/1640079-b69b52d00a44b37a.png)

![1640079-ff88d09f55f93bd2.gif](https://upload-images.jianshu.io/upload_images/1640079-ff88d09f55f93bd2.gif)
## 二、用法
1.日期选择器调用（有多种调用方式，看demo即可）
> 
[[MOFSPickerManager shareManger] showDatePickerWithTag:1 commitBlock:^(NSDate *date) {
} cancelBlock:^{
}];
参数说明
* @param title : 中间标题，一般为nil
* @param cancelTitle : 左边标题 “取消”
* @param commitTitle : 右边标题 “确定”
* @param firstDate :第一次点击的时候显示的日期
* @param minDate : 可选择的最小日期，不限制则为nil
* @param maxDate : 可选择的最大日期，不限制则为nil
* @param tag ： 同一个界面显示多个日期选择器的时候，能够记住每一个选择器最后选择的日期（注意：不要使用相同的tag值）
* @param model : UIDatePickerMode 日期模式，有四种 UIDatePickerModeTime,   UIDatePickerModeDate, UIDatePickerModeDateAndTime, UIDatePickerModeCountDownTimer
2.普通选择器调用
> 
[[MOFSPickerManager shareManger] showPickerViewWithDataArray:@[@"疾风剑豪",@"刀锋意志",@"诡术妖姬",@"狂战士"] tag:1 title:nil cancelTitle:@"取消" commitTitle:@"确定" commitBlock:^(NSString *string) {
} cancelBlock:^{
}];
传入自定义model，keyMapper为要显示的该Model对应的key
> 
        Model *a = [Model new];
        a.age = 17;
        a.name = @"疾风剑豪";
        a.userId = 0001;
        Model *b = [Model new];
        b.age = 18;
        b.name = @"刀锋意志";
        b.userId = 0002;
        Model *c = [Model new];
        c.age = 22;
        c.name = @"诡术妖姬";
        c.userId = 0003;
        [[MOFSPickerManager shareManger] showPickerViewWithCustomDataArray:@[a, b, c] keyMapper:@"name" tag:1 title:@"选择英雄" cancelTitle:@"取消" commitTitle:@"确定" commitBlock:^(id model) {
            Model *m = (Model *)model;
            NSLog(@"%@-%zd", m.name, m.userId);
        } cancelBlock:^{
        }];
3.地址选择器调用
> 
[[MOFSPickerManager shareManger] showMOFSAddressPickerWithTitle:nil cancelTitle:@"取消" commitTitle:@"完成" commitBlock:^(NSString *address, NSString *zipcode) {
} cancelBlock:^{
}];
地址选择器附带根据地址查询区域码或者根据区域码查询地址功能：
用法：
【注意：一定要用“-”间隔，可查询省份例如传参"450000"；或者省份+城市例如"450000-450900"；也可以查省+市+区例如"450000-450900-450921"
。根据地址查区域码以此类推。】
①根据区域码查询地址
> 
[[MOFSPickerManager shareManger] searchAddressByZipcode:@"450000-450900-450921" block:^(NSString *address) {
NSLog(@"%@",address);
}];
②根据地址查询区域码
> 
[[MOFSPickerManager shareManger] searchZipCodeByAddress:@"河北省-石家庄市-长安区" block:^(NSString *zipcode) {
NSLog(@"%@",zipcode);
}];
### 三、配置：
*2018-02-05更新：已去除繁琐配置*
1.将MOFSPickerManager文件夹拖到工程即可
[Demo下载](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fmemoriesofsnows%2FMOFSPickerManagerDemo)
