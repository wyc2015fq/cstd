# iOS ARKit 教程：不触摸屏幕，用空气中的手势作画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月22日 13:47:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：348
[本文来自简书，原文地址:http://www.jianshu.com/p/4cbe6b6b8ea2](http://www.jianshu.com/p/4cbe6b6b8ea2)
本文翻译自 [iOS ARKit Tutorial: Drawing in the Air with Bare Fingers](https://www.toptal.com/swift/ios-arkit-tutorial-drawing-in-air-with-fingers)，原作者是 [Osama AbdelKarim AboulHassan](https://www.toptal.com/resume/osama-abdelkarim-aboulhassan)。
最近，Apple 发布了名为 [ARKit](https://developer.apple.com/arkit/) 的全新增强现实（AR）库。在许多人看来，这只是另一个好的 AR 库而已，而不是什么值得关注的革命性技术。但如果你了解过去几年 AR 的发展，就不会如此草率地下结论。
![](https://img-blog.csdn.net/20170822134547184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在本文中会用 iOS ARKit 创建一个好玩的项目。用户把手指放在桌子上，就好像握着一只笔，点击拇指甲就可以开始绘画。完成后，用户还可以把画作转成 3D 对象，就像下面的动图展示的那样。此项目的完整源码可以在
[GitHub](https://github.com/oabdelkarim/ARPaint) 上下载。
[动图](https://uploads.toptal.io/blog/image/123773/toptal-blog-image-1502228070465-88b307905525b4a9858582687a36bcc7.gif)
## 为何现在要关注 ARKit？
每个有经验的开发者应该都知道 AR 不是什么新概念了。AR 的第一次大规模开发要追溯到网络摄像头刚开始应用的时期。那时的 app 通常用于对脸做一些变化。然而，人们很快就发现把脸变成兔子并不是什么迫切的需求，很快这波势头就降下去了！
我相信 AR 一直以来都有两个关键技术没有实现，导致它没那么实用：可用性和沉浸性。如果你观察过其它有关 AR 的不实鼓吹，就会发现这两点。举个例子，当开发者可以访问手机摄像头的时候，就出现了一波对 AR 的鼓吹。除了强势回归的伟大的变兔子工具之外，还有一波 app 可以把 3D 对象放到打印的二维码上。但这个概念从来从来都没有火过。这并不是增强现实，只是增强的二维码而已。
然后 Google 用一次科技神话震惊了我们，Google Glass。两年过去，这个神奇的产品本应来到了我们的生活，但现实却是已经死掉了！许多批评家分析 Google Glass 失败的原因，归咎于从社会角度到 Google 发布产品时的无聊方式等等方面。但在本文中，我们只关心一个原因 —— 在环境中的沉浸性。虽然 Google Glass 解决了可用性问题，但它仍然只是在空气中绘制 2D 图像而已。
像微软、Facebook 和 Apple 这样的科技泰斗都从这次深刻的教训中吸取了经验。2017 年七月，Apple 发布了美妙的 iOS ARKit 库，制造沉浸性成为了它的优先任务。需要举着手机使用对用户体验仍然有很大的影响，但 Google Glass 的教训告诉我们，硬件不是问题。
我相信很快就要进入一波新的 AR 热潮，并在在这个关键节点上，它可能会最终找到的合适的市场。历史课就上到这里，下面开始写代码，实际了解 Apple 的增强现实！
## ARKit 的沉浸功能
ARKit 提供了两个主要功能；第一个是 3D 空间里的相机位置，第二个是水平面检测。前者的意思是，ARKit 假定用户的手机是在真实的 3D 空间里移动的摄像机，所以在任意位置放置 3D 虚拟对象都会锚定在真实 3D 空间中对应的点上。对于后者来说，ARKit 可以检测诸如桌子这样的水平面，然后就可以在上面放置对象。
那么 ARKit 是怎么做到的呢？这是一项叫做视觉惯性里程计（VIO）的技术。不要担心，就像创业者乐于人们发现他们的创业公司名称背后的秘密一样，研究人员也会乐于人们破译他们命名的发明中的所有术语——所以让他们开心吧，我们继续往前看。
VIO 这项技术融合了摄像头帧画面和运动传感器来追踪设备在 3D 空间里的位置。从摄像头帧画面中追踪运动是通过检测特征点实现的，也可以说是高对比度图像中的边缘点——就像蓝色花瓶和白色桌子之间的边缘。通过检测两帧画面间特征点的相对移动距离，就可以估算出设备在 3D 空间里的位置。所以如果用户面对一面缺少特征点的白墙，或者设备移动过快导致画面模糊，ARKit 都会无法正常工作。
## 上手 iOS 中的 ARKit
写作本文时，ARKit 是 iOS 11 的一部分，仍然在 beta 版本。所以要上手的话，你需要在 iPhone 6s 或更新的设备上下载 iOS 11 Beta，当然还有新的 Xcode Beta。我们可以用
**New > Project > Augmented Reality App** 来新建一个 ARKit 项目。但是我发现使用官方 [Apple ARKit 示例](https://developer.apple.com/sample-code/wwdc/2017/PlacingObjects.zip)开始会更方便，它提供了一些必要的代码块，尤其对于平面检测很有帮助。所以，从这个示例代码开始吧，我会首先解析里面的关键点，然后将其修改为我们自己的项目。
首先，我们要确定使用哪个引擎。ARKit 可用于 Sprite SceneKit 或 Meta。在 Apple ARKit 示例里，我们是用的是 iOS SceneKit，由 Apple 提供的 3D 引擎。接下来，我们需要设置用于渲染 3D 对象的视图。添加一个
`ARSCNView` 类型的视图即可。
`ARSCNView` 是 SceneKit 主视图 `SCNView` 的子类，但它扩展了一些有用的功能。它会将设备摄像头的实时视频流渲染为场景背景，并会自动匹配 SceneKit 空间和真实世界，假定设备是这个世界里的移动 camera。
`ARSCNView` 本身不会做 AR 处理，但它需要 AR session 对象来管理设备摄像头和运动处理。所以，从赋值一个新的 session 开始：
```
self.session = ARSession()
sceneView.session = session
sceneView.delegate = self
setupFocusSquare()
```
上面的最后一行代码添加了一个视觉指示，让用户直观地了解平面检测状态。Focus Square 是示例代码提供的，而不是 ARKit 库，这也是我们用示例代码上手的重要原因之一。在示例代码里的 readme 文件里可以找到更多信息。下面这张图显示了映射在桌子上的 focus square：
![](https://img-blog.csdn.net/20170822134553122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下一步是启动 ARKit session。每次 view appears 时都要重启 session，因为停止追踪用户后，之前的 session 信息就没有价值了。所以，在 viewDidAppear 里启动 session：
```
override func viewDidAppear(_ animated: Bool) {
    let configuration = ARWorldTrackingSessionConfiguration()
    configuration.planeDetection = .horizontal
    session.run(configuration, options: [.resetTracking, .removeExistingAnchors])
}
```
在上面的代码里，设置了 ARKit session configuration 来检测平面。写作本文时，Apple 没有提供除此以外的选项。但很明显，这暗示了未来可以检测到更复杂的对象。然后，开始运行 session 并确保重置了追踪。
最后，我们需要在摄像头位置（即实际的设备角度和位置）改变时更新 Focus Square。可以在 SCNView 的 renderer delegate 函数里实现，每次 3D 引擎将要渲染新的帧时都会调用：
```
func renderer(_ renderer: SCNSceneRenderer, updateAtTime time: TimeInterval) {
    updateFocusSquare()
}
```
此时运行 app，就可以看见摄像头视频流中位于检测到的水平面上的 focus square 了。在下一个部分，我们解释平面是如何被检测到的，以及如何对应放置 focus square。
## 平面检测
ARKit 可以检测新平面，更新现有平面，或是移除它们。为了便于处理平面，我们会创建一些虚拟的 SceneKit node 来管理平面的位置信息以及对 focus square 的引用。平面是定义在 X 和 Z 方向上的，Y 则是表面的法线，也就是说，如果想在平面上绘制一个 node 的话，应保持该 node 的 Y 值与平面相同。
平面检测是通过 ARKit 提供的回调函数来完成的。举个例子，下面的回调函数会在每次检测到新平面时调用：
```
var planes = [ARPlaneAnchor: Plane]()
func renderer(_ renderer: SCNSceneRenderer, didAdd node: SCNNode, for anchor: ARAnchor) {
    if let planeAnchor = anchor as? ARPlaneAnchor {
        serialQueue.async {
            self.addPlane(node: node, anchor: planeAnchor)
            self.virtualObjectManager.checkIfObjectShouldMoveOntoPlane(anchor: planeAnchor, planeAnchorNode: node)
        }
    }
}
    
func addPlane(node: SCNNode, anchor: ARPlaneAnchor) {
    let plane = Plane(anchor)
    planes[anchor] = plane
    node.addChildNode(plane)
}
...
class Plane: SCNNode {
    
    var anchor: ARPlaneAnchor
    var focusSquare: FocusSquare?
    
    init(_ anchor: ARPlaneAnchor) {
        self.anchor = anchor
        super.init()
    }
    ...
}
```
回调函数给我们提供了两个参数，`anchor` 和 `node`。`node` 是一个普通的 SceneKit node，角度和位置与平面完全相同。它没有几何体，所以是可不见的。我们用它来添加自己的平面 node，同样也是不可见的，但会管理
`anchor` 里有关平面角度和位置的信息。
所以位置和角度是如何存储在 `ARPlaneAnchor` 中的呢？位置、角度和比例都被编码在 4x4 矩阵中。如果我可以让你学会一个数学概念的话，毫无疑问就是矩阵了。不过没关系，可以把 4x4 矩阵想象为：一个包含 4x4 浮点数字的 2D 智能 2D 数组。用某种特定的方式将这些数字乘以它在局部空间中的 3D 顶点 v1 就会得到新的 3D 顶点 v2，即 v1 在世界空间中的表示。所以，如果局部空间里的 v1 = (1, 0, 0)，并且希望把它放在世界空间中 x = 100
 的位置，相对于世界空间的 v2 就会等于 (101, 0, 0)。当然，如果还要添加绕轴旋转，背后的数学就会变得更加复杂，但好消息是我们没必要理解这背后的原理（我强烈建议看看[这篇文章](https://www.toptal.com/javascript/3d-graphics-a-webgl-tutorial#performing-spatial-transformations)中的相关部分，里面有关于此概念的深入解释）。
`checkIfObjectShouldMoveOntoPlane` 会检查是否已经绘制了对象，以及有没有对象的 y 坐标匹配新检测到的平面。
现在，回到上一部分描述的 `updateFocusSquare()`。我们想要保证 focus square 在屏幕中心，并映射到检测到的距离最近的平面上。使用如下代码实现：
```
func updateFocusSquare() {
    let worldPos = worldPositionFromScreenPosition(screenCenter, self.sceneView)
    self.focusSquare?.simdPosition = worldPos
}
func worldPositionFromScreenPosition(_ position: CGPoint, in sceneView: ARSCNView) -> float3? {
    let planeHitTestResults = sceneView.hitTest(position, types: .existingPlaneUsingExtent)
    if let result = planeHitTestResults.first {
        return result.worldTransform.translation
    }
    return nil
}
```
`sceneView.hitTest` 会搜索对应屏幕上的 2D 点的真实世界平面，方式是映射这个 2D 点到下方最近的平面上。`result.worldTransform` 是一个 4x4 矩阵，具有检测到的平面的所有 transform 信息，而
`result.worldTransform.translation` 则用于只取出位置。
现在我们已经具备所需的全部信息，以便根据屏幕上的 2D 点向检测到的平面上放置 3D 对象。所以下面开始绘制吧。
## 绘图
首先解释一下如何利用计算机视觉跟随人的手指来绘制图形。绘制是通过检测手指移动的每个位置完成的，在对应的位置放置一个顶点，并将每个顶点与前面的顶点相连。顶点可以通过一条简单的线连接，如果需要平滑的输出的话，则可以通过 Bezier 曲线完成。
为了简单起见，我们会使用一些原生的绘图方法。对于手指的新位置，我们会在被检测到的平面上放置一个非常小的圆角 box，高度几乎为零。看起来就像一个点一样。用户完成绘制并点击 3D 按钮后，则会根据用户手指的移动改变放置对象的高度。
下面的代码展示了用于表示点的 `PointNode` 类：
```
let POINT_SIZE = CGFloat(0.003)
let POINT_HEIGHT = CGFloat(0.00001)
class PointNode: SCNNode {
    
    static var boxGeo: SCNBox?
    
    override init() {
        super.init()
        
        if PointNode.boxGeo == nil {
            PointNode.boxGeo = SCNBox(width: POINT_SIZE, height: POINT_HEIGHT, length: POINT_SIZE, chamferRadius: 0.001)
            
            // 设置点的材质
            let material = PointNode.boxGeo!.firstMaterial
            material?.lightingModel = SCNMaterial.LightingModel.blinn
            material?.diffuse.contents  = UIImage(named: "wood-diffuse.jpg")
            material?.normal.contents   = UIImage(named: "wood-normal.png")
            material?.specular.contents = UIImage(named: "wood-specular.jpg")
        }
        
        let object = SCNNode(geometry: PointNode.boxGeo!)
        object.transform = SCNMatrix4MakeTranslation(0.0, Float(POINT_HEIGHT) / 2.0, 0.0)
        
        self.addChildNode(object)
        
    }
    
    . . .
```
在上面的代码把几何体沿 y 轴移动了高度的一半。这样做是为了确保对象的底部总是处于 y = 0 的位置，这样看起来就像在平面上一样。
下面，在 SceneKit 的 renderer 回调函数中，使用 `PointNode` 类绘制一个指示来表示笔尖。如果开启了绘图的话，就会在那个位置放一个点下去，如果开启的是 3D 模式，则会将绘图抬高，变成 3D 结构体：
```
func renderer(_ renderer: SCNSceneRenderer, updateAtTime time: TimeInterval) {
    updateFocusSquare()
    // 设置表示虚拟笔尖的点
    if (self.virtualPenTip == nil) {
        self.virtualPenTip = PointNode(color: UIColor.red)
        self.sceneView.scene.rootNode.addChildNode(self.virtualPenTip!)
    }
    // 绘图
    if let screenCenterInWorld = worldPositionFromScreenPosition(self.screenCenter, self.sceneView) {
            
        // 更新虚拟笔尖位置
        self.virtualPenTip?.isHidden = false
        self.virtualPenTip?.simdPosition = screenCenterInWorld
        // 绘制新的点
        if (self.inDrawMode && !self.virtualObjectManager.pointNodeExistAt(pos: screenCenterInWorld)){
            let newPoint = PointNode()
            self.sceneView.scene.rootNode.addChildNode(newPoint)
            self.virtualObjectManager.loadVirtualObject(newPoint, to: screenCenterInWorld)
        }
            
        // 将绘图转为 3D
        if (self.in3DMode ) {
            if self.trackImageInitialOrigin != nil {
                DispatchQueue.main.async {
                    let newH = 0.4 *  (self.trackImageInitialOrigin!.y - screenCenterInWorld.y) / self.sceneView.frame.height
                    self.virtualObjectManager.setNewHeight(newHeight: newH)
                }
            }
            else {
                self.trackImageInitialOrigin = screenCenterInWorld
            }
        }
            
    }
```
## 检测用户指尖
Apple 在 iOS 11 发布的另一个牛逼闪闪的库是 Vision 框架。它以一种相当方便和有效的方式提供可一些计算机视觉技术。我们会使用其中的对象追踪技术。对象追踪的工作原理如下：首先需要提供一张图像，以及图像中被追踪的对象的正方形边界坐标。然后调用几个函数来初始化追踪。最后，为其提供一个新的图像以及之前操作获得的分析结果，在新图像里该对象的位置发生了改变。如果我们给定了这些信息，它就会返回对象的新位置。
下面采用一种巧妙的方式。让用户把手放在桌上，就像在握着一支笔，然后确保指甲盖面向摄像头，然后点击屏幕上的指甲盖。这里需要说明两点。第一，指甲盖应该具有足够的独特性，以便在白色指甲盖、皮肤和桌子之间实现追踪。也就是说深色皮肤会让追踪更加可靠。第二，因为用户是把手放在桌上的，再加上我们已经检测到了桌子的平面，所以将指甲盖的位置从 2D 视图映射到 3D 环境中的话，位置就会和手指在桌子上的位置极为接近。
下面这张图显示了 Vision 库检测到的特征点：
![](https://img-blog.csdn.net/20170822134600548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后用一个触摸手势来初始化指甲盖追踪：
```
// MARK: 对象追踪
    
fileprivate var lastObservation: VNDetectedObjectObservation?
var trackImageBoundingBox: CGRect?
let trackImageSize = CGFloat(20)
    
@objc private func tapAction(recognizer: UITapGestureRecognizer) {
        
    lastObservation = nil
    let tapLocation = recognizer.location(in: view)
        
    // 用视图坐标空间设置 image 中的 rect 以便用于追踪
    let trackImageBoundingBoxOrigin = CGPoint(x: tapLocation.x - trackImageSize / 2, y: tapLocation.y - trackImageSize / 2)
    trackImageBoundingBox = CGRect(origin: trackImageBoundingBoxOrigin, size: CGSize(width: trackImageSize, height: trackImageSize))
        
    let t = CGAffineTransform(scaleX: 1.0 / self.view.frame.size.width, y: 1.0 / self.view.frame.size.height)
    let normalizedTrackImageBoundingBox = trackImageBoundingBox!.applying(t)
        
    // 将 rect 从视图坐标控件转换为图片空间
    guard let fromViewToCameraImageTransform = self.sceneView.session.currentFrame?.displayTransform(withViewportSize: self.sceneView.frame.size, orientation: UIInterfaceOrientation.portrait).inverted() else {
        return
    }
    var trackImageBoundingBoxInImage =  normalizedTrackImageBoundingBox.applying(fromViewToCameraImageTransform)
    trackImageBoundingBoxInImage.origin.y = 1 - trackImageBoundingBoxInImage.origin.y   // Image space uses bottom left as origin while view space uses top left
        
    lastObservation = VNDetectedObjectObservation(boundingBox: trackImageBoundingBoxInImage)
        
}
```
上面最棘手的部分就是如何把点击位置从 UIView 坐标控件转换到图片坐标空间。ARKit 只为我们提供了从图像坐标空间转换为 viewport 坐标控件的 displayTransform 矩阵。所以如何实现相反的操作呢？只要使用逆矩阵即可。我在这篇文章里已经尝试尽量少用数学，但在 3D 世界里有时就是难以避免。
下面。在 renderer 中提供一个新图像来追踪手指的新位置：
```
func renderer(_ renderer: SCNSceneRenderer, updateAtTime time: TimeInterval) {
    // 追踪指甲盖
    guard let pixelBuffer = self.sceneView.session.currentFrame?.capturedImage,
        let observation = self.lastObservation else {
             return
    }
    let request = VNTrackObjectRequest(detectedObjectObservation: observation) { [unowned self] request, error in
        self.handle(request, error: error)
    }
    request.trackingLevel = .accurate
    do {
        try self.handler.perform([request], on: pixelBuffer)
    }
    catch {
        print(error)
    }
    . . .
}
```
对象追踪完成后，会调用一个回调函数，用它来更新指甲盖的位置。基本就是上面在触摸手势里相反的代码：
```
fileprivate func handle(_ request: VNRequest, error: Error?) {
    DispatchQueue.main.async {
        guard let newObservation = request.results?.first as? VNDetectedObjectObservation else {
            return
        }
        self.lastObservation = newObservation
                        
        var trackImageBoundingBoxInImage = newObservation.boundingBox
            
        // 从图像空间转换到视图空间
        trackImageBoundingBoxInImage.origin.y = 1 - trackImageBoundingBoxInImage.origin.y
        guard let fromCameraImageToViewTransform = self.sceneView.session.currentFrame?.displayTransform(withViewportSize: self.sceneView.frame.size, orientation: UIInterfaceOrientation.portrait) else {
            return
        }
        let normalizedTrackImageBoundingBox = trackImageBoundingBoxInImage.applying(fromCameraImageToViewTransform)
        let t = CGAffineTransform(scaleX: self.view.frame.size.width, y: self.view.frame.size.height)
        let unnormalizedTrackImageBoundingBox = normalizedTrackImageBoundingBox.applying(t)
        self.trackImageBoundingBox = unnormalizedTrackImageBoundingBox
            
        // 获取追踪的图像在图像空间的位置在距离最近的检测到的平面上的映射
        if let trackImageOrigin = self.trackImageBoundingBox?.origin {
            self.lastFingerWorldPos = self.virtualObjectManager.worldPositionFromScreenPosition(CGPoint(x: trackImageOrigin.x - 20.0, y: trackImageOrigin.y + 40.0), in: self.sceneView)
        }            
    }
}
```
最后，绘图时使用 `self.lastFingerWorldPos` 而不是屏幕中心，这样就全部结束了。
## 谈一谈 ARKit 和未来
在这篇文章里，我们感受到了 AR 如何通过与用户的手指和现实生活中的桌子交互来实现沉浸式体验。随着计算机视觉的发展，以及新增加的对 AR 友好的硬件（如深度摄像头），我们可以就可以更多地获取身边对象的 3D 结构。
尽管微软的 Hololens 设备还没有向大众发布，但微软已经决心要赢得这场 AR 竞赛，这个设备组合了 AR 定制的硬件并带有高级 3D 环境识别技术。你可以静静看着谁会赢得这场比赛，也可以现在就加入开发沉浸式
[AR app](https://www.toptal.com/ios) 的大军！但是一定要做点对人类有意义的事，而不是把我们变成兔子。
## 附录
### Apple 的 ARKit 为开发者提供了哪些功能？
ARKit 可以让开发者在 iPhone 和 iPad 上构建沉浸式增强现实 app，通过分析摄像头视图展示的场景并找出房间里的水平面。
### 如何用 Apple 的 Vision 库来追踪对象？
Apple 的 Vision 库可以让开发者追踪视频流中的对象。开发者提供初始图像帧中待追踪对象的矩形坐标，然后提供视频帧，这个库就会返回该对象的最新位置。
### 如何上手 Apple 的 ARKit？
要上手 Apple 的 ARKit，在 iPhone 6s 或更高的设备上下载 iOS 11 并用 New > Project > Augmented Reality App 创建一个新的 ARKit 项目。同时也可以看看苹果在这里提供的 AR 示例代码：[https://developer.apple.com/arkit/](https://developer.apple.com/arkit/)
作者：张嘉夫
链接：http://www.jianshu.com/p/4cbe6b6b8ea2
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
