# 基于FBX SDK的FBX模型解析与加载 -（二） - L_Andy的专栏 - CSDN博客

2015年06月30日 10:46:20[卡哥](https://me.csdn.net/L_Andy)阅读数：1348
个人分类：[OpenGL](https://blog.csdn.net/L_Andy/article/category/2817801)



### 5. 加载材质 

Material是一个模型渲染时必不可少的部分，当然，这些信息也被存到了FBX之中（甚至各种贴图等也可以直接内嵌到FBX内部），就需要从FBX中加载这些信息以完成带有材质的渲染。材质的加载可以与Mesh的加载相结合来完成，但更好的方法是独立进行，这样各模块间的关系更清晰，但这就需要一个额外的操作，那就是关联Mesh与Material。FBX中的材质对象包含了丰富的信息，比如最常规的从Max中可以看到那些材质属性，如ambient、diffuse、specular的color和texture；shininess、opacity值等，更高级一点的属性诸如Effect的参数、源文件等都可以保存。它是尽可能保证从建模工具中导出时不丢失地保存材质信息，但我们在使用时却可以有选择地读取。

#### 5.1 关联Mesh与材质

对于Material与Mesh独立加载的系统而言，首先需要读取相关的信息将两者关联起来，这些信息其实对也都存储在KFbxMesh之内（属于几何信息的一部分吧）。每个带有材质的Mesh结点上都会包含有一个类型为KFbxGeometryElementMaterial的结点（若不含有材质则该结点为空），该结点中记录了Mesh中的多边形（这里全部为三角形）与每个材质的对应关系，读取该结点中的信息建立Mesh与Material之间的连接关系，代码如下：

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7211515#)[copy](http://blog.csdn.net/bugrunner/article/details/7211515#)

- void ConnectMaterialToMesh(KFbxMesh* pMesh , int triangleCount , int* pTriangleMtlIndex)  
- {  
- // Get the material index list of current mesh
-     KFbxLayerElementArrayTemplate<int>* pMaterialIndices;  
-     KFbxGeometryElement::EMappingMode   materialMappingMode = KFbxGeometryElement::eNONE;  
- 
- if(pMesh->GetElementMaterial())  
-     {  
-         pMaterialIndices    = &pMesh->GetElementMaterial()->GetIndexArray();  
-         materialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();  
- if(pMaterialIndices)  
-         {  
- switch(materialMappingMode)  
-             {  
- case KFbxGeometryElement::eBY_POLYGON:  
-                 {  
- if(pMaterialIndices->GetCount() == triangleCount)  
-                     {  
- for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)  
-                         {  
- int materialIndex = pMaterialIndices->GetAt(triangleIndex);  
- 
-                             pTriangleMtlIndex[triangleIndex] = materialIndex;  
-                         }  
-                     }  
-                 }  
- break;  
- 
- case KFbxGeometryElement::eALL_SAME:  
-                 {  
- int lMaterialIndex = pMaterialIndices->GetAt(0);  
- 
- for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)  
-                     {  
- int materialIndex = pMaterialIndices->GetAt(triangleIndex);  
- 
-                         pTriangleMtlIndex[triangleIndex] = materialIndex;  
-                     }  
-                 }  
-             }  
-         }  
-     }  
- }  

其中上triangleCount即为从pMesh中读取得到的三角形的数量，pTriangleMtlIndex是一个长度为triangleCount的数组，主要用来存储读取到的三角形对应的材质索引。注意：这里考虑的情况是对于一个三角形只对应一个材质，而一般情况下也是这样（如果是对应多个材质的话需要些许修改此处的代码）。完成Mesh的索引读取之后即可以将pTriangleMtlIndex中的值以合适的方式转储到对应的三角形列表中（或以其它的方式对应）以便在渲染时使用。

#### 5.2 普通材质

FBX中实际存储材质信息的位置是每个Mesh中对应的一个类型为KFbxSurfaceMaterial的结点，其里边存储了普通材质的典型信息，主要包括以下属性（有一些没有列出）：
- **ShadingModel**                 材质的光照模型，一般为两种典型的局部光照模型：Phong、Lambert 
- **Emissive**                          Emissive属性 
- **EmissiveFactor**
- **Ambient**                           Ambient属性
- **AmbientFactor**
- **Diffuse**                             Diffuse属性
- **DiffuseFactor**
- **Specular**                           Specular属性
- **SpecularFactor**
- **Shininess**                         Sepcular的Shininess属性
- **Bump**                               Normal Map相关的属性
- **NormalMap**
- **BumpFactor**
- **TransparentColor**             Transparent属性
- **TransparencyFactor**
- **Reflection**                        Reflection属性
- **ReflectionFactor**

