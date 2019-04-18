# iOS 排序 - weixin_33985507的博客 - CSDN博客
2017年06月21日 15:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
iOS开发中很少需要自己写排序的算法，因为苹果已经为我们封装好了。
# 简单的数值排序
```
NSMutableArray* arr = [NSMutableArrayarrayWithObjects:  
                           [NSNumber numberWithInt:0],  
                           [NSNumber numberWithInt:3],  
                           [NSNumber numberWithInt:2],  
                           [NSNumber numberWithInt:1],  
                           [NSNumber numberWithInt:7],  
                           [NSNumber numberWithInt:8],  
                           [NSNumber numberWithInt:9],  
                           [NSNumber numberWithInt:10],  
                           [NSNumber numberWithInt:6],  
                           [NSNumber numberWithInt:5],  
                           [NSNumber numberWithInt:4],  
                           nil];  
      
    // 升序  
    // 0 --> 10  
    [arr sortUsingComparator:^NSComparisonResult(__strongid obj1,__strong id obj2){  
        return [obj1 intValue] > [obj2intValue];  
    }];  
       
      
    // 降序  
    // 10 --> 0  
    [arr sortUsingComparator:^NSComparisonResult(__strongid obj1,__strong id obj2){  
        return [obj1 intValue] < [obj2intValue];  
    }];
```
### 复杂类型排序
```
NSMutableArray* arr = [NSMutableArray arrayWithObjects:@"C", @"A", @"H", @"I", @"B", @"D", @"J", @"E", @"F", @"G", @"K", nil];  
      
    // 升序  
    // A --> K  
    [arr sortUsingComparator:^NSComparisonResult(__strong id obj1,__strong id obj2){  
        NSString *str1=(NSString *)obj1;  
        NSString *str2=(NSString *)obj2;  
        return [str1 compare:str2];  
    }];  
      
      
    // 降序  
    // K --> A  
    [arr sortUsingComparator:^NSComparisonResult(__strong id obj1,__strong id obj2){  
        NSString *str1=(NSString *)obj1;  
        NSString *str2=(NSString *)obj2;  
        return [str2 compare:str1];  
    }];
```
