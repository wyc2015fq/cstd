# 让CoreData更简单些 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月21日 08:51:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1056
## 本文来自简书，原文地址:[http://www.jianshu.com/p/78f6fe1eff07](http://www.jianshu.com/p/78f6fe1eff07)
## 前言
本文并不是CoreData从入门到精通之类的教程, 并不会涉及到过多的原理概念描述, 而是介绍如何让CoreData的使用变得更加简单明了, 方便亲民. 全文约六千字, 预计花费阅读时间15分钟.
## 目录
- 这是什么以及如何使用
- 原理概述
- 实现细节: 单表的增删改查
- 实现细节: 一对一关系
- 实现细节: 一对多关系
- 实现细节: 多对多关系
### 一. 这是什么以及如何使用
大概是去年夏天, 因为要做数据缓存开始使用MagicalRecord. 在和同事们使用了一段时间后, 发现在复杂的业务场景中单纯的MagicalRecord应用起来还是略有些麻烦, 于是就在它的基础上再封装了一层, 最后实现的结果还是比较满意的. 因为比较实用, 而且原理很简单, 索性就开篇博客把这个工具放出来.
这里以一段和同事A(A没有任何CoreData方面的开发经验, 所以比较有代表性)的对话描述一下这个工具如何使用:
我: 东西写完了, A啊, 你过来看看, 我给你说说怎么用. 假设你现在要存一个Snack类, 类定义呢大概像下面这个样子:
```
@interface Snack : NSObject
@property (copy, nonatomic) NSString *name;
@property (copy, nonatomic) NSString *taste;
@property (assign, nonatomic) float size;
@property (assign, nonatomic) float price;
@property (assign, nonatomic) NSInteger snackId;
@end
```
你需要做的就是在.xcdatamodeld里面添加一个Entity, 随便取个名字, 比如CoreSnack吧, CoreSnack里面的字段就是你要存的那些属性名, 注意: 名字和类型尽量一一对应.
现在你有了一个本地Model类Snack和一个NSManagedObject类CoreSnack, 他们两的关系就相当于本地Model和网络数据的Protobuf/JSon, 只不过这次他们的关系是双向的, 我们不仅可以将CoreSnack转换成Snack, 也可以将Snack转换成CoreSnack.
A: 额, JSon/Protobuf/ManagedObject转Model很简单, 直接用之前写的转换工具就行了, Model怎么转ManagedObject? Model又不知道自己对应的ManagedObject类是哪一个? 还有, 难道每次转换都存一个新数据进去? 那不是好多重复数据?
我: 嗯, 你说的很对, 所以你需要在Snack里面声明它对应的ManagedObject是哪个, 还有这个ManagedObject的主键, 默认情况下, 我会用主键去重. 像这样:
```
@implementation Snack
#pragma mark - CoreData
//Model和CoreData对应关系
+ (Class)managedObjectClass {
    return NSClassFromString(@"CoreSnack");
}
//主键 (key是Model属性名, value是CoreData字段名, 一般情况下是一样的, 声明成字典只是以防万一)
+ (NSDictionary *)primaryKeys {
    return @{@"snackId" : @"snackId"};
}
@end
```
A: 奥, 行吧. 那这些东西我都声明好了的话, 我怎么存东西, 要自己调用CoreData的接口吗?
我: 不需要你调用CoreData接口, 你要做的事情很简单: 新建, 赋值, 存储. 像这样:
```
Snack *snack = [Snack new];  //新建
snack.snackId = 123;  //赋值
snack.size = ...;  //赋值
snack.name = ...;  //赋值
//... 各种赋值
[snack save];  //存储
```
A: 看着还蛮简单的, 但是万一我要存的东西比较多, 这样会不会卡UI?
我: 虽然不知道为什么一个Model会存很多东西, 但是我也提供了接口, 像这样:
```
Snack *snack = [Snack new];  //新建
//... 各种赋值
[snack saveWithCompletionHandler:^{
}];  //异步存储
```
A: 那我要存一个Snack数组的话, 怎么搞? forin吗?
我: 不行, 每次存储都是要访问数据库的, 用forin的话会多次访问数据库, 很耗时的! 如果你要存一个Model数组, 用下面这个接口:
```
NSMutableArray *snacks = [NSMutableArray array];
for (int i = 1; i < 9; i++) {
    Snack *snack = [Snack instanceWithId:i];
    [snacks addObject:snack];
}
[Snack saveObjects:snacks];  //异步存储无回调接口(数组存储只提供异步接口)
[Snack saveObjects:snacks completionHandler:^{
}];  //异步存储有回调接口
```
A: 你这个好像只能存普通数据类型, 那如果我的Model有几个属性本身也是Model, 有的属性也有对应的ManagedObject, 有的没有, 有的甚至是数组, 怎么办?
我: 没有关系, 也是一样的用法, 你只管设置, 接口会帮你存好的, 但是如果你的属性里面有映射到ManagedObject的Model数组的话, 你最好用异步存储的接口:
```
Ticket *ticket = [Ticket instanceWithId:0];  
Worker *worker = [Worker instanceWithId:0];
NSMutableArray *snacks = [NSMutableArray array];
for (int i = 10; i < 19; i++) {
    [snacks addObject:[Snack instanceWithId:i]];
}
worker.snacks = snacks;  //CoreSnack
worker.ticket = ticket;  //CoreTicket
[worker save];  //同步存储
[worker saveWithCompletionHandler:nil];//异步存储
```
A: 嗯... 东西存进去了, 那怎么取出来?
我: 因为存东西是分单个存储和数组存储, 所以取东西也给了两组接口, 我一组一组给你演示. 先是单个查询的接口:
```
[Snack findFirstByAttribute:@"snackId" withValue:@0];  //单个同步查询接口1
[Snack findFirstWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 0"]];  //单个同步查询接口2
[Snack findFirstWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 0"] sortedBy:@"price" ascending:YES];  //单个同步查询接口3
//异步查询接口直接在后面加上 completionHandler 参数即可
```
数组查询接口:
```
//    查询接口大体分两种: 条件查询和分页查询(这里列的都是同步查询接口)
//    条件查询
[Snack findAll];
[Snack findAllWithPredicate:predicate];
[Snack findAllSortedBy:someSortItem ascending:isAscend];
[Snack findAllSortedBy:someSortItem ascending:isAscend withPredicate:predicate];
[Snack findByAttribute:someAttribute withValue:aValue];
[Snack findByAttribute:someAttribute withValue:aValue andOrderBy:someSortItem ascending:isAscend];
//    分页+条件查询 page起点为0 row最大值为1000
[Snack findAllWithPage:page row:row];
[Snack findAllWithPredicate:somePredicate page:page row:row];
[Snack findAllSortedBy:someSortItem ascending:isAscend page:page row:row];
[Snack findAllSortedBy:someSortItem ascending:isAscend withPredicate:somePredicate page:page row:row];
[Snack findByAttribute:someAttribute withValue:aValue page:page row:row];
//    同理, 异步查询接口直接在后面加上 completionHandler 参数即可
```
我: 另外, 如果从CoreData取数据的时候, 某个属性也是一个Model数组, 记得在这条数据对应的Model中里面声明属性数组里面是什么Model, 这点和JSon/Protobuf是一样的, 比如上面的Worker有个snacks数组属性, 数组元素也是Model, 你就要在Worker里面这样声明一下:
```
@implementation Worker
+ (Class)managedObjectClass {
    return NSClassFromString(@"CoreWorker");
}
+ (NSDictionary *)containerPropertyKeypathsForCoreData {
    return @{@"snacks" : @"Snack"};
}
@end
```
A: 行啦, 我知道了. 现在有存有取, 那改数据怎么改, 是不是要先查询, 然后改数据, 改完了再存进去?
我: 嗯, 逻辑是这个逻辑, 但是不用你写这些代码, 你只需要提供查询条件就行了, 像这样:
```
//xxxClass - saveSanck
- (void)saveSanck {
    //在某个位置事先有存过一条CoreSnack记录
    Snack *snack = [Snack new];  //新建
    snack.snackId = 123;//  
    snack.price = 1.1;
    snack.name = @"name1";
    //... 各种赋值
    [snack save];  //存储
}
//yyyClass - modifSanck
- (void)modifSanck {
    //当你需要改这条数据的时候
    //    法1
    Snack *snack = [Snack new];  //新建
    snack.name = @"xxx";  //要改的部分直接赋值
    snack.size = 999;   //要改的部分直接赋值
    [snack saveWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 123"]];  
    //内部会以你提供的Predicate进行查询 你设置的值进行更改 最后进行更新存储
    //    法2
    Snack *snack = [Snack new];  //新建
    snack.snackId = 123;  //设置要改的那条记录对应的主键值
    snack.name = @"xxx";  //要改的部分直接赋值
    snack.size = 999;   //要改的部分直接赋值
    [snack save];  
    //如果不写查询条件 默认会以主键为作为查询条件
    //等同于[snack saveWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 123"]];  
    //    法3 和 法2 类似 不过这次会以提供的查询数组生成查询条件
    Snack *snack = [Snack new];  //新建
    snack.snackId = 123;  //设置要改的那条记录对应对应查询值
    snack.otherProperty = yyy;//设置要改的那条记录对应查询值
    snack.name = @"xxx";  //要改的部分直接赋值
    snack.size = 999;   //要改的部分直接赋值
    [snack saveWithEqualProperties:@[@"snackId", @"otherPrimaryKey"]];  
    //如果设置了查询条件数组 会以你提供的查询数组生成查询条件
    //等同于[snack saveWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 123 && otherProperty = yyy"]];  
    //note: 在数据修改时, 直接给某个值设置为nil或者0对应的CoreData在数据库中不会有任何改变 比如snack.snackId = 0和snack.name = nil都是无效的 
    //如果你确实想将某个字段设置为空, 那就设置对应的空值, 比如snack.name = @""和snack.snackId = CDZero(因为数字没有空值, 所以我声明了一个保留字)
    //同理 以上的同步操作接口加上 completionHandler 就是对应的异步操作接口
}
```
A: 有个问题, 这个接口看起来和存储接口好像啊. 如果我给的查询条件有问题, 最后没有查到数据库里面的数据, 或者数据库里面根本就没有这条数据, 会怎么样?
我: 查询查不到的话, 那就直接存一条数据进去咯, 数据存储其实调用的就是数据更新接口, 不然你以为我怎么去重的?
A: 额, 我担心我写错查询条件, 能不能查不到就不存?
我: 可以啊, 加个参数或者把save和update分开就行了, 不过我懒, 没实现, 你需要就自己实现咯!
接下来是数组的批量更新接口:
```
//将你想要更新的批量数据 放到一个数组里面 然后通过通过HHPredicate(注意: 不是NSPredicate)设置查询条件
//HHPredicate定义了查询条件的 ==(equalKeys) 关系和 in(containKeys) 关系 批量更新会根据这些关系去进行数据查询
//HHPredicte的关系键值对和Model的主键键值对一样 key是Model的属性名, value是CoreData的字段名
NSMutableArray *snacks = [NSMutableArray array];
for (int i = 1; i < 9; i++) {
    Snack *snack = [Snack new];  //新建
    snack.snackId = i;
    snack.name = ...;  //要改的部分直接赋值
    snack.size = ...;  //要改的部分直接赋值
    [snacks addObject:snack];
}
//法 1 
[Snack saveObjects:snacks checkByPredicate:[HHPredicate predicateWithEqualKeys:nil containKeys:@{@"snackId" : @"snackId"}]];
//内部会生成一个NSPredicate = [NSPredicate predicateWithFormat:@"snackId in {1, 2, 3, 4...}"]
//这里的例子没有==关系, 如果是复合键做主键的话, 会频繁用到equalKeys, 或者想优化查询速度也可以设置equalKeys
//比如设置了equalKeys{@"xxxProperty" : @"xxx"}就会生成[NSPredicate predicateWithFormat:@"xxxProperty = xxx && snackId in {1, 2, 3, 4...}"]
//法 2 因为一般设置Model的属性名和CoreData的字段名都是一样的 所以直接给个便利的方法出来
[Snack saveObjects:snacks checkByPredicate:[HHPredicate predicateWithEqualProperties:nil containProperties:@[@"snackId"]]];
```
批量更新和单个更新一样, 查询条件查不到的那部分数据就会被认为是普通存储, 会新建这部分的数据并存储到数据库中.
A: 只剩下删除了, 这部分是什么样子的?
我: 删除和更新接口差不多, 不过要简单多了, 像这样:
```
//    法1
Snack *snack = [Snack new];  //新建
[snack deleteWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 123"]];  //内部会以你提供的Predicate进行查询 然后删除
//    法2
Snack *snack = [Snack new];  //新建
snack.snackId = 123;  //设置要删除的那条记录对应的主键值
[snack delete];  //如果不写查询条件 默认会以主键为作为删除条件
//    等同于[snack deleteWithPredicate:[NSPredicate predicateWithFormat:@"snackId = 123"]];
//    法3
Snack *snack = [Snack new];  //新建
snack.snackId = 123;  //设置要删除那条记录的满足条件1
snack.otherPrimaryKey = xxx;//设置要删除那条记录的满足条件2
[snack deleteWithEqualProperties:@[@"snackId", @"otherPrimaryKey"]];
//    等同于[snack deleteWithPredicate:[NSPredicate predicateWithFormat:@"otherPrimaryKey = xxx && snackId = 123"]];
```
批量删除更简单:
```
[Snack deleteAll];  //全部删除
[Snack deleteAllMatchingPredicate:[NSPredicate predicateWithFormat:@"snackId <= 10"]];  //删除满足条件的部分
```
A: 增删改查算是齐了, 但是我听说CoreData是线程不安全的, 那我在使用的时候需要注意什么? 还有多线程的数据同步呢?
我: 多线程和数据同步的问题不需要你关心, 你只管记住上面的这些接口就行了, 比如下面的写法是完全没问题的:
```
- (void)makeSnackOnOtherThread {
    NSMutableArray *snacks = [NSMutableArray array];
    for (int i = 100; i < 109; i++) {
        [snacks addObject:[Snack instanceWithId:i]];
    }
    [Snack saveObjects:snacks]; 
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        //查询操作会等到最近的一次存储/删除/更新操作完成后才执行(即数据同步)
        NSArray *snacks = [Snack findAllSortedBy:@"snackId" ascending:YES withPredicate:[NSPredicate predicateWithFormat:@"snackId >= 100 && snackId < 109"]];
        //子线程查询的数据拿到主线程去用也完全没问题(即安全的跨线程访问)
        dispatch_async(dispatch_get_main_queue(), ^{
            [snacks enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                [obj log];
            }];
        });
    });
}
- (void)makeSnackOnOtherThread2 {
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        //子线程新建
        NSMutableArray *snacks = [NSMutableArray array];
        for (int i = 110; i < 119; i++) {
            [snacks addObject:[Snack instanceWithId:i]];
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            //但是拿到主线程存储
            [Snack saveObjects:snacks];
            dispatch_async(dispatch_get_global_queue(2, 0), ^{
                //然后又到另一个子线程查询
                NSArray *snacks = [Snack findAllSortedBy:@"snackId" ascending:YES withPredicate:[NSPredicate predicateWithFormat:@"snackId >= 110 && snackId < 119"]];
                [snacks enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    [obj log];
                }];
            });
        });
    });
}
```
A: 那敢情好, 所以现在我要操作CoreData数据只需要做三件事:
1.根据业务需求新建本地Model同时在.xcdatamodeld里面新建与Model对应的Entity, 并设置相应字段.
2.在本地Model内部声明和对应的NSManagedObject类的映射关系.
3.调用相应接口进行增删改查.
### 二. 原理概述
在说实现原理之前, 先看看现有的CoreData用起来有哪些缺点, 或者说麻烦的地方.
我们都知道, 在对CoreData做任何操作时, 都是通过一个Context来完成的, Context内部管理着和自己相关的ManagedObject, 对外则提供各种操作这些对象的相应接口, 它的本质其实是处在应用和数据库之间的一层全局缓存(在Context和实际的数据库之间还有一层NSPersistentStoreCoordinator, 但这和本文要讲的东西联系不大, 略去不谈).
缓存带来的效益是可观的, 在缓存命中的情况下, 在内存中操作数据肯定比直接访问数据库效率要高, 这在数据读写频繁或者数据量大时尤为有效. 但事有利弊, 缓存带来了效率的提升同时也引出了一些问题. 比较突出的有两点:老是被人诟病的内存问题以及因缓存数据同步而引发的实际使用时操作繁琐复杂.
第一点几乎是无解的(但iPhone的配置一直在提升, 所以这个问题现在也不算什么问题了), 我们主要聊聊第二点. 共享数据在多线程操作的情况下势必是要做数据同步的(不然我在这个线程用你在那个线程改, 你在这个线程改他在那个线程删, 数据不是全乱套了), Context作为一个全局缓存自然也不例外. 系统对此采取的措施是: 为Context配置相应的并发操作类型(NSManagedObjectContextConcurrencyType). 这个并发操作类型定义了Context的数据操作规范: 从Context读取的数据只能在当前获取数据的线程中访问,
 而更改数据的操作只能在Context对应的操作队列中执行(MainConcurrencyType的操作队列就是UI线程, 而PrivateConcurrencyType的操作队列由系统自建, 对外不可见, 外部只能通过performBlock将操作添加到这个队列执行). 这套操作规范解决了缓存内部的数据同步问题, 但却间接引起了更加麻烦的问题: 缓存间的数据同步.
