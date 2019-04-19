# Direct3D学习（六）：动画基础（2）X文件的使用 - 逍遥剑客 - CSDN博客
2007年05月29日 03:44:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3740
X文件的使用（完整） [转]
这两天精神不太好，没写，先转一篇同样内容的来凑数吧![](http://bbs.skonline.net/images/smilies/df.gif)
[[Copy to clipboard]](http://bbs.skonline.net/blog.php?tid=167263&starttime=0&endtime=0#%23%23)
CODE:
    有很多朋友也许想更加了解X文件，正好，本文将全面的介绍X文件的使用。我想这是一篇纯技术性的文档，我就不加以诗篇歌颂润色了。相信读我的文章，就像啃牙签；）好了，我们板起面孔。首先给你一个完整的印象----传说中的X文件：
xof 0302txt 0032 
template Header {
<3D82AB43-62DA-11cf-AB39-0020AF71E433>
DWORD major;
DWORD minor;
DWORD flags;
}
template Frame {
<3D82AB46-62DA-11cf-AB39-0020AF71E433>
[FrameTransformMatrix]
[Mesh]
}
Header {
1;
0;
1;
}
Frame Scene_Root {
FrameTransformMatrix {
1.000000, 0.000000, 0.000000, 0.000000,
0.000000, 1.000000, 0.000000, 0.000000,
0.000000, 0.000000, 1.000000, 0.000000,
0.000000, 0.000000, 0.000000, 1.000000;;
                     }
Frame Pyramid_Frame {
FrameTransformMatrix {
1.000000, 0.000000, 0.000000, 0.000000,
0.000000, 1.000000, 0.000000, 0.000000,
0.000000, 0.000000, 1.000000, 0.000000,
0.000000, 0.000000, 0.000000, 1.000000;;
}
Mesh PyramidMesh {
5;
0.00000;10.00000;0.00000;,
-10.00000;0.00000;10.00000;,
10.00000;0.00000;10.00000;,
-10.00000;0.00000;-10.00000;,
10.00000;0.00000;-10.00000;;
6;
3;0,1,2;,
3;0,2,3;,
3;0,3,4;,
3;0,4,1;,
3;2,1,4;,
3;2,4,3;;
MeshMaterialList {
1;
6;
0,0,0,0,0,0;;
Material Material0 {
1.000000; 1.000000; 1.000000; 1.000000;;
0.000000;  
0.050000; 0.050000; 0.050000;;
0.000000; 0.000000; 0.000000;;
                   }
                 }
                 }
                    }
}
xof 0302txt 0032
xof表示这是一个真正的X文件。0302txt表示通知程序使用Directx的X文件，版本为3.2的模版，其中txt表示此文件为文本文件，可读，并非是一个2进制文件。0032表示一个浮点数的位数为32，如果想要用64位的浮点数，可以写成0064。
    下面将分别介绍各个主题。
声明一个模版：//////////////假设声明 template ContactEntry 
首先需要用guidgen.exe产生一个GUID。产生的GUID如下：
// {4C9D055B-C64D-4bfe-A7D9-981F507E45FF}
DEFINE_GUID(<<name>>,
0x4c9d055b, 0xc64d, 0x4bfe, 0xa7, 0xd9, 0x98, 0x1f, 0x50, 0x7e, 0x45, 0xff);
之后需要在程序代码中加入：
#include "initguid.h"
// At beginning of source code file - add DEFINE_GUIDs
DEFINE_GUID(ContactEntry, 0x4c9d055b, 0xc64d, 0x4bfe, 0xa7, 0xd9, 0x98, 0x1f, 0x50, 0x7e, 0x45, 0xff);
还要在X文件中加入：
template ContactEntry {
<4C9D055B-C64D-4bfe-A7D9-981F507E45FF>
声明模版用到的数据类型：
关键字       描述
WORD         16-bit value (short)
DWORD        32-bit value (32-bit int or long)
FLOAT        IEEE float value (float)
DOUBLE       64-bit floating-point value (double)
CHAR         8-bit signed value (signed char)
UCHAR        8-bit unsigned value (unsigned char)
BYTE         8-bit unsigned value (unsigned char)
STRING       A NULL-terminated string (char[]))
array        Signifies an array of following data type to follow ([])
举例：
DWORD value;   
array STRING Text[20];//定义一个名为Text的数组，类型为STRING，大小为20。
DWORD ArraySize; array STRING Names[ArraySize]; //可以将大小设置为变量。
声明一个ContactEntry模版：
template ContactEntry {
<4C9D055B-C64D-4bfe-A7D9-981F507E45FF>
STRING Name; // The contact's name
STRING PhoneNumber; // The contact's phone number
DWORD Age; // The contact's age
}
实例化一个模版对象：
ContactEntry JimsEntry {
"Jim Adams";
"(800) 555-1212";
30;
}
{JimsEntry} 可以用这样的形式引用一个数据对象。例如，在一个animation sequence template中引用
一个Frame data object做为其内嵌数据对象。也可以利用引用表示一个数据对象的副本，没有必要重复
书写这个数据对象。
内嵌数据对象和模版约束：//////////////template ClosedTemplate {
<4C9D055B-C64D-4bfe-A7D9-981F507E45FF>
DWORD ClosedData;
}
template OpenTemplate {
<4C9D055B-C64D-4bff-A7D9-981F507E45FF>
DWORD OpenData;
[...]
}
template RestrictedTemplate {
<4C9D055B-C64D-4c00-A7D9-981F507E45FF>
DWORD RestrictedData;
[ClosedTemplate]
[OpenTemplate]
}
ClosedTemplate是标准的模版声明。
OpenTemplate中包含一个[...]，表示这是一个开放模版。开放模版允许在[]中内嵌任何数据对象。例如
，你可以实例化OpenTemplate，在里面定义一个OpenData变量和内嵌一个ClosedTemplate的实例。
RestrictedTemplate为约束模版。约束模版实例化时只允许包含它列出的数据对象，如，不能在
RestrictedTemplate包含[ClosedTemplate]，[OpenTemplate]以外的数据对象。
用DirectX .X Standard Templates工作：/////////////////X文件广泛用于包含一个mesh信息。一个Standard Templates包含了过多的信息。
Table 3.2: DirectX .X Standard Templates
Template Name               Description
Animation:                  Defines animation data for a single frame.
AnimationKey:               Defines a single key frame for the parent animation template.
AnimationOptions:           Contains animation playback information.
AnimationSet:               Contains a collection of animation templates.
Boolean:                    Holds a Boolean value.
Boolean2d:                  Holds two Boolean values.
ColorRGB:                   Contains red, green, and blue color values.
ColorRGBA:                  Contains red, green, blue, and alpha color values.
Coords2d:                   Defines two coordinate values.
FloatKeys:                  Contains an array of floating-point values.
FrameTransformMatrix:       Holds the transformation matrix for a parent Frame template.
Frame:                      A frame-of-reference template that defines a hierarchy.
Header:                     The .X file header that contains version numbers.
IndexedColor:               Contains an indexed color value.
Material:                   Contains material color values.
Matrix4x4:                  Holds a 4x4 homogenous matrix container.
Mesh:                       Contains a single mesh's data.
MeshFace:                   Holds a mesh's face data.
MeshFaceWraps:              Contains the texture wrapping for mesh faces.
MeshMaterialList:           Contains the material for face-mapping values.
MeshNormals:                Holds normals used for mesh data.
MeshTextureCoords:          Holds texture coordinates used for mesh data.
MeshVertexColors:           Holds vertex color information used for mesh vertices.
Patch:                      Defines a control patch.
PatchMesh:                  Contains a patch mesh (much like the Mesh template).
Quaternion:                 Holds a quaternion value.
SkinWeights:                Contains an array of weight values mapped to a mesh's vertices. 
Used in skinned meshes.
TextureFilename:            Contains the texture file name to use for a material.
TimedFloatKeys:             Contains an array of FloatKeys templates.
Vector:                     Holds a 3D coordinate value.
VertexDuplicationIndices:   Informs you which vertices are duplicates of other vertices.
XSkinMeshHeader:            Used by skinned meshes to define the number of bones contained 
in a mesh.
在rmxfguid.h中定义了各个模版的宏,例如:
/* {3D82AB44-62DA-11cf-AB39-0020AF71E433} */
DEFINE_GUID(TID_D3DRMMesh,
0x3d82ab44, 0x62da, 0x11cf, 0xab, 0x39, 0x0, 0x20, 0xaf, 0x71, 0xe4, 0x33);
每个模版名加上前缀TID_D3DRM就是宏定义名。
访问.X文件：/////////////////访问任何X文件首先要调用DirectXFileCreate函数创建一个IDirectXFile接口。
IDirectXFile *pDXFile = NULL;
HRESULT Result = DirectXFileCreate(&pDXFile);//用&pDXFile返回指向接口的指针。用SUCCEEDED或者
FAILED宏判断返回值是否有效。
注册一个定制ro标准模版：/////////////你可以把X文件中的模版移除，直接在代码里定义那些模版。IDirectXFile接口支持这样的特性。需要调
用IDirectXfile::RegisterTemplates函数。
HRESULT IDirectXfile::RegisterTemplates(
LPVOID pvData, // 一个定义模版数据的缓存，应该精确无误。
DWORD cbSize); // pvData缓存的字节数。
可以如下定义一个模版数据的缓存：
char *Templates = "
"xof 0303txt 0032 /  //标准X文件头。
template CustomTemplate { <4c944580-9e9a-11cf-ab43-0120af71e433> DWORD Length; array DWORD values[Length]; }";
之后在用RegisterTemplates将其注册：
pFile->RegisterTemplates(Templates, strlen(Templates));
注册标准模版：
首先需要在代码中包含rmxfguid.h和rmxftmpl.h。rmxfguid.h定义了各个标准模版的GUDI，rmxftmpl.h以
2进制数据形式定义了标准模版数据的缓存和其字节数。然后调用RegisterTemplates将其注册：
pFile->RegisterTemplates(D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
打开X文件：///////////////创建完IDirectXFile接口，注册模版之后需要打开X文件，枚举其数据对象。调用
IDirectXfile::CreateEnumObject函数。
HRESULT IDirectXfile::CreateEnumObject(LPVOID pvSource, // .X filename
DXFILELOADOPTIONS dwLoadOptions, // Load options
LPDIRECTXFILEENUMOBJECT* ppEnumObj); // Enum interface
当调用CreateEnumObject函数，用pvSource指定一个文件的名字，用ppEnumObj返回一个枚举对象接口指
针。用dwLoadOptions指定load操作方式。当指定DXFILELOAD_FROMFILE值，告诉DirectX从磁盘载入一个
文件。还有DXFILELOAD_FROMRESOURCE，DXFILELOAD_FROMMEMORY和DXFILELOAD_FROMURL分别表示从一个资
源，内存缓冲和Internet上加载X文件。当从Internet加载文件时，需要为其指定完整的网址。
下面代码从磁盘加载X文件：
// Filename = filename to load ("test.x" for example)
IDirectXFileEnumObject *pEnum;
pFile->CreateEnumObject((LPVOID)Filename, DXFILELOAD_FROMFILE, &pEnum);
Filename指向一个有效的文件名，pEnum返回一个枚举对象接口指针。
枚举数据对象：///////////////注册完模版，打开X文件并且得到一个枚举对象接口，下面需要从X文件读出数据。枚举对象接口指针指向
文件的第一个数据对象，因为每一个数据对象可能包含内嵌数据对象或者引用的数据对象，所以与第一个
数据对象同在一层级的其它数据对象为同层级数据对象。至于包含的子数据对象的类型，需要对其分别的
行进询问。
可以用 HRESULT IDirectXFileEnumObject::GetNextDataObject(LPDIRECTXFILEDATA*ppDataObj)得到一
个IDirectXFileData接口。它只有一个参数，如下：
IDirectXFileData *pData;
HRESULT hr = pEnum->GetNextDataObject(&pData);
利用此函数，可以不断地访问同一层级的数据对象接口，具体代码如下：
while(SUCCEEDED(pEnum->GetNextDataObject(&pData))) {
// 这里可以对pData数据对象进行操作。
pData->Release();//释放接口。
}
当返回值为FAILED，表示已经访问完所有的接口。当访问值为SUCCEEDED，你需要继续判断这个数据对象
是否包含子对象。利用接口IDirectXFileObject，和HRESULT IDirectXFileData::GetNextObject( 
LPDIRECTXFILEOBJECT* ppChildObj)函数，代码如下：
IDirectXFileObject *pObject;
while(SUCCEEDED(pData->GetNextObject(&pObject))) 
{
// 如果一个子对象存在，需要继续询问它，判断出它的类型为内嵌数据对象或者引用的数据对象。
pObject->Release();// 释放接口。
}
接下来询问接口，看其是否为内嵌数据对象：
IDirectXFileData *pSubData;
if(SUCCEEDED(pObject->QueryInterface( IID_IDirectXFileData, (void**)&pSubData))) {
// 如果询问内嵌数据对象成功，可以对pSubData数据对象进行操作
pSubData->Release();//释放接口。
}
看其是否为引用数据对象：
IDirectXFileDataReference *pRef;
IDirectXFileData *pSubData;
if(SUCCEEDED(pSubObj->QueryInterface( IID_IDirectXFileDataReference, (void**)&pRef))) {
// 如果询问引用的数据对象成功，解析出引用的原型。
pRef->Resolve(&pSubData);
//这里可以对pData数据对象进行操作。
pRef->Release();
pSubData->Release();//释放接口。
}
现在整理下思路：大体的思路其实很简单，首先枚举最顶层的数据对象，然后判断其是否有子对象，这个
子对象可能是内嵌对象或者引用对象二者之一，分别询问其接口，就可以判断出具体的类型。
下面是完整的Parse模版的函数：
BOOL Parse(char *Filename)
{
IDirectXFile *pFile = NULL;
IDirectXFileEnumObject *pEnum = NULL;
IDirectXFileData *pData = NULL;
if(FAILED(DirectXFileCreate(&pFile)))
return FALSE;
//注册标准模版。
if(FAILED(pFile->RegisterTemplates( (LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
return FALSE;
//创建一个枚举对象接口。
if(FAILED(pDXFile->CreateEnumObject((LPVOID)Filename, DXFILELOAD_FROMFILE, &pEnum))) {
pFile->Release();
return FALSE;
}
// 遍历所有的顶层数据对象。
while(SUCCEEDED(pEnum->GetNextDataObject(&pData))) {
// 用ParseObject解析其子数据对象。
ParseObject(pData);
pData->Release();
}
pEnum->Release();
pFile->Release();
return TRUE;
}
这个函数的主要部分在ParseObject(pData)，它负责解析子数据对象：
void ParseObject(IDirectXFileData *pData)
{
  IDirectXFileObject *pObject = NULL;
  IDirectXFileData *pSubData = NULL;
  IDirectXFileDataReference *pRef = NULL;
  while(SUCCEEDED(pData->GetNextObject(&pObject))) {
  if(SUCCEEDED(pObject->QueryInterface( IID_IDirectXFileDataReference, (void**)&pRef))) {
  pRef->Resolve(&pSubData);
  ParseObject(pSubData);
  pSubData->Release();
  pRef->Release();
                                                                                         }
  if(SUCCEEDED(pObject->QueryInterface( IID_IDirectXFileData, (void**)&pSubData))) {
    ParseObject(pSubData);
    pSubData->Release();
                                                                                    }
  pObject->Release();
                                                    } 
}
这是个第归函数，调用函数自身。判断子对象的类别，对其继续解析，直到返回值为FAILED，表示已没有
子对象。从上面可以看出，这个函数除了枚举所有的对象，并没有做任何事情，下面就要从这些数据对象
检索数据。
从数据对象检索数据：/////////////////
当你用IDirectXFileData接口指针指向一个有效的数据对象，可以调用IDirectXFileData::GetName函数得到该数据对象的名字。函数原型为：
HRESULT IDirectXFileData::GetName(
LPSTR pstrNameBuf, // 名字缓冲
LPDWORD pdwBufLen); // 名字缓冲的大小
可以这样使用这个函数：
DWORD Size;
pData->GetName(NULL, &Size);
char *Name = new char[Size];
pData->GetName(Name, &Size);
首先声明一个DWORD Size变量，调用GetName函数时将第一个参数设为NULL，在Size返回名字缓冲的大小。之后利用这个Size值创建存放名字的缓冲，再调用GetName在Name中返回数据对象的名字。
得到了数据对象的名字，你需要得到这个数据对象的模版GUID，去判断这个数据对象是否为你想使用的那个模版的数据对象。利用IDirectXFileData::GetType函数，其原型为：
HRESULT IDirectXFileData::GetType(const GUID ** ppguid);
可以这样使用这个函数：
const GUID *TemplateGUID = NULL;
pData->GetType(&TemplateGUID);//在TemplateGUID中返回该数据对象对应模版的GUID。
现在去匹配这个GUID，看它是否为你想使用的模版的数据对象。
if(*TemplateGUID == TID_D3DRMMeshNormals) {
// 如果匹配成功，这里可以继续处理这个模版的数据对象。
}
最后将介绍GetData函数，用它真正的得到了数据对象的数据。其原型为：
HRESULT IDirectXFileData::GetData(
LPCSTR szMember, // 设置为NULL
DWORD *pcbSize, // 数据的大小
void **ppvData); // 数据指针
下面介绍用GetData得到数据对象结构的大小和数据对象的数据。
假设有这样一个颜色的模版：
template ColorRGBA {
<35FF44E0-6C7C-11cf-8F52-0040333594A3>
FLOAT red;
FLOAT green;
FLOAT blue;
FLOAT alpha;
}
你想访问基于此模版的数据对象的数据你可以这样做： 
DWORD DataSize;
float *DataPtr;
pData->GetData(NULL, &DataSize, (void**)&DataPtr);
float red = *DataPtr++;
float green = *DataPtr++;
float blue = *DataPtr++;
float alpha = *DataPtr++;
得到指向数据对象的数据的指针后，就像访问一般的结构一样简单。当然，你可以做得更直接：
typedef struct {
float red, green, blue, alpha;
} sColorRGBA;//定一个结构方便访问数据对象的数据。
sColorRGBA *Color;
DWORD DataSize;
pData->GetData(NULL, &DataSize,(void**)&Color);
这样访问数据时更直接：
float red = Color->red;
float blue = Color->blue;
float green = Color->green;
float alpha = Color->alpha;
访问单个变量是很简单的，下面继续介绍访问数组或字符串。
访问数组：
DWORD DataSize;
DWORD *DataPtr;
pData->GetData(NULL, &DataSize, (void**)&DataPtr);
DWORD NumKeys = *DataPtr++;
for(DWORD i=0;i<NumKeys;i++) {
float fvalue = *(FLOAT*)DataPtr++;
访问字符串：
DWORD DataSize;
DWORD *DataPtr;
pData->GetData(NULL, &DataSize, (void**)&DataPtr);
char *StringPtr = (char*)DataPtr;
MessageBox(NULL, StringPtr, "Texture Filename", MB_OK);
为了访问数组或字符串，其本质就是把指针转化成匹配的类型，方便指针的定位操作。
    我以后还会写关于3D模型文件格式和骨骼动画，地形渲染方面的文章，请关注 ；）
                                                                         作者：迷糊小亚
                                                                         QQ：183237048 
|[逍遥剑客](http://bbs.skonline.net/viewpro.php?uid=8990)![Rank: 3](http://bbs.skonline.net/images/default/star_level2.gif)|2007-5-27 02:50|