当然，在实际应用中这些属性并不一定需要全部读取，可以根据情况选择读取即可。材质的读取代码如下所述（简略版）：

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7211515#)[copy](http://blog.csdn.net/bugrunner/article/details/7211515#)

- void LoadMaterial(KFbxMesh* pMesh)  
- {  
- int materialCount;  
-     KFbxNode* pNode;  
- 
- if(pMesh && pMesh->GetNode())  
-     {  
-         pNode         = pMesh->GetNode();  
-         materialCount = pNode->GetMaterialCount();  
-     }  
- 
- if(materialCount > 0)  
-     {  
- for(int materialIndex = 0 ; materialIndex < materialCount ; materialIndex++)  
-         {  
-             KFbxSurfaceMaterial* pSurfaceMaterial = pNode->GetMaterial(materialIndex);  
- 
-             LoadMaterialAttribute(pSurfaceMaterial);  
-         }  
-     }  
- }  

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7211515#)[copy](http://blog.csdn.net/bugrunner/article/details/7211515#)

- void LoadMaterialAttribute(KFbxSurfaceMaterial* pSurfaceMaterial)  
- {  
- // Get the name of material
-     pSurfaceMaterial->GetName();  
- 
- // Phong material
- if(pSurfaceMaterial->GetClassId().Is(KFbxSurfacePhong::ClassId))  
-     {  
- // Ambient Color
-         fbxDouble3 = ((KFbxSurfacePhong*)pSurfaceMaterial)->Ambient;  
- // ...
- 
- // Diffuse Color
-         fbxDouble3 =((KFbxSurfacePhong*)pSurfaceMaterial)->Diffuse;  
- // ...
- 
- // Specular Color
-         fbxDouble3 =((KFbxSurfacePhong*)pSurfaceMaterial)->Specular;  
- // ...
- 
- // Emissive Color
-         fbxDouble3 =((KFbxSurfacePhong*)pSurfaceMaterial)->Emissive;  
- // ...
- 
- // Opacity
-         fbxDouble1 =((KFbxSurfacePhong*)pSurfaceMaterial)->TransparencyFactor;  
- // ...
- 
- // Shininess
-         fbxDouble1 =((KFbxSurfacePhong*)pSurfaceMaterial)->Shininess;  
- // ...
- 
- // Reflectivity
-         fbxDouble1 =((KFbxSurfacePhong*)pSurfaceMaterial)->ReflectionFactor;  
- // ...
- return;  
-     }  
- 
- // Lambert material
- if(pSurfaceMaterial->GetClassId().Is(KFbxSurfaceLambert::ClassId))  
-     {  
- 
- // Ambient Color
-         fbxDouble3=((KFbxSurfaceLambert*)pSurfaceMaterial)->Ambient;  
- // ...
- 
- // Diffuse Color
-         fbxDouble3 =((KFbxSurfaceLambert*)pSurfaceMaterial)->Diffuse;  
- // ...
- 
- // Emissive Color
-         fbxDouble3 =((KFbxSurfaceLambert*)pSurfaceMaterial)->Emissive;  
- // ...
- 
- // Opacity
-         fbxDouble1 =((KFbxSurfaceLambert*)pSurfaceMaterial)->TransparencyFactor;  
- // ...
- return;  
-     }  
- }  

上述代码就可以完成对普通属性加载。另外，材质中关联的Texture也需要进行加载，这个操作一般与一个纹理管理器结合起来进行，以便对所有的Texture与Material之间形成合理的关联，这一步的操作一般如下代码所述：

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7211515#)[copy](http://blog.csdn.net/bugrunner/article/details/7211515#)

- void LoadMaterialTexture(KFbxSurfaceMaterial* pSurfaceMaterial)  
- {  
- int textureLayerIndex;  
-     KFbxProperty pProperty;  
- int texID;  
-     MaterialTextureDesc::MtlTexTypeEnum texType;  
- 
- for(textureLayerIndex = 0 ; textureLayerIndex < KFbxLayerElement::LAYERELEMENT_TYPE_TEXTURE_COUNT ; ++textureLayerIndex)  
-     {  
-         pProperty = pSurfaceMaterial->FindProperty(KFbxLayerElement::TEXTURE_CHANNEL_NAMES[textureLayerIndex]);  
- if(pProperty.IsValid())  
-         {  
- int textureCount = pProperty.GetSrcObjectCount(KFbxTexture::ClassId);  
- 
- for(int j = 0 ; j < textureCount ; ++j)  
-             {  
-                 KFbxTexture* pTexture = KFbxCast<KFbxTexture>(pProperty.GetSrcObject(KFbxTexture::ClassId,j));  
- if(pTexture)  
-                 {  
- // Use pTexture to load the attribute of current texture...
-                 }  
-             }  
-         }  
-     }  
- }  

