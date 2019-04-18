# iOS CoreData（一） - weixin_33985507的博客 - CSDN博客
2017年03月19日 18:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
# CoreData简述
CoreData是ios系统推荐给我们的数据存储方案。在ios中数据存储有多种方式，包括NSUserDefault、writeToFile...、NSArchiver等，这里我主要说一下CoreData的用法。
  CoreData是数据库吗？很显然不是，它是一个完整的数据处理方案，封装的是底层的SQL语句，所以不需要我们会写SQL语句，就能够对数据进行处理，并提供了NSFetchResultsController可以将处理的结果显示在UITableView中。但是CoreData也是有缺点的，那就是它不如SQLite性能高。
下面详述CoreData API及其相关用法。
# CoreData详述
## 一、CoreData框架
CoreData是ios3.0推出的数据存储方案。让我们先看一下CoreData API 主要架构，我们可以看到该API包含了**40**个类(Classes)，**3**个协议(Protocols)，以及**3**个参考(References)。下面我给大家汇总，一方面让大家对CoreData整体框架有一个宏观把握，另一方面也为深入研究做铺垫。
**Tab.1  Classes  Lists**
|Classes|Function|
|----|----|
|1.NSAsynchronousFetchRequest|继承自：NSPersistentStoreRequest|
|2.NSBatchDeleteRequest|继承自：NSPersistentStoreRequest|
|3.NSBatchUpdateRequest|继承自：NSPersistentStoreRequest|
|4.NSSaveChangesRequest|继承自：NSPersistentStoreRequest|
|5.**NSFetchRequest**|继承自：NSPersistentStoreRequest|
|6.NSBatchDeleteResult|继承自：NSPersistentStoreResult|
|7.NSBatchUpdateResult|继承自：NSPersistentStoreResult|
|8.NSAsynchronousFetchResult|继承自：NSPersistentStoreAsynchronousResult|
|9.NSPersistentStoreAsynchronousResult|继承自：NSPersistentStoreResult|
|10.NSPersistentStoreRequest|继承自：NSObject|
|11.NSPersistentStoreResult|继承自：NSObject|
|12.NSAtomicStore|继承自：NSPersistentStore|
|13.NSIncrementalStore|继承自：NSPersistentStore|
|14.NSPersistentStore|继承自：NSObject|
|15.NSAtomicStoreCacheNode|继承自：NSObject|
|16.NSConstraintConflict|继承自：NSObject|
|17.**NSEntityDescription**|继承自：NSObject|
|18.NSEntityMapping|继承自：NSObject|
|19.NSEntityMigrationPolicy|继承自：NSObject|
|20.NSFetchedResultsController|继承自：NSObject|
|21.NSIncrementalStoreNode|继承自：NSObject|
|22.**NSManagedObject**|继承自：NSObject|
|23.**NSManagedObjectContext**|继承自：NSObject|
|24.NSManagedObjectID|继承自：NSObject|
|25.**NSManagedObjectModel**|继承自：NSObject|
|26.NSMappingModel|继承自：NSObject|
|27.NSMergeConflict|继承自：NSObject|
|28.NSMergePolicy|继承自：NSObject|
|29.NSMigrationManager|继承自：NSObject|
|30.NSPersistentContainer|继承自：NSObject|
|31.**NSPersistentStoreCoordinator**|继承自：NSObject|
|32.NSPersistentStoreDescription|继承自：NSObject|
|33.NSExpressionDescription|继承自：NSPropertyDescription|
|34.NSFetchedPropertyDescription|继承自：NSPropertyDescription|
|35.NSRelationshipDescription|继承自：NSPropertyDescription|
|36.NSAttributeDescription|继承自：NSPropertyDescription|
|37.NSPropertyDescription|继承自：NSObject|
|38.NSPropertyMapping|继承自：NSObject|
|39.NSQueryGenerationToken|继承自：NSObject|
|40.NSFetchRequestExpression|继承自：NSExpression|
很多的类吧，有人看晕了吗？好吧，那我现在列成树形结构，大家就清晰了。
![3691932-8b3fab60283c2d3c.png](https://upload-images.jianshu.io/upload_images/3691932-8b3fab60283c2d3c.png)
Fig.1 CoreData类树形图
**Tab.2  Protocols  Lists**
|Protocols|Function|
|----|----|
|1.NSFetchedResultsControllerDelegate|继承自：NSObject|
|2.NSFetchedResultsSectionInfo|---|
|3.NSFetchRequestResult|继承自：NSObject|
**Tab.3  Preference  Lists**
|Protocols|Function|
|----|----|
|1|Core Data Constants|
|2|Core Data Enumerations|
|3|Core Data Data Types|
上面介绍了CoreData这个API的主要框架，具体可见[CoreData官方](https://developer.apple.com/reference/coredata)。
## 二、CoreData实现原理
### 原理
CoreData存储数据，有几个重要的类需要记住，这个我已经在Tab.1中用加粗进行显示。NSFetchRequest、NSEntityDescription、NSManagedObject、NSManagedObjectContext、NSManagedObjectModel。我们先看一下实现数据存储方案的原理。
![3691932-bf73456f7fd7bd80.png](https://upload-images.jianshu.io/upload_images/3691932-bf73456f7fd7bd80.png)
Fig.2 CoreData实现数据存储原理
### 几个重要的类
*1.***NSManagedObjectModel** 数据模型
  可以看做是数据库的模型结构，包含了各个实体的定义信息。有点像SQLite.sqlite文件，表示一个.xcdatamodeld文件。
```
//创建方式
// 获取模型文件路径
NSURL *modelURL = [[NSBundle mainBundle] URLForResource :modelName withExtension: @"person'];
//根据模型文件创建模型对象
NSManagedObjectModel *model = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
//也可以利用下面方法，从应用改程序包中加载.xcdatamodeld模型文件
 NSManagedObjectModel *model = [NSManagedObjectModel mergedModelFromBundles:nil];
```
*2.***NSPersistentStoreCoordinator** 持久化存储协调器
  它是将对象的管理部分和持久化部分捆绑在一起，二者之间需要该调度器进行调节。它是最接近数据底层的。用来设置CoreData存储类型和存储路径。
```
//创建方式：在创建之前必须创建NSManagedObjectModel模型。
NSPersistentStoreCoordinator *persistent = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:model];
NSError *error = nil;
NSString *sqlPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,  NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:@"person.sqlite"];
[persistent addPersistentStoreWithType:NSSQLiteStoreType configuration:nil  URL :[NSURL fileURLWithPath:sqlPath] options:nil  error:&error];
if (error) {
    if (fail) {
        fail(error);
    }
} else 
{
    self.context.persistentStoreCoordinator = self.persistent;
    if (success) {
        success();
    }
}
```
*3.***NSManagedObjectContext** 管理上下文
  被管理数据的上下文，实际上是对所有数据库操作的一个缓存层，会把你所有的操作都先缓存起来避免大量磁盘IO造成不流畅，操作完成数据库后调用save进行持久化。可以理解成用来管理.xcdatamodeld中的数据。
```
//创建
self.context = [[NSManagedObjectContext alloc] init];
NSError *error = nil;
BOOL result = [self.context save: &error];
    if (!result) {
          if (fail) {
              fail(error);
          }
    } else {
          if (success)
              success();
    }
```
*4.***NSEntityDescription**
用来描述实体，相当于数据库表中一组数据描述。
```
//创建方式：不能用alloc init方式创建，通过传入上下文和实体名称，创建一个名称对应的实体对象（相当于数据库中一组数据，包含各种字段）。
NSManagedObject  *newEntity = [NSEntityDescription insertNewObjectForEntityForName: entityName inManagedObjectContext:self.context];
```
## 三、CoreData实现数据存储
下面我们在代码层级上进行说明CoreData的使用。
*1*.  新建.xcdatamodeld文件。
![3691932-bf975017bfbbb116.png](https://upload-images.jianshu.io/upload_images/3691932-bf975017bfbbb116.png)
新建.xcdatamodelId文件
新建完成如下所示。
![3691932-018d8e88c5b57640.png](https://upload-images.jianshu.io/upload_images/3691932-018d8e88c5b57640.png)
新建完成后
*2*.   下面增加实体名称，可以点击person.xcdatamodeld下面的Add Entity。
![3691932-46fbf52442a8c5b2.png](https://upload-images.jianshu.io/upload_images/3691932-46fbf52442a8c5b2.png)
增加实体文件
注意：实体名字可以修改，但是必须以大写字母开头。
![3691932-c5a3e81ac94a0c4a.png](https://upload-images.jianshu.io/upload_images/3691932-c5a3e81ac94a0c4a.png)
修改实体名字
修改成功以后如下图所示。
![3691932-904b8faaeae8c565.png](https://upload-images.jianshu.io/upload_images/3691932-904b8faaeae8c565.png)
实体名字修改完成
*3*. 增加实体里面的键值对，也就是增加数据库里面的字段。
![3691932-d5445cb23252b6ab.png](https://upload-images.jianshu.io/upload_images/3691932-d5445cb23252b6ab.png)
增加字段
*4*.  创建关联类操控CoreData实体对象。
![3691932-e1daa4f0a9ad5d85.png](https://upload-images.jianshu.io/upload_images/3691932-e1daa4f0a9ad5d85.png)
创建关联类
选择要管理的数据实体模型，这里默认给你已经勾选好了。
![3691932-b0172fcee618b929.png](https://upload-images.jianshu.io/upload_images/3691932-b0172fcee618b929.png)
选择数据模型
选择要管理的实体，这里默认给你已经勾选好了。
![3691932-18cda41d6c080b9a.png](https://upload-images.jianshu.io/upload_images/3691932-18cda41d6c080b9a.png)
选择管理实体
这里可以修改关联生成文件的类型是oc还是swift，我选择oc。
![3691932-def43f4617004f57.png](https://upload-images.jianshu.io/upload_images/3691932-def43f4617004f57.png)
修改关联文件的类型
选择person.xcdatamodeld文件，再选择Editor-->Create NSManageObjectSubclass自动生成四个文件。
![3691932-ba819e2539490bba.png](https://upload-images.jianshu.io/upload_images/3691932-ba819e2539490bba.png)
关联生成的四个文件
Command + B 编译一下。
![3691932-b752071a0765deb4.png](https://upload-images.jianshu.io/upload_images/3691932-b752071a0765deb4.png)
编译结果
这个结果很让我诧异，知道是有的内容重复了但是不知道什么原因，后来在网上还是找到了解决办法。
![3691932-7ba154c20fa788f0.png](https://upload-images.jianshu.io/upload_images/3691932-7ba154c20fa788f0.png)
解决办法
就是把红色框框内部的编译文件删除，避免重复编译。然后就好了。下面我们看看关联的NSManagedObject的子类里面都是什么。
```
PersonEntity+CoreDataClass.h文件中
```
```
#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
NS_ASSUME_NONNULL_BEGIN
@interface PersonEntity : NSManagedObject
@end
NS_ASSUME_NONNULL_END
#import "PersonEntity+CoreDataProperties.h"
```
```
PersonEntity+CoreDataClass.m文件中
```
```
#import "PersonEntity+CoreDataClass.h"
@implementation PersonEntity
@end
```
```
PersonEntity+CoreDataProperties.h文件中
```
```
NS_ASSUME_NONNULL_BEGIN
@interface PersonEntity (CoreDataProperties)
+ (NSFetchRequest<PersonEntity *> *)fetchRequest;
@property (nonatomic) double weight;
@property (nullable, nonatomic, copy) NSString *name;
@property (nullable, nonatomic, copy) NSString *location;
@property (nonatomic) int16_t identify;
@property (nonatomic) BOOL gender;
@property (nonatomic) double height;
@property (nonatomic) int16_t age;
@end
NS_ASSUME_NONNULL_END
```
```
PersonEntity+CoreDataProperties.m
```
```
@implementation PersonEntity (CoreDataProperties)
+ (NSFetchRequest<PersonEntity *> *)fetchRequest {
    return [[NSFetchRequest alloc] initWithEntityName:@"PersonEntity"];
}
@dynamic weight;
@dynamic name;
@dynamic location;
@dynamic identify;
@dynamic gender;
@dynamic height;
@dynamic age;
@end
```
**注意**：这里有几点需要说明：
- 这四各类都继承自NSManagedObject。
- name和location可以是nullable，表示CoreData数据库存储的对象可能为nil，字段值可以为NULL。
- PersonEntity+CoreDataProperties这个类不能利用alloc init方式进行创建。苹果API提供了几个专门创建实体对象的方法。
## 四、CoreData实现数据的增删改查
*1*.  insert数据的增加
```
a) 根据Entity和Context获取一个新的NSManagedObject
    NSManagedObject *newEntity = [NSEntityDescription insertNewObjectForEntityForName:entityName inManagedObjectContext:self.context];
b) 通过setValue:forkey:给NSManagedObject对象赋值
   [newEntity setValue forKey: key];
c) 保存修改
   NSError *error = nil;
   BOOL result = [self.context  save: &error];
```
*2*.  delete数据的删除
```
a) 通过查询获得需要删除的NSManagedObject, 一般用NSPredicate语句。
b) 在for循环中，调用deleteObject：方法逐个删除。
   [self.context deleteObject: entity];
c) self.context 进行保存。
```
*3*.  update数据的修改
```
a) 通过查询找到需要修改的NSManagedObject 的集合，一般用NSPredicate。
b) 在for循环中，调用NSManagedObject的setValue：forkey: 方法给各个属性赋值。
c) 上下文进行保存。
```
*4*.  read数据的查询
```
a) 创建请求对象。
  NSFetchRequest *request = [[NSFetchRequest alloc] init];
b) 设置需要查询的实体描述
  NSEntityDescription *desc = [NSEntityDescription entityForName:self.entityName  inManagedObjectContext: self.context];
c) 设置查询条件。
   NSPredicate *predicate = [NSPredicate predicateWithFormat:filterString];
d) 将实体和排序都赋值给请求对象。
  request.entity = desc;
  request.predicate = predicate;
  request.sortDescriptors = descArr;
e) 开始查询
  NSError *error = nil;
  // NSManagedObject对象集合
  NSArray *objs = [self.context executeFetchRequest:request error:&error];
  // 查询结果数目
  NSUInteger count = [self.context countForFetchRequest:request error:&error];
```
从上面可以看到，增删改查四种，查询是最复杂的，而且删除和修改也是基于查询的，可以这么认为查询如果会了，那么增删改查就差不多了。其他的就是保存等其他的操作，那就简单了。
## 五、CoreData封装
下面对CoreData进行封装。
```
DDCoreDataManager.h文件
```
```
#import <Foundation/Foundation.h>
#import "CoreData/CoreData.h"
#define kDDCoreDataManagerModelName        (@"person")
@interface DDCoreDataManager : NSObject
@property (readonly, nonatomic, strong) NSManagedObjectContext *manageObjectContext;
@property (readonly, nonatomic, strong) NSManagedObjectModel *manageObjectModel;
@property (readonly, nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;
+ (instancetype)shareCoreDataManager;
- (void)saveContext;
@end
```
```
DDCoreDataManager.m文件
```
```
#import "DDCoreDataManager.h"
@implementation DDCoreDataManager
@synthesize manageObjectContext = _manageObjectContext;
@synthesize manageObjectModel = _manageObjectModel;
@synthesize persistentStoreCoordinator = _persistentStoreCoordinator;
static DDCoreDataManager *coreDataManager;
#pragma mark - 单例
+ (instancetype)shareCoreDataManager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        coreDataManager = [[self alloc] init];
    });
    return coreDataManager;
}
#pragma mark - 被管理对象模型
- (NSManagedObjectModel *)manageObjectModel
{
    if (_manageObjectModel != nil) {
        return _manageObjectModel;
    }
    
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:kDDCoreDataManagerModelName withExtension:@"momd"];
    _manageObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return _manageObjectModel;
}
#pragma mark - document目录
- (NSURL *)applicationDocumentDirectory
{
    return [[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask].lastObject;
}
#pragma mark - 实例化调度器
- (NSPersistentStoreCoordinator *)persistentCoordinator
{
    if (_persistentStoreCoordinator != nil) {
        return _persistentStoreCoordinator;
    }
    
    NSURL *coordinatorStoreURL = [[self applicationDocumentDirectory] URLByAppendingPathComponent:[NSString stringWithFormat:@"%@.sqlite",kDDCoreDataManagerModelName]];
    
    NSError *error = nil;
    _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self manageObjectModel]];
    if (![_persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:coordinatorStoreURL options:nil error:&error]) {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        dict[NSLocalizedDescriptionKey] = @"Failed to initialize the application's saved data";
        dict[NSLocalizedFailureReasonErrorKey] =  @"There was an error creating or loading the application's saved data.";
        dict[NSUnderlyingErrorKey] = error;
        error = [NSError errorWithDomain:@"your error domain" code: 666    userInfo:dict];
        abort();
    }
    
    return _persistentStoreCoordinator;
}
#pragma mark - 实例化上下文context
- (NSManagedObjectContext *)manageObjectContext
{
    if (_manageObjectContext != nil) {
        return _manageObjectContext;
    }
    NSPersistentStoreCoordinator *coordinator = [self persistentCoordinator];
    if (_persistentStoreCoordinator != nil) {
        _manageObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSPrivateQueueConcurrencyType];
        _manageObjectContext.persistentStoreCoordinator = coordinator;
    }
    return _manageObjectContext;
}
#pragma mark - 上下文进行保存
- (void)saveContext
{
    NSManagedObjectContext *context = [self manageObjectContext];
    if (context != nil) {
        NSError *saveError = nil;
        if ([context hasChanges] && ![context save:&saveError]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", saveError, [saveError userInfo]);
        abort();
        }
    }
    
}
@end
```
# 总结
前面对CoreData进行了仔细的说明和封装，有什么不对的地方还请大家留言和批评指正。
# 致谢
尽管CoreData出的较早，但是使用起来总感觉没那么顺，看了不少技术大牛的博客，多敲代码和时间长了才慢慢了解和熟悉。这里借鉴了很多技术大牛的经验，感谢！
# 相关资料和博客
- [ios开发：Core Data 周周周少爷](http://www.cnblogs.com/zyi1992/p/4839339.html)
- [iOS CoreData介绍和使用（以及一些注意事项)---Pocket (ps：我已经关注了您，哈哈)](https://www.jianshu.com/p/113047a478c5)
