# UE4学习笔记: Functions - 逍遥剑客 - CSDN博客
2015年02月15日 02:47:11[xoyojank](https://me.csdn.net/xoyojank)阅读数：7625
**函数声明**
UE4中有两种函数, 一种是传统的C++函数, 一种是UFunction. UFunction通过加入一些描述符来支持一些特殊功能
```
UFUNCTION([specifier, specifier, ...], [meta(key=value, key=value, ...)])
ReturnType FunctionName([Parameter, Parameter, ...])
```
主要有几种应用:
- 在Blueprint中调用
- 做为delegate, 如与按键消息进行绑定
- 网络回调和数据同步
- 做为命令行的指令执行, 同UE3的exec关键字
**函数说明符**
- Blueprint相关
- - BlueprintAuthorithOnly: 没有网格授权的话不会在blueprint中执行
- BlueprintCallable: 可以在blueprint中执行
- BlueprintCosmetic: 表现函数, 不会在服务器执行
- BlueprintImplementableEvent: 可以在blueprint中重写
- BlueprintNativeEvent: 设计为被blueprint重写, 但是仍然包含native的实现
- BlueprintPure: 不会影响自身对象
- Network相关
- - Client: 只在客户端执行
- Server: 只在服务端执行
- NetMulticast: 会在服务器和所有客户端执行
- Reliable: 函数执行通过网络进行复制, 并且是可靠的
- Unreliable: 函数在复制执行时可能失败
- Editor相关
- - Category
- 其它
- - CustomThunk: 提供自定义的execFoo, 而不是生成的
- Exec: 可以在命令行中被执行
**元数据说明符**
- BlueprintInternalUseOnly
- BlueprintProtected: 只在blueprint中调用
- DeprecatedFunction
- DeprecationMessage
- UnsafeDuringActorConstruction
**函数参数说明符**
- Out: 传引用
- Optional: 可选, 取默认值
**委托(Delegate)**
`DECLARE_DELEGATE_OneParam( FStringDelegate, FString );`
```
FSharedRef< FLogWriter > LogWriter( **new** FLogWriter() );
WriteToLogDelegate.BindSP( LogWriter, &FLogWriter::WriteToLog );
```
`WriteToLogDelegate.Execute( TEXT( "Delegates are spiffy!" ) );`
建议Delegate传递时使用引用, 避免传值引起内存分配
multi-cast与event的区别只有声明event的类本身可以调用event的Broadcast, IsBound, Clear函数
dynamic delegate是用函数名进行绑定, 所以可以进行序列化, 但速度相对较慢
**定时器(Timer)**
`GetWorldTimerManager().SetTimer(**this**, &AMatineeActor::CheckPriorityRefresh, 1.0f, **true**);`
`GetWorldTimerManager().ClearTimer(**this**, &AMatineeActor::CheckPriorityRefresh);`
`GetWorldTimerManager().IsTimerActive(**this**, &APlayerController::DelayedPrepareMapChange)`
Timer不是线程安全的, 只能在游戏线程使用


