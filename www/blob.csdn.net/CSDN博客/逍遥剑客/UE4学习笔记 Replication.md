# UE4学习笔记: Replication - 逍遥剑客 - CSDN博客
2015年02月19日 05:00:22[xoyojank](https://me.csdn.net/xoyojank)阅读数：5775
**Actors**
客户端复制actor被spawn出来时, 只有Location和Rotation在PreBeginPlay()和PostBeginPlay()时是合法的
被复制的Actor只有在服务器关闭他们的复制通道时才被销毁
Actor的属性复制是可靠的. 只会从服务器往客户端同步
FReplicationFlags
|**属性**|**描述**|
|----|----|
|bNetOwner|在目标机器上被PlayerController控制|
|bNetInitial|初始网络更新|
|bNetSimulated|Actor是 RemoteRole 模拟的.|
|bRepPhysics|Actor的ReplicatedMovement.bRepPhysics 为true|
**Components(组件)**
多数componet是不复制的, 因为多数逻辑是在actor中完成的, 只需要同步compoent相关的调用即可, 除非component本身需要直接进行属性和事件的复制.
组件复制分两种:
- 静态组件(actor创建时创建的)会在spawn actor时自动创建
- 动态组件(服务器运行时创建的)会像actor复制一样创建和删除
AActorComponent::SetIsReplicated(true)
**Variables(变量)**
服务器的变量改动时会发送到客户端进行更新, 客户端的自行更改会被覆盖.
UPROPERTY(Replicated)
TArray<UAttributeSet*>SpawnedAttributes;
UPROPERTY(transient, ReplicatedUsing=OnRep_IsActive)
uint32 bIsActive:1;
**函数调用**
其实就是RPC. 
服务器调用客户端执行
```
UFUNCTION( Client );
**void** ClientRPCFunction();
```
客户端调用服务器执行
```
UFUNCTION( Server );
**void** ServerRPCFunction();
```
服务器调用, 服务器和客户端执行
```
UFUNCTION( NetMulticast );
**void** MulticastRPCFunction();
```
一些注意事项:
- 必须从actor中调用
- actor必须是复制的
- client必须拥有相应的actor
- 多播的函数如果是客户端调用的, 不会被同步到服务器
默认RPC是不可靠的, 除非加个标记:
```
UFUNCTION( Client, Reliable );
**void** ClientRPCFunction();
```
RPC函数可以加入校验函数:
```
UFUNCTION( Server, WithValidation );
**void** SomeRPCFunction( int32 AddHealth );
```
如果校验不没通过, 服务器会踢掉这个客户端
```
**bool** SomeRPCFunction _Validate( int32 AddHealth )
{
    If ( AddHealth > MAX_ADD_HEALTH )
    {
        **return** **false**;                       // This will disconnect the caller
    }
**return** **true**;                              // This will allow the RPC to be called
}
**void** SomeRPCFunction _Implementation( int32 AddHealth )
{
    Health += AddHealth;
}
```
**其它**
并不是所有的actor都会被同步的, 而与你”相关”的, 比如不在你视野范围内的就不需要同步
