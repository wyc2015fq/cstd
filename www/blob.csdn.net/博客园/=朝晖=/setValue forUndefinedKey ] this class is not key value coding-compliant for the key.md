# setValue:forUndefinedKey:]: this class is not key value coding-compliant for the key - =朝晖= - 博客园
# [setValue:forUndefinedKey:]: this class is not key value coding-compliant for the key](https://www.cnblogs.com/dhcn/p/7130904.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/h3c4lenovo/article/details/8556887
今天在写自定义UITableViewCell的时候遇到一个Crash，特别无语，折腾了大半天，终于解决了。
Bug如下：
**Terminating app due to uncaught exception 'NSUnknownKeyException', reason: '[<NSObject 0x8a4b500> setValue:forUndefinedKey:]: this class is not key value coding-compliant for the key strss.'**
先说解决办法吧：
![](http://img.my.csdn.net/uploads/201301/30/1359536319_1099.png)
然后讲故事：
### 1.如何使用IOS UITableView?
参考：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/h3c4lenovo/article/details/8307253
### 2.如何使用自定义的UITableViewCell实现UITableView？
参考：http://www.howzhi.com/group/iosDevelop/discuss/2068
### 3.单独创建的xib/nib文件如何与.h/.m文件关联起来？
定义xib中File's Owner 或 View的Custom Class 如上图所示。注：网上大部分说报错的原因要么就是存在无效的连接，或者File's Owner的custom class没有连接。
### 4.代码中如何引用自定义的TableViewCell？
**[cpp]**[view plain](http://blog.csdn.net/h3c4lenovo/article/details/8556887#)[copy](http://blog.csdn.net/h3c4lenovo/article/details/8556887#)
- - (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath  
- {  
- static NSString *CustomCellIdentifier = @"testCellIdentifier";  
- 
- static BOOL nibsRegistered = NO;  
- if (!nibsRegistered) {  
-         UINib *nib = [UINib nibWithNibName:@"testCell" bundle:nil];  
-         [tableView registerNib:nib forCellReuseIdentifier:CustomCellIdentifier];  
-         nibsRegistered = YES;  
-     }  
- 
-     testCell *cell = [tableView dequeueReusableCellWithIdentifier:CustomCellIdentifier];  
- 
-     [cell setMyText:@"xxx"];  
- 
- return cell;  
- }  
- 
