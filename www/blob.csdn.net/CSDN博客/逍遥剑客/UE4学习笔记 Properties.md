# UE4学习笔记: Properties - 逍遥剑客 - CSDN博客
2015年02月17日 03:49:17[xoyojank](https://me.csdn.net/xoyojank)阅读数：5284
跟Class一样, 类成员变量也可以像C#那样加一堆的描述
```
UPROPERTY([specifier, specifier, ...], [meta(key=value, key=value, ...)])
Type VariableName;
```
主要有4种基础类型:
- 整数
- 浮点数
- 布尔值
- - 除bool外, 可以使用bitfield, 如**uint32 bIsHungry : 1;**
- 字符串
- - FString: 动态字符串
- FName: 常量字符串(快速拷贝和比较)
- FText: 用于本地化
属性的一些说明符:
- Property Window相关
- - AdvancedDisplay
- Category
- EditAnywhere
- EditDefaultsOnly: 只在在archetype中编辑
- EditFixedSize: 用于动态数组
- EditInline: 属性是对象引用时可以直接在当前面板编辑
- EditInstanceOnly: 与EditDefaultsOnly相反
- NoClear
- NonTransactional: 改变这个值不会产生undo/redo记录
- SimpleDisplay
- VisibleAnywhere
- VisibleDefaultsOnly
- VisbileInstanceOnly
- Blueprint相关
- - BlueprintAssignable
- BlueprintCallable
- BlueprintReadOnly
- BlueprintReadWrite
- 网络相关
- - Replicated: 通过网络复制
- ReplicatedUsing=FunctionName
- RepRetry: struct属性专用, 发送失败后重试
- 功能相关
- - Config/GlobalConfig: 值保存在ini文件里, 也意味着运行时是只读的
- Interp: 可以在Matinee中随时间变化
- Localized: 一般用于本地化字符串, 只读
- SaveGame
- AssetRegistrySearchable
- 其它
- - Const
- DuplicateTransient: 对象拷贝时还原默认值
- Export: 对象属性拷贝时进行深度拷贝
- Instanced: Editinline+Export
- Native: ???
- NoExport
- Ref: 函数参数用
- SerializeText
- Transient
