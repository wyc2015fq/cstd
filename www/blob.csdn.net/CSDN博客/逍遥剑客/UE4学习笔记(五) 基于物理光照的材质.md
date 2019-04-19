# UE4学习笔记(五): 基于物理光照的材质 - 逍遥剑客 - CSDN博客
2014年10月19日 01:18:02[xoyojank](https://me.csdn.net/xoyojank)阅读数：14402
个人分类：[图形技术																[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)](https://blog.csdn.net/xoyojank/article/category/337624)
UE4相对于UE3, 渲染上最大的改变就是引入了基于物理的光照, 而且在移动设备上也可以使用
# 光照
加入了基于图像的环境光照(IBL), 使得金属质感和粗糙程度的材质表现力大大提升
![](https://img-blog.csdn.net/20141019004937812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Diffuse
就是Lambert, 比较成熟的做法
## Specular
- 高光分布- PC上是Trowbridge-Reitz (GGX)
- Mobile是Blinn
- 几何阴影(粗糙度)- Schlick
- 菲涅尔- Schlick
## IBL
- 环境色- 预生成Cubemap进行采样
- 环境高光- PC上使用2D的LUT(查找表)
- Mobile使用类似COD:Black Ops的做法
# 材质
## 主要参数
- BaseColor- 这个没什么好说的, 从3D游戏一开始就存在
- 在UE4里结合其它参数, 可以使用纯色表现出非常不错的质感, 可以节省一张贴图
- 另一个方面来说, UE4中卡通风格(大面积色块)可以有更加丰富的光照细节
- Roughness- 粗糙还是光滑
- 意味着反射周边环境的光照
- Metallic- 金属质感调节
- 意味着反射周边环境的颜色
- Specular- 金属这个参数不起作用
- 默认设置成0.5(通常不用连接)
## 材质层
![](https://img-blog.csdn.net/20141019011334428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 灯光
局部灯光不再限于一个发光点了
- 面积光
- 球形光
![](https://img-blog.csdn.net/20141019011711721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 参考资料
https://www.unrealengine.com/blog/physically-based-shading-on-mobile
https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/PhysicallyBased/index.html
http://seblagarde.wordpress.com/2011/08/17/hello-world/
http://www.filmicworlds.com/2014/02/24/physically-based-specular-for-artists/
