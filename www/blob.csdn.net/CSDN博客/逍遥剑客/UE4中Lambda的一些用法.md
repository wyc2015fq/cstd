# UE4中Lambda的一些用法 - 逍遥剑客 - CSDN博客
2016年10月19日 15:01:32[xoyojank](https://me.csdn.net/xoyojank)阅读数：4734标签：[UE4																[lambda](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)](https://so.csdn.net/so/search/s.do?q=UE4&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
# 跟Delegate一起用
```
FTimerDelegate TimerCallback;
TimerCallback.BindLambda([]
{
    // callback;
});
FTimerHandle Handle;
GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 5.0f, false);
```
# UI回调事件
```
SNew(SButton).OnClicked_Lambda([&]()
 {
     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Button Clicked!"));
     return FReply::Handled();
 })
```
# 跨线程执行
```
FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
        {
            // game thread code
        }
        , TStatId(), nullptr, ENamedThreads::GameThread);
```
# 批量执行
```
void USkeletalMeshComponent::SetAllBodiesBelowSimulatePhysics( const FName& InBoneName, bool bNewSimulate, bool bIncludeSelf )
{
    int32 NumBodiesFound = ForEachBodyBelow(InBoneName, bIncludeSelf, /*bSkipCustomPhysicsType=*/ false, [bNewSimulate](FBodyInstance* BI)
    {
        BI->SetInstanceSimulatePhysics(bNewSimulate);
    });
...
}
```
