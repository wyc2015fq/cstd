# Direct3D资源 - 逍遥剑客 - CSDN博客
2007年12月28日 23:06:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2183标签：[direct3d](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)
个人分类：[图形技术](https://blog.csdn.net/xoyojank/article/category/337624)
CSDN,终于能登录了.......
**资源属性**
- Usage. 资源的用途, 如纹理或渲染目标. 
- Format. 数据的格式, 如一个二维表面的像素格式. 
- Pool. 资源所分配的内存空间类型. 
- Type.资源的类型,如顶点缓冲区或一个渲染目标. 
**关于内存池:**
|**D3DPOOL_DEFAULT**|资源保存到显存或AGP内存中,发挥更高的性能.设备丢失后,必须在Reset()之前释放,并在恢复设备时重新创建|
|----|----|
|**D3DPOOL_MANAGED**|自动在系统内存中进行备份,设备丢失与恢复过程中D3D会自动进行资源的恢复注:RenderTarget不支持这种方式|
|**D3DPOOL_SCRATCH**|位于系统的临时内存中,不能直接用于渲染,只能进行加锁,复制等操作|
|**D3DPOOL_SYSTEMMEM**|位于系统内存中,不会丢失|
**资源类型**
D3DRTYPE_SURFACE 
表面. 
D3DRTYPE_VOLUME 
立体资源. 
D3DRTYPE_TEXTURE 
纹理. 
D3DRTYPE_VOLUMETEXTURE 
立体纹理. 
D3DRTYPE_CubeTexture 
立方体纹理. 
D3DRTYPE_VERTEXBUFFER 
顶点缓冲区. 
D3DRTYPE_INDEXBUFFER 
索引缓冲区. 
**加锁属性**
- D3DLOCK_DISCARD 更新整个区域
- D3DLOCK_READONLY 只读属性
- D3DLOCK_NOOVERWRITE 保证不覆盖,立即返回内存指针,提高系统性能
- D3DLOCK_NOSYSLOCK 在加锁过程中可以进行其他操作
- D3DLOCK_NO_DIRTY_UPDATE 不设置Dirty标记,有特殊需要时用
Lock()函数需要一定的时间,D3D会暂停其他的显示操作,直到Lock()结束.设置NOSYSLOCK后可以在加锁的同时执行其他的显示操作,如鼠标的移动
**资源间的关系**
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/D3D_Resource_Relations.JPG)
