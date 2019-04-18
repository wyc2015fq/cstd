# iOS Swift4.0 CoreData（一） - weixin_33985507的博客 - CSDN博客
2017年11月16日 15:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
CoreData是苹果在数据存储的亲生儿子，是在Cocoa平台上管理数据层模型和数据持久化的要数据库框架，近年swift的版本更新上基本都会有CoreData相关的更新，说明苹果还是挺重视CoreData的；CoreData为我们提供了可视化的数据表结构，使得我们更加清晰地看到表与表（实体与实体）之间的关系，CoreData底层是基于SQLite数据库的，使用CoreData管理应用程序中的模型层对象。CoreData提供了与对象生命周期和对象图管理相关联的常见任务的通用和自动化解决方案，包括持久性。
CoreData可以使我们更加容易的使用数据库，因为不需要我们编写任何的SQL语句，CoreData底层为我们封装好了模型层的大部分工作，这将有效的提升我们的工作效率，也使得数据库操作更加地面向对象。
虽然CoreData看起来很好，但是里面相关的概念比较多，想学好CoreData，必须先了解好这些概念以及它的存储过程，先来看一个图：
![2635420-643ce9e61ef86a5a.jpeg](https://upload-images.jianshu.io/upload_images/2635420-643ce9e61ef86a5a.jpeg)
CoreData结构图.jpeg
从上图可以看出，CoreData主要分为两部分，上层是模型层，模型层有NSManagedObjectContext上下文管理着，而底层则是由SQLite实现的持久化部分，通过NSPersistentStore和底层SQL数据库交互，完成存储过程。而这两部分又是由NSPersistentStoreCoordinator持久化协调器关联起来的，上层的存储的数据交由持久化协调器，由协调器指定相关的NSPersistentStore进行相关的数据库存取操作。
使用CoreData要做的第一件事，就是要创建CoreData栈，在iOS 10开始，苹果已经为我们将CoreData栈封装到一个叫NSPersistentContainer的对象里面，我们直接用就好了，但是，我觉得还是要一步一步自己创建CoreData栈，这样做我们才能熟悉CoreData的存储过程。
（1）创建一个新项目，同时勾选 “use Core Data” 选项
![2635420-7f0806dac6529c4f.png](https://upload-images.jianshu.io/upload_images/2635420-7f0806dac6529c4f.png)
Jietu20171116-114024.png
项目创建好，我们可以看到有一个.xcdatamodeld文件，这个代表着数据库文件结构的文件
![2635420-f43b8c20657baa7a.png](https://upload-images.jianshu.io/upload_images/2635420-f43b8c20657baa7a.png)
Jietu20171116-114105.png
（2）然后点击.xcdatamodeld文件，添加实体，修改实体名字（必须大写字母开头），为实体添加属性（属性必须以小写字母开头），右侧面板可以设置属性的默认值，最大最小值等等属性
![2635420-ff6decf1e32a09b2.png](https://upload-images.jianshu.io/upload_images/2635420-ff6decf1e32a09b2.png)
Jietu20171116-115121.png
对于每一个实体，build之后Xcode都会自动帮我们生成相应的实体类代码，如果不想要自动生成，可以选择手动生成，并且可以选择手动生成的语言
![2635420-b8aad090308427bc.png](https://upload-images.jianshu.io/upload_images/2635420-b8aad090308427bc.png)
Jietu20171116-150259.png
![2635420-61bc8988933973a8.png](https://upload-images.jianshu.io/upload_images/2635420-61bc8988933973a8.png)
Jietu20171116-150319.png
手动生成实体类代码的步骤：选择xcdatamodeld文件-->editor-->create NSManagedObject SubClass,然后按提示点击就行了。最终你会在项目文件里面看到新生成的实体类代码文件
![2635420-a8270fdd2e1a811b.png](https://upload-images.jianshu.io/upload_images/2635420-a8270fdd2e1a811b.png)
Jietu20171116-150644.png
![2635420-2310506d5f56dc9f.png](https://upload-images.jianshu.io/upload_images/2635420-2310506d5f56dc9f.png)
Jietu20171116-150717.png
创建数据模型就是这几个步骤了。
（3）接下来是创建CoreData栈了。
在那之前，先来认识几个对象：
NSManagedObject:数据模型（实体）的信息
NSPersistentCoordinator:模型层和持久化层的协调器
NSManagedObjectContext:内存中数据模型的上下文
再来复习一下这张图：
![2635420-7e25221f9f2bdeb2.jpeg](https://upload-images.jianshu.io/upload_images/2635420-7e25221f9f2bdeb2.jpeg)
CoreData结构图.jpeg
以下操作都在AppDelegate.swift文件中
创建NSManagedObjectModel对象(mom)：
```
//managedObjectModel
lazy var managedObjectModel : NSManagedObjectModel {
    guard let modelURL = Bundle.main.url(forResource: "CoreDataDemo", withExtension: "momd") else {
        fatalError("failed to find data model")
    }
    guard let mom = NSManagedObjectModel(contentsOf: modelURL) else {
        fatalError("Failed to create model from file: \(modelURL)")
    }
    return mom
}()
```
获取文件URL要注意的是，文件名与建立的.xcdatamodeld文件名相同，但是扩展名是“momd”，以为系统编译后生成的文件扩展名是.momd。
创建持久化协调器（psc）：
```
lazy var persistanceCoordinator : NSPersistentStoreCoordinator = {    
    let psc = NSPersistentStoreCoordinator(managedObjectModel: managedObjectModel)
    let dirURL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).last
    let fileURL = URL(string: "db.sqlite", relativeTo: dirURL)
    
    do {
        try psc.addPersistentStore(ofType: NSSQLiteStoreType, configurationName: nil, at: fileURL, options: nil)
    } catch {
        fatalError("Error configuring persistent store: \(error)")
    }
    return psc
}()
```
持久化器是根据NSManagedObjectModel创建的，创建时指定数据库创建的路径。
创建模型文件管理上下文（moc）：
```
lazy var managedObjectContext : NSManagedObjectContext = {
    let moc = NSManagedObjectContext(concurrencyType:.mainQueueConcurrencyType)
    moc.persistentStoreCoordinator = persistanceCoordinator
    return moc
}()
```
创建上下文的时候要制定数据库并发操作的类型，这个之后会详细讲，现在先设定为   mainQueueConcurrencyType ，最后，为上下文指定持久化器。
至此，CoreData栈的构建就已经完成，代码以及CoreData的结构图都已经给出，两个对比一下其实很容易理解CoreData各部分的联系。
这是CoreData的第一部分，后续还会有其他部分上传上来。如有错误，欢迎指正，交流。
