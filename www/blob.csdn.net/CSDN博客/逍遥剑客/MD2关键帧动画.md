# MD2关键帧动画 - 逍遥剑客 - CSDN博客
2007年06月04日 01:00:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3259
本想用骨骼动画的，无奈.x格式的太过于复杂，而且游戏业里也不怎么用。
只是把它的原理弄明白一些了，但是，我弱小的意志在DirectX SDK那个1000多行的SkinedMesh例子面前顺利地崩溃了，心想，还是先从最基本的关键帧动画开始做吧-_-!
确定文件格式：md2（正好连找都不用找了，老师给了）
以下是我Copy的：
**MD2****文件格式简介**
MD2是Quake2中使用的模型文件格式，由于其比较简单，容易实现，所以应用很广，是一种经典的动画模型格式。该文件格式由2部分组成：一部分是文件头，包含了文件ID号、版本号和有关模型的各种数据的起始地址等；另一部分是文件的主体，包含了有关模型的各种数据，如顶点数据、纹理数据、法向量数据等。
MD2是基于关键帧动画的，关键帧插值的数学公式为：
**p(t) = p(0) + t ( p1 - p0 )**
其中：
t — 当前时间。0表示开始，1表示结束；
p(t) — 时间t 时方程的值；
p0 — 起始位置；
p1 — 结束位置。
MD2共有16个关键帧：
start:0 end:39 name:stand
start:40 end:45 name:run
start:46 end:53 name:attack
start:54 end:65 name:pain
start:66 end:71 name:jump
start:72 end:83 name:flip
start:84 end:94 name:salute
start:95 end:111 name:taunt
start:112 end:122 name:wave
start:123 end:134 name:point
start:135 end:153 name:crstnd
start:154 end:159 name:crwalk
start:160 end:168 name:crattack
start:169 end:172 name:crpain
start:173 end:177 name:crdeath
start:178 end:197 name:death
说白了，一个模型有16个动作，每个动作有很多帧组成，每帧由很多三角形组成一个网络，每个三角形由三个顶点组成，每个顶点由x,y,z三个坐标组成，每个……（再说就欠揍了）
那么怎么让它动呢？知道怎么放电影不？就是一帧帧地画就行了！
这是我改写的类：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#pragma once
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <iostream>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <d3dx9.h>
/**//***************************************************************************/
/**//**/
/**//* File: XMD2Model.h                                                       */
/**//* Author: bkenwright@screentoys.net                                       */
/**//* Date: 10-11-2002                                                        */
/**//**/
/**//***************************************************************************/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// This file holds our self contained .md2 (quake2) class for loading in and
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// displaying our .md2 file in directX3D.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stMd2Header
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int magic;              // The magic number used to identify the file.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int version;            // The file version number (must be 8).
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int skinWidth;          // The width in pixels of our image.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int skinHeight;         // The height in pixels of our image.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int frameSize;          // The size in bytes the frames are.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numSkins;           // The number of skins associated with the model.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numVertices;  // The number of vertices.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numTexCoords; // The number of texture coordinates.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numTriangles; // The number of faces (polygons).
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numGlCommands;      // The number of gl commands.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numFrames;          // The number of animated frames.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int offsetSkins;  // The offset in the file for the skin data.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int offsetTexCoords;// The offset in the file for the texture data.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int offsetTriangles;// The offset in the file for the face data.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int offsetFrames; // The offset in the file for the frames data.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int offsetGlCommands;// The offset in the file for the gl commands data.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int offsetEnd;          // The end of the file offset.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Some structures to hold or read in data in.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stMd2Skins
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)char skinName[64];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stMd2TexCoords
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)short u, v;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stMd2Triangles
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)short vertexIndex[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)short texIndex[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stMd2Vertices
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float vertex[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float normal[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stMd2Frames
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)char name[16];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stMd2Vertices* pFinalVerts;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// These two variables are declared in dxdraw.cpp thats why they have the
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// extern keyword in front of them.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)extern LPDIRECT3DDEVICE9 g_pd3dDevice;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stKeyFrame
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int start;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int end;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)char szName[16];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct stKey
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int numKeys;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stKeyFrame *pKey;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Our DirectX3D structure definition.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct my_vertex
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    D3DXVECTOR3 m_vecPos; //位置
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    D3DCOLOR m_dwDiffuse; //颜色
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    D3DXVECTOR2 m_vecTex; //纹理坐标
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
/**//***************************************************************************/
/**//**/
/**//* The XMD2Model class, yup its name speaks for itself, it loads the 3D     */
/**//* model data from the .md2 file and then we can access its public data    */
/**//* variables to use the data.                                              */
/**//**/
/**//***************************************************************************/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class XMD2Model
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)    XMD2Model()...{ m_vertex_description = (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1); };
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)~XMD2Model()...{};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool ImportMD2(char* szFileName, char* szTexName);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void Release();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void RenderFrame();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void Animate(UINT iAnimKey);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)protected:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void ReadMD2Data();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void SetUpFrames();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void SetUpDX();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool Timer(float* t);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void SetDXVertices(UINT iAnimKey);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    FILE* m_fp;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)protected:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stMd2Header       m_Md2Header;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stMd2Skins        *m_pSkins;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stMd2Triangles    *m_pTriangles;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stMd2TexCoords  *m_pTexCoords;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stMd2Frames       *m_pFrames;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)protected:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stKey             m_Keys;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int m_curFrame;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int m_nextFrame; 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int m_curAnimKey;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)protected:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float m_lastTime;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float m_elapsedTime;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)protected:// dx variables
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    UINT m_vertex_description;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    IDirect3DTexture9* m_pTexture;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    IDirect3DVertexBuffer9* m_vb; // vertex buffer
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "XMD2Model.h"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
/**//***************************************************************************/
/**//**/
/**//* The action implimentations of our XMD2Model class.                      */
/**//**/
/**//***************************************************************************/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)bool XMD2Model::ImportMD2(char* szFileName, char* szTexName)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_fp = fopen(szFileName, "rb");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    ReadMD2Data();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    SetUpFrames();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    SetUpDX();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fclose(m_fp);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Load or textures into our DX.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Use D3DX to create a texture from a file based image
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szTexName, &m_pTexture ) ) )
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        MessageBox(NULL, "加载纹理失败！", NULL, MB_OK);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returnfalse;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)returntrue;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void XMD2Model::ReadMD2Data()
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fread(&m_Md2Header, 1, sizeof(m_Md2Header), m_fp);    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Allocate memory for our data so we can read it in.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_pSkins          =new stMd2Skins  [ m_Md2Header.numSkins ];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_pTexCoords      =new stMd2TexCoords[ m_Md2Header.numTexCoords ];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_pTriangles      =new stMd2Triangles[ m_Md2Header.numTriangles ];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_pFrames         =new stMd2Frames [ m_Md2Header.numFrames ];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// -1- Seek to the start of our skins name data and read it in.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fseek(m_fp, m_Md2Header.offsetSkins, SEEK_SET);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fread(m_pSkins, sizeof(stMd2Skins), m_Md2Header.numSkins, m_fp);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// -2- Seek to the start of our Texture Coord data and read it in.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fseek(m_fp, m_Md2Header.offsetTexCoords, SEEK_SET);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fread(m_pTexCoords, sizeof(stMd2TexCoords), m_Md2Header.numTexCoords, m_fp);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// -3- Seek to the start of the Triangle(e.g. Faces) data and read that in.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fseek(m_fp, m_Md2Header.offsetTriangles, SEEK_SET);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fread(m_pTriangles, sizeof(stMd2Triangles), m_Md2Header.numTriangles, m_fp);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// -4- Finally lets read in "one" of the frames, the first one.!
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)struct stAliasVerts
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)byte vertex[3]; // an index reference into the location of our vertexs
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)byte lightNormalIndex; // in index into which tex coords to use.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    };
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)struct stAliasFrame
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float scale[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float translate[3];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)char name[16];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        stAliasVerts aliasVerts[1];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    };
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    unsigned char largebuffer[50000];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    stAliasFrame* pTempFrame = (stAliasFrame*) largebuffer;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    fseek(m_fp, m_Md2Header.offsetFrames, SEEK_SET);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for(int iFrame=0; iFrame< m_Md2Header.numFrames; iFrame++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        fread(pTempFrame, 1, m_Md2Header.frameSize, m_fp); // We have read in all the frame data here (into a temporyary!!..eeEKK)..
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        m_pFrames[iFrame].pFinalVerts =new stMd2Vertices[ m_Md2Header.numVertices ];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        strcpy( m_pFrames[iFrame].name,  pTempFrame->name );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// CONVERSION!  A few things before we can use our read in values,
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// for some reason the Z and Y need to be swapped, as Z is facing up
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// and Y is facing into the screen.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Also our texture coordinates values are between 0 and 256, we just
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// divide them all by 256 which makes them between 0 and 1.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Swap Z<->Y
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for(int i=0; i< m_Md2Header.numVertices; i++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            m_pFrames[iFrame].pFinalVerts[i].vertex[0] = pTempFrame->aliasVerts[i].vertex[0] * pTempFrame->scale[0] 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)+ pTempFrame->translate[0];      // x
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            m_pFrames[iFrame].pFinalVerts[i].vertex[2] =-1*(pTempFrame->aliasVerts[i].vertex[1] * pTempFrame->scale[1] 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)+ pTempFrame->translate[1]); // z
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            m_pFrames[iFrame].pFinalVerts[i].vertex[1] = pTempFrame->aliasVerts[i].vertex[2] * pTempFrame->scale[2] 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)+ pTempFrame->translate[2];      // y
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Scale Textures.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for (int j=0; j< m_Md2Header.numTexCoords; j++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// WARNING.. you can't put a decimal number into a short...e.g.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// you can't put 0.1 into a unsigned short int, it will be changed to 0.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)/**//*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        m_pTexCoords[j].u = m_pTexCoords[j].u ;// 256; //float(m_Md2Header.skinWidth);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        m_pTexCoords[j].v = m_pTexCoords[j].v ;// 256; //float(m_Md2Header.skinHeight);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void XMD2Model::SetUpFrames()
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//stand01, stand02... walk01 etc.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//First lets see how many key frames there are.. e.g.  walk, run, stand etc.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_Keys.numKeys =0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)char strName[16] =...{0};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)char strLastName[16] =...{0};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)char strVeryFirstName[16] =...{0};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for(int iFrame=0; iFrame< m_Md2Header.numFrames; iFrame++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        strcpy(strName, m_pFrames[iFrame].name);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int stringLength = strlen( strName );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)for(int i=0; i< stringLength; i++)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// a:97   A:65   z:122  Z:90
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if( (strName[i] <'A') || (strName[i] >'z' ) )
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// Its an integer if we are here.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                strName[i] ='
