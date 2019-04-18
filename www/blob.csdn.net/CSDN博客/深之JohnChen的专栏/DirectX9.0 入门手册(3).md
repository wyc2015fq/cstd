# DirectX9.0 入门手册(3)  - 深之JohnChen的专栏 - CSDN博客

2005年12月19日 14:37:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2463标签：[direct3d																[struct																[float																[null																[存储																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)
个人分类：[游戏空间](https://blog.csdn.net/byxdaz/article/category/160195)

 第一次写这种文章，再加上自己也是菜鸟，虽然有部分内容是参考一些书籍及自己翻译一些英文文章所写出来的，但我相信文章里面肯定也还有很多bug，如果你发现了请告诉我，或者你有什么想法也可以和我交流，我诚心和志同道合的人成为朋友。


·顶点属性与顶点格式





顶点可谓是3D世界中的基本元素。在计算机所能描绘的3D世界中，任何物体都是由多边形构成的，可以是三边形，也可以是四边形等。由于三边形，即三角形所具有的特殊性质决定其在3D世界中得到广泛的使用。构成三角形需要三个点，这些点的性质就是这章所要讲的内容。





也许你已经知道顶点的结构定义，你可能会奇怪为什么D3D会知道我们“随便”定义的那些结构呢？其实那些顶点的定义可不是那么随便的哦。下面列举在Direct3D中，顶点所具有的所有属性。





（1）位置：顶点的位置，可以分别指定x,y,x三个值，也可以使用D3DXVECTOR3结构来定义。





（2）RHW：齐次坐标W的倒数。如果顶点为变换顶点的话，就要有这个值。设置这个值意味着你所定义的顶点将不需要Direct3D的辅助（不能作变换、旋转、放大缩小、光照等），要求你自己对顶点数据进行处理。至于W是什么，W和XYZ一样，只是一个四元组的一部分。RHW的英文是Reciprocal of the Homogenous W，即1/W，它是为了处理矩阵的工作变得容易一些（呼，线性代数的东东快都忘了，要恶补一下才行）。一般设RHW的值为1.0。





（3）混合加权：用于矩阵混合。高级应用，这里不讲了（其实我不会，^_^）





（4）顶点法线：学过高等数学就应该知道法线是什么吧？在这里是指经过顶点且和由顶点引出的边相垂直的线，即和三角形那个面垂直。用三个分量来描述它的方向，这个属性用于光照计算。





（5）顶点大小：设定顶点的大小，这样顶点就可以不用只占一个像素了。





（6）漫反射色：即光线照射到物体上产生反射的着色。理解这个比较麻烦，因为3D光照和真实光照没什么关系，不能像理解真实光照那样去理解3D光照。





（7）镜面反射色：它可以让一个3D物体的表面看起来很光滑。





（8）纹理坐标：如果想要在那些用多边形组成的物体上面贴上纹理，就要使用纹理坐标。由于纹理都是二维的，所以用两个值就可以表示纹理上面某一点的位置。在纹理坐标中，只能在0.0到1.0之间取值。例如(0.0 , 0.0)表示纹理的左上角，（1.0 , 1.0）表示纹理的右下角。





好了，请记住上面属性的顺序。我们定义一个顶点结构的时候，不一定要包括全部的属性，但是一定要按照上面的顺序来定义。例如：





struct MYVERTEX 





{ 





D3DXVECTOR3 position; 





float rhw; 





D3DCOLOR color; 





} 





上面定义了一个有漫反射色的变换顶点。





定义完了顶点的结构后，我们就要告诉D3D我们定义的是什么格式。为了方便，我们通常会用#define来定义一个叫做描述“灵活顶点格式”（FVF：Flexible Vertex Format）的宏。例如：#define MYFVF D3DFVF_XYZ | D3DFVF_NORMAL。根据之前定义的顶点属性结构体，我们要定义相对应的宏。假如顶点结构中有位置属性，那么就要使用D3DFVF_XYZ；如果是变换顶点的话，就要使用D3DFVF_XYZRHW；如果使用了漫反射色属性的话，就要使用D3DFVF_DIFFUSE。这些值是可以组合使用的，像上面那样用“|”符号作为连结符。定义完灵活顶点格式后，使用IDirect3DDevice9::SetVertexShader函数来告诉D3D我们所定义的顶点格式，例如：g_pD3DDevice->SetVertexShader( MYFVF ); 





·顶点缓冲





处理顶点信息的地方有两个，一个是在数组里，另一个是在D3D所定义的顶点缓冲里。换个说法的话就是一个在我们所能直接操作的内存里，另一个在D3D管理的内存里。对于我们这些对操作系统底层了解不多的菜鸟来说，直接操作内存实在是太恐怖了，所以还是交给D3D帮我们处理吧，虽然不知道背后有些什么操作。要想把顶点信息交给D3D处理，我们就要先创建一个顶点缓冲区，可以使用IDirect3DDevice9->CreateVertexBuffer，它的原型是：














**Length****：**缓冲区的长度。通常是顶点数目乘以顶点大小，使用Sizeof( MYVERTEX )就可以知道顶点的大小了。





**Usage****：**高级应用。设为0就可以了。





**FVF****：**就是我们之前定义的灵活顶点格式。





**Pool****：**告诉D3D将顶点缓冲存储在内存中的哪个位置。高级应用，通常可取的三个值是：D3DPOOL_DEFAULT，D3DPOOL_MANAGED，D3DPOOL_SYSTEMMEM。多数情况下使用D3DPOOL_DEFAULT就可以了。





**ppVertexBuffer****：**返回来的指向IDirect3DVertexBuffer9的指针。之后对顶点缓冲进行的操作就是通过这个指针啦。到这里还要再提醒一下，对于这些接口指针，在使用完毕后，一定要使用Release来释放它。





**pSharedHandle****：**设为NULL就行了。





得到一个指向IDirect3DVertexBuffer9的指针后，顶点缓冲也就创建完毕了。现在要做的就是把之前保存在数组中的顶点信息放在顶点缓冲区里面。首先，使用IDirect3DVertexBuffer9::Lock来锁定顶点缓冲区：













**OffsetToLock****：**指定要开始锁定的缓冲区的位置。通常在起始位置0开始锁定。





**SizeToLock****：**指定在锁定的缓冲区的大小。设为0的话就是表示要锁定整个缓冲区。





**ppbData****：**用来保存返回的指向顶点缓冲区的指针。通过这个指针来向顶点缓冲区填充数据。





**Flags****：**高级应用。通常设为0。





填充为顶点缓冲区后，使用IDirect3DDevice9::Unlock来解锁。





最后在渲染的时候使用IDirect3DDevice9::SetStreamSource来告诉D3D要渲染哪个顶点缓冲区里面的顶点。













**StreamNumber****：**设置数据流的数量。顶点缓冲最多可以使用16个数据流。确定所支持的数据流的数量，可以检查D3DCAPS中的MaxStreams成员的值。通常设为0，表示使用单数据流。





**pStreamData****：**要与数据流绑定的数据。在这里我们要把顶点缓冲区与数据流绑定。





**OffsetInBytes****：**设置从哪个位置开始读数据。设为0表示从头读起。





**Stride****：**数据流里面数据单元的大小。在这里是每个顶点的大小。





·索引缓冲





很多时候，相邻的三角形会共用一些顶点，例如组成四方形的两个三角形就共用了一条边，即共用了两个顶点信息。如果不使用索引，我们需要六个顶点的信息来绘制这个四方形，但实际上绘制一个四方形只要四个顶点信息就足够了。如果使用了索引就不一样了，在顶点缓冲区里我们可以只保存四个顶点的信息，然后通过索引来读取顶点信息。要使用索引得先创建一个索引缓冲。也许读到这里你会有个疑问，创建一个索引缓冲不就更浪费内存空间了吗？其实不然，索引缓冲区的元素保存的是数字，一个数字所占用的内存肯定要比一个顶点所占用的小得多啦。当你节省了几千个顶点，你就会发现浪费那么一点点索引缓冲区是很值得的。





创建索引缓冲的函数是：IDirect3DDevice9::CreateIndexBuffer 













**Length****：**索引缓冲区的长度。通常使用索引数目乘以sizeof（WORD）或sizeof(DWORD)来设置，因为索引号的数据类型是字节（WORD）或双字节（DWORD），嗯，一个WORD只有两个字节，DWORD也就只有四个字节，比顶点的大小小多了吧。





**Usage****：**和CreateVertexBuffer中的Usage设置一样。一般设为0。





**Format****：**设置索引格式。不是D3DFMT_INDEX16就是D3DFMT_INDEX32的啦。





**Pool****：**又是和CreateVertexBuffer中的一样。一般设为D3DPOOL_DEFAULT。





**ppIndexBuffer****：**指向IDirect3DIndexBuffer9的指针。操作索引缓冲区就靠它的啦。记得使用完后要Release啊。





和填充顶点缓冲区一样，要填充索引缓冲区，要先使用IDirect3DIndexBuffer9::Lock来锁定缓冲区。













是不是和IDirect3DVertexBuffer9::Lock一样呢？具体说明也可以参照上面的内容。填充完之后使用IDirect3DIndexBuffer9::UnLock来解锁。





最后使用IDirect3DDevice9::SetIndices来告诉设备要使用哪个索引。











**pIndexData****：**设置使用哪个索引缓冲。





**BaseVertexIndex****：**设置以顶点缓冲区中的哪个顶点为索引0。









有关顶点的知识就说到这了。一下章说说点、线、三角形这种D3D所支持的图元（drawing primitives）。




