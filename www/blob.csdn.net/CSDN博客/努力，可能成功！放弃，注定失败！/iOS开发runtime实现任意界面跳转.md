# iOS开发runtime实现任意界面跳转 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月06日 17:43:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1489
## iOS开发runtime实现任意界面跳转
### 1.在开发中我们常常有这种需求：比如说现在有一组列表，服务器返回的数据在这组列表中，点击列表的某一行是根据服务器返回的Action字段决定跳转，这个时候的界面跳转其实是不确定的，我们需要根据服务器返回的数据动态判断，这时，就可以通过运行时，只需要传入控制器的名称以及控制器的属性，我们就可以直接执行push操作，下面说说详细实现过程。
### 2.首先我们创建一个tableView，里面的数据源是下面这样的：
```
// customTitle对应设置控制器的标题，color对应颜色，action对应跳转哪个控制器
- (NSArray *)dataArray
{
    if (_dataArray == nil) {
        _dataArray = @[
                       @{@"customtitle":@"第四页",@"action":@"CWFourViewController",@"color":[UIColor redColor]},
                       @{@"customtitle":@"第一页",@"action":@"CWFirstViewController",@"color":[UIColor greenColor]},
                       @{@"customtitle":@"第二页",@"action":@"CWSecondViewController",@"color":[UIColor grayColor]},
                       @{@"customtitle":@"第三页",@"action":@"CWThirdViewController",@"color":[UIColor lightGrayColor]},
                       @{@"customtitle":@"第四页",@"action":@"CWFourViewController",@"color":[UIColor purpleColor]}
                       ];
    }
    return _dataArray;
}
```
## 3.下面是具体实现过程
. 首先需要设置tableView的数据源方法和代理方法
```
#pragma mark - Table view data source
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dataArray.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell" forIndexPath:indexPath];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
    }
    cell.textLabel.text = self.dataArray[indexPath.row][@"customtitle"];
    return cell;
}
// 主要是下面这步，通过传入控制器名称和action以及当前的导航控制器就可以执行push操作
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString *vcName = self.dataArray[indexPath.row][@"action"];
    [SkipCenterTool runtimePush:vcName dic:self.dataArray[indexPath.row] nav:self.navigationController];
}
```
. 我们下面就来看看运行时的具体实现过程,主要过程就是根据类名创建一个类，并且注册你创建的这个类并且实例化，然后首先遍历这个类的所有属性，看有没有传入字典中对应的，如果有，就直接用KVC赋值，如果没有，继续遍历，如果当前类中没有找到，就到当前类的父类中继续遍历，重复上述操作：
```
// push控制器
+ (void)runtimePush:(NSString *)vcName dic:(NSDictionary *)dic nav:(UINavigationController *)nav {
    //类名(对象名)
    NSString *class = vcName;
    const char *className = [class cStringUsingEncoding:NSASCIIStringEncoding];
    Class newClass = objc_getClass(className);
    if (!newClass) {
        //创建一个类
        Class superClass = [NSObject class];
        newClass = objc_allocateClassPair(superClass, className, 0);
        //注册你创建的这个类
        objc_registerClassPair(newClass);
    }
    // 创建对象(写到这里已经可以进行随机页面跳转了)
    id instance = [[newClass alloc] init];
    //下面是传值－－－－－－－－－－－－－－
    [dic enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
        if ([self checkIsExistPropertyWithInstance:instance verifyPropertyName:key]) {
            //kvc给属性赋值
            NSLog(@"%@,%@",obj,key);
            [instance setValue:obj forKey:key];
        }else {
            NSLog(@"不包含key=%@的属性",key);
        }
    }];
    [nav pushViewController:instance animated:YES];
}
/**
 *  检测对象是否存在该属性
 */
+ (BOOL)checkIsExistPropertyWithInstance:(id)instance verifyPropertyName:(NSString *)verifyPropertyName
{
    unsigned int outCount, i;
    // 获取对象里的属性列表
    objc_property_t * properties = class_copyPropertyList([instance
                                                           class], &outCount);
    for (i = 0; i < outCount; i++) {
        objc_property_t property =properties[i];
        //  属性名转成字符串
        NSString *propertyName = [[NSString alloc] initWithCString:property_getName(property) encoding:NSUTF8StringEncoding];
        // 判断该属性是否存在
        if ([propertyName isEqualToString:verifyPropertyName]) {
            free(properties);
            return YES;
        }
    }
    free(properties);
    // 再遍历父类中的属性
    Class superClass = class_getSuperclass([instance class]);
    //通过下面的方法获取属性列表
    unsigned int outCount2;
    objc_property_t *properties2 = class_copyPropertyList(superClass, &outCount2);
    for (int i = 0 ; i < outCount2; i++) {
        objc_property_t property2 = properties2[i];
        //  属性名转成字符串
        NSString *propertyName2 = [[NSString alloc] initWithCString:property_getName(property2) encoding:NSUTF8StringEncoding];
        // 判断该属性是否存在
        if ([propertyName2 isEqualToString:verifyPropertyName]) {
            free(properties2);
            return YES;
        }
    }
    free(properties2); //释放数组
    return NO;
}
```
. 这里我在测试父类中提供了两个属性：
```
@property (nonatomic, strong) UIColor *color;
@property (nonatomic, strong) NSString *customtitle;
// 实现过程
- (void)setColor:(UIColor *)color
{
    _color = color;
    self.view.backgroundColor = color;
}
- (void)setCustomtitle:(NSString *)customtitle
{
    _customtitle = customtitle;
    self.title = customtitle;
}
```
## 4.下面看看测试结果，由此可见，我们已经实现了自由跳转：，并且可以更改属性值
![](http://upload-images.jianshu.io/upload_images/1517213-f91ac843eef826f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
测试结果1
![](http://upload-images.jianshu.io/upload_images/1517213-36ccb1c6d6cac0e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
测试结果1
![](http://upload-images.jianshu.io/upload_images/1517213-b2259a0e60413500.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
测试结果1
## 5.现在分享源代码如下：
[Github地址](https://github.com/kingcong/SkipCenter)
## 6.希望喜欢的朋友给个喜欢或关注，非常感谢！！
文／风过无痕sama（简书作者）
原文链接：http://www.jianshu.com/p/1bcf5a238534
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
