# UE4学习笔记: Gameplay Classes - 逍遥剑客 - CSDN博客
2015年02月14日 04:10:36[xoyojank](https://me.csdn.net/xoyojank)阅读数：4851
增加类
编辑器中有一个类向导, 可以生成类所需要的文件和基础宏
![](https://img-blog.csdn.net/20150214040523100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20150214041002671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
头文件
由于UE4对C++做了扩展, 实现了如序列化反射等特性, 跟Qt的做法类似: 在代码中加入一些宏关键字, 在编译期间生成相应的C++代码. 因此, 对于每个类来说, 都有会生成一个ClassName.generated.h的文件
类声明
```
UCLASS([specifier, specifier, ...], [meta(key=value, key=value, ...)])
**class** ClassName : ParentName
{
    GENERATED_UCLASS_BODY()
}
```
UCLASS可以指定一说明符, 用于描述类的行为, 类似C#
- 功能相关
- - Abstract: 抽象基类, 无法实例化
- Config: 指定配置文件, 跟UE3一样
- Const
- ConvertsionRoot: 子类只能cast到这一级
- CustomConstructor: 不生成默认的构造函数
- DefaultToInstanced
- DependsOn
- Deprecated: 不会被序列化或保存
- MinimalAPI/RequiredAPI: 只暴露指定函数给其它模块, 减少编译时间
- NonTransient
- PerObjectConfig
- Within: 只能用在指定类里
- Property Window相关
- - AutoExpandCategories/AutoCollapseCategories
- AdvancedClassDisplay
- CollapseCategories/DontCollapseCategories
- EditInlineNew: 可以在属性窗口中创建
- HideCategories/HideDropdown
- HideFunctions/ShowFunctions
- Blueprint相关
- - Blueprintable/NotBlueprintable
- BlueprintType
- Editor相关
- - ClassGroup: ActorBrowser里的分组
- NotPlaceable
- Placeable
类实现
需要在cpp中加入IMPLEMENT_CLASS宏
构造函数
```
AMyClass::AMyClass(**const** FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}
```
ObjectInitializer用于从archetype或CDO中初始化属性. 
```
ATimelineTestActor::ATimelineTestActor(**const** **class** FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Structure to hold one-time initialization
    **struct** FConstructorStatics
    {
        ConstructorHelpers::FObjectFinder<UStaticMesh> Object0;
        FConstructorStatics()
            : Object0(TEXT("StaticMesh'/Game/UT3/Pickups/Pickups/Health_Large/Mesh/S_Pickups_Base_Health_Large.S_Pickups_Base_Health_Large'"))
        {
        }
    };
    **static** FConstructorStatics ConstructorStatics;
    // Property initialization
    StaticMesh = ConstructorStatics.Object0.Object;
}
```
ConstructorStatics用于初始化复杂类型, 避免每次构造产生额外性能开销. 常用于Asset引用, Class引用, Name类型等
