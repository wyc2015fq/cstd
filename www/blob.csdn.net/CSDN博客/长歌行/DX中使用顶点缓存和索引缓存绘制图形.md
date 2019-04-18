# DX中使用顶点缓存和索引缓存绘制图形 - 长歌行 - CSDN博客





2012年12月30日 15:56:20[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1356








最早学习3D的时候，大概都是将数据存在内存中，然后调用相应的函数绘制。

例如：（OpenGL的例子）



```cpp
glClear(GL_COLOR_BUFFER_BIT) ;
 glColor3f(1.0, 1.0, 1.0) ; 
 glBegin(GL_LINES) ;
 	glVertex3f(1.0, 1.0, 0.0) ;
 	glVertex3f(2.0, 2.0, 0.0) ;
 glEnd() ;
 glFlush();
```


但是对于某些固定的物体的绘制，如果将这些物体的信息放在显存中，再进行绘制，会获得更快的速度。



3D空间中绘制一个物体，要知道的信息有，物体的顶点信息和顶点的顺序信息（索引信息）。将这些信息存在显存中即可完成绘制的准备工作。




下面就是DX中定义的数据结构：



```cpp
IDirect3DVertexBuffer9* VB = 0;
 IDirect3DIndexBuffer9*  IB = 0;
```


如果要绘制一个金字塔的四个面（底面不绘制），那么可以按照以下的流程：






一，定义顶点的结构

如果不需要颜色可以定义为



```cpp
struct vertex
 {
 	float m_pos[3];
 };
```


需要颜色可以定义为





```cpp
struct Vertex_VC
 {
 	float m_Position[3];
 	// R,G,B,A in OpenGL
 	// B,G,R,A in Direct3D 9
 	// Direct3D 10可以是两种其中之1, 看设置而定.
 	unsigned char m_RGBA[4];
 };
```


需要光照什么的也可以添加相关的信息。



总之顶点的结构是灵活的。选择很多



```cpp
#define D3DFVF_RESERVED0        0x001
 #define D3DFVF_POSITION_MASK    0x400E
 #define D3DFVF_XYZ              0x002
 #define D3DFVF_XYZRHW           0x004
 #define D3DFVF_XYZB1            0x006
 #define D3DFVF_XYZB2            0x008
 #define D3DFVF_XYZB3            0x00a
 #define D3DFVF_XYZB4            0x00c
 #define D3DFVF_XYZB5            0x00e
 #define D3DFVF_XYZW             0x4002
 
 #define D3DFVF_NORMAL           0x010
 #define D3DFVF_PSIZE            0x020
 #define D3DFVF_DIFFUSE          0x040
 #define D3DFVF_SPECULAR         0x080
 
 #define D3DFVF_TEXCOUNT_MASK    0xf00
 #define D3DFVF_TEXCOUNT_SHIFT   8
 #define D3DFVF_TEX0             0x000
 #define D3DFVF_TEX1             0x100
 #define D3DFVF_TEX2             0x200
 #define D3DFVF_TEX3             0x300
 #define D3DFVF_TEX4             0x400
 #define D3DFVF_TEX5             0x500
 #define D3DFVF_TEX6             0x600
 #define D3DFVF_TEX7             0x700
 #define D3DFVF_TEX8             0x800
 
 #define D3DFVF_LASTBETA_UBYTE4   0x1000
 #define D3DFVF_LASTBETA_D3DCOLOR 0x8000
 
 #define D3DFVF_RESERVED2         0x6000  // 2 reserved bits
```


例如：





```cpp
device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
```
是设置顶点格式为位置和颜色






二、创建索引

一般创建的索引为静态缓存，放置在显卡中。

D3DUSAGE_DYNAMIC为动态缓存，放置在AGP存储区中




创建定点缓存



```cpp
HRESULT hr = device->CreateVertexBuffer(
 			5 * sizeof(vertex),
 			0,
 			D3DFVF_XYZ,
 			D3DPOOL_DEFAULT,
 			&g_vb,
 			0);
```


使用Lock来给缓存加数据，这里使用的顶点结构只有xyz，反正顶点在内存中就是3个float排列表示顶点位置



```cpp
vertex	*vs;
 g_vb->Lock(0, 0, (void**)&vs, 0);
 
 //top
 vs[0].m_pos[0] = 0.0;
 vs[0].m_pos[1] = 1.0;
 vs[0].m_pos[2] = 0.0;
 
 vs[1].m_pos[0] = -0.5;
 vs[1].m_pos[1] = 0.0;
 vs[1].m_pos[2] = 0.5;
 
 vs[2].m_pos[0] = 0.5;
 vs[2].m_pos[1] = 0.0;
 vs[2].m_pos[2] = 0.5;
 
 vs[3].m_pos[0] = 0.5;
 vs[3].m_pos[1] = 0.0;
 vs[3].m_pos[2] = -0.5;
 
 vs[4].m_pos[0] = -0.5;
 vs[4].m_pos[1] = 0.0;
 vs[4].m_pos[2] = -0.5;
 
 g_vb->Unlock();
```


创建索引缓存



```cpp
hr = device->CreateIndexBuffer(
 	12 * sizeof(WORD),
 	0,
 	D3DFMT_INDEX16,
 	D3DPOOL_DEFAULT,
 	&g_ib,
 	0);
```


加入数值



```cpp
unsigned short *index = NULL;
 g_ib->Lock(0, 0, (void**)&index, 0);
 index[0] = 0;
 index[1] = 1;
 index[2] = 2;
 index[3] = 0;
 index[4] = 2;
 index[5] = 3;
 index[6] = 0;
 index[7] = 3;
 index[8] = 4;
 index[9] = 0;
 index[10] = 4;
 index[11] = 1;
 g_ib->Unlock();
```

三、绘制
绘制之前，设置好投影，目标位置等

然后指定数据输入源，顶点格式，索引缓存



```cpp
device->SetStreamSource(0, g_vb, 0, sizeof(vertex));
 device->SetFVF(D3DFVF_XYZ);
 device->SetIndices(g_ib);
```


开始绘制：



```cpp
device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,5,0,4);
```




绘制部分代码：



```cpp
Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
 Device->BeginScene();
 
 Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
 Device->SetIndices(IB);
 Device->SetFVF(D3DFVF_XYZ);
 
 // Draw cube.
 Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 4);
 
 Device->EndScene();
 Device->Present(0, 0, 0, 0);
```












