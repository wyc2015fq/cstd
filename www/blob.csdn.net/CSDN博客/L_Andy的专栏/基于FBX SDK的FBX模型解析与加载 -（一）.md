# 基于FBX SDK的FBX模型解析与加载 -（一） - L_Andy的专栏 - CSDN博客

2015年06月30日 10:44:51[卡哥](https://me.csdn.net/L_Andy)阅读数：2454
个人分类：[OpenGL](https://blog.csdn.net/L_Andy/article/category/2817801)



### 1. 简介 

FBX是Autodesk的一个用于跨平台的免费三维数据交换的格式（最早不是由Autodesk开发，但后来被其收购），目前被 众多的标准建模软件所支持，在游戏开发领域也常用来作为各种建模工具的标准导出格式。Autodesk提供了基于C++（还有Python）的SDK来实现对FBX格式的各种读写、修改以及转换等操作，之所以如此是因为FBX的格式不是公开的，这也是FBX的诟病之一。与FBX相对的则是格式开源的Collada，它的应用也很广泛。总体来说这两种格式还是各有优劣不相上下，关于两种格式在游戏开发中使用的对比与讨论也比较多，可见GameDev中的帖子：[http://www.gamedev.net/topic/467753-collada-vs-autodesk-fbx](http://www.gamedev.net/topic/467753-collada-vs-autodesk-fbx) ,
 这里就不再论述了。大多数情况下我们是需要解析模型在程序中渲染使用，因此这里主要讨论使用FBX SDK来对FBX模型进行解析与加载（主要包括几何网格、材质、Light与Camera、Skeleton动画等），而对于在建模工具中可能涉及到的FBX写出等则没有涉及。

### 2. FBX SDK的配置

首先，在使用前需要下载安装FBX的SDK，可以从Autodesk的网站上进行获得最新的版本[http://usa.autodesk.com/adsk/servlet/index?siteID=123112&id=7478532](http://usa.autodesk.com/adsk/servlet/pc/item?id=10775855\&siteID=123112)(需要填些基本信息注册下)。安装之后在VS里边的配置就跟D3D类似。其中的Samples基本上涵盖了FBX相关的应用，可以在使用之前好好研究一下。最新的SDK版本（2012版）与之前的版本会在细节上有所不同（有些较大的改动是实现某些功能
 的API接口的修改，具体细节可以用2012的Programmer's guide中找到），而且支持最新的FBX格式，因此最好就直接用最新的版本。

### 3. FBX模型的组织结构

FBX是以scene graph的结构来存储模型的所有信息（也可以认为是一个多叉树），类似于OSG中的组织方式，这一点可以从SDK所带的Sample里边很清楚的看出来。一个较为典型的模型的组织结构与下图所示：
![](http://hi.csdn.net/attachment/201201/19/4491947_1326954151M96A.png)

整个Scene是从一个空属性的根结点开始，其中每个结点均是一个KFbxNode的对象，所有对象之间的关联均是双向的，比如从子结点可以索引到父结点，从父结点也可以索引到子结点；从单个结点可以索引到整个Scene，从Scene也可以索引到该结点。每个结点都会有一个标记属性的Enum值，比如eMesh、eLight、eCamera或eSkeleton等，分别用来标记当前结点是Mesh、Light、Camera或Skeleton。在整个结构的遍历过程中可以通过判断不同的结点属性而进行不同的处理操作。

在进行使用SDK进行FBX的处理操作之前需要先初始化两个必须的FBX对象：KFbxSdkManager和KFbxScene。前者用来对所有的FBX对象进行内在管理，所有使用SDK加载的资源均在此对象的管控之下，最终的资源释放也由其来完成。有了内存管理器之后再在其上创建一个相关的KFbxScene对象之后即可以进行模型的加截与处理了。KFbxScene其实相当于Manager提供的整个场景对象的一个入口。两个对象的初始化与配置代码如下所述：

***初始化SDKManager***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- bool FBXImporter::Initialize()  
- {  
- // Create the FBX SDK Manager, destroy the old manager at first
- if(mpFBXSDKManager)  
-     {  
-        mpFBXSDKManager->Destroy();  
-     }  
-     mpFBXSDKManager = KFbxSdkManager::Create();  
- 
- if(mpFBXSDKManager == NULL)  
-     {  
- returnfalse;  
-     }  
- 
- // Create an IOSettings object
-     KFbxIOSettings* ios = KFbxIOSettings::Create(mpFBXSDKManager , IOSROOT);  
-     mpFBXSDKManager->SetIOSettings(ios);  
- 
- // Load plug-ins from the executable directory
-     KString lExtension = "dll";  
-     KString lPath = KFbxGetApplicationDirectory();  
-     mpFBXSDKManager->LoadPluginsDirectory(lPath.Buffer() , lExtension.Buffer());  
- 
- // Create the entity that hold the whole Scene
-     mpFBXSDKScene = KFbxScene::Create(mpFBXSDKManager , "");  
- 
- returntrue;  
- }  

***FbxScene的初始化***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- bool FBXImporter::LoadScene(constchar* pSeneName)  
- {  
- if(mpFBXSDKManager == NULL)  
-     {  
- returnfalse;  
-     }  
- 
- // Get the file version number generate by the FBX SDK.
-     KFbxSdkManager::GetFileFormatVersion(mSDKVersion.mMajor , mSDKVersion.mMinor , mSDKVersion.mRevision);  
- 
- // Create an importer.
-     KFbxImporter* pKFBXImporter = KFbxImporter::Create(mpFBXSDKManager , "");  
- 
- // Initialize the importer by providing a filename
-     FBXFileVersion fileVersion;  
- 
- bool importStatus = pKFBXImporter->Initialize(fileName , -1 , mpFBXSDKManager->GetIOSettings());  
-     lImporter->GetFileVersion(fileVersion.mMajor , fileVersion.mMinor , fileVersion.mRevision);  
- 
- if(!importStatus)  
-     {  
- returnfalse;  
-     }  
- 
- // Import the scene
-     mpFBXScene->Clear();  
- 
-     importStatus = pKFBXImporter->Import(m_mpFBXScene);  
- 
- // Destroy the importer.
-     pKFBXImporter->Destroy();  
- 
- return importStatus;  
- }  

在完成了对KFbxScene的初始化操作之后即可以从其中得到整个模型的所有信息。由于FBX的是采用了类似于树的结构来进行存储，因而就很容易使用类似于树的递归方法来遍历其中的每个结点，并根据结点的属性选择合适的处理操作，下述代码就完成了从根结点开始的全局遍历：

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ProcessNode(KFbxNode* pNode)  
- {  
-     KFbxNodeAttribute::EAttributeType attributeType;  
- 
- if(pNode->GetNodeAttribute())  
-     {  
- switch(pNode->GetNodeAttribute()->GetAttributeType())  
-         {  
- case KFbxNodeAttribute::eMESH:  
-             ProcessMesh(pNode);  
- break;  
- case KFbxNodeAttribute::eSKELETON:  
-             ProcessSkeleton(pNode);  
- break;  
- case KFbxNodeAttribute::eLIGHT:  
-             ProcessLight(pNode);  
- break;  
- case KFbxNodeAttribute::eCAMERA:  
-             ProcessCamera();  
- break;  
-         }  
-     }  
- 
- for(int i = 0 ; i < pNode->GetChildCount() ; ++i)  
-     {  
-         ProcessNode(pNode->GetChild(i));  
-     }  
- }  

上述代码比较简单，直接传入由KFbxScene中获得的根结点之后即可遍历到每一个子结点。在FBX的存储中，每个父结点可以包含多个子结点，但每个子结点只有一个根结点，而且这其中的联系是双向的，这样很方便，比如在处理Skeleton时就常常需要从子结点中得到父结点的matrix等信息，而这种双向关系使得这些操作很容易实现。注意，上述代码中有pNode->GetNodeAttribute()检查操作是必须的，因为并不是所有的结点都有相应的属性（Attribute也是以子结点的方式关联到当前的结点上的，因而可能为空）。

### 4. 加载几何网格

FBX对几何网格支持得还是很好的，Nurbes、Polygon、Triangle等均可以存储。不过为了简化加载和处理时的操作，最好直接在FBX导出插件中选择一种统一的模式。比如可以在导出生成FBX时选中Triangluation的属性，那么FBX导出插件会自动把所有的Nurbes、Polygon三角化为三角形进行存储。当然，这个过程也可以在模型进行加载时来进行。这样在得到的FBX中就只有三角形这样一种网格模型，方便了加载的操作。模型的几何数据主要包括以下部分：
- **Vertex      **组成网格的顶点信息，这一部分是必须的。
- **Color**每个顶点的颜色。
- **Normal**每个顶点所对应的法向，是由FBX导出插件计算生成，可以是逐面片或逐顶点。
- **Tangent**每个顶点所对应的切向，是由FBX导出插件计算生成，可以是逐面片或逐顶点。
- **UV**每个顶点所对应的贴图UV值，一般来说，每个UV对应一个Layer，一个顶点可以有多个UV通道，这在读入的时间需要进行判断

几何网格的加载比较简单，直接递归地从根结点来遍历整个graph，检测当前的结点是否为eMESH的属性，若是即处理其中的几何数据，主要代码如下所示：

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ProcessMesh(KFbxNode* pNode)  
- {  
-     KFbxMesh* pMesh = pNode->GetMesh();  
- if(pMesh == NULL)  
-     {  
- return;  
-     }  
- 
-     D3DXVECTOR3 vertex[3];  
-     D3DXVECTOR4 color[3];  
-     D3DXVECTOR3 normal[3];  
-     D3DXVECTOR3 tangent[3];  
-     D3DXVECTOR2 uv[3][2];  
- 
- int triangleCount = pMesh->GetPolygonCount();  
- int vertexCounter = 0;  
- 
- for(int i = 0 ; i < triangleCount ; ++i)  
-     {  
- for(int j = 0 ; j < 3 ; j++)  
-         {  
- int ctrlPointIndex = pMesh->GetPolygonVertex(i , j);  
- 
- // Read the vertex
-             ReadVertex(pMesh , ctrlPointIndex , &vertex[j]);  
- 
- // Read the color of each vertex
-             ReadColor(pMesh , ctrlPointIndex , vertexCounter , &color[j]);  
- 
- // Read the UV of each vertex
- for(int k = 0 ; k < 2 ; ++k)  
-             {  
-                 ReadUV(pMesh , ctrlPointIndex , pMesh->GetTextureUVIndex(i, j) , k , &(uv[j][k]));  
-             }  
- 
- // Read the normal of each vertex
-             ReadNormal(pMesh , ctrlPointIndex , vertexCounter , &normal[j]);  
- 
- // Read the tangent of each vertex
-             ReadTangent(pMesh , ctrlPointIndex , vertexCounter , &tangent[j]);  
- 
-             vertexCounter++;  
-         }  
- 
- // 根据读入的信息组装三角形，并以某种方式使用即可，比如存入到列表中、保存到文件等...
-     }  
- }  

上述代码完成了从一个Node里边读出相应的网格信息。首先，从Node里边得到相应KFbxMesh指针，可知，如果该Node不是eMESH属性的话那么该指针就为空，后继操作不能再进行。注意其中用triangleCount变量来存储pMesh->GetPolygonCount()的值，这主要是在前面也提到过了，假定对于所有的FBX模型在存储时均选定了Triangulation的操作，因而其中存储的Polygon是三角形，如此一来每个里边一定只包含3个顶点，依次读入这3个顶点所对应的各属性信息即可。在FBX中对于每个顶点所对应的各种额外属性，比如Normal、Tangent、UV等均可对应多个通道，这可以通过在每个Mesh里边增加相应属性的一个Layer即可实现，在使用FBX
 SDK写出FBX文件时很容易做到。比如上述代码中就从FBX中读出4个UV通道中的值（第一个是正常的贴图通道，第二层是LightMap的通道）。vertexCounter是记录已经处理过的顶点的数目，这主要是顶点信息读取在某些映射模式下（比如下述使用到vertexCounter的eBY_POLYGON_VERTEX等）需要知道其在全局顶ControlPoints中的信息，因而增加这样的一个变量来进行记录。

***读入顶点：***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ReadVertex(KFbxMesh* pMesh , int ctrlPointIndex , D3DXVECTOR3* pVertex)  
- {  
-     KFbxVector4* pCtrlPoint = pMesh->GetControlPoints();  
- 
-     pVertex->x = pCtrlPoint[ctrlPointIndex].GetAt(0);  
-     pVertex->y = pCtrlPoint[ctrlPointIndex].GetAt(1);  
-     pVertex->z = pCtrlPoint[ctrlPointIndex].GetAt(2);  
- }  

***读入Color:***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ReadColor(KFbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR4* pColor)  
- {  
- if(pMesh->GetElementVertexColorCount < 1)  
-     {  
- return;  
-     }  
- 
-     KFbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);  
- switch(pVertexColor->GetMappingMode())  
-     {  
- case KFbxGeometryElement::eBY_CONTROL_POINT:  
-         {  
- switch(pVertexColor->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pColor->x = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;  
-                     pColor->y = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;  
-                     pColor->z = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;  
-                     pColor->w = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);  
-                     pColor->x = pVertexColor->GetDirectArray().GetAt(id).mRed;  
-                     pColor->y = pVertexColor->GetDirectArray().GetAt(id).mGreen;  
-                     pColor->z = pVertexColor->GetDirectArray().GetAt(id).mBlue;  
-                     pColor->w = pVertexColor->GetDirectArray().GetAt(id).mAlpha;  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
- 
- case KFbxGeometryElement::eBY_POLYGON_VERTEX:  
-         {  
- switch (pVertexColor->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pColor->x = pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;  
-                     pColor->y = pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;  
-                     pColor->z = pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;  
-                     pColor->w = pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;  
-                 }  
- break;  
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);  
-                     pColor->x = pVertexColor->GetDirectArray().GetAt(id).mRed;  
-                     pColor->y = pVertexColor->GetDirectArray().GetAt(id).mGreen;  
-                     pColor->z = pVertexColor->GetDirectArray().GetAt(id).mBlue;  
-                     pColor->w = pVertexColor->GetDirectArray().GetAt(id).mAlpha;  
-                 }  
- break;  
- default:  
- break;  
-             }  
-         }  
- break;  
-     }  
- }  

***读入UV:***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ReadUV(KFbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , D3DXVECTOR2* pUV)  
- {  
- if(uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)  
-     {  
- returnfalse;  
-     }  
- 
-     KFbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);  
- 
- switch(pVertexUV->GetMappingMode())  
-     {  
- case KFbxGeometryElement::eBY_CONTROL_POINT:  
-         {  
- switch(pVertexUV->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pUV->x = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex).GetAt(0);  
-                     pUV->y = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex).GetAt(1);  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);  
-                     pUV->x = pVertexUV->GetDirectArray().GetAt(id).GetAt(0);  
-                     pUV->y = pVertexUV->GetDirectArray().GetAt(id).GetAt(1);  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
- 
- case KFbxGeometryElement::eBY_POLYGON_VERTEX:  
-         {  
- switch (pVertexUV->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
-                     pUV->x = pVertexUV->GetDirectArray().GetAt(textureUVIndex).GetAt(0);  
-                     pUV->y = pVertexUV->GetDirectArray().GetAt(textureUVIndex).GetAt(1);  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
-     }  
- }  

***读入Normal:***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ReadNormal(KFbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR3* pNormal)  
- {  
- if(pMesh->GetElementNormalCount() < 1)  
-     {  
- return;  
-     }  
- 
-     KFbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);  
- switch(leNormal->GetMappingMode())  
-     {  
- case KFbxGeometryElement::eBY_CONTROL_POINT:  
-         {  
- switch(leNormal->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pNormal->x = leNormal->GetDirectArray().GetAt(ctrlPointIndex).GetAt(0);  
-                     pNormal->y = leNormal->GetDirectArray().GetAt(ctrlPointIndex).GetAt(1);  
-                     pNormal->z = leNormal->GetDirectArray().GetAt(ctrlPointIndex).GetAt(2);  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);  
-                     pNormal->x = leNormal->GetDirectArray().GetAt(id).GetAt(0);  
-                     pNormal->y = leNormal->GetDirectArray().GetAt(id).GetAt(1);  
-                     pNormal->z = leNormal->GetDirectArray().GetAt(id).GetAt(2);  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
- 
- case KFbxGeometryElement::eBY_POLYGON_VERTEX:  
-         {  
- switch(leNormal->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pNormal->x = leNormal->GetDirectArray().GetAt(vertexCounter).GetAt(0);  
-                     pNormal->y = leNormal->GetDirectArray().GetAt(vertexCounter).GetAt(1);  
-                     pNormal->z = leNormal->GetDirectArray().GetAt(vertexCounter).GetAt(2);  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = leNormal->GetIndexArray().GetAt(vertexCounter);  
-                     pNormal->x = leNormal->GetDirectArray().GetAt(id).GetAt(0);  
-                     pNormal->y = leNormal->GetDirectArray().GetAt(id).GetAt(1);  
-                     pNormal->z = leNormal->GetDirectArray().GetAt(id).GetAt(2);  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
-     }  
- }  

***读入Tangent:***

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7210511#)[copy](http://blog.csdn.net/bugrunner/article/details/7210511#)

- void ReadTangent(KFbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , D3DXVECTOR3* pTangent)  
- {  
- if(pMesh->GetElementTangentCount() < 1)  
-     {  
- return;  
-     }  
- 
-     KFbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);  
- 
- switch(leTangent->GetMappingMode())  
-     {  
- case KFbxGeometryElement::eBY_CONTROL_POINT:  
-         {  
- switch(leTangent->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pTangent->x = leTangent->GetDirectArray().GetAt(ctrlPointIndex).GetAt(0);  
-                     pTangent->y = leTangent->GetDirectArray().GetAt(ctrlPointIndex).GetAt(1);  
-                     pTangent->z = leTangent->GetDirectArray().GetAt(ctrlPointIndex).GetAt(2);  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);  
-                     pTangent->x = leTangent->GetDirectArray().GetAt(id).GetAt(0);  
-                     pTangent->y = leTangent->GetDirectArray().GetAt(id).GetAt(1);  
-                     pTangent->z = leTangent->GetDirectArray().GetAt(id).GetAt(2);  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
- 
- case KFbxGeometryElement::eBY_POLYGON_VERTEX:  
-         {  
- switch(leTangent->GetReferenceMode())  
-             {  
- case KFbxGeometryElement::eDIRECT:  
-                 {  
-                     pTangent->x = leTangent->GetDirectArray().GetAt(vertecCounter).GetAt(0);  
-                     pTangent->y = leTangent->GetDirectArray().GetAt(vertecCounter).GetAt(1);  
-                     pTangent->z = leTangent->GetDirectArray().GetAt(vertecCounter).GetAt(2);  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eINDEX_TO_DIRECT:  
-                 {  
- int id = leTangent->GetIndexArray().GetAt(vertecCounter);  
-                     pTangent->x = leTangent->GetDirectArray().GetAt(id).GetAt(0);  
-                     pTangent->y = leTangent->GetDirectArray().GetAt(id).GetAt(1);  
-                     pTangent->z = leTangent->GetDirectArray().GetAt(id).GetAt(2);  
-                 }  
- break;  
- 
- default:  
- break;  
-             }  
-         }  
- break;  
-     }  
- }  

上述几个Normal、Tangent、UV等信息读取的函数的实现其实都差不多，首先需要判断有没有相应的Layer关联在当前的Mesh中，若有则获取其地址，然后根据不同的映射方式使用不同的方法从内存中读取相应的值即可。

完成了这些基本几何信息的读取之后即可以使用其进行渲染了：

![](http://hi.csdn.net/attachment/201201/19/4491947_13269617104R47.png)

下一篇来介绍一下材质的读取与使用...