我们来看看缓存内数据同步是如何引出缓存间数据同步问题的:
1.数据更改操作只能在Context对应的操作队列执行, 如果此时的Context对应的操作队列是主线程, 耗时的数据操作就会卡UI, 这是不能接受的.
2.耗时操作不要在UI线程做, 于是通常我们会至少建立一个子线程的Context来做耗时操作.
3.多个Context其实也就是多个缓存块, 它们之间是各自独立毫无联系的, 这意味着在一个Context进行的任何操作另一个Context是不知情的, 所以你不能指望在子线程存储后再去UI线程读取, 没有用. 即使以后允许这样做, 你也必须等到子线程数据更新完成后才能到主线程进行查询, 否则就是数据错乱. 另外, 因为从Context读取的数据只能在当前获取数据的线程中访问, 所以也不能从子线程读取数据后直接传递到UI线程使用, 没有用...
4.按下葫芦浮起瓢, 为了解决缓存内数据同步, 我们不得不处理缓存间数据同步, 而且, 这个过程系统能提供的帮助十分有限...(缓存间数据同步目前业界有许多方案, 诸如Context操作完成后发通知让其他Context进行数据同步, 两层/三层基于child/parentContext的设计等等, 另外还涉及到数据传递和数据冲突解决, 内容较多且与本文无关, 这里不做细表)
哎, 光是各种Coordinator/Context/Objcet/ConcurrencyType对象之间的关系就够麻烦的了, 现在还要自己搞数据同步, 这让很多刚接触CoreData的同学热情大减, 纷纷表示CoreData太复杂了, 还是用SQLite/Realm吧...
好了, 上面长篇大论一顿分析, 现在终于讲到本文的目的了: 如何实现一套支持线程安全且简单易用的CoreData工具?
回答这个问题我们需要把视线挪到分析缓存间数据同步的第一点, 然后想一想: 如果一开始Context就支持在子线程操作数据同时也能在UI线程访问数据而且还自带数据同步特效, 后面的一系列问题不就都没有了吗?
把这个想法当成一个需求, 我们来做一个简单的可行性分析. 这个需求一共三点:子线程操作数据, UI线程访问数据, 多线程数据同步. 第一点很简单, 直接设置Context并发操作类型为PrivateConcurrencyType就行了, 第三点也很简单, 多线程数据同步就是加锁嘛, 读写频繁的话把锁换成dispatch_barrier_async/sync和dispatch_async/sync的组合就行了.
第二点就麻烦一点了, 因为NSManagedObject是和Context强关联的, 想要脱离Context的线程限制进行数据访问是不太现实的. 对此, 我们需要绕一个小弯, 即在可访问的线程中将NSManagedObject的值映射到一个可以跨线程访问的对象上(也就是我们的Model), 在待使用线程使用这个映射对象而不是NSManagedObject, 借此解决跨线程访问的问题. 最后, 当我们在需要对数据进行任何修改时, 先将映射对象还原相应的NSManagedObject, 再通过Context去到子线程执行对应的操作.
 饶了一个弯, 不过还好, 因为最麻烦的互相转换的工具很久以前就已经实现了, 直接把之前写的[Protobuf解析器](http://www.jianshu.com/p/851b21870d91)简单改改就可以用了.
原理大概就是这样了, 概括起来其实我们只做了两件事情:
1.通过dispatch_barrier_async/sync和dispatch_async/sync的组合做多线程数据同步.
2.通过NSManagedObject和Model之间的互相转换做跨线程访问和CoreData数据操作.
进一步的, 这套路其实算是ORM的变种实现(CoreData本身其实就是ORM的一种实现, 默认映射关系是SQLite--NSManagedObject). 理论上, 我们只要换一个数据转换工具, 重写一下数据操作接口, 那么下层即使换掉CoreData改用SQLite/Realm/xxx也是一样的.
### 三. 实现细节: 单表的增删改查
上面说到的原理很简单, 具体实现起来也很简单, 这里我就简单贴贴代码, 主要说一下细节问题就好.
- 查询
```
//某个分页同步查询接口
+ (NSArray *)findAllWithPage:(NSUInteger)page row:(NSUInteger)row {
    return [self objectsWithManagedObjectsFetchHandler:^NSArray *(id managedObjectClass) {
        return [self managedObjectsWithFetchRequest:[managedObjectClass MR_requestAllInContext:self.saveContext] page:page row:row];
    }];
}
...若干同步查询接口
//某个不分页异步查询接口
+ (void)findAllWithCompletionHandler:(void (^)(NSArray *objects))completionHandler {
    [self converObjectsWithManagedObjectsFetchHandler:^NSArray *(id managedObjectClass) {
        return [managedObjectClass MR_findAllInContext:self.saveContext];
    } completionHandler:completionHandler];
}
...若干异步查询接口
```
查询的实现很简单, 通过Model声明的映射关系拿到NSManagedObject类, 然后执行查询, 将查询结果转换成Model传递出来即可, 因为这部分逻辑都是一样的, 所以就直接写出一系列通用方法, 各个查询接口调用这些通用方法即可. 具体的方法实现如下:
```
//某个同步查询通用方法
+ (NSArray *)objectsWithManagedObjectsFetchHandler:(NSArray *(^)(id managedObjectClass))fetchHandler {
    IfInvalidManagedObjectClassReturn(nil);
    __block NSArray *objects;
    dispatch_sync(self.perfromQueue, ^{
        objects = [self objectsWithManagedObjects:fetchHandler(managedObjectClass)];
    });
    return objects;
}
//某个异步查询通用方法
+ (void)converObjectsWithManagedObjectsFetchHandler:(NSArray *(^)(id managedObjectClass))fetchHandler completionHandler:(void (^)(NSArray *objects))completionHandler {
    IfInvalidManagedObjectClassBreak;
    dispatch_async(self.perfromQueue, ^{
        NSArray *objects = [self objectsWithManagedObjects:fetchHandler(managedObjectClass)];
        dispatch_async(dispatch_get_main_queue(), ^{
            completionHandler ? completionHandler(objects) : nil;
        });
    });
}
```
所有的查询方法最后都会走到一个解析方法去做数据转换, 该方法如下:
```
+ (instancetype)objectWithManagedObject:(NSManagedObject *)managedObject {
    if (managedObject == nil) { return nil; }
    id object = [self new];
    HHClassInfo *classInfo = [NSObject managedObjectClassInfoWithObject:object];
    NSDictionary *containerPropertyKeypaths = [(id)classInfo.cls respondsToSelector:@selector(containerPropertyKeypathsForCoreData)] ? [classInfo.cls containerPropertyKeypathsForCoreData] : nil;
    for (HHPropertyInfo *property in classInfo.properties) {
        if ([(id)managedObject respondsToSelector:property->_getter]) {
            id propertyValue = [managedObject valueForKey:property->_getPath];
            if (propertyValue != nil) {
                switch (property->_type) {
                    case HHPropertyTypeBool:
                    case HHPropertyTypeInt8:
                    case HHPropertyTypeUInt8:
                    case HHPropertyTypeInt16:
                    case HHPropertyTypeUInt16:
                    case HHPropertyTypeInt32:
                    case HHPropertyTypeUInt32: {
                        if ([propertyValue respondsToSelector:@selector(intValue)]) {
                            ((void (*)(id, SEL, int))(void *) objc_msgSend)(object, property->_setter, [propertyValue intValue]);
                        }
                    }   break;
                    //...各种格式的数据赋值         
                    case HHPropertyTypeCustomObject: {
                        propertyValue = [property->_cls objectWithManagedObject:propertyValue];
                        if (propertyValue) {
                            ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, propertyValue);
                        }
                    }   break;
                    case HHPropertyTypeArray: {
                        if ([propertyValue isKindOfClass:[NSString class]]) {
                            if ([propertyValue length] > 0) {
                                ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, [propertyValue componentsSeparatedByString:@","]);
                            }
                        } else {
                            id objectsClass = NSClassFromString(containerPropertyKeypaths[property->_name]);
                            if (!objectsClass) { break; }
                            NSMutableArray *objects = [NSMutableArray array];
                            for (id managedObj in propertyValue) {
                                id value = [objectsClass objectWithManagedObject:managedObj];
                                if (value) { [objects addObject:value]; }
                            }
                            if (objects.count > 0) {
                                ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, objects);
                            }
                        }
                    }   break;
                    //...各种格式的数据赋值  
                }
            }
        }
    }
    return object;
}
```
这块的逻辑和Protobuf解析差不多([Protobuf解析的具体逻辑](http://www.jianshu.com/p/851b21870d91)), 无非就是各种数据格式的赋值, 自定义类和数组属性的特殊处理. 不过和之前的解析不同, 在数组这块多了一个字符串判断,
 然后才是数组属性的解析, 简单解释下:
CoreData默认是不能存数组的, 要存数组需要走transformable然后自己手写序列化, 这个过程其实也不过是一次中间值转化, 存储时将数组转化为Data, 获取时将Data转化回数组. 但我们这套工具的目的是使用简单, 所以不想让使用者关心这些东西, 默认我会在存储时将数组通过逗号分割拼装成字符串放进数据库, 然后在获取时将字符串再解析回数组. 另外, transformable的存储方式是不支持条件查询的, 因为它是存储的是Data, 无从比对, 但分割字符串的方式显然是支持条件查询的, 而且还不用使用者写任何多余代码.
 (当然, transformable还有其他的应用场景, 比如存UIImage之类的对象, 这种情况还是需要自己手写序列化的, 但这种情况不属于数组, 属于FoundationObject)
- 增和改
```
//通过默认的主键去重进行存储或者更新
- (void)save {
    IfUndefinedPrimaryKeyBreak;
    [self saveWithPredicate:[[HHPredicate predicateWithEqualKeys:[objectClass primaryKeys]] makePredicateWithObjcet:self]];
}
- (void)saveWithEqualProperties:(NSArray *)properties {
    [self saveWithPredicate:[HHPredicate makePredicateWithObjcet:self equalProperties:properties]];
}
//通过给定的条件去重进行存储或者更新
- (void)saveWithPredicate:(NSPredicate *)predicate {
    IfInvalidManagedObjectClassBreak;
    dispatch_barrier_sync(NSObject.perfromQueue, ^{
        //通过给定的条件进行查询 有就更新 没有就新建
        NSManagedObject *managedObject = [NSObject managedObjectWithClass:managedObjectClass predicate:predicate];
        //通过Model配置NSManagedObject然后存储
        [managedObject saveWithObject:self];
    });
}
//通过给定的条件进行查询 有就更新 没有就新建
+ (NSManagedObject *)managedObjectWithClass:(id)managedObjectClass predicate:(NSPredicate *)predicate {
    if (predicate == nil || managedObjectClass == nil || ![managedObjectClass isSubclassOfClass:[NSManagedObject class]]) { return nil; }
    NSManagedObject *managedObject = [managedObjectClass MR_findFirstWithPredicate:predicate inContext:NSObject.saveContext];
    if (managedObject != nil) {
        return managedObject;
    } else {
        return [managedObjectClass MR_createEntityInContext:NSObject.saveContext];
    }
}
```
为了方便, 添加数据和修改数据走的是一个接口, 同时添加和更新还分为单个操作和批量操作, 这里先介绍单个操作. 代码中的注释应该很明显了, 我们直接跳到配置并存储数据的部分:
```
- (void)saveWithObject:(id)object {
    if (!object || [object isKindOfClass:[self class]]) { return ; }
    [self configWithObject:object];
    [NSObject.saveContext MR_saveToPersistentStoreAndWait];
}
- (void)configWithObject:(id)object {
    if (!object || [object isKindOfClass:[self class]]) { return ; }
    HHClassInfo *classInfo = [NSObject managedObjectClassInfoWithObject:object];
    NSDictionary *containerPropertyKeypaths = [(id)classInfo.cls respondsToSelector:@selector(containerPropertyKeypathsForCoreData)] ? [classInfo.cls containerPropertyKeypathsForCoreData] : nil;
    for (HHPropertyInfo *property in classInfo.properties) {
        if ([self respondsToSelector:property->_getter]) {
            id propertyValue = [object valueForKey:property->_name];
            if (propertyValue != nil) {
                if (property->_type >= HHPropertyTypeBool && property->_type < HHPropertyTypeArray) {
                    float number = [propertyValue floatValue];
                    if (number == 0) { continue; }
                    if (number == CDZero) { propertyValue = @0; }
                } else if (property->_type == HHPropertyTypeCustomObject) {
                    if (![(id)property->_cls respondsToSelector:@selector(primaryKeys)]) { continue; }
                    NSPredicate *predicate = [[HHPredicate predicateWithEqualKeys:[property->_cls primaryKeys]] makePredicateWithObjcet:propertyValue];
                    NSManagedObject *managedObject = [NSObject managedObjectWithClass:[property->_cls matchedManagedObjectClass] predicate:predicate];
                    [managedObject configWithObject:propertyValue];
                    propertyValue = managedObject;
                } else if (property->_type == HHPropertyTypeArray) {
                    if ([propertyValue count] == 0) {
                        [self setValue:nil forKeyPath:property->_getPath];
                        continue;
                    }
                    //数组且数组内只是普通数据类型
                    id element = [propertyValue firstObject];
                    if ([element isKindOfClass:[NSString class]] ||
                        [element isKindOfClass:[NSNumber class]]) {
                        propertyValue = [propertyValue componentsJoinedByString:@","];
                    } else {//数组且数组内也是NSManagedObject
                        //...这部分和批量操作差不多 下文介绍
                    }
                }
                if (propertyValue) { [self setValue:propertyValue forKeyPath:property->_getPath]; }
            }
        }
    }
}
```
通过Model配置NSManagedObject也就是Model解析的逆操作, 而NSManagedObject本身只支持KVC的方式进行赋值, 所以比起Model解析部分的各种MsgSend和数据格式判断要简单的多, 这里我只介绍一点: 数据初始化和数据合并.
当我们在对CoreData做修改操作时其实就是一次数据合并操作, 我们将此时需要修改的值覆盖数据库原有的值, 但是不需要修改的部分是不变的, 这可以看做是两个Model各自将一部分数据进行组装生成第三个合并Model. 在这套工具中, 第一个Model就是数据库中原有的值, 第二个Model就是我们想要修改数据的的值, 合并的逻辑是将第二个Model中不为空的部分(也就是我们设置修改的部分)赋值给第一个Model, 然后将更新后的Model存回数据库. 但这有一个问题, 那就是如果我就是想将数据库中的值置空怎么办?
目前我的处理是, 如果你确实想将某个值置空, 那就传对应的空值而不是nil, 因为直接设置nil是不做覆盖的. 比如, 你想将某个字符串属性置空, 那就传@"", 数组就传@[], 如果你想将某个数字置0, 那就传CDZero(这是我声明的一个保留字, 因为数字在KVC获取时是不可能为空的, 拿到的都是0), 这些空值在被覆盖后重新获取时会被判定为nil, 以达到置空的目的.
当我们向CoreData增加数据时其实做的是数据初始化, 但因为数据初始化是数据合并的子集, 所以数据初始化就直接用数据合并的逻辑了.
单个数据添加和修改说完了, 接下来看看批量添加和修改:
```
//批量添加/更新便利方法1
+ (void)saveObjects:(NSArray *)objects {
    [self saveObjects:objects completionHandler:nil];
}
//批量添加/更新便利方法2
+ (void)saveObjects:(NSArray *)objects completionHandler:(void (^)())completionHandler {
    HHPredicate *predicate;
    if (objects.count > 0) {
        id objectClass = [objects.firstObject class];
        if ([objectClass respondsToSelector:@selector(primaryKeys)]) {
            predicate = [HHPredicate predicateWithContainKeys:[objectClass primaryKeys]];
        }
    }
    [self saveObjects:objects checkByPredicate:predicate completionHandler:completionHandler];
}
//批量添加/更新便利方法3
+ (void)saveObjects:(NSArray *)objects checkByPredicate:(HHPredicate *)predicate {
    [self saveObjects:objects checkByPredicate:predicate completionHandler:nil];
}
//实际执行批量添加/更新的方法
+ (void)saveObjects:(NSArray *)objects checkByPredicate:(HHPredicate *)predicate completionHandler:(void (^)())completionHandler {
    id managedObjectClass = [self matchedManagedObjectClass];
    if (objects.count == 0 || managedObjectClass == nil || predicate == nil) {
        DispatchCompletionHandlerOnMainQueue;
    } else {
        dispatch_barrier_async(NSObject.perfromQueue, ^{
            //1. 根据查询条件的查询数据中已有的部分
            NSArray *managedObjects = [managedObjectClass MR_findAllWithPredicate:[predicate makePredicateWithObjcets:objects] inContext:self.saveContext];
            //2. 以 HHPredicate 中的唯一标识符规则从NSManagedObject处生成一个的标识符X数组
            NSMutableArray *managedObjectIdentifierArray = [NSMutableArray array];
            for (NSManagedObject *managedObject in managedObjects) {
                id managedObjectIdentifier = [predicate identifierWithManagedObjcet:managedObject];
                if (managedObjectIdentifier) {
                    [managedObjectIdentifierArray addObject:managedObjectIdentifier];
                }
            }
            //3. 遍历需要更新/存储的Model数组
            for (id object in objects) {
                //3.1 以 HHPredicate 中的唯一标识符规则从Model处也生成一个标识符Y
                NSManagedObject *managedObject;
                id objectIdentifier = [predicate identifierWithObjcet:object];
                //3.2 如果标识符Y和步骤2中生成的标识符X匹配, 说明它是已经存在于数据库中, 即修改操作
                if ([managedObjectIdentifierArray containsObject:objectIdentifier]) {
                    managedObject = [managedObjects objectAtIndex:[managedObjectIdentifierArray indexOfObject:objectIdentifier]];
                } else {//3.3 标识符Y和标识符X不匹配, 说明是添加操作
                    managedObject = [managedObjectClass MR_createEntityInContext:self.saveContext];
                }
                //3.4 根据Model配置managedObject(新建的或者数据库本来就有的)
                [managedObject configWithObject:object];
            }
            //4. 将添加/修改提交到数据库
            [self.saveContext MR_saveToPersistentStoreAndWait];
            DispatchCompletionHandlerOnMainQueue;
        });
    }
}
```
```
- (NSString *)identifierWithObjcet:(id)object {
    return [self identifierWithKeys:self.containKeys.allKeys objcet:object];
}
- (NSString *)identifierWithManagedObjcet:(id)managedObject {
    return [self identifierWithKeys:self.containKeys.allValues objcet:managedObject];
}
- (NSString *)identifierWithKeys:(NSArray *)keys objcet:(id)object {
    if (keys.count > 0) {
        if (keys.count > 1) {
            keys = [keys sortedArrayUsingComparator:^NSComparisonResult(NSString * _Nonnull obj1, NSString *  _Nonnull obj2) {
                return [obj1 compare:obj2];
            }];
        }
        NSMutableString *identifier = [NSMutableString string];
        [keys enumerateObjectsUsingBlock:^(id  _Nonnull key, NSUInteger idx, BOOL * _Nonnull stop) {
            [identifier appendFormat:@"%@:", [object valueForKey:key]];
        }];
        return [identifier copy];
    }
    return nil;
}
```
可以看见, 批量操作比单个操作要复杂一些, 因为批量操作中常常同时存在添加和更新. 举个例子: 数据库中第一天存了一些用户好友, 第二天可能这些好友有些改了昵称/头像/个性签名然后用户自己在网页端又新添加了一些好友, 此时我们直接调用接口拉取下来的第一页几十条数据中就肯定有部分是修改, 有部分是添加的, 如果让使用者自己查询然后区分哪部分是添加, 哪部分是修改, 无疑增加了使用复杂度, 所以这些东西我也选择由工具自己来做而不是抛给使用者. 这也是为什么从一开始, 数据更新和数据存储就是走的一个接口的原因, 因为批量操作遇到这种情况简直不要太多.
我们通过HHPredicate来生成去重的查询条件, 同时还生成Model和NSManagedObject的唯一标识符, 那么这个唯一标识符是怎么生成的呢? 其实很简单, HHPredicate定义了equal(==)和contain(in)关系, equal中的字段定义了整个待操作的数组Model值相同的字段, 通常这部分用作缩小查询范围加快查询速度, 是可有可无的. contain定义了数组中每个Model都不相同的字段, 是必须要有的, 通常这个字段就是主键. 举个例子: 比如我们要存一个User数组,
 这个数组中每个User的主键UserId肯定都是不同的, 当然, 其他的字段诸如年龄, 名字可能也不同, 但是我们只需要一个字段就足够标识了, 所以此时contain定义就填UserId. 那equal定义呢? 你可以不填, 但是如果这些User确实有一个字段全都一样, 比如全都是xxx公司的员工, 那你可以在equal定义填上xxx公司, 这样的查询会比较快.
equal定义在复合键做主键时特别有用, 因为很多时候批量操作只有一部分是完全不同的, 另一部分都是一样的. 比如三年二班的学生, 他们的学号通常是不同的1到100, 但是班级都是三年二班. 单凭学号不足以唯一标识一个学生, 毕竟其他班也有1到100的学号, 但是加上班级后就可以了. 在实际使用中, 通常一个手机可以有若干个账号, 每个账号都有若干好友/作品..., 单凭好友/作品Id是不足以做唯一标识符的, 还必须加上当前登录的用户Id, 显然, 这个用户的所有缓存数据操作的登录用户Id都是一样的, 这时候equal定义就显得比较有用了.
- 删除
```
- (void)delete {
    IfUndefinedPrimaryKeyBreak;
    [self deleteWithPredicate:[[HHPredicate predicateWithEqualKeys:[objectClass primaryKeys]] makePredicateWithObjcet:self]];
}
- (void)deleteWithEqualProperties:(NSArray *)properties {
    [self deleteWithPredicate:[HHPredicate makePredicateWithObjcet:self equalProperties:properties]];
}
- (void)deleteWithPredicate:(NSPredicate *)predicate {
    IfInvalidManagedObjectClassBreak;
    dispatch_barrier_sync(NSObject.perfromQueue, ^{
        NSManagedObject *managedObject;
        if (predicate) {
            managedObject = [managedObjectClass MR_findFirstWithPredicate:predicate inContext:[self class].saveContext];
        }
        if (managedObject) {
            [managedObject MR_deleteEntityInContext:[self class].saveContext];
            [[self class].saveContext MR_saveToPersistentStoreAndWait];
        }
    });
}
```
```
+ (void)deleteAllMatchingPredicate:(NSPredicate *)predicate  {
    [self deleteAllMatchingPredicate:predicate completionHandler:nil];
}
+ (void)deleteAllMatchingPredicate:(NSPredicate *)predicate completionHandler:(void (^)())completionHandler {
    IfInvalidManagedObjectClassBreak;
    dispatch_barrier_async(NSObject.perfromQueue, ^{
        [managedObjectClass MR_deleteAllMatchingPredicate:predicate inContext:self.saveContext];
        [self.saveContext MR_saveToPersistentStoreAndWait];
        DispatchCompletionHandlerOnMainQueue;
    });
}
```
数据删除最简单, 默认以Model声明的主键生成查询条件进行查询, 然后将查询到的NSManagedObject删除即可, 同步删除走dispatch_barrier_sync, 异步删除走dispatch_barrier_async.
单表的所有操作大概就是这样了, 整个工具的核心代码其实只有三百行代码不到的样子, 非常简单(目前我司的表结构都是单表, 多个表之间的联系通过外键维持. 单表这部分已经在上线项目中稳定运行了快一年了).
### 四. 实现细节: 一对一关系
本来文章到这就算结尾了, 因为原理和实现已经说得很清楚了, 有什么需求都可以自己加, 但想想关系这一块自己不用别人可能要用, 就顺便实现一下. 直接上代码吧:
```
@implementation Team
//@property (strong, nonatomic) Coach *coach;
//标识一下一对一关系, key是关系对象在自己这边的属性名, value是自己在关系对象的属性名
+ (NSDictionary *)oneToOneRelationship {
    return @{@"coach" : @"team"};
}
@end
```
```
@implementation Coach
//@property (strong, nonatomic) Team *team;
//标识一下一对一关系, key是关系对象在自己这边的属性名, value是自己在关系对象的属性名
+ (NSDictionary *)oneToOneRelationship {
    return @{@"team" : @"coach"};
}
@end
```
```
+ (instancetype)objectWithManagedObject:(NSManagedObject *)managedObject ignoreProperties:(NSSet *)ignoreProperties {
    if (managedObject == nil) { return nil; }
    //这里以Team - Coach举例 此时获取的是CoreTeam 它的一对一关系是CoreCoach
    //此时的managedObject是CoreTeam 转换出的object是team 
    //managedObject.coach是CoreCoach 也就是下面的PropertyValue
    id object = [self new];
    HHClassInfo *classInfo = [NSObject managedObjectClassInfoWithObject:object];
    NSDictionary *oneToOneRelationship = [(id)classInfo.cls respondsToSelector:@selector(oneToOneRelationship)] ? [classInfo.cls oneToOneRelationship] : nil;
    for (HHPropertyInfo *property in classInfo.properties) {
        if ([(id)managedObject respondsToSelector:property->_getter]) {
            id propertyValue = [managedObject valueForKey:property->_getPath];
            if (propertyValue != nil) {
                switch (property->_type) {
                   //...其他属性 略
                    case HHPropertyTypeCustomObject: {
                        if ([ignoreProperties containsObject:property->_name]) { break; }
                        //1. 从 一对一关系表 中取出对应的属性名
                        NSString *oneToOneTargetName = oneToOneRelationship[property->_name];
                        NSMutableSet *ignorePropertyNames = [NSMutableSet setWithSet:ignoreProperties];
                        !oneToOneTargetName ?: [ignorePropertyNames addObject:oneToOneTargetName];
                        //2. 将managedObject.coach(CoreCoach)转换成对应的Coach 因为此时的CoreCoach.team就是object本身 所以这里要在Coach转换时忽略team属性 不然就是死循环
                        propertyValue = [property->_cls objectWithManagedObject:propertyValue ignoreProperties:ignorePropertyNames];
                        if (oneToOneTargetName) {
                            id propertyValueClass = [propertyValue class];
                            if ([propertyValueClass respondsToSelector:@selector(oneToOneRelationship)] &&
                                [[propertyValueClass oneToOneRelationship].allKeys containsObject:oneToOneTargetName]) {
                                //3.将Model对应的 一对一关系属性 设置成自己
                                //即: object.coach.team = object(object == team)
                                [propertyValue setValue:object forKey:oneToOneTargetName];
                            }
                        }
                        //4. 将自己对应的 一对一关系属性 设置为Model
                        //即:object.coach = propertyValue(propertyValue == coach)
                        ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, propertyValue);
                    }   break;
                    //...其他属性 略
                }
            }
        }
    }
    return object;
}
```
```
Team *team = [Team instanceWithId:1];
Coach *coach = [Coach instanceWithId:1];
team.coach = coach;
//coach.team = team; 不需要这句 CoreData会根据声明自动建立一对一关系
[team save];
```
首先我们在Team和Coach双方都声明一下一对一关系, 这个关系声明其实就是双方对应关系的PropertyName, 觉得绕的话, 直接打开CoreData图形化界面, 照着上面的剪头填写就行了, 站在CoreTeam的立场看, 它的关系属性名是coach, 而自己在对方的属性是team, 所以在Team.m里的关系描述就是@{@"coach" : @"team"}, 同理, Coach.m里面就是@{@"team" : @"coach"}.
![](http://upload-images.jianshu.io/upload_images/2595746-bc2a5c66e1711b8a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2017-04-09 下午4.21.51.png
因为一对一关系其实就是两个CustomObject属性互相引用, 所以们只需要在HHPropertyTypeCustomObject处加上这个引用关系即可. 不过和普通的单向CustomObject属性不同, 互相引用的属性在解析时需要注意一下循环解析的情况. 仍以Team-Coach关系举例: 解析CoreTeam时会顺带解析CoreCoach, 而解析CoreCoach时又会去解析CoreTeam... 这就循环解析了, 所以我们需要在第二层解析处破除一下这个循环.
另外, 因为一对一关系的两个对象实际上也就是循环引用, 会有内存泄漏, 直接使用NSManagedObject时我们不需要关心这个泄漏, 因为它本身Context中不释放的缓存, 一出生就自带内存泄漏了. 但是我们转换出来的Model不能这样搞, 在用完以后需要进行破环清理, 像这样:
```
[team clearRelationship];\\单个数据的关系清理
[teams clearRelationship];\\数组数据的关系清理 你不需要自己forin
```
```
- (void)clearRelationship {
    if ([self isKindOfClass:[NSDictionary class]]) {
        [[(NSDictionary *)self allValues] clearRelationship];
    } else if ([self isKindOfClass:[NSSet class]]) {
        [[(NSSet *)self allObjects] clearRelationship];
    } else if ([self isKindOfClass:[NSArray class]]) {
        for (id object in (NSArray *)self) { [object clearRelationship]; }
    } else {
        NSDictionary *relationship = [self relationshipForObject:self];
        [relationship enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
            id relateObject = [self valueForKey:key];
            NSDictionary *objcetRelationship = [self relationshipForObject:relateObject];
            [objcetRelationship enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
                [[relateObject valueForKey:key] setValue:nil forKey:obj];
            }];
        }];
    }
}
- (NSDictionary *)relationshipForObject:(id)object {
    id cls = [object class];
    NSDictionary *oneToOneRelationship = [cls respondsToSelector:@selector(oneToOneRelationship)] ? [cls oneToOneRelationship] : nil;
    NSDictionary *oneToManyRelationship = [cls respondsToSelector:@selector(oneToManyRelationship)] ? [cls oneToManyRelationship] : nil;
    if (oneToOneRelationship || oneToManyRelationship) {
        NSMutableDictionary *relationship = [NSMutableDictionary dictionary];
        [relationship setValuesForKeysWithDictionary:oneToOneRelationship];
        [relationship setValuesForKeysWithDictionary:oneToManyRelationship];
        return relationship;
    }
    return nil;
}
```
clearRelationship的实现很简单, 直接根据一对一关系表将循环引用的部分置空就行了, 需要注意的是: 这个置空关系是从被引用的一方清空的, 而不是直接清空当前对象. 什么意思呢, 比如team生成时引用了coach, 直接设置team.coach.team = nil只能破除team和coach之间的循环引用, 如果coach本身还有其他的一对一属性, 那么被释放的只有team, coach和它自己的循环引用属性依然不会释放, 所以, 我们要从coach端挨个释放.
### 五. 实现细节: 一对多关系
一对多和一对一使用方法差不多, 这里以Team-Players举例, 代码如下:
```
@implementation Team
//设置数组元素为Model的属性对应的Model类
+ (NSDictionary *)containerPropertyKeypathsForCoreData {
    return @{@"players" : @"Player"};
}
//设置一对多关系
+ (NSDictionary *)oneToManyRelationship {
    return @{@"players" : @"team"};
}
@end
```
```
@implementation Player
//设置一对一关系
+ (NSDictionary *)oneToOneRelationship {
    return @{@"team" : @"players"};
}
@end
```
```
//一对多关系解析
+ (instancetype)objectWithManagedObject:(NSManagedObject *)managedObject ignoreProperties:(NSSet *)ignoreProperties cacheTable:(NSMutableDictionary *)cacheTable {
    if (managedObject == nil) { return nil; }
    //此时的managedObject是CoreTeam 转换出的object是team
    //managedObject.players是CorePlayer数组 也就是下面的PropertyValue
    id object = [self new];
    HHClassInfo *classInfo = [NSObject managedObjectClassInfoWithObject:object];
    NSDictionary *containerPropertyKeypaths = [(id)classInfo.cls respondsToSelector:@selector(containerPropertyKeypathsForCoreData)] ? [classInfo.cls containerPropertyKeypathsForCoreData] : nil;
    NSDictionary *oneToOneRelationship = [(id)classInfo.cls respondsToSelector:@selector(oneToOneRelationship)] ? [classInfo.cls oneToOneRelationship] : nil;
    NSDictionary *oneToManyRelationship = [(id)classInfo.cls respondsToSelector:@selector(oneToManyRelationship)] ? [classInfo.cls oneToManyRelationship] : nil;
    for (HHPropertyInfo *property in classInfo.properties) {
        if ([(id)managedObject respondsToSelector:property->_getter]) {
            id propertyValue = [managedObject valueForKey:property->_getPath];
            if (propertyValue != nil) {
                switch (property->_type) {
                    //...其他属性 略
                    //一对多关系解析
                    case HHPropertyTypeArray: {
                        //1. 从容器属性中 取出的容器元素Model对应的类名
                        //也就是team.players<Player *>
                        id objectsClass = NSClassFromString(containerPropertyKeypaths[property->_name]);
                        if (!objectsClass || [ignoreProperties containsObject:property->_name]) { break; }
                        //2. 从 一对多关系表 中取出对应的属性名
                        NSMutableSet *ignorePropertyNames = [NSMutableSet setWithSet:ignoreProperties];
                        NSString *oneToManyTargetName = oneToManyRelationship[property->_name];
                        !oneToManyTargetName ?: [ignorePropertyNames addObject:oneToManyTargetName];
                        //3. forin解析CorePlayer数组为Player数组
                        NSMutableArray *objects = [NSMutableArray array];
                        for (id managedObj in propertyValue) {
                            id value = [objectsClass objectWithManagedObject:managedObj ignoreProperties:ignorePropertyNames cacheTable:cacheTable];
                            //4. 每个Player都有一个队伍 即player.team = object(object == team)
                            [value setValue:object forKey:oneToManyTargetName];
                            if (value) { [objects addObject:value]; }
                        }
                        //5.每个team都有多个的队员 即team.player = objects(objects是Player数组)
                        ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, objects);
                        }
                    }   break;
                    //...其他属性 略
                }
            }
        }
    }
    return object;
}
```
```
NSMutableArray *players = [NSMutableArray array];
for (int i = 1; i < 4; i++) {
    [players addObject:[Player instanceWithId:i]];
}
Team *team = [Team instanceWithId:1];
team.players = players;//只设置任意一边的关系即可
[team save];
[team clearRelationship];//不用的时候记得清理
```
和一对一关系一样, 我们需要在各自的.m声明相应的关系, 站在team的角度看, 它和Player的关系是一对多的(一个队伍有多个队员), 所以在Team.m一对多(oneToManyRelationship)填上@{@"players" : @"team"}, 但站在player的角度看, 它和Team的关系是一对一(但一个队员只属于一只队伍)的, 所以在Player.m一对一(oneToOneRelationship)填上@{@"team"
 : @"players"}.
![](http://upload-images.jianshu.io/upload_images/2595746-f0f30bd05d8a66bf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2017-04-09 下午5.26.58.png
一对多的解析在Team这一方很简单, 只是简单的把CorePlayer数组转换成Player数组即可, 但是对Player这一方却需要加上一些小小的改动, 因为Player对Team是一对一的, 所以我们从数据库取出无论多少个CorePlayer这些CorePlayer对应的CoreTeam都应该是同一个, 也就是说我们不需要针对每个被解析的Player都解析一次Team, 只需要在第一次解析后保存一下Team, 之后的解析直接使用即可, 类似于TableViewCell的重用, 大概是这样:
```
+ (instancetype)objectWithManagedObject:(NSManagedObject *)managedObject ignoreProperties:(NSSet *)ignoreProperties cacheTable:(NSMutableDictionary *)cacheTable {
    if (managedObject == nil) { return nil; }
    //Player-Team Player对Team是一对一, 但是Team对Player是一对多
    //所以在设置关系是不能直接设置team.players = object 而是 team.players = @[object,...]
    id object = [self new];
    HHClassInfo *classInfo = [NSObject managedObjectClassInfoWithObject:object];
    NSDictionary *oneToOneRelationship = [(id)classInfo.cls respondsToSelector:@selector(oneToOneRelationship)] ? [classInfo.cls oneToOneRelationship] : nil;
    for (HHPropertyInfo *property in classInfo.properties) {
        if ([(id)managedObject respondsToSelector:property->_getter]) {
            id propertyValue = [managedObject valueForKey:property->_getPath];
            if (propertyValue != nil) {
                switch (property->_type) {
                        //...其他属性 略
                    case HHPropertyTypeCustomObject: {
                        if ([ignoreProperties containsObject:property->_name]) { break; }
                        //1.从 一对一关系表 中取出对应的关系表
                        NSString *oneToOneTargetName = oneToOneRelationship[property->_name];
                        //2.以NSManagedObject的地址判断重用表中是否有可重用数据
                        NSString *cachedObjectKey = [NSString stringWithFormat:@"%p", propertyValue];
                        if ([cacheTable.allKeys containsObject:cachedObjectKey]) {
                            propertyValue = cacheTable[cachedObjectKey];
                        } else {
                            //3.没有可重用数据 进入解析流程 并将解析结果放入重用表
                            NSMutableSet *ignorePropertyNames = [NSMutableSet setWithSet:ignoreProperties];
                            !oneToOneTargetName ?: [ignorePropertyNames addObject:oneToOneTargetName];
                            propertyValue = [property->_cls objectWithManagedObject:propertyValue ignoreProperties:ignorePropertyNames cacheTable:cacheTable];
                            !propertyValue ?: [cacheTable setObject:propertyValue forKey:cachedObjectKey];
                        }
                        if (oneToOneTargetName) {
                            //4.如果关系的另一方也是一对一关系直接设置即可
                            id propertyValueClass = [propertyValue class];
                            if ([propertyValueClass respondsToSelector:@selector(oneToOneRelationship)] &&
                                [[propertyValueClass oneToOneRelationship].allKeys containsObject:oneToOneTargetName]) {
                                [propertyValue setValue:object forKey:oneToOneTargetName];
                            } else if ([propertyValueClass respondsToSelector:@selector(oneToManyRelationship)] && [[propertyValueClass oneToManyRelationship].allKeys containsObject:oneToOneTargetName]) {
                                //5.如果关系的另一方是一对多关系需要设置自己到它的数组中
                                NSMutableArray *objects = [NSMutableArray arrayWithArray:[propertyValue valueForKey:oneToOneTargetName]];
                                [objects addObject:object];
                                [propertyValue setValue:objects forKey:oneToOneTargetName];
                            }
                        }
                        ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, propertyValue);
                    }   break;
                        //...其他属性 略
                }
            }
        }
    }
    return object;
}
```
### 六. 实现细节: 多对多关系
多对多关系其实就是两个一对多的组合, 直接在HHPropertyTypeArray的代码基础上稍作修改即可:
```
case HHPropertyTypeArray: {
    id objectsClass = NSClassFromString(containerPropertyKeypaths[property->_name]);
    if (!objectsClass || [ignoreProperties containsObject:property->_name]) { break; }
    NSMutableSet *ignorePropertyNames = [NSMutableSet setWithSet:ignoreProperties];
    NSString *oneToManyTargetName = oneToManyRelationship[property->_name];
    !oneToManyTargetName ?: [ignorePropertyNames addObject:oneToManyTargetName];
    NSMutableArray *objects = [NSMutableArray array];
    for (id managedObj in propertyValue) {
        NSString *cachedObjectKey = [NSString stringWithFormat:@"%p", managedObj];
        id objValue = cacheTable[cachedObjectKey];;
        if (!objValue) {
            objValue = [objectsClass objectWithManagedObject:managedObj ignoreProperties:ignorePropertyNames cacheTable:cacheTable];
            !objValue ?: [cacheTable setObject:objValue forKey:cachedObjectKey];
        }
        if (objValue) {
            [objects addObject:objValue];
            if (oneToManyTargetName) {
                id objValueClass = [objValue class];
                if ([objValueClass respondsToSelector:@selector(oneToOneRelationship)] &&
                    [[objValueClass oneToOneRelationship].allKeys containsObject:oneToManyTargetName]) {
                    [objValue setValue:object forKey:oneToManyTargetName];
                } else if ([objValueClass respondsToSelector:@selector(oneToManyRelationship)] && [[objValueClass oneToManyRelationship].allKeys containsObject:oneToManyTargetName]) {
                    NSMutableArray *objValueObjects = [NSMutableArray arrayWithArray:[objValue valueForKey:oneToManyTargetName]];
                    [objValueObjects addObject:object];
                    [objValue setValue:objValueObjects forKey:oneToManyTargetName];
                }
            }
        }
    }
    ((void (*)(id, SEL, id))(void *) objc_msgSend)(object, property->_setter, objects);
}   break;
```
### 七.待优化
个人比较懒散, 目前我只实现了一些自己用得上的基本功能, 还有很多可以优化的点, 比如:
1.目前对所有的数据操作都是共用的一个队列, 其实可以建立一个队列池针对性的进行数据同步.
2.HHPredicate目前只定义了==和in关系, 其实完全可以加上大于, 小于, like%...等等的操作, 到时候写成Masonry那样的链式调用或者NSMutableAttributedString那样的无限AddAttribute也行.
3.CoreData默认是懒加载的, 对于那些拥有一对多关系的类, 我们不需要在一开始就将所有的属性数组都从加载出来, 选择性的加载即可.
...
## 写在最后
感觉写了好多字啊, 其实核心代码只有300行左右...
最后说一点吧, 因为这套工具是 对象映射+CoreData操作, 可能有些朋友会担心有效率问题, 其实不用担心, 对象映射的效率在之前的文章我有提过, 很快! 最耗时的其实是CoreData本身的存储操作, 但这部分显然是无法优化的(可能以后会变好). 所以, 如果你接受不了原生的存储速度的话, 你应该放弃CoreData, 拥抱SQLite/Realm...
[本文附带的demo地址](https://github.com/HeiHuaBaiHua/TMagicalRecord)
