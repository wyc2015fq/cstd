# UE4中的DynamicTexture数据更新 - 逍遥剑客 - CSDN博客
2015年10月18日 23:34:37[xoyojank](https://me.csdn.net/xoyojank)阅读数：5194
最近在UE4中实现了程序实时生成的Mesh顶点动画， 使用的顶点数目很多（几十万量级）
一开始是创建Dynamic Vertex Buffer， 然后每帧去更新顶点数据，发现效率比较低
效率的瓶颈在顶点坐标的计算上， 毕竟数量有点多
于是改成了基于Vertex Texture(Material中的World Position Offset)的实现，那VB就不用更新了， 只需要每帧更新Texture
这么做虽然传输的数据量是一致的， 但是可以把顶点坐标的一部分计算转入GPU端， 大大减轻了CPU压力
实际测下来发现GameThread的时间消耗仍然很高， 那进一步的优化就是把这部分操作扔进后台线程
```cpp
void ADynamicTextureTestCharacter::UpdateInGameThread()
{
	FTexture2DMipMap& Mip = DynamicTexture->PlatformData->Mips[0];
	FVector4* Dest = (FVector4*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	WriteTextureData(Dest);
	Mip.BulkData.Unlock();
	DynamicTexture->UpdateResource();
}
```
改完一测， Crash了, 仔细一看， 原来是FTexture2D::UpdateResource()中会重新创建D3D Texture对象，相关函数必须是GameThread中调用才可以
本身这种数据更新的方式就有问题， 能不能直接更新到对应的D3D Texture中呢？搜索UE4的代码， 发现FTwitchLiveStreaming::UpdateWebCamTexture()中有比较高效的实现， 大致思路就是把数据发到RenderThread去直接更新， 调用的是RHIUpdateTexture2D
```cpp
void ADynamicTextureTestCharacter::UpdateInRenderThread()
{
	FVector4* Dest = DataBuffer.GetData();
	WriteTextureData(Dest);
	struct FUpdateTextureContext
	{
		uint8* SourceBuffer;	// Render thread assumes ownership
		uint32 BufferPitch;
		FTexture2DResource* DestTextureResource;
	} UpdateTextureContext =
	{
		(uint8*)Dest,
		sizeof(FVector4) * DynamicTexture->GetSizeX(),
		(FTexture2DResource*)DynamicTexture->Resource
	};
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		UpdateDynamicTexture,
		FUpdateTextureContext, Context, UpdateTextureContext,
		{
		const FUpdateTextureRegion2D UpdateRegion(
		0, 0,		// Dest X, Y
		0, 0,		// Source X, Y
		Context.DestTextureResource->GetSizeX(),	// Width
		Context.DestTextureResource->GetSizeY());	// Height
		RHIUpdateTexture2D(
			Context.DestTextureResource->GetTexture2DRHI(),	// Destination GPU texture
			0,												// Mip map index
			UpdateRegion,									// Update region
			Context.BufferPitch,						// Source buffer pitch
			Context.SourceBuffer);							// Source buffer pointer
	});
}
```
这种做法不再权限于GameThread运行， 所以对性能影响比较小。 实际测试下来调用线程的执行时间比上一种做法要快1ms左右， 这对于FPS影响还是挺大的
参考资料
https://wiki.unrealengine.com/Dynamic_Textures
