# iOS开发 GPS定位 指定语言 返回中文地点 - 月若无涯的专栏 - CSDN博客
2015年04月07日 12:34:09[月若无涯](https://me.csdn.net/u010124617)阅读数：4759
在iOS开发中，用到定位时经常会遇到这个头疼的问题：系统语言为英文时返回英文或拼音，系统语言是中文时才返回中文。
下面是强制系统获取的定位地点为中文。方案是网上的，但优化了一下，当逆地理编码结束后才恢复系统语言，避免一些极端情况下的不生效问题。
实测可行。有问题欢迎留言。
`
```
-(void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation 
fromLocation:(CLLocation *)oldLocation{
    // 获取经纬度
    NSLog(@"纬度:%f",newLocation.coordinate.latitude);
    NSLog(@"经度:%f",newLocation.coordinate.longitude);
    // 停止位置更新
    [manager stopUpdatingLocation];
    // 保存 Device 的现语言 (英语 法语 ，，，)
    NSMutableArray *userDefaultLanguages = [[NSUserDefaults standardUserDefaults]
    objectForKey:@"AppleLanguages"];
    // 强制 成 简体中文
    [[NSUserDefaults standardUserDefaults] setObject:[NSArray arrayWithObjects:@"zh-hans",nil] 
    forKey:@"AppleLanguages"];
    // 逆地理编码
    CLGeocoder * geoCoder = [[CLGeocoder alloc] init];
    [geoCoder reverseGeocodeLocation:newLocation completionHandler:^(NSArray *placemarks, NSError *error) {
        if(!error){
            for (CLPlacemark * placemark in placemarks) {
                NSString *cityName = [placemark locality];
                NSLog(@"cityName===》%@", cityName);//这里可看到输出为中文
                break;
            }
        }
        // 还原Device 的语言
        [[NSUserDefaults standardUserDefaults] setObject:userDefaultLanguages forKey:@"AppleLanguages"];
    }];
}
```
`
