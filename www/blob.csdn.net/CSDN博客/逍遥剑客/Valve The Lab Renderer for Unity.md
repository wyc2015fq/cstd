# Valve: The Lab Renderer for Unity - 逍遥剑客 - CSDN博客
2016年06月13日 23:53:07[xoyojank](https://me.csdn.net/xoyojank)阅读数：6185
# 来源
[https://www.assetstore.unity3d.com/en/#!/content/63141](https://www.assetstore.unity3d.com/en/#!/content/63141)
[https://steamcommunity.com/games/250820/announcements/detail/604985915045842668](https://steamcommunity.com/games/250820/announcements/detail/604985915045842668)
前几天Valve放出来他们在The Lab中基于Unity做的一些渲染优化相关的东西, 对于VR开发者来说还是非常有参考价值的. 
# 分析
## Single-Pass Forward Rendering and MSAA
正好前不久Oculus也开源了他们在[UE4基础上实现的Forward+渲染器](https://developer.oculus.com/blog/introducing-the-oculus-unreal-renderer/), 大体上能看出一个趋势, 那就是延迟渲染方式不是非常适合VR下的渲染. 我觉得可以从这几个角度去分析:
- 延迟渲染对于带宽的消耗非常大, 与VR渲染的高分辨率和高刷新率相矛盾
- 延迟渲染只能使用后期反锯齿技术, 而现阶段VR分辨率不足更加重了对反锯齿的需求, 前向渲染器可以使用硬件的MSAA, 相对于后期反锯齿可以保留更多的细节
- VR中很多后期效果不再需要, 引擎默认的渲染器中可以屏蔽掉这些效果, 结省资源从而提高效率
The Lab的渲染器支持18个动态光, 并且可以投影. 
## Adpative Quality
其实就是Dynamic Resolution Rendering, 对于PC平台还是有意义的, 因为需要兼容很多性能不一样的显卡. 在VR中帧率的重要性是大于分辨率的, 宁愿降画质也不能降帧数. 
不过SteamVR一直不肯加AsyncTimeWarp, 可能就是因为这个原因, 觉得可以用这个技术来保证流畅. 但是并不是所有的性能问题或者顿卡都是GPU瓶颈, 有时候也是CPU瓶颈, 所以我觉得AsyncTimeWarp的Reprojection技术还是非常有用的, 我们自己开发的一个CPU-Bound VRDemo在Oculus上明显比在Vive上体验要好. 
## Custom Shaders
说是渲染器, 但并没有包含Native代码, 只是一些Shader, 而且不能与Unity的Shader材质混用.  
![这里写图片描述](https://img-blog.csdn.net/20160613235501565)
- Renering Mode增加了Additive, 做特效的同学用得比较多
- Specular Mode可以选择None, 对没有高光的表面有性能提升
- Cube Map Scalar, 可以单独为每个物体调节环境贴图的强度
- Override Lightmap, 配合Component可以替换Lightmap
## GPU Flushing
这个本来不是应该Unity做的工作吗? 不知道有多大的性能提升
# 使用方法
- 在Player Settings中打开Forward Rendering
- 把Color Space改成Linear(Unity就是一直赖在Gamma空间)
- 把ValveCamera.cs加到主相机上
- 把ValveRealtimeLight.cs加到所有实时的灯光上
- 设置Shadow Cascades的分级成No Cascades
- 设置Pixel Light Count成99
其实一些设置在导入时就会自动提示的, 如图: 
![这里写图片描述](https://img-blog.csdn.net/20160613232807102)
# 其它
camera里的Shadow Texture比较有意思, 把多个灯的阴影打包到了一起 
![这里写图片描述](https://img-blog.csdn.net/20160613231935347)
Unity默认的雾效不再兼容, 需要使用ValveFog.cs, 支持距离雾和高度雾: 
![这里写图片描述](https://img-blog.csdn.net/20160613234333545)
# 总结
总的来说, 这不是一个完整的渲染器实现, 更像是一套自定义光照系统, 在前向渲染的基础上支持了足够多的实时灯光, 比较适用于PC平台的真实感材质. 