#### 5.3 硬件相关的材质与Effect

有过建模经验的童鞋都知道，在3D Max或Maya中可以为某些材质指定特定的Shader来完成特定的效果，这些模型在保存时也会保存相应的硬件相关的Shader到FBX模型中，因而针对这样属性的材质也需要特别的代码来进行加载。FBX里边支持嵌入CG、HLSL、GLSL等主流着色语言，而着色语言的类型在解析时也很容易得到。

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7211515#)[copy](http://blog.csdn.net/bugrunner/article/details/7211515#)

- void LoadMaterialAttribute(KFbxSurfaceMaterial* pSurfaceMaterial)  
- {  
-     KFbxImplementation* pImplementation;  
-     KString implemenationType;  
- 
-     pImplementation = GetImplementation(pSurfaceMaterial , ImplementationHLSL);  
-     KString implemenationType = "HLSL";  
- 
- if(pImplementation)  
-     {  
-         LoadMaterialEffect(pSurfaceMaterial , pImplementation , &implemenationType);  
-     }  
- }  

上述代码可以与前面的Material属性读取的代码合并。FBX一般通过一个类型为KFbxImplementation的对象将硬件相关的Shader与Material进行关联，可以使用如上的代码实现两者之间关联的情况的获取，其中ImplementationHLSL为一个标识HLSL类型Shader的宏，若是CG则用ImplementationCGFX。如果当前Material中包含了HLSL类型Shader之后，那么就可以得到一个不为空的KFbxImplementation类型的指针，在其中就可以解析该Shader的属性，否则，则该指针为空，说明些材质关联了其它类似的Shader或是不包含Shader。通过KFbxImplementation来获取Effect对应的属性的代码如下所示：

