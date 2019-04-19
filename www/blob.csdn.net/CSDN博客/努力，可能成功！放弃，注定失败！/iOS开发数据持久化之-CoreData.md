# iOS开发数据持久化之-CoreData - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月08日 23:54:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1186
> 
CoreData使用了面向对象的方式来操作数据,负责在数据库中存储数据.它的底层就是使用类似于SQL的技术来实现的.CoreData提供了一种简便的对象持久化管理方式,让我们可以不关心数据的存储,只需关心对象的增加,删除,更改,读写就好了.
## CoreData介绍
- CoreData是苹果公司封装的数据持久化框架,在iOS3.0中开始开放.
- 它允许用户按照实体-属性-值模型组织数据,并以二级制,XML或者sqlite数据文件的格式进行持久化
## 优点
- 它是苹果公司原生态的产品.
- 它可以节省代码量,大概是30%~70%
- 它支持可视化建模
- CoreData支持数据库版本升级
## 构成
我们在创建工程的时候,勾选Use Core Data选项,系统就会在AppDelegate自动帮我们生成包含CoreData的属性和方法.并且生成一个test.xcdatamodeld的文件.
![](http://upload-images.jianshu.io/upload_images/1230517-1ce9b719fc3dba11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Use Core Data
在AppDelegate文件中
![](http://upload-images.jianshu.io/upload_images/1230517-b5b3fbc92c9cb1f0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
APPDelegate.h
上面图片是系统自动帮我们生成的属性和方法代码.它们的作用分别是:
**三个属性:**
1.managedObjectContext,是被管理的数据上下文.它被用来操作实际的内容,进行插入,查询,删除数据.
2.managedObjectModel,它是一个被管理的数据模型.可以简单的理解为可视化建模文件,我们在可视化建模中是Entity自动生成Model,就是这个对象,方便让文件存储助理来进行管理.
3.persistentStoreCoordinator,这是一个持久化的存储助理.它是CoreData的核心,他负责连接所有的模块,包括真实的存储文件.
**两个方法:**
1.saveContext,它的作用就是将我们在内存中的数据进行持久化.
2.applicationDocumentsDirectory,这个方法是用来获取真实文件的路径的.
## 使用CoreData
这是使用一个简单的例子,对CoreData的增,删,改,查的使用进行说明.
![](http://upload-images.jianshu.io/upload_images/1230517-6eef84f68b9dffb2.gif?imageMogr2/auto-orient/strip)
创建Entity
我们使用CoreData实现数据在tableView上的展示(查),添加(增),删除(删),点击更改(改).
#### UI界面
在storyboard中创建一个UITableViewController,把它放在NavgationController中,设置一个右按钮.为其关联方法,并且指定其为 initial View Controller.创建类文件,并进行关联,设置cell的样式为Subtitle.
在这里不详细赘述,如果想要了解可以移步[[http://www.jianshu.com/p/872b84d982ae](http://www.jianshu.com/p/872b84d982ae)]
![](http://upload-images.jianshu.io/upload_images/1230517-4fe230d3fbc4a22b.gif?imageMogr2/auto-orient/strip)
UI界面搭建,简单演示
#### 创建文件
选中.xcdatamodeld文件,按照上面的步骤创建一个Person实例,点击任务栏的Editor按钮,选择下图红框中的选项创建文件.
![](http://upload-images.jianshu.io/upload_images/1230517-37deb6807edafeae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建Person文件
因为我们创建工程的时候,系统已经为我们在AppDelegate生成了一些属性和方法.我们只需要在tableView中引入APPDelegate的头文件就好了.
##### 代码
![](http://upload-images.jianshu.io/upload_images/1230517-41f684ce02202bbe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
属性
声明两个属性,一个是上下文对象,我们使用它来处理所有关于存储的相关请求.另外一个就是数据源啦,用来在tableView上展示的数据.
既然声明了属性,我们就需要在为他们初始化,我们在**ViewDidLoad**方法中为其初始化,因为我们的managedObjectContext是来自与AppDelegate,所以选择下面的初始化方法.
```
// 进行数据初始化
    AppDelegate *dele = [UIApplication sharedApplication].delegate;
    self.myContext = dele.managedObjectContext;
    self.allData = [NSMutableArray array];
    // 通过CoreData读取本地所有的数据
    [self getAllDataFromCoreData];
```
点击添加按钮添加数据
```
// 添加数据
- (IBAction)addAction:(UIBarButtonItem *)sender {
    // 创建student对象
    // 创建一个实体描述对象
    NSEntityDescription *stuDis = [NSEntityDescription entityForName:@"Student" inManagedObjectContext:self.myContext];
    Student *stu = [[Student alloc]initWithEntity:stuDis insertIntoManagedObjectContext:self.myContext];
    // 给属性赋值
    stu.name = @"张三";
    stu.age = arc4random() % 73 + 1;
    // 更新数据源
    [self.allData addObject:stu];
    // 修改界面
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:self.allData.count - 1 inSection:0];
    [self.tableView insertRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationLeft];
    // 将数据保存到文件中进行持久化
    NSError *error = nil;
    [self.myContext save:&error];
    if (nil != error) {
        NSLog(@"数据库持久化,存在问题");
    }
    [((AppDelegate *)[UIApplication sharedApplication].delegate) saveContext];
}
```
从数据库查询数据
```
- (void)getAllDataFromCoreData {
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Student" inManagedObjectContext:self.myContext];
    [fetchRequest setEntity:entity];
    // 排序条件
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"age"
                                                                   ascending:YES];
    [fetchRequest setSortDescriptors:[NSArray arrayWithObjects:sortDescriptor, nil]];
    NSError *error = nil;
    NSArray *fetchedObjects = [self.myContext executeFetchRequest:fetchRequest error:&error];
    if (fetchedObjects == nil) {
        NSLog(@"两手空空,你让我如何盆满钵满");
    }
    // 将查询到的数据添加到数据源
    [self.allData addObjectsFromArray:fetchedObjects];
    // 从新加载tableView
    [self.tableView reloadData];
}
```
点击cell改变数据
```
// 点击cell的响应事件
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    // 先查询
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Student" inManagedObjectContext:self.myContext];
    [fetchRequest setEntity:entity];
    // Specify how the fetched objects should be sorted
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"age"
                                                                   ascending:YES];
    [fetchRequest setSortDescriptors:[NSArray arrayWithObjects:sortDescriptor, nil]];
    NSError *error = nil;
    NSArray *fetchedObjects = [self.myContext executeFetchRequest:fetchRequest error:&error];
    Student *stu = self.allData[indexPath.row];
    stu.name = @"尼古拉斯-赵四";
    stu.age = 15;
    // 更新数据源
    [self.allData removeAllObjects];
    [self.allData addObjectsFromArray:fetchedObjects];
    // 刷新UI
    [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationLeft];
    // 将修改本地持久化
    [self.myContext save:nil];
}
```
设置编辑时间 右滑删除
```
// 当点击tableViewCell的删除按钮的时候回调用(提交编辑请求的时候)
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
        // 获取当前cell代表的数据
        Student *stu = self.allData[indexPath.row];
        // 更新数据源
        [self.allData removeObject:stu];
        // 更新UI
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        // 将临时数据库里进行删除并进行本地持久化
        [self.myContext deleteObject:stu];
        [self.myContext save:nil];
}
```
其他的方法,设置分区数,行数,以及cell
```
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.allData.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"main_cell" forIndexPath:indexPath];
    Student *stu = self.allData[indexPath.row];
    cell.textLabel.text = stu.name;
    cell.detailTextLabel.text = [NSString stringWithFormat:@"%d",stu.age];
    return cell;
}
```
## 最后的效果
![](http://upload-images.jianshu.io/upload_images/1230517-8f1d477cebe11881.gif?imageMogr2/auto-orient/strip)
效果

[](http://www.jianshu.com/sign_in)
