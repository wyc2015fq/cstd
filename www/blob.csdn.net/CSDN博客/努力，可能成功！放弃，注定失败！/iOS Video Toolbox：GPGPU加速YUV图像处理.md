# iOS Video Toolbox：GPGPU加速YUV图像处理 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月23日 15:55:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1090
本文档以YUV亮度减半为例，描述了OpenGL ES 3.0 Transform Feedback在Video Toolbox解码回调中进行YUV图像处理的具体实现，同时比较多种绘制模式之间的性能差异以及在合理的模式下使用图元重启（Primitive Restart，配合glDrawElements），对比图元重启对性能的影响，**目标是利用GPU并行计算能力，做通用计算（比如浮点计算），减轻非游戏及GPU密集型应用的CPU压力**，是文档[iOS
 GPGPU 编程：Transform Feedback实现图像对比度调整](http://www.jianshu.com/p/dcb6543745d5)和[iOS Video Toolbox：读写解码回调函数CVImageBufferRef的YUV图像](http://www.jianshu.com/p/dac9857b34d0)的拓展。
**目录：**
|- 同步解码与CPU实现YUV亮度减半
|- 同步解码与GPU逐点绘制实现YUV亮度减半
|-- Transform Feedback逐点计算
|-- 逐点绘制的性能分析与优化提示
|--- （逐点绘制）CPU等待GPU完成（气泡效应）
|---未初始化颜色附着
|--- 未初始化纹理数据
|--- 逻辑缓冲区存储
|--- 逻辑缓冲区加载
|--- 冗余调用
|- 同步解码与GPU逐行绘制实现YUV亮度减半
|-- 逐行绘制的性能分析
除非特别标注，此文档数据皆来自iPad Air 2（iOS 9.3.4）真机测试，测试视频数据为1920×1080@24FPS、时长181.44秒。
## 1、同步解码与CPU实现YUV亮度减半
为比较GPU性能，先给出CPU同步解码且在解码回调函数中实现YUV亮度减半操作，朴素实现如代码段1所示，**代码中的除2操作只是出于演示目的，换成移位操作按理会对性能有所改善，但是，如果除以非2倍数则无法使用移位操作了，感谢@Bepartofyou指出此示例的不适当问题。我原本是想表现CPU做浮点操作与GPU的性能差异，只是恰巧Y通道是8位整数值。**
```cpp
```cpp
// 代码段1-1：同步解码
VTDecodeFrameFlags decodeFlags = 0;
VTDecodeInfoFlags outFlags = 0;
VTDecompressionSessionDecodeFrame(decompressionSession, 
    sampleBufferRef, 
    decodeFlags, 
    NULL, 
    &outFlags);
// 代码段1-2：解码回调
void decoderCallback(void *decompressionOutputRefCon,
                     void *sourceFrameRefCon,
                     OSStatus status,
                     VTDecodeInfoFlags infoFlags,
                     CVImageBufferRef imageBuffer,
                     CMTime presentationTimeStamp,
                     CMTime presentationDuration ) {
    if (noErr != status) {
        return;
    }
    if (!imageBuffer) {
        return;
    }
    CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    void *lumaPlane = CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
    int width = CVPixelBufferGetWidthOfPlane(imageBuffer, 0);
    int height = CVPixelBufferGetHeightOfPlane(imageBuffer, 0);
    for (int line = 0; line < height; ++line) {
        for (int col = 0; col < width; ++col) {
            currentLumaPlane[col] /= 2;
        }
        currentLumaPlane += width;
    }
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
}
```
```
在iPhone 5（iOS 9.2）上运行了2055.8秒，平均每帧消耗0.47秒。
![](http://upload-images.jianshu.io/upload_images/1613657-5563ba9326abf480.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPhone 5（iOS 9.2）CPU做Y通道亮度减半的资源消耗
在iPhone 6p（iOS 9.3.5）上运行了95.7秒，平均每帧消耗0.02秒，可见两年后的设备性能确实是大幅提升。
![](http://upload-images.jianshu.io/upload_images/1613657-0374a5c902107bbf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPhone 6p（iOS 9.3.5）CPU做Y通道亮度减半的资源消耗
iPad Air 2与iPhone 6p表现接近。
![](http://upload-images.jianshu.io/upload_images/1613657-800767942dca41f4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Ari 2（iOS 9.3.4）CPU做Y通道亮度减半的资源消耗
## 2、同步解码与GPU逐点绘制实现YUV亮度减半
因iOS平台限制，Transform Feedback必须使用GLKit，故解码回调函数的实现代码变得略微复杂。
### 2.1、Transform Feedback逐点计算
使用顶点反馈的具体表现是，在同步进行CPU和GPU处理时，每次创建纹理都需要占用主线程资源且为了让CPU及时获取GPU处理数据还强制了CPU等待GPU命令队列完成，失去了GPU的并行特性，造成汽泡效应，如代码段2所示。**当然，这种实现是非常不合理的，比如，纹理的创建可以在子线程实现。**
```cpp
```cpp
// 代码段2-1：通知主线程
dispatch_sync(dispatch_get_main_queue(), ^{
    // 通知主线程绘制
});
// 代码段2-2：Y通道创建纹理
- (void)generateTextureFromCVPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    [lock lock];
    isNewPixelBuffer = true;
    CVOpenGLESTextureCacheFlush(textureCache, 0);
    if (texture) {
        CFRelease(texture);
    }
    int imageWidth = (int)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    int imageHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
    CVReturn createResult = CVOpenGLESTextureCacheCreateTextureFromImage(NULL,
        textureCache,
        pixelBuffer,
        NULL,
        GL_TEXTURE_2D,
        GL_LUMINANCE,
        imageWidth, imageHeight,
        GL_LUMINANCE,
        GL_UNSIGNED_BYTE,
        0,
        &texture);
    if (kCVReturnSuccess != createResult) {
        NSLog(@"CVOpenGLESTextureCacheCreateTextureFromImage failed.");
    }
    glBindTexture(CVOpenGLESTextureGetTarget(texture), CVOpenGLESTextureGetName(texture));
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_WRAP_S, 
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_WRAP_T, 
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MAG_FILTER, 
        GL_NEAREST);
    [lock unlock];
}
// 代码段2-3：GPU处理Y通道数据
// 进行变换反馈，CPU等待GPU运行结束，获取GPU内存且进行跨距处理
GLuint *addr = glMapBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 
    0, 
    pointsCount * sizeof(GLuint), 
    GL_MAP_READ_BIT);
GLuint *pAddr = addr;
if (addr) {
    uint8_t *luma = calloc(pointsCount, sizeof(uint8_t)), *pLuma = luma;
    for (int line = 0; line < 1080; ++line) {
        for (int col = 0; col < 1920; ++col) {
            luma[0] = addr[0];
            ++luma;
            ++addr;
        }
    }
    luma = pLuma;
    addr = pAddr;
    // 处理luma数据
    free(luma);
}
glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
```
```
现在，真机运行查看性能表现。
![](http://upload-images.jianshu.io/upload_images/1613657-6a230a39f6fe0d05.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2使用顶点着色器1920x1080画点实现Y通道亮度减半的CPU资源消耗
![](http://upload-images.jianshu.io/upload_images/1613657-225b6bd6173dd8ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 CPU时间消耗测试概览
**可见，CPU压力几乎全在主线程，但负荷较低，其余核心几乎处于休眠状态，主线程多次出现尖峰可能由创建纹理的代码引起，后续文档会对此进行优化。**下面观察GPU的资源占用。
![](http://upload-images.jianshu.io/upload_images/1613657-7962ce97c1bec4d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 使用顶点着色器1920x1080画点实现Y通道亮度减半的GPU资源消耗
苹果使用PowerVR的图形处理器，而PowerVR处理顶点是基于Tiler（贴片）方式实现的，那么上图反应了我们代码确实只使用了顶点功能，因为PowerVR处理片段着色器对应于RENDER功能。至少，我们的初步目标实现了：在非游戏和图形密集计算的应用中，使用GPU计算、解放CPU压力。接下来，进行代码优化，确定此方式是否达到实际产品需求。
### 2.2、逐点绘制的性能分析与优化提示
在Instruments的帮助下，本节进行性能分析，当前实现方式存在不少问题，如下图所示。然而，由于当前我对OpenGL ES了解较少，暂未找到合适的优化办法。**由于这些现象在用画线、画三角形时同样存在，故不一一阐述，若有解决方案，则在此给出。**
![](http://upload-images.jianshu.io/upload_images/1613657-9c53dfc3b3ffd1f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 GPU绘点实现存在的问题
![](http://upload-images.jianshu.io/upload_images/1613657-1e3ea34293bd9633.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 GPU绘点性能分析（接口调用统计）
![](http://upload-images.jianshu.io/upload_images/1613657-e6707e40a870705c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 GPU 绘点性能分析（帧统计）
从帧统计上看，程序表现正常。
#### 2.2.1、（逐点绘制）CPU等待GPU完成（气泡效应）
*Your application called Finish, forcing the CPU to wait for the GPU to complete some work. Performance is maximized when the CPU and GPU work in parallel. Calling Finish is generally only needed for benchmarking and certain special cases in a multi-context scenario. It may be possible to remove this Finish call to improve performance.*
由于GPU处理完的数据为4字节，而Y通道的数据类型是单字节，映射GPU内存后需进行跨距内存拷贝，则此步难以避免。
#### 2.2.2、未初始化颜色附着
*Renderbuffer #1 - Your application presented a renderbuffer, but did not render content to all pixels in the renderbuffer. The contents of any pixels not rendered by your application are unspecified. To fix this, either clear the renderbuffer before rendering the frame or add drawing calls that render to the untouched pixels.*
本文档非常规使用Transform Feedback，最终并不向渲染缓冲区写入数据，而是映射回主存进行操作，故忽略此问题。
#### 2.2.3、未初始化纹理数据
`glEndTransformFeedback();`
结束变换反馈时，有如下优化提示。
*Texture #1 - Your application made a drawing call using a texture that contains uninitialized data. If a draw call uses uninitialized data, the rendering results are incorrect and unpredictable. One way to fix this issue is to provide image data to any TexImage2D calls instead of a NULL pointer.*
使用CVOpenGLESTextureCacheCreateTextureFromImage创建纹理时，因没显示调用glTexImage系列接口，做性能分析时总会有此提示。同样，暂且忽略。
#### 2.2.4、逻辑缓冲区存储
`glMapBufferRange`
*OpenGL ES performed a logical buffer store operation. This is typically caused by switching framebuffer or not discarding buffers at the end of the rendering loop. For best performance keep logical buffer store operations to a minimum amd discard the depth buffer at the end of each frame. See the EXT_discard_framebuffer extension for more details*
这里或许可以利用GLKit进行优化。
#### 2.2.5、逻辑缓冲区加载
*Your application caused OpenGL ES to perform a framebuffer load operation, where the framebuffer must be loaded by the GPU before rendering. This is typically caused by failing to perform a fullscreen clear operation at the beginning of each frame. If possible, you should do so to improve performance.*
同上。
#### 2.2.6、冗余调用
*This command was redundant:*
`glBindFramebuffer(GL_FRAMEBUFFER, 1u)`
*A GL function call that sets a piece of GL state to its current value has been detected. Minimize the number of these redundant state calls, since these calls are performing unnecessary work.*
![](http://upload-images.jianshu.io/upload_images/1613657-78cfa9e02c6c0aae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
冗余调用的栈桢信息
使用GLKViewController时，这是框架的调用，与用户无关，因此我们修改不了，这点从栈栈帧信息上也可验证。
既然，这些都做不了，那啥，你说个锤子。别捉鸡，我们改绘制方式还不行吗？来，喝完这杯，还有三杯。
## 3、同步解码与GPU逐行绘制实现YUV亮度减半
**使用上一节生成的1920x1080个点坐标，只修改绘制参数，从绘点GL_POINTS到画线GL_LINES后，帧率提高了一倍，达到每秒6帧，时间减半，不过，CPU占用略有提高，平均为9%。**从这一表现，可推断，将绘制方式改成三角形网格（Mesh），那么，帧率将继续提高。
由于点坐标没重新生成，意味着，(0, 0)(0, 1)连成线、(0, 2)(0, 3)连成线，而(0, 1)(0, 2)两点则未结成线，以此类推。然而，绘制的结果画面是完整的，我的理解是，参考退化三角形的概念，这种方式生成了『退化直线』，即长度0，因为在线绘制的场合，光栅化的结果中最小绘制单位是像素点，而此处两个点之间再无间隙可供插值或填充。
![](http://upload-images.jianshu.io/upload_images/1613657-a6d4a417a3b5a151.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 使用顶点着色器1920x1080画线实现Y通道亮度减半的GPU资源消耗
![](http://upload-images.jianshu.io/upload_images/1613657-1f35611813e579c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPad Air 2 使用顶点着色器1920x1080画线实现Y通道亮度减半的CPU资源消耗
文／熊皮皮（简书作者）
原文链接：http://www.jianshu.com/p/dc39c41c2915
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