**[cpp]**[view
 plain](http://blog.csdn.net/bugrunner/article/details/7211515#)[copy](http://blog.csdn.net/bugrunner/article/details/7211515#)

- void LoadMaterialEffect(KFbxSurfaceMaterial* pSurfaceMaterial , const KFbxImplementation* pImplementation , KString* pImplemenationType)  
- {  
-     KFbxBindingTable const* lRootTable = pImplementation->GetRootTable();  
-     fbxString lFileName                = lRootTable->DescAbsoluteURL.Get();  
-     fbxString lTechniqueName           = lRootTable->DescTAG.Get();  
- 
- // Name of the effect file
-     lFileName.Buffer();  
- 
-     KFbxBindingTable const* pBTable = pImplementation->GetRootTable();  
- size_t entryCount = pBTable->GetEntryCount();  
- 
- for(size_t i = 0 ; i < entryCount ; ++i)  
-     {  
- const KFbxBindingTableEntry& btEntry = pBTable->GetEntry(i);  
- constchar* pEntrySrcType = btEntry.GetEntryType(true);  
-         KFbxProperty fbxProperty;  
- 
- // Name of Parameter
-         btEntry.GetDestination();  
- 
- // Semantic of Parameter
-         btEntry.GetDestination();  
- 
- if(strcmp(KFbxPropertyEntryView::sEntryType , pEntrySrcType) == 0)  
-         {  
-             fbxProperty = pSurfaceMaterial->FindPropertyHierarchical(btEntry.GetSource());  
- if(!fbxProperty.IsValid())  
-             {  
-                 fbxProperty = pSurfaceMaterial->RootProperty.FindHierarchical(btEntry.GetSource());  
-             }  
-         }  
- else
-         {  
- if(strcmp(KFbxConstantEntryView::sEntryType , pEntrySrcType) == 0)  
-             {  
-                 fbxProperty = pImplementation->GetConstants().FindHierarchical(btEntry.GetSource());  
-             }  
-         }  
- 
- if(fbxProperty.IsValid())  
-         {  
- if(fbxProperty.GetSrcObjectCount(FBX_TYPE(KFbxTexture)) > 0)  
-             {  
- // Texture Parameter
- for(int j = 0 ; j < fbxProperty.GetSrcObjectCount(FBX_TYPE(KFbxFileTexture)) ; ++j)  
-                 {  
-                     KFbxFileTexture* pFileTexture = fbxProperty.GetSrcObject(FBX_TYPE(KFbxFileTexture) , j);  
-                 }  
- 
- for(int j = 0 ; j < fbxProperty.GetSrcObjectCount(FBX_TYPE(KFbxLayeredTexture)) ; ++j)  
-                 {  
-                     KFbxLayeredTexture* pLayeredTexture = fbxProperty.GetSrcObject(FBX_TYPE(KFbxLayeredTexture) , j);  
-                 }  
- 
- for(int j = 0 ; j < fbxProperty.GetSrcObjectCount(FBX_TYPE(KFbxProceduralTexture)) ; ++j)  
-                 {  
-                     KFbxProceduralTexture* pProceduralTexture = fbxProperty.GetSrcObject(FBX_TYPE(KFbxProceduralTexture) , j);  
-                 }  
-             }  
- else
-             {  
- // Common Parameter
-                 KFbxDataType dataType = fbxProperty.GetPropertyDataType();  
- 
- // Bool value
- if(DTBool == dataType)  
-                 {  
- bool boolValue = KFbxGet<bool>(fbxProperty);  
-                 }  
- 
- // Integer value
- if(DTInteger == dataType || DTEnum == dataType)  
-                 {  
- int intValue = KFbxGet<int>(fbxProperty);  
-                 }  
- 
- // Float
- if(DTFloat == dataType)  
-                 {  
- float floatValue = KFbxGet<float>(fbxProperty);  
-                 }  
- 
- // Double
- if(DTDouble == dataType)  
-                 {  
- double doubleValue = (float)KFbxGet<double>(fbxProperty);  
-                 }  
- 
- // Double2
- if(DTDouble2 == dataType)  
-                 {  
-                     fbxDouble2 lDouble2 = KFbxGet<fbxDouble2>(fbxProperty);  
-                     D3DXVECTOR2 double2Value = D3DXVECTOR2((float)lDouble2[0] , (float)lDouble2[1]);  
-                 }  
- 
- // Double3
- if(DTDouble3  == dataType || DTVector3D == dataType || DTColor3 == dataType)  
-                 {  
-                     fbxDouble3 lDouble3 = KFbxGet<fbxDouble3>(fbxProperty);  
-                     D3DXVECTOR3 double3Value = D3DXVECTOR3((float)lDouble3[0] , (float)lDouble3[1] , (float)lDouble3[2]);  
-                 }  
- 
- // Double4
- if(DTDouble4  == dataType || DTVector4D == dataType || DTColor4 == dataType)  
-                 {  
-                     fbxDouble4 lDouble4 = KFbxGet<fbxDouble4>(fbxProperty);  
-                     D3DXVECTOR4 double4Value = D3DXVECTOR4((float)lDouble4[0] , (float)lDouble4[1] , (float)lDouble4[2] , (float)lDouble4[3]);  
-                 }  
- 
- // Double4x4
- if(DTDouble44 == dataType)  
-                 {  
-                     fbxDouble44 lDouble44 = KFbxGet<fbxDouble44>(fbxProperty);  
- 
-                     D3DXMATRIX double4x4Value;  
- 
- for(int i = 0 ; i < 4 ; ++i)  
-                     {  
- for(int j = 0 ; j < 4 ; ++j)  
-                         {  
-                             double4x4Value.m[i][j] = (float)lDouble44[i][j];  
-                         }  
-                     }  
-                 }  
- 
- // String
- if(DTString == dataType || DTUrl == dataType || DTXRefUrl == dataType)  
-                 {  
- char* pStringBuffer =(KFbxGet<fbxString>(fbxProperty)).Buffer();  
-                 }  
-             }  
-         }  
-     }  
- }  

可以解析到的Effect的主要属性包括Shader所对应的源文件、Shader中提供的各种外部参数的初始设定等（比如在3D Max中通过UI控件所调节的参数的数值）。具体的方法代码里边已经比较明确了，这里就不在赘述了。后续的一些操作就要看整个材质与Effect部分的数据结构如何组织以及如何与你自己的代码整合。

#### 5.4 根据材质优化Mesh

通过FBX导出之后得到的FBX模型在存储时一般会以几何属性为首要考量因素来生成整个文件的Scene graph，因此上述解析得到的几何网格与Material之间的映射关系可能并不适合于直接进行绘制，一般需要重新再组织。比如其间的映射关系可能是
- Triangle0 -> Material1
- Triangle1 -> Material0
- Triangle2 -> Material1
- ...

如果一个应用的渲染流程使用了Material之间的最少切换次数来作为渲染的首要考虑的话，那么就不能直接 使用Triangle的顺序来生成渲染Buffer，而需要根据Material对其进行再排序并重新组织几何数据间的次序。

完成上述加载之后即可实现带有材质的渲染效果：

![](http://hi.csdn.net/attachment/201201/19/4491947_1326982411jX56.png)

