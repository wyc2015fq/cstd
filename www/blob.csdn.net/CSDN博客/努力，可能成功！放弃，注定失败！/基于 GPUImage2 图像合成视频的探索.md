# 基于 GPUImage2 图像合成视频的探索 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月23日 13:20:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1571
> 
转载请注明出处：[http://posts.enumsblog.com/posts/17014](http://posts.enumsblog.com/posts/17014)
如果还有不知道[「GPUImage」](https://github.com/BradLarson/GPUImage)的请自觉面壁。
它被用 Swift 改写后，就变成了[「GPUImage2」](https://github.com/BradLarson/GPUImage2)。
# 引言
最近业余时间在开发一款视频相关的 App，原本打算基于自己开源的计算机视觉库[「EMCVLib」](https://github.com/trmbhs/EMCVLib)做，功能上基于`OpenCV`已做好了封装，但现在已经全部撤掉了。原因有两点：
- OpenCV 的 bug 非常多。笔者通过修改源码已经解决了一些问题了，但仍然没法爬出这个坑。
- C++ 层异常无法捕获。OpenCV 拥有丰富的函数，但在输入不合法时，会直接崩溃，这是无法容忍的。
再之，笔者用 Swift 开发，使用 OpenCV 也十分不方便，最终不得不找其他方法，最终找到了`GPUImage2`。
# 原理
先来简单介绍一下`GPUImage2`的工作流程，来一段伪代码：
```
var input: SomeInput
var filter: SomeFilter
var output: SomeOutput
input --> filter --> output
```
它通过重载了`-->`操作符，将输入、过滤器和输出串在一起。输入可以是图像、相机或视频。过滤器是可选的，可以是滤镜。输出可以是视图或者是视频文件等。这三者用`-->`符号串在一起，就是它的工作流程了。
# 任务
现在任务来了，有 100 张大小一致的图片，要将这些图片合成一个视频怎么办呢？
根据上面提到的工作流程，由于过滤器是可选的，这里并不需要过滤器，因此确定输入和输出的组件就可以了。
显而易见，这里的输入是`PictureInput`，输出是`MovieOutput`，然后直接
```
pictureInput1
 --> movieOutput
```
就好了。所以很快就能写出像下面这样的代码：
```
func imageToMovie(imagePaths: [String], outputURL: URL, size: Size) {
    let output = try! MovieOutput.init(URL: outputURL, size: size, liveVideo: true)
    output.startRecording()
    for path in imagePaths {
        autoreleasepool(invoking: {
            let img = UIImage.init(contentsOfFile: path)!
            let input = PictureInput.init(image: img)
            input --> output
            input.processImage()
            input.removeAllTargets()
        }
    }
    output.finishRecording()
}
```
是不是很简单呢？咦，为什么输出的文件大小是 0 kB 。等待，好像少了点什么。
![](http://upload-images.jianshu.io/upload_images/538901-498cb76c96891704.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
17014_1.jpg
# 探索
100 张图片生成视频，好像忘记指定帧率了，这个要在哪里设置呢？完全没有地方可以设置啊。因此这里涉及到一个问题，视频是凭空产生的，每一帧的时间戳如何处理。
先来看一下接口吧，哇，`GPUImage2`真是简陋呢，`MovieOutput`的接口只有这么几个：
```
public class MovieOutput : ImageConsumer, AudioEncodingTarget {
    public let sources: GPUImage.SourceContainer
    public let maximumInputs: UInt
    public init(URL: URL, size: GPUImage.Size, fileType: String = default, liveVideo: Bool = default, settings: [String : AnyObject]? = default) throws
    public func startRecording()
    public func finishRecording(_ completionCallback: (() -> Swift.Void)? = default)
    public func newFramebufferAvailable(_ framebuffer: GPUImage.Framebuffer, fromSourceIndex: UInt)
    public func activateAudioTrack()
    public func processAudioBuffer(_ sampleBuffer: CMSampleBuffer)
}
```
嗯，真的没有设置帧率的地方，找源码吧。最终在这个方法中找到了想要的东西：
```
public func newFramebufferAvailable(_ framebuffer:Framebuffer, fromSourceIndex:UInt) {
    defer {
        framebuffer.unlock()
    }
    guard isRecording else { return }
    // Ignore still images and other non-video updates (do I still need this?)
    guard let frameTime = framebuffer.timingStyle.timestamp?.asCMTime else { return }
    // If two consecutive times with the same value are added to the movie, it aborts recording, so I bail on that case
    guard (frameTime != previousFrameTime) else { return }
    if (startTime == nil) {
        if (assetWriter.status != .writing) {
            assetWriter.startWriting()
        }
        assetWriter.startSession(atSourceTime: frameTime)
        startTime = frameTime
    }
    ......
}
```
为什么之前输出的文件是 0 kB 呢，因为在这里的守护代码中，由于时间戳是空，因此就直接返回了，所以根本没有输出任何内容。那么这个时间戳是哪里来的呢？继续追进去。
```
public enum FramebufferTimingStyle {
    case stillImage
    case videoFrame(timestamp:Timestamp)
    func isTransient() -> Bool {
        switch self {
            case .stillImage: return false
            case .videoFrame: return true
        }
    }
    var timestamp:Timestamp? {
        get {
            switch self {
                case .stillImage: return nil
                case let .videoFrame(timestamp): return timestamp
            }
        }
    }
}
```
嗯，很好，一个枚举。由于是图片输入，因此这个枚举应该是`stillImage`选项，没有时间戳。如果想让代码正常工作，要在这里加上时间戳才行。
接着看，时间戳是在一个被传进来的`FrameBuffer`对象里的，这玩意又是哪来的？
看了两眼构造方法，跟 OpenGL 上下文有关，初步推测是对 OpenGL 中的 FrameBuffer 的一个封装。
![](http://upload-images.jianshu.io/upload_images/538901-d4119749a71a8bb1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
17014_2.jpg
FrameBuffer 在 OpenGL 中的概念可以简单理解为一块画布，这东西是要占一大块显存的可不是谁都能有的。果不其然，在输入的`PictureInput`中找到了。
```
public class PictureInput: ImageSource {
    public let targets = TargetContainer()
    var imageFramebuffer:Framebuffer!
    var hasProcessedImage:Bool = false
    ......
}
```
一个输入图片对应一个 FrameBuffer，说得过去。现在已经基本确定要改的属性了。至于这个属性不是 public 的，直接把源码改了。
那么现在已经找到要改的属性了，那么该怎么改呢？继续看源码，发现时间戳是个 struct：
```
public struct Timestamp : Comparable {
    public init(value: Int64, timescale: Int32, flags: GPUImage.TimestampFlags, epoch: Int64)
}
```
四个莫名其妙的参数，这怎么写？
![](http://upload-images.jianshu.io/upload_images/538901-f8db9a348f2f9b52.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
17003_6.jpg
经过修改官方例子，把视频输入和视频输出接在一起，以观察时间戳结构最终得出了时间戳结构，下面是修改过的例子代码，可直接在 macOS 上运行：
```
import Cocoa
import GPUImage
@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    @IBOutlet weak var window: NSWindow!
    @IBOutlet weak var renderView: RenderView!
    var movie:MovieInput!
    var output: MovieOutput!
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        let movieURL = URL(fileURLWithPath:"输入视频路径")!
        do {
            movie = try MovieInput(url:movieURL, playAtActualSpeed:true)
            output = try MovieOutput.init(URL: URL.init(fileURLWithPath: "输出视频路径"), size: 视频长宽)
            output.startRecording()
            movie --> output
            movie.runBenchmark = true
            movie.start()
        } catch {
            print("Couldn't process movie with error: \(error)")
        }
    }
}
```
至于最终的构造方法，有兴趣的读者可以自己跑一下代码输出时间戳看看，具体在这里就不给出了。在加入了时间戳后，由图片合成视频的函数已经可以正常工作了。
```
func imageToMovie(imagePaths: [String], outputURL: URL, size: Size) {
    let output = try! MovieOutput.init(URL: outputURL, size: size, liveVideo: true)
    output.startRecording()
    for path in imagePaths {
        autoreleasepool(invoking: {
            let img = UIImage.init(contentsOfFile: path)!
            let timestamp = Timestamp.init(...)
            input.imageFramebuffer.timingStyle = FramebufferTimingStyle.videoFrame(timestamp: timestamp)
            let input = PictureInput.init(image: img)
            input --> output
            input.processImage()
            input.removeAllTargets()
        }
    }
    output.finishRecording()
}
```
# 结语
本文通过简单修改源码完成了将多张图片合成视频的功能，主要提供了一个寻找问题的思路。GPUImage 是款非常著名的图像处理库，希望自己能多多吸收这些优秀框架处理问题的方式。
