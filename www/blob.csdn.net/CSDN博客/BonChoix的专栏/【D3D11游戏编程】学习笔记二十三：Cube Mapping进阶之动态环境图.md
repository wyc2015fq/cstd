# 【D3D11游戏编程】学习笔记二十三：Cube Mapping进阶之动态环境图 - BonChoix的专栏 - CSDN博客





2013年02月24日 07:53:45[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：16381标签：[Cube Mapping																[D3D11																[Render to texture																[Render to Texture																[动态环境图](https://so.csdn.net/so/search/s.do?q=动态环境图&t=blog)
个人分类：[计算机图形学																[游戏编程](https://blog.csdn.net/BonChoix/article/category/1249753)](https://blog.csdn.net/BonChoix/article/category/1253344)

所属专栏：[D3D11游戏编程](https://blog.csdn.net/column/details/d3d11-bonchoix.html)





（注：【D3D11游戏编程】学习笔记系列由CSDN作者BonChoix所写，转载请注明出处：[http://blog.csdn.net/BonChoix](http://blog.csdn.net/BonChoix)，谢谢~）



       在前面两篇介绍Cube Mapping的文章中，我们所使用到的Cube Map都是事先制作好的，这样的一个好处就是运行时效率很高，适合于大多数情形。但如果对于即时动态变化的场景来说，依靠静态图来实现反射效果就不再适用了。因为在不同时刻，一个物体周围的场景是不断变化的，想要把这些变化在物表的反射中体现出来，就需要一张动态的环境图。



##        1、Cube Map相关参数

       动态的环境图显然是不可能事先制作出来的，而需要在运行时实时的计算得出。在前面我们也不止一次地提到过环境图的获得方法：即把照相机放置于物体中心，视角调整为90度，向其正X、负X、正Y、负Y、正Z、负Z六个方向上拍摄对应的六张照片，这样得到的六张图即组成了当前时刻该物体周边的环境图。此外，这里还要指出下，仅仅拥有的这六张图是不够，六张图的顺序、相机摆放也很重要，这样才能被正确地映射（至少在使用HLSL中的Sample函数时是这样）。

       在前面我们仅仅是用到了已经制作好的cube map，因此这些因素就不需要再去关注了。但现在我们需要自己来生成环境图，因此必须要搞清楚。在拍摄环境图时，我们遵循的顺序为：正负X、正负Y、正负Z。如下图为一张环境图的展开图及其对应的序号：（这里针对的是左手坐标系）

![](https://img-my.csdn.net/uploads/201302/24/1361654407_5519.gif)

        关于相机摆放，从上图的展开情形应该也能看得出来：默认情形下相机面朝Z+方向。因此对于正负X面的两张图，只需要相应地让相机水平旋转到正X和负X面上即可；对于正负Y面，需要把相机沿X轴旋转使其仰面朝天、低头朝下摆放；正负Z面同正负X面类似，水平旋转使其对准Z+和Z-即可。

       涉及到程序中对相机摆放的设置，我们需要指定的基本参数依然是：位置、观察点、相机对应的up向量。假设一个物体位于（x, y, z）处，现在拍摄它的环境图，相机的设置相关代码如下：

```cpp
XMFLOAT3 ups[6] = 
	{
		XMFLOAT3( 0.f, 1.f, 0.f),
		XMFLOAT3( 0.f, 1.f, 0.f),
		XMFLOAT3( 0.f, 0.f,-1.f),
		XMFLOAT3( 0.f, 0.f, 1.f),
		XMFLOAT3( 0.f, 1.f, 0.f),
		XMFLOAT3( 0.f, 1.f, 0.f)
	};

	XMFLOAT3 targets[6] = 
	{
		XMFLOAT3( x + 1.f, 0.f, 0.f),
		XMFLOAT3( x - 1.f, 0.f, 0.f),
		XMFLOAT3( 0.f, y + 1.f, 0.f),
		XMFLOAT3( 0.f, y - 1.f, 0.f),
		XMFLOAT3( 0.f, 0.f, z + 1.f),
		XMFLOAT3( 0.f, 0.f, z - 1.f)
	};

	for(UINT i=0; i<6; ++i)
	{
		m_dynamicCameras[i].LookAt(XMFLOAT3(x, y, z), targets[i], ups[i]);
		m_dynamicCameras[i].SetLens(XM_PI*0.5f,1.f,1.f,1000.f);
		m_dynamicCameras[i].UpdateView();
	}
```

       这里六个ups向量分别对应拍摄六张图时相机对应的上方向， targets为对应的六个观察点，通过LookAt函数设置相机摆放，SetLens设置相机高、宽比为1且视角为90度。UpdateView函数更新相机相关矩阵。


##        2. Render to Texture技术

       好了，现在相机已经准备就绪，可以进行拍摄了。接下来的问题是，拍到的照片放哪儿去？ 我们拍照的目的是获得环境图，并把该环境图当作纹理资源接下来用于映射。因此我们现在还需要一个用于保存拍照结果的缓冲区。这就涉及到图形技术上一个新的的概念：**Render To Texture**。

       为此，我们要创建一个空的纹理，该纹理包含六张普通二维纹理，分别用于保存六张拍摄到的照片。为了让拍摄的内容保存到该纹理中，我们需要将该纹理当作新的Render Target，分别使用六个相机对应的变换矩阵如同正常情况一样对场景进行六次绘制。每次绘制时，使用纹理中相应的那一张作为当前的Render Target，这样场景绘制完六次后，其所得到的内容将会留在对应的纹理上。

       之前所有的程序中，我们的Render Target一直是后缓冲区，绘制完后通过交换链将内容显示到前端（显示器）。这里我们使用了另一张纹理作为Render Target， 即把当前一帧绘制到纹理中，因此这种技术称为“绘制到纹理中”，即“Render To Target”，就是这么通俗。。。。

       Render To Target现在被广泛应用于各种特效的实现中，这里仅仅是其应用之一，其他方面的应用也特别多，比如用于阴影绘制的Shadow Mapping，其中使用的Shadow Map就是通过Render To Texture技术得到的（不过这里并不是将纹理作为Render Target而绘制到其中的，而是通过渲染场景将深度信息记录到我们额外创建的深度缓冲区中去，这个深度缓冲区就是我们要的Shadow
 Map），有关Shadow Mapping的详细介绍在后面会有，敬请关注~



##        3. 环境图的创建、绘制及使用

       继续回来正题上来，通过Render To Target， 我们如愿获得了“梦寐以求”的环境图~ 到这步为止，相比之前的静态环境图例子，我们仅仅相当于获得了该图片。为了将其用于后面的映射， 如前面一样，我们要使用该纹理对应的纹理视图，即D3D11中的Shader Resource View。

       在前面介绍纹理基础的文章中，提到了从纹理（ID3D11Texture2D）创建相应视图的方法，这里是一样的道理。

       注意到，这里创建纹理资源视图用到的纹理，与之前作为Render Target的纹理，是同一个！在物理内存中只存在一个纹理，但我们将其用于两个地方：作为Render Target，以及作为Shader Resource。换句话说，该纹理可以被绑定到渲染管线的多个阶段，只要在创建纹理时指明其可以被绑定到的阶段（D3D11_BIND_XXX），这样对于每个不同的用途，分别创建相应的视图（View）即可。   这正是D3D11处理纹理的方法，详细情况可以参考[D3D11中纹理的使用](http://blog.csdn.net/bonchoix/article/details/8449513#t11)。

       纹理资源视图也创建好，之后就是使用它进行Cube Mapping了，相关步骤与前面的静态环境图例子完全一样。



       接下来，我们把重点放在环境图纹理的创建、绘制上面来。

###               3.1 环境图及其相关视图的创建

        首先，我们即将创建的纹理需要包含六张普通2维纹理，前面刚开始介绍Cube Map时也提过，d3d11中的ID3D11Texture2D不仅可以用来代表一张2维纹理，也可以用于代表一个纹理数组，以及每张纹理的所有mip链。

       其次，该纹理需要绑定到管线的两个阶段：Render Target和Shader Resource。

       除了这两点核心问题之外，再加上一些比如纹理宽度、高度等信息即可。以下为创建纹理相关代码：

```cpp
D3D11_TEXTURE2D_DESC cubeMapDesc;			//纹理描述
	cubeMapDesc.Width = m_cubeMapWidth;			//宽和高，必须为一样，比如512.
	cubeMapDesc.Height = m_cubeMapHeight;
	cubeMapDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;		//数据格式，跟正常情况一样
	//指定两个绑定阶段：Render Target 和 Shader Resource
	cubeMapDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	cubeMapDesc.ArraySize = 6;					//该纹理数组中包含6张纹理
	cubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeMapDesc.CPUAccessFlags = 0;
	cubeMapDesc.MipLevels = 0;					//0，表示产生所有的mip链
	//Misc flags: 第一个指定该纹理用于cube map，第二个指定让系统自己产生所有的mip链
	cubeMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;	
	cubeMapDesc.SampleDesc.Count = 1;			//这里不使用多重采样抗锯齿
	cubeMapDesc.SampleDesc.Quality = 0;
	
	//使用以上描述创建纹理
	ID3D11Texture2D *cubeMap(NULL);
	m_d3dDevice->CreateTexture2D(&cubeMapDesc,0,&cubeMap);
```



       纹理创建好了，现在需要针对Render Target和Shader Resource分别对其创建相应的视图：

       首先针对六个面分别创建Render Target视图：

```cpp
D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = cubeMapDesc.Format;				//格式一样
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;		//指明这是一个纹理数组
	rtvDesc.Texture2DArray.ArraySize = 1;							//每个视图中针对其中一张纹理
	rtvDesc.Texture2DArray.MipSlice = 0;							//每个视图只使用最高层的mip链
	
	//逐个创建视图
	for(UINT i=0; i<6; ++i)
	{
		//每个视图使用对应的那张纹理
		//这里指定了纹理数组中的起始索引，因为上面ArraySize指定为1，即只使用一张，
		//因此这样就单独锁定一个纹理了
		rtvDesc.Texture2DArray.FirstArraySlice = i;					
		m_d3dDevice->CreateRenderTargetView(cubeMap,&rtvDesc,&m_dynamicRTV[i]);
	}
```



       然后是Shader Resource视图：

```cpp
D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = cubeMapDesc.Format;				//格式一样
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;			//指定这里将它当作cube map
	srvDesc.TextureCube.MipLevels = -1;					//-1 表示使用其所有的mip链（有多少使用多少）
	srvDesc.TextureCube.MostDetailedMip = 0;			//指定最精细的mip层，0表示高层
	//创建视图
	m_d3dDevice->CreateShaderResourceView(cubeMap,&srvDesc,&m_dynamicSRV);
```



       这样，用于绘制环境图的核心问题解决了，现在就等着往该纹理中绘制内容了。不过还有一个小问题尚未解决：绘制场景离不开深度缓冲区，而深度缓冲区与Render Target是一一对应的，因此不能使用后缓冲区对应的那个深度缓冲区了，需要我们自己再创建一个，创建方法是完全一样的（惟一的区别是，这里不需要模板缓冲区，因此相比正常情况下的数据格式：DXGI_FORMAT_D24_UNORM_S8_UINT，这里我们使用DXGI_FORMAT_D32_FLOAT）：

```cpp
D3D11_TEXTURE2D_DESC dsDesc;					//缓冲区描述
	dsDesc.Width = m_cubeMapWidth;					//尺寸与cube map一致
	dsDesc.Height = m_cubeMapHeight;
	dsDesc.Format = DXGI_FORMAT_D32_FLOAT;			//这里我们只需要深度值，不需要模板值
	dsDesc.ArraySize = 1;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.SampleDesc.Count = 1;					//同cube map一致，不使用MSAA
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;
	dsDesc.MipLevels = 1;
	
	ID3D11Texture2D *depthStencilBuffer(NULL);
	m_d3dDevice->CreateTexture2D(&dsDesc,0,&depthStencilBuffer);
	m_d3dDevice->CreateDepthStencilView(depthStencilBuffer,0,&m_dynamicDSV);
```



###               3.2 环境图的绘制 及 使用

       现在是绘制场景的时候了，整个绘制过程分为2个pass：

       第一个pass的目的是绘制物体周边的环境，保存到纹理中。这次绘制不包含该物体本身，除此之外，一切绘制按正常流程进行即可。该阶段包含六次场景绘制，即针对六个面，使用相应的照相机、设置好相应的Render Target逐个绘制：

```cpp
//便于循环，使用一个共享指针，每个面绘制时指向不同的Render Target
	ID3D11RenderTargetView *rtv[1] = {0};
	//便于循环，使用一个共享指针，每个面绘制时指向不同的Camera
	Camera *tmpCamera(NULL);
	//设置好相应的viewport
	m_deviceContext->RSSetViewports(1,&m_dynamicViewport);
	//针对六个面，绘制场景六次
	for(UINT i=0; i<6; ++i)
	{
		rtv[0] = m_dynamicRTV[i];
		tmpCamera = &m_dynamicCameras[i];
		
		//设置相应Render Target
		m_deviceContext->OMSetRenderTargets(1,&rtv[0],m_dynamicDSV);
		
		//跟正常绘制一样，清屏操作
		m_deviceContext->ClearRenderTargetView(rtv[0],reinterpret_cast<const float*>(&Colors::Silver));
		m_deviceContext->ClearDepthStencilView(m_dynamicDSV,D3D11_CLEAR_DEPTH,1.0f,0); 
		
		//场景绘制过程省略
		……
	}
	
	//六次绘制完成后，自动产生mip链
	m_deviceContext->GenerateMips(m_dynamicSRV);
```



       第二个pass中，除了绘制pass 1中的所有内容外，还包括使用刚得到的环境图作为纹理资源，通过cube mapping对该物体进行绘制，从而实现物体表面的反射效果。当然，现在第帧的环境图是即时更新的，从而实现了我们想要的动态反射效果。如下所示：

```cpp
//恢复正常Render Target，即后缓冲区
	m_deviceContext->OMSetRenderTargets(1,&m_renderTargetView,m_depthStencilView);
	//恢复正常viewport
	m_deviceContext->RSSetViewports(1,&m_viewport);
	
	//清屏
	m_deviceContext->ClearRenderTargetView(m_renderTargetView,reinterpret_cast<const float*>(&Colors::Silver));
	m_deviceContext->ClearDepthStencilView(m_depthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.f,0);
	
	//场景渲染过程省略
	……
```



       至此，动态cube mapping效果大功告成！ 



##        4. 小结

       总结一下整个绘制过程：

       1. 设置摄像机参数。针对每个面，分别指定好相应的相机参数，包括：位置、观察点、up向量，得到相应的相机变换矩阵；

       2. 创建一个空的cube map，并分别创建其对应的Render Target视图、Shader Resource视图, 以及对应的深度缓冲区；

       3. 绘制场景：包括两个pass:

              3.1. 使用创建的cube map作为render target，并使用相应的深度缓冲区、视口变换矩阵、相机矩阵，按正常过程绘制整个场景（不包含反射物体本身）；

              3.2 恢复使用后缓冲区作为Render Target，及深度缓冲区、视口变换矩阵、相机矩阵，按同上一样的方法绘制整个场景，包含使用新的环境图绘制反射物体本身。



##        5. 示例程序

       作为本节的示例程序，为了展示动态反射的效果，场景内容在上一篇文章的基础上加入了一个绕圆球转动的箱子，如下是一张运行截图：

![](https://img-my.csdn.net/uploads/201302/24/1361661740_5058.png)

       当然，为了更直观地感受动态的效果，还需要自行运行程序。



       此外，关于代码，还有很大的改进空间，比如场景绘制过程，其实多次绘制场景的过程中绝大多数的代码是完全一样的，为了减少代码冗余，可以把它们写到一个函数中。针对绘制与不绘制反射物体本身的区别，可以通过一个布尔变量来控制。感兴趣的读者可以自己来优化一下。



       最后需要指出的是，动态环境图的实现成本是相当高的。毕竟，相比使用静态图，动态图的获得需要额外的六次整个场景的绘制！ 这样每一帧中需要对场景进行高达七次的绘制！ 把上图中的帧率与上一篇文章的帧率对比一比即明显地感受到区别。因此，除非必要，尽量使用静态环境图来实现反射，仅仅当有必要需要特别突出显示某物体的动态反射时再使用动态环境图。 此外，动态环境图的尺寸的选择也会大大的影响渲染成本，比如512 * 512与1024 * 1024， 每张图就有4倍的区别，这样绘制六张后，将是24倍的区别！ 因此也需要尽可能地减小其尺寸，本节中使用的尺寸为512
 * 512.



       本节完。



       点击[这里](http://download.csdn.net/detail/bonchoix/5069027)下载程序源代码、可执行文件。](https://so.csdn.net/so/search/s.do?q=Render to Texture&t=blog)](https://so.csdn.net/so/search/s.do?q=Render to texture&t=blog)](https://so.csdn.net/so/search/s.do?q=D3D11&t=blog)](https://so.csdn.net/so/search/s.do?q=Cube Mapping&t=blog)




