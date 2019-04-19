# iOS 10 SceneKit 新特性 - SceneKit 制作 3D 场景框架 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月31日 16:57:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2388
开头语：
今天早上很开心，掘金的微博转发我第一个[blog 服务端Swift - Vapor （一）](http://www.jianshu.com/p/3fc28570d951)，让正在愁期末考试的我一下心花怒放。正好掘金赐给我一个内测功能（原创文章编写），于是今天打算撸一篇，希望长辈指导。
今天的主题是探索**iOS10 SceneKit**的新功能，你可以观看今年WWDC的视频[Advances in SceneKit Rendering](https://developer.apple.com/videos/play/wwdc2016/609/)
[SceneKit](https://developer.apple.com/scenekit/) 介绍，SceneKit类似于Unity是一个制作3D物件的框架，很多3D游戏都可以基于这个框架进行开发，自几年前和Swift一起推出到现在广受欢迎。比如今年推出的[Swift
 Playground](http://www.apple.com/swift/playgrounds/)就有用到SceneKit开发一个3D游戏帮助小盆友学习coding。
![](http://upload-images.jianshu.io/upload_images/2751003-dc9da93e9da0ff8f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift Playground
你可以看这里对SceneKit有点基础的了解[Objc-SceneKit介绍](https://www.objccn.io/issue-18-3/)
今年的SceneKit已经支持iWatch了，大东目前没iWatch，不过觉得屏幕太小了，最多就是几个3D的小部件吧。
# Physically based rendering
今年最大的更新是提供了Physically based rendering（PBR），不懂可以上网查，简单的说就是让3D的物品更加3D了，我后面重点就是谈谈这个新的功能。
> 
What's new in iOS 10 SceneKit: A new Physically Based Rendering (PBR) system allows you to leverage the latest in 3D graphics research to create more realistic results with simpler asset authoring
官方文档谈到的
> 
PBR materials require only three fundamental properties— **diffuse**, metalness, and roughness—to produce a wide range of realistic shading effects.
相比之前，现在映射一个3D物体变得很容易，只要实现这3个变量 diffuse metalness roughness 即可。我们做一个小Demo更加帮助理解。
首先下载自己喜欢的物品材质，所谓的材质可以理解一个为意见物品的“皮肤”，你可以选择喜欢的贴图来实现不同的效果。[下载网址](http://freepbr.com/materials/scuffed-copper-pbr-metal-material/)
```
```
let materialPrefixes : [String] = ["Copper-scuffed"]
let material = sphereNode.geometry?.firstMaterial
// Declare that you intend to work in PBR shading mode
// Note that this requires iOS 10 and up
material?.lightingModel = SCNMaterial.LightingModel.physicallyBased
// Setup the material maps for your object
let materialFilePrefix = materialPrefixes[0]
material?.diffuse.contents = UIImage(named: "\(materialFilePrefix)-albedo.png")
material?.roughness.contents = UIImage(named: "\(materialFilePrefix)-roughness.png")
material?.metalness.contents = UIImage(named: "\(materialFilePrefix)-metal.png")
```
```
这里就实现了物品材质的选择，但是球的背景图是怎么实现的，怎么会有倒摄的效果呢？
其实实现也很简单，假设你把一个物体的LightingModel定位为PBR，只要提供一张全景照片就能够将照片隐射到上面去
```
let env = UIImage(named: "interior_hdri_29_20150416_1169368110.jpg")
scene.lightingEnvironment.contents = env
```
注意，这里的全景照片指的不是手机拍的全景照片而是高质量，专业效果拍摄出来的图片，如果照片不合格，就会出现错误，如下示例图。
![](http://upload-images.jianshu.io/upload_images/2751003-99e020212b857cf0.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图片源于网络
当然还要有后面背景图
```bash
let bg = UIImage(named: "interior_hdri_29_20150416_1169368110.jpg")
scene.background.contents = bg
```
注意观察到它们是同一张图
最后就会呈现如下的效果图，我用Swift Playground可使，真是太强大了。我还加入了Core Motion，可以移动iPad来切换不同的视角，这同时也是VR View的制作方式呀，以后更新如何开发一个属于iOS 的VR应用，并连接SceneKit，感觉以后发展可观。
![](http://upload-images.jianshu.io/upload_images/2751003-8b7b9c11596d3913.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
当然你还可以新建一个.scn文件来制作
- 先拖一个SphereNode
![](http://upload-images.jianshu.io/upload_images/2751003-0c934a5b3e54a487.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 更改Material的属性
![](http://upload-images.jianshu.io/upload_images/2751003-3a7a5d8a5c572d1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 更改Lighting Environment 和 Background 属性
![](http://upload-images.jianshu.io/upload_images/2751003-522c1d8948a5f695.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
可以自己尝试不同的3D物件，在Object-library上拖拽到.scn文件中即可
# Physically Based Lights
> 
Authors of PBR scene content often prefer working in physically based terms, so you can now define lighting using intensity (in lumens) and color temperature (in degrees Kelvin), and import specifications for real-world light fixtures using the IESProfileURL
 property.
![](http://upload-images.jianshu.io/upload_images/2751003-080cb54111912f4f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
今年对scnLight 的更新是新增一种灯源，就是可以在网上找到一种文件格式.ies，就可以导入不同格式的灯源。这对设计师来说应该是一个福利
```
//add a liteNode
let liteNode = SCNNode()
liteNode.light = SCNLight()
liteNode.light?.iesProfileURL = URL(fileReferenceLiteralResourceName: "LF6N_1_42TRT_F6LS73.ies")
liteNode.light?.type = .IES
```
# HDR Camera
> 
Add even more realism with the new HDR features and effects in the SCNCamera class. With HDR rendering, SceneKit captures a much wider range of brightness and contrast in a scene, then allows you to customize the tone mapping that adapts that scene for the
 narrower range of a device’s display. Enable exposure adaptation to create automatic effects when, for example, the player in your game moves from a darkened area into sunlight. Or use vignetting, color fringing, and color grading to add a filmic look to your
 game.
![](http://upload-images.jianshu.io/upload_images/2751003-eda664cc179d8fbd.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我也做了一个Demo来尝试新的api效果，大东表示非常赞
